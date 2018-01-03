#pragma once
//黑名单人脸库查询
int CheckBlack(CString strIP, CString strSession, CString strName, char*chJson, int jsonlen);
//黑名单人脸库添加
int AddBlack(CString strIP, CString strSession, CString strName, char*chJson, int jsonlen);
//黑名单人脸库修改
int EditBlack(CString strIP, CString strSession, CString strID, CString strName, char*chJson, int jsonlen);
//黑名单人脸库删除
int DeleteBlack(CString strIP, CString strSession, CString strID, char*chJson, int jsonlen);