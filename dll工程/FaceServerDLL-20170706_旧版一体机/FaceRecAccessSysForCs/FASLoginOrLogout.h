
#pragma once
class __declspec(dllexport) CFASLoginOrLogout  
{
public:
	CFASLoginOrLogout();
	~CFASLoginOrLogout();
	//��¼һ���
	int DeviceLogin(char * pIP,char * pSession);
	//�ǳ�һ���
	int DeviceLogout(char * pIP,char * pSession);
};
