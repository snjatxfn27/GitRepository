
// CollectionAnfContrast.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

static CRITICAL_SECTION cSECTION;
// CCollectionAnfContrastApp: 
// �йش����ʵ�֣������ CollectionAnfContrast.cpp
//

class CCollectionAnfContrastApp : public CWinApp
{
public:
	CCollectionAnfContrastApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CCollectionAnfContrastApp theApp;