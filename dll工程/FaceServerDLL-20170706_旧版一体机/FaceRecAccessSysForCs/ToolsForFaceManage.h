// ToolsForFaceManage.h: interface for the ToolsForFaceManage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLSFORFACEMANAGE_H__7582BFC5_FE5A_4A82_A298_B3E218A5593B__INCLUDED_)
#define AFX_TOOLSFORFACEMANAGE_H__7582BFC5_FE5A_4A82_A298_B3E218A5593B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

	//增加一条人信息
	int AddPersonInfo(CString strIP,CString strSession,CString facedb,CString strName,CString strSex, CString strBirth,CString strNativeplace,CString strPhone,CString strAddress, CString strIdCard,char* pPersonid);
	//删除一条人信息（被删除的person中包含的face和feature将会一起被删除）
	int DeletePersonInfo(CString strIP,CString strSession,CString facedb,CString personid);
	//通过个人信息模糊删除个人信息
	int DeletePersonInfobyIdcard(CString strIP,CString strSession,CString facedb,CString strIdcard);
	//更新一条人信息
	int UpdatePersonInfo(CString strIP,CString strSession,CString facedb,CString personid,CString strName,CString strSex, CString strBirth,CString strNativeplace,CString strPhone,CString strAddress, CString strIdCard);
	//增加或更新一个人脸
	int AddFaceInfo(CString strIP,CString strSession,CString strEImage,char *EFeature,CString facedb,CString personid);

#endif // !defined(AFX_TOOLSFORFACEMANAGE_H__7582BFC5_FE5A_4A82_A298_B3E218A5593B__INCLUDED_)
