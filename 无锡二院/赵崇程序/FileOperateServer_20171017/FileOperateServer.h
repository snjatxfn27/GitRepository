
// FileOperateServer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CFileOperateServerApp: 
// �йش����ʵ�֣������ FileOperateServer.cpp
//

class CFileOperateServerApp : public CWinApp
{
public:
	CFileOperateServerApp();
	CString m_strFilePath;
// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CFileOperateServerApp theApp;