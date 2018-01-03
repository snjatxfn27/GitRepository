// FRDLLTest.h : main header file for the FRDLLTEST application
//

#if !defined(AFX_FRDLLTEST_H__198AA969_C5F9_4637_A5CE_C47535E1E139__INCLUDED_)
#define AFX_FRDLLTEST_H__198AA969_C5F9_4637_A5CE_C47535E1E139__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFRDLLTestApp:
// See FRDLLTest.cpp for the implementation of this class
//
class CFRDLLTestApp;
extern CFRDLLTestApp theApp;

class CFRDLLTestApp : public CWinApp
{
public:
	CFRDLLTestApp();
	CString m_strDeviceIP;
	CString m_strAdoIP;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFRDLLTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFRDLLTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRDLLTEST_H__198AA969_C5F9_4637_A5CE_C47535E1E139__INCLUDED_)
