
// udp-genDlg.cpp : ���� ����������
//

#include "stdafx.h"
#include "udp-gen.h"
#include "udp-genDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ������ IP ������
#define RS_IP_VERSION		0x40

// IP ����� ������������
#define IP_FLAG_FO_MASK	0xE000
#define IP_FLAG_MORE_FRAG	0x2000
#define IP_FLAG_DONT_FRAG	0x4000

// IP ��� �������
#define IP_TOS_0x00		0x00
#define IP_TOS_0x02		0x02
#define IP_TOS_0x04		0x04
#define IP_TOS_0x08		0x08
#define IP_TOS_0x10		0x10




// ���������� ���� CAboutDlg ������������ ��� �������� �������� � ����������

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ������ ����������� ����
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

// ����������
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


// ���������� ���� CudpgenDlg




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


// ����������� ��������� CudpgenDlg

BOOL CudpgenDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���������� ������ "� ���������..." � ��������� ����.

	// IDM_ABOUTBOX ������ ���� � �������� ��������� �������.
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

	// ������ ������ ��� ����� ����������� ����. ����� ������ ��� �������������,
	//  ���� ������� ���� ���������� �� �������� ����������
	SetIcon(m_hIcon, TRUE);			// ������� ������
	SetIcon(m_hIcon, FALSE);		// ������ ������

	// TODO: �������� �������������� �������������

	return TRUE;  // ������� �������� TRUE, ���� ����� �� ������� �������� ����������
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

// ��� ���������� ������ ����������� � ���������� ���� ����� ��������������� ����������� ���� �����,
//  ����� ���������� ������. ��� ���������� MFC, ������������ ������ ���������� ��� �������������,
//  ��� ������������� ����������� ������� ��������.

void CudpgenDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �������� ���������� ��� ���������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ������������ ������ �� ������ ����������� ��������������
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ��������� ������
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ������� �������� ��� ������� ��� ��������� ����������� ������� ��� �����������
//  ���������� ����.
HCURSOR CudpgenDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CudpgenDlg::OnIpnFieldchangedIpaddress2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: �������� ���� ��� ����������� �����������
	*pResult = 0;
}


// �������� ������
void CudpgenDlg::SendPacketBTN()
{
	// ������������� ���������� Winsock
	rs_init(2, 2);

	// ������������� RAW-������
	SOCKET rsock = WSASocket(AF_INET, SOCK_RAW, IPPROTO_RAW, NULL, 0, WSA_FLAG_OVERLAPPED);
	if(rsock == INVALID_SOCKET) {
		CString ErrorSessage;
		ErrorSessage.Format(L"������ �%d !", WSAGetLastError());
		MessageBox(ErrorSessage, L"������", MB_OK | MB_ICONERROR);
		return;
	} 

	// ��������� ���� �������
	rs_set_tos(rsock, IP_TOS_0x00);

	// ��������� ����� ��� RAW-������
	rs_set_raw(rsock);

	// ������ �������
	UpdateData(TRUE);

	// ���������� ��������� IP
	struct ip_header iph;
	iph.ver_ihl  = RS_IP_VERSION;
	iph.tos      = IP_TOS_0x00;
	iph.id       = 0;
	iph.flags_fo = IP_FLAG_DONT_FRAG;
	iph.ttl      = 64;
	iph.proto    = IPPROTO_UDP;
	iph.src_addr = htonl(SourceIP);
	iph.dst_addr = htonl(DestinationIP);

	// ���������� ��������� TCP
	struct udp_header udph;
	udph.src_port = htons(SourcePort);
	udph.dst_port = htons(DestinationPort);

	// ���������� ���������.
	char * message_data;
	USES_CONVERSION;
	message_data = (char *)T2A((LPCTSTR)MessageText);
	
	// �������� ������
	if(SOCKET_ERROR == rs_send_udp(rsock, iph, udph, reinterpret_cast<unsigned char *>(message_data), sizeof(message_data))) {
		MessageText = L"������ ��������!";
		UpdateData(FALSE);
	}

	// ���������� ������
	rs_exit();

}


// ����������� ����� IP
unsigned short CudpgenDlg::rs_crc(unsigned short * buffer, int length)
{
	unsigned long crc = 0;
	
	// ���������� CRC 
	while(length > 1) {
		crc += *buffer++;
		length -= sizeof(unsigned short);
	}
	if(length)
		crc += *(unsigned char*)buffer;
	
	// ��������� ���������� 
	crc =  (crc >> 16) + (crc & 0xffff);
	crc += (crc >> 16); 
	
	// ���������� ��������������� �������� 
	return (unsigned short)(~crc);
}


// ������� CRC � UDP �������
unsigned short CudpgenDlg::rs_pseudo_crc(char * data, int data_length, unsigned int src_addr, unsigned int dst_addr, int packet_length, unsigned char proto)
{
	char * buffer; 
	unsigned int full_length; 
	unsigned char header_length;
	struct pseudo_header ph; 
	unsigned short p_crc = 0;

	// ���������� ��������� ��������������� 
	ph.src_addr = src_addr; 
	ph.dst_addr = dst_addr; 
	ph.zero = 0; 
	ph.proto = proto;
	ph.length = htons(packet_length);
	header_length = sizeof(struct pseudo_header);
	full_length = header_length + data_length; 
	buffer = (char *)calloc(full_length, sizeof(char)); 

	// ��������� ��������������� 
	memcpy(buffer, &ph, header_length); 
	memcpy(buffer + header_length, data, data_length); 

	// ���������� CRC. 
	p_crc = rs_crc((unsigned short*)buffer, full_length);
	free(buffer); 
	return p_crc;
}

// ������������� ���������� Winsock
int CudpgenDlg::rs_init(int v_major, int v_minor)
{
	WSADATA wsadata;
	
	// ������������� WinSock �������� ������
	if(WSAStartup(MAKEWORD(v_major, v_minor), &wsadata))
		return WSAGetLastError();
	
	// �������� ������ WinSock 
	if(LOBYTE(wsadata.wVersion) != v_minor || HIBYTE(wsadata.wVersion) != v_major) { 
		rs_exit();
		return WSAGetLastError(); 
	}

	return 0;
}


// ��������� RAW ������
int CudpgenDlg::rs_set_tos(SOCKET s, unsigned char new_tos)
{
	int tos = new_tos;
	int tos_len = sizeof(tos);
	int per = setsockopt(s, IPPROTO_IP, 3, (char *)&tos, tos_len);
	
	if(per == SOCKET_ERROR)
		return WSAGetLastError();
	
	return 0;
}


// ��������� ����� RAW ��� ������
int CudpgenDlg::rs_set_raw(SOCKET s)
{
	unsigned int use_own_header = 1;

	// ��������� ����� RAW ��� ������, ��� ������� � ���
	// ��� �� ������� ����� ����������� ��������� �������
	if(setsockopt(s, IPPROTO_IP, 2, (char*)&use_own_header, sizeof(use_own_header)) == SOCKET_ERROR)
		return WSAGetLastError();
	
	return 0;
}


// �������� ������� IP
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

	// ���������� ����� � ��������� ������ 
	header_length = sizeof(struct ip_header); 
	packet_length = header_length + data_length; 

	// ��������� CRC. 
	iph.crc = 0; 

	// ���������� ��������� ����� ��������� IP
	iph.ver_ihl = RS_IP_VERSION;

	// ���� ����� ������ �� ������, �� ����� ������ 
	// �������������� � ����� ��������� 
	if(!(iph.ver_ihl & 0x0F)) 
		iph.ver_ihl |= 0x0F & (header_length / 4);
	buffer = (char *)calloc(packet_length, sizeof(char)); 

	// ����������� ��������� ������ � ����� ( CRC ����� 0). 
	memcpy(buffer, &iph, sizeof(struct ip_header));

	// ����������� ������ � ����� 
	if(data)
		memcpy(buffer + header_length, data, data_length); 

	// ���������� CRC. 
	iph.crc = rs_crc((unsigned short *)buffer, packet_length); 

	// ����������� ��������� ������ � ����� (CRC ���������). 
	memcpy(buffer, &iph, sizeof(struct ip_header)); 

	// �������� IP ������ � ����.
	result = sendto(s, buffer, packet_length, 0, (struct sockaddr *)&target, sizeof(target));
	free(buffer);
	return result;
}


// �������� ������� UDP
int CudpgenDlg::rs_send_udp(SOCKET s, struct ip_header iph, struct udp_header udph, unsigned char * data, int data_length)
{
	char * buffer; 
	int result; 
	unsigned char header_length;
	unsigned int packet_length; 

	// ���������� ���� ������ � ���������. 
	header_length = sizeof (struct udp_header); 
	packet_length = header_length + data_length; 

	// ��������� CRC. 
	udph.crc = 0;

	// ���� ����� ������ �� ������, �� ����� ������ 
	// �������������� � ����� ���������
	if(!udph.length)
		udph.length = htons(packet_length); 
	buffer = (char *)calloc(packet_length, sizeof(char)); 

	// ����������� ��������� ������ � ����� (CRC ����� 0). 
	memcpy(buffer, &udph, sizeof(struct udp_header));

	// ����������� ��������� ����� �������� ������ (������) 
	if(data)
		memcpy(buffer + header_length, data, data_length); 

	// ���������� CRC. 
	udph.crc = rs_pseudo_crc(buffer, packet_length, iph.src_addr, iph.dst_addr, packet_length, IPPROTO_UDP); 

	// ����������� ��������� ������ � ����� (CRC ���������). 
	memcpy(buffer, &udph, sizeof(struct udp_header)); 

	// �������� IP ������ � ��������� UDP �������. 
	result = rs_send_ip(s, iph, reinterpret_cast<unsigned char *>(buffer), packet_length, udph.dst_port); 
	free(buffer); 
	return result;
}


// ��������������� ���������� Winsock
int CudpgenDlg::rs_exit(void)
{
	// �������� ���������� Winsock
	WSACleanup();
	return 0;
}
