#if !defined(AFX_TOOLS_H__06691DCF_C5C7_4441_AD12_C54E3873B6DE__INCLUDED_)
#define AFX_TOOLS_H__06691DCF_C5C7_4441_AD12_C54E3873B6DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Tools.h : header file
//

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
