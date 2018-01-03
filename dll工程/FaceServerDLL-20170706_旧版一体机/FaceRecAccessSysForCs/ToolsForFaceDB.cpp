// ToolsForFaceDB.cpp: implementation of the ToolsForFaceDB class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FaceRecAccessSys.h"
#include "ToolsForFaceDB.h"
#include "json/json.h"
#include "FASStatusQuery.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//一体机人脸库列表查询
int FaceDBQuery(CString strIP,CString strSession,struct StructDblist * pStructDblist,int * pTotal_count,int * pList_size)
{
	CString strContent;
	strContent.Format("{}");
	int iContentLen = strContent.GetLength();
	CString strContentLen;
	strContentLen.Format("%d",iContentLen);
	CString strSend;
	strSend.Format("%s%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s%s%s",
		"GET /facedb?skip=0&top=300 HTTP/1.1\n",
		"Date: Mon, 21 Mar 2016 11:30:17 GMT\n",
		"Accept: */*\n",
		"Host: ",strIP,"\n",
		"Content-Length: ",iContentLen,"\n",
		"Accept-Language: zh-cn\n",
		"User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1)\n",
		"Pragma: no-cache\n",
		"Cookie: facecloud_session=",strSession,":root\n",
		"Connection: close\n\n",
		strContent,"\n");
	SOCKET sockClient=socket(AF_INET,SOCK_STREAM,0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr=inet_addr(strIP);//该处地址应为空闲一体机地址
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(8000);
	connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
	int sendlen = strSend.GetLength();
	int re = send(sockClient,strSend.GetBuffer(strSend.GetLength()),strSend.GetLength(),0);
	TRACE("send %d / %d Byte\n",re,sendlen);
	char revbuf[1000*1000];
	memset(revbuf,0,1000*1000);
	re = recv(sockClient,revbuf,1000*1000,0);
	closesocket(sockClient);
	CString strRecv = revbuf;

//	截取字符串
// 	int iStart(0),iMid(0),iEnd(0);
// 	iStart = strRecv.Find("\"ret\":");
// 	iStart = iStart + 7;
// 	iEnd = strRecv.Find(",",iStart);
// 	CString strRet = strRecv.Mid(iStart,iEnd-iStart);
// 	if(strRet != "0")
// 		return "FAULT";
// 	iStart = strRecv.Find("total_count");
// 	iStart = iStart + 14;
// 	iEnd = strRecv.Find(",",iStart);
// 	CString strTotalcount = strRecv.Mid(iStart,iEnd-iStart);
// 	TRACE("Totalcount = %s\n",strTotalcount);
	//解析json结构
	int strJsonstart = strRecv.Find("{",0);
	strRecv.Delete(0,strJsonstart);
	Json::Reader reader;
	Json::Value root_Value,facedb_list_Value;
	BOOL readresult = reader.parse(strRecv.GetBuffer(strRecv.GetLength()),root_Value);
	if (!readresult)
	{
		return -1;
	}
	int ret = root_Value["ret"].asInt();
	if (ret!=0)
	{
		return ret;
	}
	* pTotal_count = root_Value["total_count"].asInt();
	* pList_size = root_Value["list_size"].asInt();
	//char aTotalCount[20];
	//char aListsize[20];
	//itoa(intTotal_count, aTotalCount, 10);//int2char*
	//itoa(intList_size, aListsize, 10);//int2char*
	CString strDbid;
	CString strDbname,strDbname_encode;
	CString strDburl;
	int i = 0;
	while(TRUE)
	{
	facedb_list_Value = root_Value["facedb_list"][i];
	if (facedb_list_Value.isNull())
	{
		break;
	}
	strDbid = facedb_list_Value["db_id"].asCString();
	strDbname = facedb_list_Value["name"].asCString();
	strDbname_encode = UTF8ToGB2312(strDbname.GetBuffer(strDbname.GetLength()));
	strDburl = facedb_list_Value["url"].asCString();
	strcpy(pStructDblist[i].db_id,strDbid);
	strcpy(pStructDblist[i].name,strDbname_encode);
	strcpy(pStructDblist[i].url,strDburl);
	i++;
	}
	return 0;
}
//指定ID的人脸库详细详细信息查询
int GetOneFaceDBInfo(CString strIP,CString strSession,CString strDbid,struct StructDbInfo * pStructDbinfo)
{
	CString strContent;
	strContent.Format("{}");
	int iContentLen = strContent.GetLength();
	CString strContentLen;
	strContentLen.Format("%d",iContentLen);
	CString strSend;
	strSend.Format("%s%s%s%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s%s%s",
		"GET /facedb/",strDbid," HTTP/1.1\n",
		"Date: Mon, 21 Mar 2016 11:30:17 GMT\n",
		"Accept: */*\n",
		"Host: ",strIP,"\n",
		"Content-Length: ",iContentLen,"\n",
		"Accept-Language: zh-cn\n",
		"User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1)\n",
		"Pragma: no-cache\n",
		"Cookie: facecloud_session=",strSession,":root\n",
		"Connection: close\n\n",
		strContent,"\n");
	TRACE(strSend);
	SOCKET sockClient=socket(AF_INET,SOCK_STREAM,0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr=inet_addr(strIP);//该处地址应为空闲一体机地址
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(8000);
	connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
	int sendlen = strSend.GetLength();
	int re = send(sockClient,strSend.GetBuffer(strSend.GetLength()),strSend.GetLength(),0);
	TRACE("send %d / %d Byte\n",re,sendlen);
	char revbuf[1000];
	memset(revbuf,0,1000);
	re = recv(sockClient,revbuf,1000,0);
	closesocket(sockClient);
	CString strRecv = revbuf;
	//解析json结构
	int strJsonstart = strRecv.Find("{",0);
	strRecv.Delete(0,strJsonstart);
	Json::Reader reader;
	Json::Value root_Value;
	BOOL readresult = reader.parse(strRecv.GetBuffer(strRecv.GetLength()),root_Value);
	if (!readresult)
	{
		return -1;
	}
	int ret = root_Value["ret"].asInt();
	if (ret!=0)
	{
		return ret;
	}
	CString m_strDbid;
	CString strDbname,strDbname_encode;
	CString strDbcreate_date;
	CString strDbuser;
	CString strDbsimilarity;
	CString strDbstate;
	CString strDbperson_count;
	CString strDbface_count;
	CString strDbinst_num;
	m_strDbid = root_Value["facedb_data"]["db_id"].asCString();
	strDbname = root_Value["facedb_data"]["name"].asCString();
	strDbname_encode = UTF8ToGB2312(strDbname.GetBuffer(strDbname.GetLength()));
	strDbcreate_date = root_Value["facedb_data"]["create_date"].asCString();
	strDbuser = root_Value["facedb_data"]["user"].asCString();
	strDbsimilarity.Format("%d",root_Value["facedb_data"]["similarity"].asInt());
	strDbstate = root_Value["facedb_data"]["state"].asCString();
	strDbperson_count.Format("%d",root_Value["facedb_data"]["person_count"].asInt());
	strDbface_count.Format("%d",root_Value["facedb_data"]["face_count"].asInt());
	strDbinst_num.Format("%d",root_Value["facedb_data"]["inst_num"].asInt());
	strcpy(pStructDbinfo->db_id,m_strDbid);
	strcpy(pStructDbinfo[0].name,strDbname_encode);
	strcpy(pStructDbinfo[0].create_date,strDbcreate_date);
	strcpy(pStructDbinfo[0].user,strDbuser);
	strcpy(pStructDbinfo[0].similarity,strDbsimilarity);
	strcpy(pStructDbinfo[0].state,strDbstate);
	strcpy(pStructDbinfo[0].person_count,strDbperson_count);
	strcpy(pStructDbinfo[0].face_count,strDbface_count);
	strcpy(pStructDbinfo[0].inst_num,strDbinst_num);
	return 0;
}
//一体机创建人脸库
int FaceDBCreate(CString strIP,CString strSession,CString strDbName,struct StructDbCreateReturn * pStructDbCreateReturn)
{
	CString strDbName_encode = GB2312ToUTF8(strDbName.GetBuffer(strDbName.GetLength()));
	CString strContent;
	strContent.Format("{}");
	int iContentLen = strContent.GetLength();
	CString strContentLen;
	strContentLen.Format("%d",iContentLen);
	CString strSend;
	strSend.Format("%s%s%s%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s%s%s",
		"PUT /facedb?dbname=",strDbName_encode,"&inst_num=3&state=ignore&similarity=20 HTTP/1.1\n",
		"Date: Mon, 21 Mar 2016 11:30:17 GMT\n",
		"Accept: */*\n",
		"Host: ",strIP,"\n",
		"Content-Length: ",iContentLen,"\n",
		"Accept-Language: zh-cn\n",
		"User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1)\n",
		"Pragma: no-cache\n",
		"Cookie: facecloud_session=",strSession,":root\n",
		"Connection: close\n\n",
		strContent,"\n");
//	TRACE(strSend);
	SOCKET sockClient=socket(AF_INET,SOCK_STREAM,0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr=inet_addr(strIP);//该处地址应为空闲一体机地址
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(8000);
	connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
	int sendlen = strSend.GetLength();
	int re = send(sockClient,strSend.GetBuffer(strSend.GetLength()),strSend.GetLength(),0);
	TRACE("send %d / %d Byte\n",re,sendlen);
	char revbuf[1000];
	memset(revbuf,0,1000);
	re = recv(sockClient,revbuf,1000,0);
	closesocket(sockClient);
	CString strRecv = revbuf;
	//解析json结构
	int strJsonstart = strRecv.Find("{",0);
	strRecv.Delete(0,strJsonstart);
	Json::Reader reader;
	Json::Value root_Value;
	BOOL readresult = reader.parse(strRecv.GetBuffer(strRecv.GetLength()),root_Value);
	if (!readresult)
	{
		return -1;
	}
	int intRet = root_Value["ret"].asInt();
	if (intRet!=0)
	{
		//strRet.Format("%d",intRet);
		//return strRet;
		return intRet;
	}
	CString strFacedData_id = root_Value["facedb_data"]["db_id"].asCString();
	CString strFacedData_name = root_Value["facedb_data"]["name"].asCString();
	CString strFacedData_name_encode = UTF8ToGB2312(strFacedData_name.GetBuffer(strFacedData_name.GetLength()));
	CString strFacedData_createdate = root_Value["facedb_data"]["create_date"].asCString();
	CString strFacedData_user = root_Value["facedb_data"]["user"].asCString();
	CString strFacedData_state = root_Value["facedb_data"]["state"].asCString();
	CString strFacedData_similarity;
	CString strFacedData_person_count;
	CString strFacedData_face_count;
	CString strFacedData_inst_num;
	strFacedData_similarity.Format("%d",root_Value["facedb_data"]["similarity"].asInt());
	strFacedData_person_count.Format("%d",root_Value["facedb_data"]["person_count"].asInt());
	strFacedData_face_count.Format("%d",root_Value["facedb_data"]["face_count"].asInt());
	strFacedData_inst_num.Format("%d",root_Value["facedb_data"]["inst_num"].asInt());
	strcpy(pStructDbCreateReturn[0].pFacedData_id,strFacedData_id);
    strcpy(pStructDbCreateReturn->pFacedData_name,strFacedData_name_encode);
	strcpy(pStructDbCreateReturn->pFacedData_createdate,strFacedData_createdate);
	strcpy(pStructDbCreateReturn->pFacedData_user,strFacedData_user);
	strcpy(pStructDbCreateReturn->pFacedData_state,strFacedData_state);
	strcpy(pStructDbCreateReturn->pFacedData_similarity,strFacedData_similarity);
    strcpy(pStructDbCreateReturn->pFacedData_person_count,strFacedData_person_count);
	strcpy(pStructDbCreateReturn->pFacedData_face_count,strFacedData_face_count);
	strcpy(pStructDbCreateReturn->pFacedData_inst_num,strFacedData_inst_num);
	return 0;
}

//一体机删除一个指定名称的人脸库
int FaceDBDelete(CString strIP,CString strSession,CString strDbName)
{
	CString strDbName_encode = GB2312ToUTF8(strDbName.GetBuffer(strDbName.GetLength()));
	CString strContent;
	strContent.Format("{}");
	int iContentLen = strContent.GetLength();
	CString strContentLen;
	strContentLen.Format("%d",iContentLen);
	CString strSend;
	strSend.Format("%s%s%s%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s%s%s",
		"DELETE /facedb?dbname=",strDbName_encode,"&skip=0&top=1 HTTP/1.1\n",
		"Date: Mon, 21 Mar 2016 11:30:17 GMT\n",
		"Accept: */*\n",
		"Host: ",strIP,"\n",
		"Content-Length: ",iContentLen,"\n",
		"Accept-Language: zh-cn\n",
		"User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1)\n",
		"Pragma: no-cache\n",
		"Cookie: facecloud_session=",strSession,":root\n",
		"Connection: close\n\n",
		strContent,"\n");
//	TRACE(strSend);
	SOCKET sockClient=socket(AF_INET,SOCK_STREAM,0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr=inet_addr(strIP);//该处地址应为空闲一体机地址
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(8000);
	connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
	int sendlen = strSend.GetLength();
	int re = send(sockClient,strSend.GetBuffer(strSend.GetLength()),strSend.GetLength(),0);
	TRACE("send %d / %d Byte\n",re,sendlen);
	char revbuf[1000];
	memset(revbuf,0,1000);
	re = recv(sockClient,revbuf,1000,0);
	closesocket(sockClient);
	CString strRecv = revbuf;
	//解析json结构
	int strJsonstart = strRecv.Find("{",0);
	strRecv.Delete(0,strJsonstart);
	Json::Reader reader;
	Json::Value root_Value,facedb_list_Value;
	BOOL readresult = reader.parse(strRecv.GetBuffer(strRecv.GetLength()),root_Value);
	if (!readresult)
	{
		return -1;
	}
	int intRet = root_Value["ret"].asInt();
	if (intRet!=0)
	{	
		//itoa(intRet,(LPSTR)(LPCTSTR)strRet,10);
		//return strRet;
		return intRet;
	}
	int intTotal_count = root_Value["total_count"].asInt();
	int intList_size = root_Value["list_size"].asInt();
	char aTotalCount[20];
	char aListsize[20];
	itoa(intTotal_count, aTotalCount, 10);
	itoa(intList_size, aListsize, 10);
	CString strDbid;
	CString strDbname,strDbname_encode;
	CString strDburl;
	int i = 0;
	while(TRUE)
	{
		facedb_list_Value = root_Value["facedb_list"][i];
		if (facedb_list_Value.isNull())
		{
			break;
		}
		strDbid = facedb_list_Value["db_id"].asCString();
		strDbname = facedb_list_Value["name"].asCString();
		strDbname_encode = UTF8ToGB2312(strDbname.GetBuffer(strDbname.GetLength()));
		strDburl = facedb_list_Value["url"].asCString();
	i++;
	}
	return 0;
}