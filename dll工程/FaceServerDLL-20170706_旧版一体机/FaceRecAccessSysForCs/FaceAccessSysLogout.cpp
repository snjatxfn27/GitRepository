// FaceAccessSysLogout.cpp: implementation of the FaceAccessSysLogout class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FaceRecAccessSys.h"
#include "FaceAccessSysLogout.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFaceAccessSysLogout::CFaceAccessSysLogout()
{

}

CFaceAccessSysLogout::~CFaceAccessSysLogout()
{

}
CString CFaceAccessSysLogout::Logout(char *strIP)
{
	CString se = Logout(strIP);
	AfxMessageBox(se);
}
