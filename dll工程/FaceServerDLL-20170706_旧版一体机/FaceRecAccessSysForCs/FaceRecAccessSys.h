// FaceRecAccessSys.h : main header file for the FACERECACCESSSYS DLL
//

#if !defined(AFX_FACERECACCESSSYS_H__D12B9E83_77E9_412A_8724_1EED535805D8__INCLUDED_)
#define AFX_FACERECACCESSSYS_H__D12B9E83_77E9_412A_8724_1EED535805D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif


#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFaceRecAccessSysApp
// See FaceRecAccessSys.cpp for the implementation of this class
//

class CFaceRecAccessSysApp : public CWinApp
{
public:
	CFaceRecAccessSysApp();
	CString BatchInfoPublish(CString strDbId,CString strDbIp,CString strDbName,CString strDbUser,CString strDbPwd);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFaceRecAccessSysApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CFaceRecAccessSysApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FACERECACCESSSYS_H__D12B9E83_77E9_412A_8724_1EED535805D8__INCLUDED_)
