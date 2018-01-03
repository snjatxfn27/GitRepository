

#include "stdafx.h"
#include "FaceRecAccessSys.h"
#include "FASStatusQuery.h"


CFASStatusQuery::CFASStatusQuery()
{

}

CFASStatusQuery::~CFASStatusQuery()
{

}
//一体机人脸库列表查询
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

//一体机创建人脸库
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

//一体机删除人脸库（库名称精确匹配）
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

//指定ID的人脸库详细详细信息查询
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
