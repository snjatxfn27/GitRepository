// ToolsForExecute.h: interface for the ToolsForExecute class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLSFOREXECUTE_H__64515A8C_B7DB_409D_8DE8_083D4CAE61B0__INCLUDED_)
#define AFX_TOOLSFOREXECUTE_H__64515A8C_B7DB_409D_8DE8_083D4CAE61B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//��ѯ������б�
int CameraQuery(CString strIP,CString strSession,struct StructCameraList * pStructCameralist,int * pTotal_count,int * pList_size);
//��������
int CameraAdd(CString strIP,CString strSession,CString strCName,int intCMode,CString strRtsp,CString strDbid,int intNodeid,int intFixedhost,int intPolicyid);
//ɾ�������
int CameraDelete(CString strIP,CString strSession,CString strCamereId);
//���������
int CameraConfig(CString strIP,CString strSession,CString strCamereId,CString strCName,int intCMode,CString strRtsp,CString strDbid,int intNodeid,int intFixedhost,int intPolicyid);
//��ʼ��������
int CameraRecognitionStart(CString strIP,CString strSession,CString strCamereId);
//ֹͣ��������
int CameraRecognitionStop(CString strIP,CString strSession,CString strCamereId);
#endif // !defined(AFX_TOOLSFOREXECUTE_H__64515A8C_B7DB_409D_8DE8_083D4CAE61B0__INCLUDED_)
