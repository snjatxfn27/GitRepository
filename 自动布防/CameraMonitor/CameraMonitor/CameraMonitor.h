
// CameraMonitor.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCameraMonitorApp: 
// �йش����ʵ�֣������ CameraMonitor.cpp
//

class CCameraMonitorApp : public CWinApp
{
public:
	CCameraMonitorApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCameraMonitorApp theApp;