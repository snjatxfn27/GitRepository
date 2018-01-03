// ToolsForFaceManage.cpp: implementation of the ToolsForFaceManage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FaceRecAccessSys.h"
#include "ToolsForFaceManage.h"
#include "json/json.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
//创建人信息 在person表中增加个人信息。
int AddPersonInfo(CString strIP,CString strSession,CString facedb,CString strName,CString strSex, CString strBirth,CString strNativeplace,CString strPhone,CString strAddress,CString strIdCard,char* pPersonid)
{
	CString strBody,strSend;
	CString strAddr_encode = GB2312ToUTF8(strAddress.GetBuffer(strAddress.GetLength()));
	CString strName_encode = GB2312ToUTF8(strName.GetBuffer(strName.GetLength()));
	CString strNativeplace_encode = GB2312ToUTF8(strNativeplace.GetBuffer(strNativeplace.GetLength()));
	strBody.Format("{}");
	int iBodylen = strBody.GetLength();
	strSend.Format("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s%s",
		"PUT /facedb/",facedb,"/persons?name=",strIdCard,"&id_card=",strIdCard,"&card_type=128&sex=",strSex,"&birth=",strBirth,"&phone=",strPhone,"&native_place=",strNativeplace_encode,"&addr=",strName_encode," HTTP/1.1\n",
		"Accept: */*\n",
		"Host:",strIP,"\n",
		"Content-Length:",iBodylen,"\n",
		"Accept-Language: zh-cn\n",
		"User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1)\n",
		"Pragma: no-cache\n",
		"Cookie: facecloud_session=",strSession,":root\n",
		"Connection: close\n\n",
		strBody
		);
	SOCKET sockClient=socket(AF_INET,SOCK_STREAM,0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr=inet_addr(strIP);//该处地址应为空闲一体机地址
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(8000);
	connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
	int sendlen = strSend.GetLength();
	int re = send(sockClient,strSend.GetBuffer(strSend.GetLength()),strSend.GetLength(),0);
	char revbuf[1000];
	memset(revbuf,0,1000);
	re = recv(sockClient,revbuf,1000,0);
	closesocket(sockClient);
	CString strRecv = revbuf;
	//解析json结构
	int strJsonstart = strRecv.Find("{",0);
	strRecv.Delete(0,strJsonstart);
	Json::Reader reader;
	Json::Value root_Value,Camera_list_Value;
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
	int person_begin = strRecv.Find("\"person_data\":");
	person_begin = strRecv.Find("\"person_id\":",person_begin);
	int person_end = strRecv.Find("\",",person_begin + 14);
	CString person_id = strRecv.Mid(person_begin+14,person_end-person_begin-14);
	strcpy(pPersonid,person_id);
	return 0;
}

//修改个人信息 在person表中修改个人信息。
int UpdatePersonInfo(CString strIP,CString strSession,CString facedb,CString personid,CString strName,CString strSex, CString strBirth,CString strNativeplace,CString strPhone,CString strAddress,CString strIdCard)
{

	CString strBody,strSend;
	CString strAddr_encode = GB2312ToUTF8(strAddress.GetBuffer(strAddress.GetLength()));
	CString strName_encode = GB2312ToUTF8(strName.GetBuffer(strName.GetLength()));
	CString strNativeplace_encode = GB2312ToUTF8(strNativeplace.GetBuffer(strNativeplace.GetLength()));
	strBody.Format("{}");
	int iBodylen = strBody.GetLength();
	strSend.Format("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s%s",
		"PUT /facedb/",facedb,"/persons/",personid,"?ops=post&name=",strIdCard,"&id_card=",strIdCard,"&card_type=128&sex=",strSex,"&birth=",strBirth,"&phone=",strPhone,"&native_place=",strNativeplace_encode,"&addr=",strName_encode," HTTP/1.1\n",
		"Accept: */*\n",
		"Host:",strIP,"\n",
		"Content-Length:",iBodylen,"\n",
		"Accept-Language: zh-cn\n",
		"User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1)\n",
		"Pragma: no-cache\n",
		"Cookie: facecloud_session=",strSession,":root\n",
		"Connection: close\n\n",
		strBody
		);
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
	Json::Value root_Value,Camera_list_Value;
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
	return 0;
}

//添加一个人脸到人脸库
int AddFaceInfo(CString strIP,CString strSession,CString strEImage,char *EFeature,CString facedb,CString personid)
{
//	strEImage = "/91000321";
	CString strBody, CFeature;
	CFeature = EFeature;
	strBody.Format("{\"image_data\":{\"type\":\"jpg\",\"content\":\"%s\"},\"feature_data\":{\"content\":\"%s\"},\"blur\":1}",strEImage,CFeature);
	int iBodylen = strBody.GetLength();
	CString strSend;
	strSend.Format("%s%s%s%s%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s%s",
		"PUT /facedb/",facedb,"/persons/",personid,"/faces HTTP/1.1\n",
		"Accept: */*\n",
		"Host:",strIP,"\n",
		"Content-Length: ",iBodylen,"\n",
		"Accept-Language: zh-cn\n",
		"User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1)\n",
		"Pragma: no-cache\n",
		"Cookie: facecloud_session=",strSession,":root\n",
		"Connection: close\n\n",
		strBody
		);
	SOCKET sockClient=socket(AF_INET,SOCK_STREAM,0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr=inet_addr(strIP);//该处地址应为空闲一体机地址
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(8000);
	connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
	int sendlen = strSend.GetLength();
	int re = send(sockClient,strSend.GetBuffer(strSend.GetLength()),strSend.GetLength(),0);
	char revbuf[1000*1000];
	memset(revbuf,0,1000*1000);
	re = recv(sockClient,revbuf,1000*1000,0);
	closesocket(sockClient);
	CString strRecv = revbuf;
	//解析json结构
	int strJsonstart = strRecv.Find("{",0);
	strRecv.Delete(0,strJsonstart);
	Json::Reader reader;
	Json::Value root_Value,Camera_list_Value;
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
	return 0;
}
//删除一个人信息
int DeletePersonInfo(CString strIP,CString strSession,CString facedb,CString personid)
{

	CString strBody,strSend;
	strBody.Format("{}");
	int iBodylen = strBody.GetLength();
	strSend.Format("%s%s%s%s%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s%s",
		"DELETE /facedb/",facedb,"/persons/",personid," HTTP/1.1\n",
		"Accept: */*\n",
		"Host:",strIP,"\n",
		"Content-Length:",iBodylen,"\n",
		"Accept-Language: zh-cn\n",
		"User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1)\n",
		"Pragma: no-cache\n",
		"Cookie: facecloud_session=",strSession,":root\n",
		"Connection: close\n\n",
		strBody
		);
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
	Json::Value root_Value,Camera_list_Value;
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
	return 0;
}

//通过个人信息模糊删除个人信息
int DeletePersonInfobyIdcard(CString strIP,CString strSession,CString facedb,CString strIdcard)
{

	CString strBody,strSend;
	strBody.Format("{}");
	int iBodylen = strBody.GetLength();
	strSend.Format("%s%s%s%s%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s%s",
		"DELETE /facedb/",facedb,"/persons?id_card=",strIdcard," HTTP/1.1\n",
		"Accept: */*\n",
		"Host:",strIP,"\n",
		"Content-Length:",iBodylen,"\n",
		"Accept-Language: zh-cn\n",
		"User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1)\n",
		"Pragma: no-cache\n",
		"Cookie: facecloud_session=",strSession,":root\n",
		"Connection: close\n\n",
		strBody
		);
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
	Json::Value root_Value,Camera_list_Value;
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
	return 0;
}