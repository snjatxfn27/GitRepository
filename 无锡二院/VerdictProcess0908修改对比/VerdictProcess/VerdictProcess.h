
// VerdictProcess.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CVerdictProcessApp: 
// �йش����ʵ�֣������ VerdictProcess.cpp
//

class CVerdictProcessApp : public CWinApp
{
public:
	CVerdictProcessApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CVerdictProcessApp theApp;