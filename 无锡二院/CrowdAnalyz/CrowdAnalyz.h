
// CrowdAnalyz.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

#define WIDTH 1600
#define HEIGHT 900

// CCrowdAnalyzApp: 
// 有关此类的实现，请参阅 CrowdAnalyz.cpp
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

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CCrowdAnalyzApp theApp;