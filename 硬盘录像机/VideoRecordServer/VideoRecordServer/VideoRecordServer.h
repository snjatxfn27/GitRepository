
// VideoRecordServer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

static CRITICAL_SECTION cSECTION;

// CVideoRecordServerApp: 
// �йش����ʵ�֣������ VideoRecordServer.cpp
//

class CVideoRecordServerApp : public CWinApp
{
public:
	CVideoRecordServerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CVideoRecordServerApp theApp;