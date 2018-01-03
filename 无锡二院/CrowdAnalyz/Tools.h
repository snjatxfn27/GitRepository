#if !defined(AFX_TOOLS_H__06691DCF_C5C7_4441_AD12_C54E3873B6DE__INCLUDED_)
#define AFX_TOOLS_H__06691DCF_C5C7_4441_AD12_C54E3873B6DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Tools.h : header file
//
#include "stdafx.h"
/////////////////////////////////////////////////////////////////////////////
// CTools window

class CTools : public CWnd
{
// Construction
public:
	CTools();

// Attributes
public:

// Operations
public:
	/*
	�����ݱ���Ϊbase64����ʽ��pSrcΪԭ����ָ�룬pDstΪ���������ָ��
	*/
	int EncodeBase64(const unsigned char* pSrc, char* pDst, int nSrcLen, int nMaxLineLen);

	/*
	��base64��������ݽ���
	*/
	int DecodeBase64(const char* pSrc, unsigned char* pDst, int nSrcLen);

	/*
	�����磨��ҳ������������һ��jpgͼƬ
	src:ԭͼλ�ã���/data/20170404/imgCurFaceJpg/curface_7102��
	dst:����Ŀ��λ�ã���data/20170404/imgCurFaceJpg/curface_7102��
	����ֵ��0��ȷ��
	*/
	int DownloadImageFromFileServer(CString IP, int port, CString src, CString dst);


	/*
	�����ݿ�imginfo���л�ȡһ�Ű���ͼƬ
	dst:����ͼƬת��Ϊjpg��ʽ�󣬴�ŵ�λ��
	*/
	int GetJPGFromDB(CString csDBIp, CString csDBName, CString csDBUser, CString csDBPwd, CString strIDCard, CString dst);
	//int GetJPGFromDB(CString strIDCard, CString dst);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTools)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTools();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTools)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLS_H__06691DCF_C5C7_4441_AD12_C54E3873B6DE__INCLUDED_)
