
// WatchDog.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#define WIDTH 1024
#define HEIGHT 768

#include "resource.h"		// 主符号


// CWatchDogApp: 
// 有关此类的实现，请参阅 WatchDog.cpp
//

class CWatchDogApp : public CWinApp
{
public:
	CWatchDogApp();
public:
	//GDI+
	ULONG_PTR m_pGdiToken;

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CWatchDogApp theApp;