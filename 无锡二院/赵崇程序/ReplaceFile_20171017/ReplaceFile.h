
// ReplaceFile.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CReplaceFileApp: 
// �йش����ʵ�֣������ ReplaceFile.cpp
//

class CReplaceFileApp : public CWinApp
{
public:
	CReplaceFileApp();

public:
	CString m_strDBIP;
	CString m_strDBName;
	CString m_strDBUsername;
	CString m_strDBPassword;
	CString m_strSrcPath;
	CString m_strDstPath;

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CReplaceFileApp theApp;