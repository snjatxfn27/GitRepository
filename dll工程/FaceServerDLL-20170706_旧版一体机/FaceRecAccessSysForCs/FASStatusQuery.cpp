

#include "stdafx.h"
#include "FaceRecAccessSys.h"
#include "FASStatusQuery.h"


CFASStatusQuery::CFASStatusQuery()
{

}

CFASStatusQuery::~CFASStatusQuery()
{

}
//һ����������б��ѯ
int CFASStatusQuery::FASFaceDBQuery(char * pIP,char * pSession,struct StructDblist * pStructDblist,int * pTotal_count,int * pList_size)
{	
// 	CString strTotalcount = Login(pIP);
// 	pTotalcount = (LPSTR)(LPCTSTR)strTotalcount;
	CString strIP;
	CString strSession;
	strIP.Format("%s",pIP);
	strSession.Format("%s",pSession);
	int m_result = FaceDBQuery(strIP,strSession,pStructDblist,pTotal_count,pList_size);
	if(m_result != 0)
	{
		return m_result;
	}
	return 0;
}

//һ�������������
int CFASStatusQuery::FASFaceDBCreate(char * pIP,char * pSession,char * pDbName,struct StructDbCreateReturn * pStructDbCreateReturn)
{
	CString strIp;
	CString strSession;
	CString strDbName;
	strIp.Format("%s",pIP);
	strSession.Format("%s",pSession);
	strDbName.Format("%s",pDbName);
	int m_result = FaceDBCreate(strIp,strSession,strDbName,pStructDbCreateReturn);
	if(m_result != 0)
	{
		return m_result;
	}
	return 0;
}

//һ���ɾ�������⣨�����ƾ�ȷƥ�䣩
int CFASStatusQuery::FASFaceDBDelete(char * pIP,char * pSession,char * pDbName)
{
	CString strIp;
	CString strSession;
	CString strDbName;
	strIp.Format("%s",pIP);
	strSession.Format("%s",pSession);
	strDbName.Format("%s",pDbName);
	int m_result = FaceDBDelete(strIp,strSession,strDbName);
	if(m_result != 0)
	{
		return m_result;
	}
	return 0;
}

//ָ��ID����������ϸ��ϸ��Ϣ��ѯ
int CFASStatusQuery::FASFaceDBInfo(char * pIP,char * pSession,char * pDbid,struct StructDbInfo * pStructDbinfo)
{
	CString strIp;
	CString strSession;
	CString strDbid;
	strIp.Format("%s",pIP);
	strSession.Format("%s",pSession);
	strDbid.Format("%s",pDbid);
	int m_result = GetOneFaceDBInfo(strIp,strSession,strDbid,pStructDbinfo);
	if(m_result != 0)
	{
		return m_result;
	}
	return 0;
}
