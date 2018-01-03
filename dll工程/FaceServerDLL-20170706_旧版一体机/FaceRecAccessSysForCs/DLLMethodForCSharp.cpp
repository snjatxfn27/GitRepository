// DLLMethodForCSharp.cpp: implementation of the DLLMethodForCSharp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FaceRecAccessSys.h"
#include "DLLMethodForCSharp.h"
#include "ToolsForFaceManage.h"
#include "ToolsForRecognition.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
/****************************************************
��¼�ǳ�
*****************************************************/
// ��¼һ���
extern "C" __declspec(dllexport) int  DeviceLogin(char * pIP,char * pSession)
{
	int m_result = Login(pIP,pSession);
	return m_result;
}
// �ǳ�һ���
extern "C" __declspec(dllexport) int  DeviceLogout(char * pIP,char * pSession)
{
	int m_result = Logout(pIP,pSession);
	return m_result;
}
////////////////////////////////////////////////////////////////////////////////////
/****************************************************
���������
*****************************************************/
//һ���������б��ѯ
extern "C" __declspec(dllexport) int FASCameraQuery(char * pIP,char * pSession,StructCameraList * pStructCameralist,int * pTotal_count,int * pList_size)
{	

	CString strIP;
	CString strSession;
	strIP.Format("%s",pIP);
	strSession.Format("%s",pSession);
	int m_result = CameraQuery(strIP,strSession, pStructCameralist,pTotal_count,pList_size);
	if(m_result != 0)
	{
		return m_result;
	}
	return 0;
}

//һ������һ�������
extern "C" __declspec(dllexport) int FASCameraAdd(char * pIP,char * pSession,char * pCName,int intCMode,char * pRtsp,char * pDbid,int intNodeid,int intFixedhost,int intPolicyid)
{	
	CString strIP;
	CString strSession;
	CString	strCName;
	CString	strRtsp;
	CString	strDbid;
	strIP.Format("%s",pIP);
	strSession.Format("%s",pSession);
	strCName.Format("%s",pCName);
	strRtsp.Format("%s",pRtsp);
	strDbid.Format("%s",pDbid);
	CameraAdd(strIP,strSession,strCName,intCMode,strRtsp,strDbid,intNodeid,intFixedhost,intPolicyid);
	return 0;
}
//һ���ɾ��һ�������
extern "C" __declspec(dllexport) int FASCameraDelete(char * pIP,char * pSession,char * pCameraId)
{	
	CString strIP;
	CString strSession;
	CString	strCameraId;
	strIP.Format("%s",pIP);
	strSession.Format("%s",pSession);
	strCameraId.Format("%s",pCameraId);
	CameraDelete(strIP,strSession,pCameraId);
	return 0;
}
//һ�������һ�������
extern "C" __declspec(dllexport) int FASCameraConfig(char * pIP,char * pSession,char * pCameraId,char * pCName,int intCMode,char * pRtsp,char * pDbid,int intNodeid,int intFixedhost,int intPolicyid)
{	
	CString strIP;
	CString strSession;
	CString	strCameraId;
	CString	strCName;
	CString	strRtsp;
	CString	strDbid;
	strIP.Format("%s",pIP);
	strSession.Format("%s",pSession);
	strCameraId.Format("%s",pCameraId);
	strCName.Format("%s",pCName);
	strRtsp.Format("%s",pRtsp);
	strDbid.Format("%s",pDbid);
	CameraConfig(strIP,strSession,pCameraId,strCName,intCMode,strRtsp,strDbid,intNodeid,intFixedhost,intPolicyid);
	return 0;
}

//һ���һ���������ʼ��������
extern "C" __declspec(dllexport) int FASCameraRecognitionStart(char * pIP,char * pSession,char * pCameraId)
{	
	CString strIP;
	CString strSession;
	CString	strCameraId;
	strIP.Format("%s",pIP);
	strSession.Format("%s",pSession);
	strCameraId.Format("%s",pCameraId);
	int m_result = CameraRecognitionStart(strIP,strSession,pCameraId);
	if(m_result != 0)
	{
		return m_result;
	}
	return 0;
}

//һ���һ�������ֹͣ��������
extern "C" __declspec(dllexport) int FASCameraRecognitionStop(char * pIP,char * pSession,char * pCameraId)
{	
	CString strIP;
	CString strSession;
	CString	strCameraId;
	strIP.Format("%s",pIP);
	strSession.Format("%s",pSession);
	strCameraId.Format("%s",pCameraId);
	int m_result = CameraRecognitionStop(strIP,strSession,pCameraId);
	if(m_result != 0)
	{
		return m_result;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////
/****************************************************
���Թ���
*****************************************************/
//�����б��ѯ
extern "C" __declspec(dllexport) int FASPolicyQuery(char * pIP,char * pSession,char * pPolicy_id,struct StructPolicyList * pStructPolicylist,int * pmt_policy_num)
{	
	CString strIP;
	CString strSession;
	CString strPolicy_id;
	strIP.Format("%s",pIP);
	strSession.Format("%s",pSession);
	strPolicy_id.Format("%s",pPolicy_id);
	PolicyQuery(strIP,strSession,strPolicy_id,pStructPolicylist,pmt_policy_num);
	return 0;
}

//���һ������
extern "C" __declspec(dllexport) int FASPolicyAdd(char * pIP,char * pSession,char * pPolicyname,int intmonitor_type,char * palarm_type,int inttime_schema_id,int intsimilarity,int intsimilarity2,int intsimilarity3,int intmax_return,int intscene_saved,int intmin_pixel,int intmax_face_num,int intzoom_times,int intstorage_day, int intbest_face,int intspeed_mode,int intface_adjust,int intmin_point,int intskip_dup,int intskip_time)
{	
	CString strIP;
	CString strSession;
	CString strPolicyname;
	CString strPolicyalarm_type;
	strIP.Format("%s",pIP);
	strSession.Format("%s",pSession);
	strPolicyname.Format("%s",pPolicyname);
	strPolicyalarm_type.Format("%s",palarm_type);
	PolicyAdd(strIP,strSession,strPolicyname,intmonitor_type,strPolicyalarm_type,inttime_schema_id,intsimilarity,intsimilarity2,intsimilarity3,intmax_return,intscene_saved,intmin_pixel,intmax_face_num,intzoom_times,intstorage_day,intbest_face,intspeed_mode,intface_adjust,intmin_point,intskip_dup,intskip_time);
	return 0;
}

//�޸�һ������
extern "C" __declspec(dllexport) int FASPolicyModify(char * pIP,char * pSession,char * pMt_policy_id,char * pPolicyname,int intmonitor_type,char * palarm_type,int inttime_schema_id,int intsimilarity,int intsimilarity2,int intsimilarity3,int intmax_return,int intscene_saved,int intmin_pixel,int intmax_face_num,int intzoom_times,int intstorage_day, int intbest_face,int intspeed_mode,int intface_adjust,int intmin_point,int intskip_dup,int intskip_time)
{	
	CString strIP;
	CString strSession;
	CString strMt_policy_id;
	CString strPolicyname;
	CString strPolicyalarm_type;
	strIP.Format("%s",pIP);
	strSession.Format("%s",pSession);
	strPolicyname.Format("%s",pPolicyname);
	strPolicyalarm_type.Format("%s",palarm_type);
	strMt_policy_id.Format("%s",pMt_policy_id);
	PolicyModify(strIP,strSession,strMt_policy_id,strPolicyname,intmonitor_type,strPolicyalarm_type,inttime_schema_id,intsimilarity,intsimilarity2,intsimilarity3,intmax_return,intscene_saved,intmin_pixel,intmax_face_num,intzoom_times,intstorage_day,intbest_face,intspeed_mode,intface_adjust,intmin_point,intskip_dup,intskip_time);
	return 0;
}

//ɾ��һ������
extern "C" __declspec(dllexport) int FASPolicyDelete(char * pIP,char * pSession,char * pMt_policy_id)
{	
	CString strIP;
	CString strSession;
	CString strMt_policy_id;
	CString strPolicyname;
	CString strPolicyalarm_type;
	strIP.Format("%s",pIP);
	strSession.Format("%s",pSession);
	strMt_policy_id.Format("%s",pMt_policy_id);
	PolicyDelete(strIP,strSession,strMt_policy_id);
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////
/****************************************************
���������
*****************************************************/
//һ����������б��ѯ
extern "C" __declspec(dllexport) int FASFaceDBQuery(char * pIP,char * pSession,struct StructDblist * pStructDblist,int * pTotal_count,int * pList_size)
{	
	// 	CString strTotalcount = Login(pIP);
	// 	pTotalcount = (LPSTR)(LPCTSTR)strTotalcount;
	CString strIP;
	CString strSession;
	strIP.Format("%s",pIP);
	strSession.Format("%s",pSession);
	FaceDBQuery(strIP,strSession,pStructDblist,pTotal_count,pList_size);
	return 0;
}

//һ�������������
extern "C" __declspec(dllexport) int FASFaceDBCreate(char * pIP,char * pSession,char * pDbName,struct StructDbCreateReturn * pStructDbCreateReturn)
{
	CString strIp;
	CString strSession;
	CString strDbName;
	strIp.Format("%s",pIP);
	strSession.Format("%s",pSession);
	strDbName.Format("%s",pDbName);
	CString strRet = FaceDBCreate(strIp,strSession,strDbName,pStructDbCreateReturn);
	if (strRet=="2002")
	{
		strDbName = "���ݿ������ظ���";
		strcpy(pDbName,strDbName);
	}
	strcpy(pDbName,strRet);
	return 0;
}

//һ���ɾ�������⣨�����ƾ�ȷƥ�䣩
extern "C" __declspec(dllexport) int FASFaceDBDelete(char * pIP,char * pSession,char * pDbName)
{
	CString strIp;
	CString strSession;
	CString strDbName;
	strIp.Format("%s",pIP);
	strSession.Format("%s",pSession);
	strDbName.Format("%s",pDbName);
	CString strRet = FaceDBDelete(strIp,strSession,strDbName);
	if (strRet=="2002")
	{
		strDbName = "���ݿ������ظ���";
		strcpy(pDbName,strDbName);
	}
	strcpy(pDbName,strRet);
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////
/****************************************************
����������Ϣ����
*****************************************************/

//��������
extern "C" __declspec(dllexport) int FASAddPersonInfo(char * pIP,char * pSession,char * pFacedb,char * pName,char * pSex,char *pBirth,char * pNativeplace,char * pPhone,char * pAddress,char * pIdCard,char *pPersonid)
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

//�޸�����
extern "C" __declspec(dllexport) int FASUpdatePersonInfo(char * pIP,char * pSession,char * pFacedb,char * pPersonid,char * pName,char * pSex,char *pBirth,char * pNativeplace,char * pPhone,char * pAddress,char * pIdCard)
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
//�޸�����
extern "C" __declspec(dllexport) int FASAddFaceInfo(char * pIP,char * pSession,char * pEImage,char *EFeature,char * pFacedb,char * pPersonid)
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
//ɾ��һ������Ϣ
extern "C" __declspec(dllexport) int FASDeletePersonInfo(char * pIP,char * pSession,char * pFacedb,char * pPersonid)
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
//ͨ��������Ϣģ��ɾ��������Ϣ
extern "C" __declspec(dllexport) int FASDeletePersonInfobyIdcard(char * pIP,char * pSession,char * pFacedb,char * pIdcard)
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
////////////////////////////////////////////////////////////////////////////////////
/****************************************************
����ʶ��
*****************************************************/

//������ȡ����
extern "C" __declspec(dllexport) int FASFeatureDetect(char * pIP,char * pSession,char * pImage,char *pEImage,char *pEFeature)
{	
	CString strIP;
	CString strSession;
	CString strImage;
	strIP.Format("%s",pIP);
	strSession.Format("%s",pSession);
	strImage.Format("%s",pImage);
	int m_result = FeatureDetect(strIP,strSession,strImage,pEImage,pEFeature);
	if(m_result != 0)
	{
		return m_result;
	}
	return 0;
}

//����ʶ��
extern "C" __declspec(dllexport) int FASImgRecongnition(char * pIP,char * pSession,char * pFacedb,char *pImage,char *pTop,char * pSimilarity, int * plistsize,struct StructRecognitionList * pStructRecognitionlist)
{	
	CString strIP;
	CString strSession;
	CString strFacedb;
	CString strImage;
	CString strTop;
	CString strSimilarity;
	
	strIP.Format("%s",pIP);
	strSession.Format("%s",pSession);
	strFacedb.Format("%s",pFacedb);
	strImage.Format("%s",pImage);
	strTop.Format("%s",pTop);
	strSimilarity.Format("%s",pSimilarity);
	int m_result = ImgRecognition(strIP,strSession,strFacedb,strImage,strTop,strSimilarity,plistsize,pStructRecognitionlist);
	if(m_result != 0)
	{
		return m_result;
	}
	return 0;
}

//����ʶ��FASFeatureMatching
extern "C" __declspec(dllexport) int FASFeatureMatching(char * pIP,char * pSession,char * pFacedb,char *pFeature,char *pTop,char * pSimilarity, int * plistsize,struct StructRecognitionList * pStructRecognitionlist)
{	
	CString strIP;
	CString strSession;
	CString strFacedb;
	CString strFeature;
	CString strTop;
	CString strSimilarity;
	
	strIP.Format("%s",pIP);
	strSession.Format("%s",pSession);
	strFacedb.Format("%s",pFacedb);
	strFeature.Format("%s",pFeature);
	strTop.Format("%s",pTop);
	strSimilarity.Format("%s",pSimilarity);
	int m_result = FeatureMatching(strIP,strSession,strFacedb,strFeature,strTop,strSimilarity,plistsize,pStructRecognitionlist);
	if(m_result != 0)
	{
		return m_result;
	}
	return 0;
}

//�����Ա�ʶ��
extern "C" __declspec(dllexport) int FASImgAgeSexAnalysis(char * pIP,char * pSession,struct StructRecognitionList * pStructRecognitionlist,char * pImage)
{	
	CString strIP;
	CString strSession;
	CString strImage;
	strIP.Format("%s",pIP);
	strSession.Format("%s",pSession);
	strImage.Format("%s",pImage);
	int m_result = ImgAgeSexAnalysis(strIP,strSession,pStructRecognitionlist,strImage);
	if(m_result != 0)
	{
		return m_result;
	}
	return 0;
}
