#pragma once

class __declspec(dllexport) CFaceAccessSys
{
public:
	CFaceAccessSys(void);
	~CFaceAccessSys(void);
	//�ӱ������ݿ�ѡ��⣬������ע�ᡣ
	int BatchInfoPublish(char * pLocalDbId,char * pLocalDbIp,char * pDbName,char * pDbUser,char * pDbPwd,char * pFaceDbIp,char * pFaceDbId,int * pi_Count_Success);
};