
// FASDLLTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CFASDLLTestApp: 
// �йش����ʵ�֣������ FASDLLTest.cpp
//

class CFASDLLTestApp : public CWinApp
{
public:
	CFASDLLTestApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CFASDLLTestApp theApp;