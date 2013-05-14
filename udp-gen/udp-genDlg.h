
// udp-genDlg.h : ���� ���������
//

#pragma once


// ���������� ���� CudpgenDlg
class CudpgenDlg : public CDialogEx
{
// ��������
public:
	CudpgenDlg(CWnd* pParent = NULL);	// ����������� �����������

// ������ ����������� ����
	enum { IDD = IDD_UDPGEN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// ��������� DDX/DDV


// ����������
protected:
	HICON m_hIcon;

	// ��������� ������� ����� ���������
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnIpnFieldchangedIpaddress2(NMHDR *pNMHDR, LRESULT *pResult);
	// IP �����������
	DWORD SourceIP;
	// ���� �����������
	short SourcePort;
	// IP ����������
	DWORD DestinationIP;
	// ���� ����������
	int DestinationPort;
	// ����� ����������� ������
	CString MessageText;

	// ��������� ��������� IP
	struct ip_header {
		unsigned char	ver_ihl;	// ����� ��������� (4 ����) 
		                            // (���������� � ������ �� 32 ����) +
		                            // + ����� ������ ��������� (4 ����)
		unsigned char	tos;		// ��� ������� 
		unsigned short	tlen;		// ����� ����� ������ 
		unsigned short	id;		    // ������������� ������
		unsigned short	flags_fo;	// ����������� ����� (3 ����) 
		                            // + �������� ��������� (13 ���)
		unsigned char	ttl;		// ����� ����� ������
		unsigned char	proto;		// �������� �������� ������ 
		unsigned short	crc;		// CRC ���������
		unsigned int	src_addr;	// IP-����� �����������
		unsigned int	dst_addr;	// IP-����� ����������
	};

	// ��������� ��������� UDP
	struct udp_header {
		unsigned short   src_port ;	// ����� ����� ����������� 
		unsigned short   dst_port ;	// ����� ����� ���������� 
		unsigned short   length;	// ����� ���������� 
		unsigned short   crc;		// ����������� ����� ���������
	}; 

	// ��������������� ��� UDP �������
	struct pseudo_header {
		unsigned int src_addr;		// ����� ����������� 
		unsigned int dst_addr;		// ����� ���������� 
		unsigned char zero ;		// ��������� ��������� 
		unsigned char proto;		// ��������
		unsigned short length;		// ����� ��������� 
	};


	// �������� ������
	afx_msg void SendPacketBTN();
	// ����������� ����� IP
	unsigned short rs_crc(unsigned short * buffer, int length);
	// ������� CRC � UDP �������
	unsigned short rs_pseudo_crc(char * data, int data_length, unsigned int src_addr, unsigned int dst_addr, int packet_length, unsigned char proto);
	// ������������� ���������� Winsock
	int rs_init(int v_major, int v_minor);
	// ��������� RAW ������
	int rs_set_tos(SOCKET s, unsigned char new_tos);
	// ��������� ����� RAW ��� ������
	int rs_set_raw(SOCKET s);
	// �������� ������� IP
	int rs_send_ip(SOCKET s, struct ip_header iph, unsigned char * data, int data_length, unsigned short dst_port_raw);
	// �������� ������� UDP
	int rs_send_udp(SOCKET s, struct ip_header iph, struct udp_header udph, unsigned char * data, int data_length);
	// ��������������� ���������� Winsock
	int rs_exit(void);
};
