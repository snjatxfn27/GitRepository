
// DisWatchDog.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDisWatchDogApp: 
// �йش����ʵ�֣������ DisWatchDog.cpp
//

class CDisWatchDogApp : public CWinApp
{
public:
	CDisWatchDogApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDisWatchDogApp theApp;