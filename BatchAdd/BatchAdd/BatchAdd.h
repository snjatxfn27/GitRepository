
// BatchAdd.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

static CRITICAL_SECTION cSECTION;

// CBatchAddApp: 
// �йش����ʵ�֣������ BatchAdd.cpp
//

class CBatchAddApp : public CWinApp
{
public:
	CBatchAddApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CBatchAddApp theApp;