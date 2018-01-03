#include "stdafx.h"
#include "FaceRecAccessSys.h"
#include "FASCameraManage.h"

CFASCameraManage::CFASCameraManage()
{

}

CFASCameraManage::~CFASCameraManage()
{

}
//һ���������б��ѯ
int CFASCameraManage::FASCameraQuery(char * pIP,char * pSession,struct StructCameraList * pStructCameralist,int * pTotal_count,int * pList_size)
{	
	CString strIP;
	CString strSession;
	strIP.Format("%s",pIP);
	strSession.Format("%s",pSession);
	int m_result = CameraQuery(strIP,strSession,pStructCameralist,pTotal_count,pList_size);
	if(m_result != 0)
	{
		return m_result;
	}
	return 0;
}

//һ������һ�������
int CFASCameraManage::FASCameraAdd(char * pIP,char * pSession,char * pCName,int intCMode,char * pRtsp,char * pDbid,int intNodeid,int intFixedhost,int intPolicyid)
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
	int m_result = 	CameraAdd(strIP,strSession,strCName,intCMode,strRtsp,strDbid,intNodeid,intFixedhost,intPolicyid);
	if(m_result != 0)
	{
		return m_result;
	}
	return 0;
}
//һ���ɾ��һ�������
int CFASCameraManage::FASCameraDelete(char * pIP,char * pSession,char * pCameraId)
{	
	CString strIP;
	CString strSession;
	CString	strCameraId;
	strIP.Format("%s",pIP);
	strSession.Format("%s",pSession);
	strCameraId.Format("%s",pCameraId);
	int m_result = CameraDelete(strIP,strSession,pCameraId);
	if(m_result != 0)
	{
		return m_result;
	}
	return 0;
}
//һ�������һ�������
int CFASCameraManage::FASCameraConfig(char * pIP,char * pSession,char * pCameraId,char * pCName,int intCMode,char * pRtsp,char * pDbid,int intNodeid,int intFixedhost,int intPolicyid)
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
	int m_result = CameraConfig(strIP,strSession,pCameraId,strCName,intCMode,strRtsp,strDbid,intNodeid,intFixedhost,intPolicyid);
	if(m_result != 0)
	{
		return m_result;
	}
	return 0;
}

//һ���һ���������ʼ��������
int CFASCameraManage::FASCameraRecognitionStart(char * pIP,char * pSession,char * pCameraId)
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
int CFASCameraManage::FASCameraRecognitionStop(char * pIP,char * pSession,char * pCameraId)
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