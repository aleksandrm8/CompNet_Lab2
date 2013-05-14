
// udp-genDlg.h : файл заголовка
//

#pragma once


// диалоговое окно CudpgenDlg
class CudpgenDlg : public CDialogEx
{
// Создание
public:
	CudpgenDlg(CWnd* pParent = NULL);	// стандартный конструктор

// Данные диалогового окна
	enum { IDD = IDD_UDPGEN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnIpnFieldchangedIpaddress2(NMHDR *pNMHDR, LRESULT *pResult);
	// IP отправителя
	DWORD SourceIP;
	// Порт отправителя
	short SourcePort;
	// IP получателя
	DWORD DestinationIP;
	// Порт получателя
	int DestinationPort;
	// Текст содержимого пакета
	CString MessageText;

	// Структура заголовка IP
	struct ip_header {
		unsigned char	ver_ihl;	// Длина заголовка (4 бита) 
		                            // (измеряется в словах по 32 бита) +
		                            // + Номер версии протокола (4 бита)
		unsigned char	tos;		// Тип сервиса 
		unsigned short	tlen;		// Общая длина пакета 
		unsigned short	id;		    // Идентификатор пакета
		unsigned short	flags_fo;	// Управляющие флаги (3 бита) 
		                            // + Смещение фрагмента (13 бит)
		unsigned char	ttl;		// Время жизни пакета
		unsigned char	proto;		// Протокол верхнего уровня 
		unsigned short	crc;		// CRC заголовка
		unsigned int	src_addr;	// IP-адрес отправителя
		unsigned int	dst_addr;	// IP-адрес получателя
	};

	// Структура заголовка UDP
	struct udp_header {
		unsigned short   src_port ;	// номер порта отправителя 
		unsigned short   dst_port ;	// номер порта получателя 
		unsigned short   length;	// длина датаграммы 
		unsigned short   crc;		// контрольная сумма заголовка
	}; 

	// Псевдозаголовок для UDP пакетов
	struct pseudo_header {
		unsigned int src_addr;		// адрес отправителя 
		unsigned int dst_addr;		// адрес получателя 
		unsigned char zero ;		// начальная установка 
		unsigned char proto;		// протокол
		unsigned short length;		// длина заголовка 
	};


	// Отправка пакета
	afx_msg void SendPacketBTN();
	// Контрольная сумма IP
	unsigned short rs_crc(unsigned short * buffer, int length);
	// Подсчёт CRC в UDP пакетах
	unsigned short rs_pseudo_crc(char * data, int data_length, unsigned int src_addr, unsigned int dst_addr, int packet_length, unsigned char proto);
	// Инициализация библиотеки Winsock
	int rs_init(int v_major, int v_minor);
	// Настройка RAW сокета
	int rs_set_tos(SOCKET s, unsigned char new_tos);
	// Установка опции RAW для сокета
	int rs_set_raw(SOCKET s);
	// Отправка пакетов IP
	int rs_send_ip(SOCKET s, struct ip_header iph, unsigned char * data, int data_length, unsigned short dst_port_raw);
	// Отправка пакетов UDP
	int rs_send_udp(SOCKET s, struct ip_header iph, struct udp_header udph, unsigned char * data, int data_length);
	// Деинициализация библиотеки Winsock
	int rs_exit(void);
};
