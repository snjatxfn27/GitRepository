// ToolsForFaceManage.h: interface for the ToolsForFaceManage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLSFORFACEMANAGE_H__7582BFC5_FE5A_4A82_A298_B3E218A5593B__INCLUDED_)
#define AFX_TOOLSFORFACEMANAGE_H__7582BFC5_FE5A_4A82_A298_B3E218A5593B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

	//����һ������Ϣ
	int AddPersonInfo(CString strIP,CString strSession,CString facedb,CString strName,CString strSex, CString strBirth,CString strNativeplace,CString strPhone,CString strAddress, CString strIdCard,char* pPersonid);
	//ɾ��һ������Ϣ����ɾ����person�а�����face��feature����һ��ɾ����
	int DeletePersonInfo(CString strIP,CString strSession,CString facedb,CString personid);
	//ͨ��������Ϣģ��ɾ��������Ϣ
	int DeletePersonInfobyIdcard(CString strIP,CString strSession,CString facedb,CString strIdcard);
	//����һ������Ϣ
	int UpdatePersonInfo(CString strIP,CString strSession,CString facedb,CString personid,CString strName,CString strSex, CString strBirth,CString strNativeplace,CString strPhone,CString strAddress, CString strIdCard);
	//���ӻ����һ������
	int AddFaceInfo(CString strIP,CString strSession,CString strEImage,char *EFeature,CString facedb,CString personid);

#endif // !defined(AFX_TOOLSFORFACEMANAGE_H__7582BFC5_FE5A_4A82_A298_B3E218A5593B__INCLUDED_)
