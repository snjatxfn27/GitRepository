
// WatchDog.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#define WIDTH 1024
#define HEIGHT 768

#include "resource.h"		// ������


// CWatchDogApp: 
// �йش����ʵ�֣������ WatchDog.cpp
//

class CWatchDogApp : public CWinApp
{
public:
	CWatchDogApp();
public:
	//GDI+
	ULONG_PTR m_pGdiToken;

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CWatchDogApp theApp;