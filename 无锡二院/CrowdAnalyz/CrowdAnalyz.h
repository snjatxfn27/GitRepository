
// CrowdAnalyz.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

#define WIDTH 1600
#define HEIGHT 900

// CCrowdAnalyzApp: 
// �йش����ʵ�֣������ CrowdAnalyz.cpp
//

class CCrowdAnalyzApp : public CWinApp
{
public:
	CCrowdAnalyzApp();
public:
	//GDI+
	ULONG_PTR m_pGdiToken;

	CString m_strDBIP;
	CString m_strDBName;
	CString m_strDBUsername;
	CString m_strDBPassword;
	CString m_strServerIP;

	CString m_strBID, m_strWID, m_strTBID, m_strTEID;

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CCrowdAnalyzApp theApp;