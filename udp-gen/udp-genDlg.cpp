
// udp-genDlg.cpp : файл реализации
//

#include "stdafx.h"
#include "udp-gen.h"
#include "udp-genDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Версия IP пакета
#define RS_IP_VERSION		0x40

// IP флаги фрагментации
#define IP_FLAG_FO_MASK	0xE000
#define IP_FLAG_MORE_FRAG	0x2000
#define IP_FLAG_DONT_FRAG	0x4000

// IP тип сервиса
#define IP_TOS_0x00		0x00
#define IP_TOS_0x02		0x02
#define IP_TOS_0x04		0x04
#define IP_TOS_0x08		0x08
#define IP_TOS_0x10		0x10




// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// диалоговое окно CudpgenDlg




CudpgenDlg::CudpgenDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CudpgenDlg::IDD, pParent)
	, SourceIP(0x7F000001)
	, SourcePort(1024)
	, DestinationIP(0x7F000001)
	, DestinationPort(1234)
	, MessageText(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CudpgenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_IPAddress(pDX, IDC_IPADDRESS2, SourceIP);
	DDX_Text(pDX, IDC_EDIT1, SourcePort);
	DDX_IPAddress(pDX, IDC_IPADDRESS3, DestinationIP);
	DDX_Text(pDX, IDC_EDIT2, DestinationPort);
	DDX_Text(pDX, IDC_EDIT3, MessageText);
}

BEGIN_MESSAGE_MAP(CudpgenDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS2, &CudpgenDlg::OnIpnFieldchangedIpaddress2)
	ON_BN_CLICKED(IDC_BUTTON1, &CudpgenDlg::SendPacketBTN)
END_MESSAGE_MAP()


// обработчики сообщений CudpgenDlg

BOOL CudpgenDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Задает значок для этого диалогового окна. Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CudpgenDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок. Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CudpgenDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CudpgenDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CudpgenDlg::OnIpnFieldchangedIpaddress2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	*pResult = 0;
}


// Отправка пакета
void CudpgenDlg::SendPacketBTN()
{
	// Инициализация библиотеки Winsock
	rs_init(2, 2);

	// Инициализация RAW-сокета
	SOCKET rsock = WSASocket(AF_INET, SOCK_RAW, IPPROTO_RAW, NULL, 0, WSA_FLAG_OVERLAPPED);
	if(rsock == INVALID_SOCKET) {
		CString ErrorSessage;
		ErrorSessage.Format(L"Ошибка №%d !", WSAGetLastError());
		MessageBox(ErrorSessage, L"Ошибка", MB_OK | MB_ICONERROR);
		return;
	} 

	// Установка типа сервиса
	rs_set_tos(rsock, IP_TOS_0x00);

	// Установка опций для RAW-сокета
	rs_set_raw(rsock);

	// Чтение адресов
	UpdateData(TRUE);

	// Подготовка заголовка IP
	struct ip_header iph;
	iph.ver_ihl  = RS_IP_VERSION;
	iph.tos      = IP_TOS_0x00;
	iph.id       = 0;
	iph.flags_fo = IP_FLAG_DONT_FRAG;
	iph.ttl      = 64;
	iph.proto    = IPPROTO_UDP;
	iph.src_addr = htonl(SourceIP);
	iph.dst_addr = htonl(DestinationIP);

	// Подготовка заголовка TCP
	struct udp_header udph;
	udph.src_port = htons(SourcePort);
	udph.dst_port = htons(DestinationPort);

	// Подготовка сообщения.
	char * message_data;
	USES_CONVERSION;
	message_data = (char *)T2A((LPCTSTR)MessageText);
	
	// Отправка пакета
	if(SOCKET_ERROR == rs_send_udp(rsock, iph, udph, reinterpret_cast<unsigned char *>(message_data), sizeof(message_data))) {
		MessageText = L"Ошибка отправки!";
		UpdateData(FALSE);
	}

	// Завершение работы
	rs_exit();

}


// Контрольная сумма IP
unsigned short CudpgenDlg::rs_crc(unsigned short * buffer, int length)
{
	unsigned long crc = 0;
	
	// Вычисление CRC 
	while(length > 1) {
		crc += *buffer++;
		length -= sizeof(unsigned short);
	}
	if(length)
		crc += *(unsigned char*)buffer;
	
	// Закончить вычисления 
	crc =  (crc >> 16) + (crc & 0xffff);
	crc += (crc >> 16); 
	
	// Возвращаем инвертированное значение 
	return (unsigned short)(~crc);
}


// Подсчёт CRC в UDP пакетах
unsigned short CudpgenDlg::rs_pseudo_crc(char * data, int data_length, unsigned int src_addr, unsigned int dst_addr, int packet_length, unsigned char proto)
{
	char * buffer; 
	unsigned int full_length; 
	unsigned char header_length;
	struct pseudo_header ph; 
	unsigned short p_crc = 0;

	// Заполнение структуры псевдозаголовка 
	ph.src_addr = src_addr; 
	ph.dst_addr = dst_addr; 
	ph.zero = 0; 
	ph.proto = proto;
	ph.length = htons(packet_length);
	header_length = sizeof(struct pseudo_header);
	full_length = header_length + data_length; 
	buffer = (char *)calloc(full_length, sizeof(char)); 

	// Генерация псевдозаголовка 
	memcpy(buffer, &ph, header_length); 
	memcpy(buffer + header_length, data, data_length); 

	// Вычисление CRC. 
	p_crc = rs_crc((unsigned short*)buffer, full_length);
	free(buffer); 
	return p_crc;
}

// Инициализация библиотеки Winsock
int CudpgenDlg::rs_init(int v_major, int v_minor)
{
	WSADATA wsadata;
	
	// Инициализация WinSock заданной версии
	if(WSAStartup(MAKEWORD(v_major, v_minor), &wsadata))
		return WSAGetLastError();
	
	// Проверка версии WinSock 
	if(LOBYTE(wsadata.wVersion) != v_minor || HIBYTE(wsadata.wVersion) != v_major) { 
		rs_exit();
		return WSAGetLastError(); 
	}

	return 0;
}


// Настройка RAW сокета
int CudpgenDlg::rs_set_tos(SOCKET s, unsigned char new_tos)
{
	int tos = new_tos;
	int tos_len = sizeof(tos);
	int per = setsockopt(s, IPPROTO_IP, 3, (char *)&tos, tos_len);
	
	if(per == SOCKET_ERROR)
		return WSAGetLastError();
	
	return 0;
}


// Установка опции RAW для сокета
int CudpgenDlg::rs_set_raw(SOCKET s)
{
	unsigned int use_own_header = 1;

	// Установка опции RAW для сокета, что говорит о том
	// что мы вручную будем формировать заголовки пакетов
	if(setsockopt(s, IPPROTO_IP, 2, (char*)&use_own_header, sizeof(use_own_header)) == SOCKET_ERROR)
		return WSAGetLastError();
	
	return 0;
}


// Отправка пакетов IP
int CudpgenDlg::rs_send_ip(SOCKET s, struct ip_header iph, unsigned char * data, int data_length, unsigned short dst_port_raw)
{
	char * buffer;
	int result; 
	sockaddr_in target; 
	unsigned char header_length;
	unsigned int packet_length;
	memset(&target, 0, sizeof(target));
	target.sin_family = AF_INET; 
	target.sin_addr.s_addr = iph.dst_addr; 
	target.sin_port = dst_port_raw; 

	// Вычисление длины и заголовка пакета 
	header_length = sizeof(struct ip_header); 
	packet_length = header_length + data_length; 

	// Установка CRC. 
	iph.crc = 0; 

	// Заполнение некоторых полей заголовка IP
	iph.ver_ihl = RS_IP_VERSION;

	// Если длина пакета не задана, то длина пакета 
	// приравнивается к длине заголовка 
	if(!(iph.ver_ihl & 0x0F)) 
		iph.ver_ihl |= 0x0F & (header_length / 4);
	buffer = (char *)calloc(packet_length, sizeof(char)); 

	// Копирование заголовка пакета в буфер ( CRC равно 0). 
	memcpy(buffer, &iph, sizeof(struct ip_header));

	// Копирование данных в буфер 
	if(data)
		memcpy(buffer + header_length, data, data_length); 

	// Вычисление CRC. 
	iph.crc = rs_crc((unsigned short *)buffer, packet_length); 

	// Копирование заголовка пакета в буфер (CRC посчитана). 
	memcpy(buffer, &iph, sizeof(struct ip_header)); 

	// Отправка IP пакета в сеть.
	result = sendto(s, buffer, packet_length, 0, (struct sockaddr *)&target, sizeof(target));
	free(buffer);
	return result;
}


// Отправка пакетов UDP
int CudpgenDlg::rs_send_udp(SOCKET s, struct ip_header iph, struct udp_header udph, unsigned char * data, int data_length)
{
	char * buffer; 
	int result; 
	unsigned char header_length;
	unsigned int packet_length; 

	// Вычисление длин пакета и заголовка. 
	header_length = sizeof (struct udp_header); 
	packet_length = header_length + data_length; 

	// Установка CRC. 
	udph.crc = 0;

	// Если длина пакета не задана, то длина пакета 
	// приравнивается к длине заголовка
	if(!udph.length)
		udph.length = htons(packet_length); 
	buffer = (char *)calloc(packet_length, sizeof(char)); 

	// Копирование заголовка пакета в буфер (CRC равно 0). 
	memcpy(buffer, &udph, sizeof(struct udp_header));

	// Копирование протокола более высокого уровня (данных) 
	if(data)
		memcpy(buffer + header_length, data, data_length); 

	// Вычисление CRC. 
	udph.crc = rs_pseudo_crc(buffer, packet_length, iph.src_addr, iph.dst_addr, packet_length, IPPROTO_UDP); 

	// Копирование заголовка пакета в буфер (CRC посчитана). 
	memcpy(buffer, &udph, sizeof(struct udp_header)); 

	// Отправка IP пакета с вложенным UDP пакетом. 
	result = rs_send_ip(s, iph, reinterpret_cast<unsigned char *>(buffer), packet_length, udph.dst_port); 
	free(buffer); 
	return result;
}


// Деинициализация библиотеки Winsock
int CudpgenDlg::rs_exit(void)
{
	// Закрытие библиотеки Winsock
	WSACleanup();
	return 0;
}
