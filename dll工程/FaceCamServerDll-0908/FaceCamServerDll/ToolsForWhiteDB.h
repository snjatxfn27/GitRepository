#pragma once
//白名单人脸库查询
int CheckWhite(CString strIP, CString strSession, CString strName, char*chJson, int jsonlen);
//白名单人脸库添加
int AddWhite(CString strIP, CString strSession, CString strName, char*chJson, int jsonlen);
//白名单人脸库修改
int EditWhite(CString strIP, CString strSession, CString strID, CString strName, char*chJson, int jsonlen);
//白名单人脸库删除
int DeleteWhite(CString strIP, CString strSession, CString strID, char*chJson, int jsonlen);