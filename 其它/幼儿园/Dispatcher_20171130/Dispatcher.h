
// Dispatcher.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CDispatcherApp: 
// 有关此类的实现，请参阅 Dispatcher.cpp
//

class CDispatcherApp : public CWinApp
{
public:
	CDispatcherApp();
public:
	CString m_strDBIP;
	CString m_strDBName;
	CString m_strDBUsername;
	CString m_strDBPassword;
	CString m_strDetectServerIP;

	CString m_strTotalBlackDBIDs,m_strTotalWhiteDBIDs;
	double m_dbTotalSim;
// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CDispatcherApp theApp;