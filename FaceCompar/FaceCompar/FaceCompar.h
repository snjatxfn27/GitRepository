
// FaceCompar.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CFaceComparApp: 
// �йش����ʵ�֣������ FaceCompar.cpp
//

class CFaceComparApp : public CWinApp
{
public:
	CFaceComparApp();

// ��д
public:
	virtual BOOL InitInstance();
	ULONG_PTR m_pGdiToken;

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CFaceComparApp theApp;