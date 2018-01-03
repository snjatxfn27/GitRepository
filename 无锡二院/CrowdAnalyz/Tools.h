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
	将数据编码为base64的形式，pSrc为原数据指针，pDst为编码后数据指针
	*/
	int EncodeBase64(const unsigned char* pSrc, char* pDst, int nSrcLen, int nMaxLineLen);

	/*
	将base64编码的数据解码
	*/
	int DecodeBase64(const char* pSrc, unsigned char* pDst, int nSrcLen);

	/*
	从网络（网页服务器）下载一张jpg图片
	src:原图位置，如/data/20170404/imgCurFaceJpg/curface_7102。
	dst:本地目标位置，如data/20170404/imgCurFaceJpg/curface_7102。
	返回值：0正确；
	*/
	int DownloadImageFromFileServer(CString IP, int port, CString src, CString dst);


	/*
	从数据库imginfo表中获取一张靶向图片
	dst:靶向图片转换为jpg格式后，存放的位置
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
