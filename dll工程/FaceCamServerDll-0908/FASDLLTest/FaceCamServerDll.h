// FaceCamServerDll.h : FaceCamServerDll DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������



// CFaceCamServerDllApp
// �йش���ʵ�ֵ���Ϣ������� FaceCamServerDll.cpp
//

class CFaceCamServerDllApp : public CWinApp
{
public:
	CFaceCamServerDllApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
