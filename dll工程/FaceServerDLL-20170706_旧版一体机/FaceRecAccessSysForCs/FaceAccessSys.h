#pragma once

class __declspec(dllexport) CFaceAccessSys
{
public:
	CFaceAccessSys(void);
	~CFaceAccessSys(void);
	//从本地数据库选择库，并批量注册。
	int BatchInfoPublish(char * pLocalDbId,char * pLocalDbIp,char * pDbName,char * pDbUser,char * pDbPwd,char * pFaceDbIp,char * pFaceDbId,int * pi_Count_Success);
};