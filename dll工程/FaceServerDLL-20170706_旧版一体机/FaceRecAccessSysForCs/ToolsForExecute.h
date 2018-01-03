// ToolsForExecute.h: interface for the ToolsForExecute class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLSFOREXECUTE_H__64515A8C_B7DB_409D_8DE8_083D4CAE61B0__INCLUDED_)
#define AFX_TOOLSFOREXECUTE_H__64515A8C_B7DB_409D_8DE8_083D4CAE61B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//查询摄像机列表
int CameraQuery(CString strIP,CString strSession,struct StructCameraList * pStructCameralist,int * pTotal_count,int * pList_size);
//添加摄像机
int CameraAdd(CString strIP,CString strSession,CString strCName,int intCMode,CString strRtsp,CString strDbid,int intNodeid,int intFixedhost,int intPolicyid);
//删除摄像机
int CameraDelete(CString strIP,CString strSession,CString strCamereId);
//配置摄像机
int CameraConfig(CString strIP,CString strSession,CString strCamereId,CString strCName,int intCMode,CString strRtsp,CString strDbid,int intNodeid,int intFixedhost,int intPolicyid);
//开始人脸布控
int CameraRecognitionStart(CString strIP,CString strSession,CString strCamereId);
//停止人脸布控
int CameraRecognitionStop(CString strIP,CString strSession,CString strCamereId);
#endif // !defined(AFX_TOOLSFOREXECUTE_H__64515A8C_B7DB_409D_8DE8_083D4CAE61B0__INCLUDED_)
