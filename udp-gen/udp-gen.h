
// udp-gen.h : ������� ���� ��������� ��� ���������� PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"		// �������� �������


// CudpgenApp:
// � ���������� ������� ������ ��. udp-gen.cpp
//

class CudpgenApp : public CWinApp
{
public:
	CudpgenApp();

// ���������������
public:
	virtual BOOL InitInstance();

// ����������

	DECLARE_MESSAGE_MAP()
};

extern CudpgenApp theApp;