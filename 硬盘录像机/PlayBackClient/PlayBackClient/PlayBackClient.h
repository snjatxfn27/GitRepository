
// PlayBackClient.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPlayBackClientApp: 
// �йش����ʵ�֣������ PlayBackClient.cpp
//

class CPlayBackClientApp : public CWinApp
{
public:
	CPlayBackClientApp();
	ULONG_PTR m_pGdiToken;


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CPlayBackClientApp theApp;