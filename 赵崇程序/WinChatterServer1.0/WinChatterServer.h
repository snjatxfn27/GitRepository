
// WinChatterServer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

#define getsqlstr(x) (char*)_bstr_t(pSet->GetCollect(_variant_t(x)))
#define getsqlvt(x) pSet->GetCollect(_variant_t(x))
#define getsqlint(x) pSet->GetCollect(_variant_t(x)).iVal
#define getsqllong(x) pSet->GetCollect(_variant_t(x)).lVal
#define getsqldouble(x) pSet->GetCollect(_variant_t(x)).dblVal

// CWinChatterServerApp: 
// �йش����ʵ�֣������ WinChatterServer.cpp
//

class CWinChatterServerApp : public CWinApp
{
public:
	CWinChatterServerApp();
public:
	CString m_strDBIP;
	CString m_strDBName;
	CString m_strDBUsername;
	CString m_strDBPassword;

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CWinChatterServerApp theApp;