
#pragma once
class __declspec(dllexport) CFASLoginOrLogout  
{
public:
	CFASLoginOrLogout();
	~CFASLoginOrLogout();
	//登录一体机
	int DeviceLogin(char * pIP,char * pSession);
	//登出一体机
	int DeviceLogout(char * pIP,char * pSession);
};
