
// GuardServer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

static CRITICAL_SECTION cSECTION;
// CGuardServerApp: 
// �йش����ʵ�֣������ GuardServer.cpp
//

class CGuardServerApp : public CWinApp
{
public:
	CGuardServerApp();

// ��д
public:
	virtual BOOL InitInstance();
	ULONG_PTR m_pGdiToken;
// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CGuardServerApp theApp;