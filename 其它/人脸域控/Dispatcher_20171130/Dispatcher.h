
// Dispatcher.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDispatcherApp: 
// �йش����ʵ�֣������ Dispatcher.cpp
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
// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CDispatcherApp theApp;