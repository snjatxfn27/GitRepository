
// WinChatterServer.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

#define getsqlstr(x) (char*)_bstr_t(pSet->GetCollect(_variant_t(x)))
#define getsqlvt(x) pSet->GetCollect(_variant_t(x))
#define getsqlint(x) pSet->GetCollect(_variant_t(x)).iVal
#define getsqllong(x) pSet->GetCollect(_variant_t(x)).lVal
#define getsqldouble(x) pSet->GetCollect(_variant_t(x)).dblVal

// CWinChatterServerApp: 
// 有关此类的实现，请参阅 WinChatterServer.cpp
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

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CWinChatterServerApp theApp;