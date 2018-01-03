// FASFaceManage.cpp: implementation of the FASFaceManage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FaceRecAccessSys.h"
#include "FASFaceManage.h"
#include "ToolsForFaceManage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFASFaceManage::CFASFaceManage()
{

}

CFASFaceManage::~CFASFaceManage()
{

}
//新增人脸
int CFASFaceManage::FASAddPersonInfo(char * pIP,char * pSession,char * pFacedb,char * pName,char * pSex,char *pBirth,char * pNativeplace,char * pPhone,char * pAddress,char * pIdCard,char *pPersonid)
{	
	CString strIP;
	CString strSession;
	CString strFacedb;
	CString strName;
	CString strSex;
	CString strBirth;
    CString strNativeplace;
	CString strPhone;
	CString strAddress;
	CString strIdCard;
	strIP.Format("%s",pIP);
	strSession.Format("%s",pSession);
	strFacedb.Format("%s",pFacedb);
	strName.Format("%s",pName);
	strSex.Format("%s",pSex);
	strBirth.Format("%s",pBirth);
	strNativeplace.Format("%s",pNativeplace);
	strPhone.Format("%s",pPhone);
	strAddress.Format("%s",pAddress);
	strIdCard.Format("%s",pIdCard);
	int m_result = AddPersonInfo(strIP,strSession,strFacedb,strName,strSex,strBirth,strNativeplace,strPhone,strAddress,strIdCard,pPersonid);
	if(m_result != 0)
	{
		return m_result;
	}
	return 0;
}

//修改人脸
int CFASFaceManage::FASUpdatePersonInfo(char * pIP,char * pSession,char * pFacedb,char * pPersonid,char * pName,char * pSex,char *pBirth,char * pNativeplace,char * pPhone,char * pAddress,char * pIdCard)
{	
	CString strIP;
	CString strSession;
	CString strFacedb;
	CString strPersonid;
	CString strName;
	CString strSex;
	CString strBirth;
    CString strNativeplace;
	CString strPhone;
	CString strAddress;
	CString strIdCard;
	strIP.Format("%s",pIP);
	strSession.Format("%s",pSession);
	strFacedb.Format("%s",pFacedb);
	strPersonid.Format("%s",pPersonid);
	strName.Format("%s",pName);
	strSex.Format("%s",pSex);
	strBirth.Format("%s",pBirth);
	strNativeplace.Format("%s",pNativeplace);
	strPhone.Format("%s",pPhone);
	strAddress.Format("%s",pAddress);
	strIdCard.Format("%s",pIdCard);
	int m_result = UpdatePersonInfo(strIP,strSession,strFacedb,strPersonid,strName,strSex,strBirth,strNativeplace,strPhone,strAddress,strIdCard);
	if(m_result != 0)
	{
		return m_result;
	}
	return 0;
}
//修改人脸
int CFASFaceManage::FASAddFaceInfo(char * pIP,char * pSession,char * pEImage,char *EFeature,char * pFacedb,char * pPersonid)
{	
	CString strIP;
	CString strSession;
	CString strFacedb;
	CString strEImage;
	CString strPersonid;
	strIP.Format("%s",pIP);
	strSession.Format("%s",pSession);
	strFacedb.Format("%s",pFacedb);
	strEImage.Format("%s",pEImage);
	strPersonid.Format("%s",pPersonid);
	int m_result = AddFaceInfo(strIP,strSession,strEImage,EFeature,strFacedb,strPersonid);
	if(m_result != 0)
	{
		return m_result;
	}
	return 0;
}
//删除一个人信息
int CFASFaceManage::FASDeletePersonInfo(char * pIP,char * pSession,char * pFacedb,char * pPersonid)
{	
	CString strIP;
	CString strSession;
	CString strFacedb;
	CString strPersonid;
	strIP.Format("%s",pIP);
	strSession.Format("%s",pSession);
	strFacedb.Format("%s",pFacedb);
	strPersonid.Format("%s",pPersonid);
	int m_result = DeletePersonInfo(strIP,strSession,strFacedb,strPersonid);
	if(m_result != 0)
	{
		return m_result;
	}
	return 0;
}
//通过个人信息模糊删除个人信息
int CFASFaceManage::FASDeletePersonInfobyIdcard(char * pIP,char * pSession,char * pFacedb,char * pIdcard)
{	
	CString strIP;
	CString strSession;
	CString strFacedb;
	CString strIdCard;
	strIP.Format("%s",pIP);
	strSession.Format("%s",pSession);
	strFacedb.Format("%s",pFacedb);
	strIdCard.Format("%s",pIdcard);
	int m_result = DeletePersonInfobyIdcard(strIP,strSession,strFacedb,strIdCard);
	if(m_result != 0)
	{
		return m_result;
	}
	return 0;
}