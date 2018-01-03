#include "stdafx.h"
#include "FaceRecAccessSys.h"
#include "FASLoginOrLogout.h"


CFASLoginOrLogout::CFASLoginOrLogout()
{

}

CFASLoginOrLogout::~CFASLoginOrLogout()
{

}
// ��¼һ���
int CFASLoginOrLogout:: DeviceLogin(char * pIP,char * pSession)
{
	int m_result = Login(pIP,pSession);
	if(m_result != 0)
	{
		TRACE("login fault!\n");
		return m_result;
	}
	return 0;
}
// �ǳ�һ���
int CFASLoginOrLogout:: DeviceLogout(char * pIP,char * pSession)
{
	int m_result = Logout(pIP,pSession);
	if(m_result != 0)
	{
		TRACE("logout fault!\n");
		return m_result;
	}
	return 0;
}