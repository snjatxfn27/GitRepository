// HardDiskVideo.h : HardDiskVideo DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CHardDiskVideoApp
// �йش���ʵ�ֵ���Ϣ������� HardDiskVideo.cpp
//

class CHardDiskVideoApp : public CWinApp
{
public:
	CHardDiskVideoApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
