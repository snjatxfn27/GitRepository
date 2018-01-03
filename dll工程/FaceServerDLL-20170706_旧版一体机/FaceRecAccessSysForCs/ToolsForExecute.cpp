// ToolsForExecute.cpp: implementation of the ToolsForExecute class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FaceRecAccessSys.h"
#include "ToolsForExecute.h"
#include "json/json.h"
#include "FASCameraManage.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//一体机摄像机列表查询
int CameraQuery(CString strIP,CString strSession,struct StructCameraList * pStructCameralist,int * pTotal_count,int * pList_size)
{
	CString strContent;
	strContent.Format("{}");
	int iContentLen = strContent.GetLength();
	CString strContentLen;
	strContentLen.Format("%d",iContentLen);
	CString strSend;
	strSend.Format("%s%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s%s%s",
		"GET /rtmonitor/manage?&skip=0&top=300 HTTP/1.1\n",
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
	* pTotal_count = root_Value["total"].asInt();
	* pList_size = root_Value["list_size"].asInt();
	CString strCameraid;
	CString strCameraname;
	CString strCameraurl;
	CString strCameramode;
	CString strCamerastate;
	CString strCamerapolicyid;
	CString strCameraidlist;
	CString strCameranodeid;
	CString strCamerafixedhost;
	CString strCamerabelonghost;
	CString strCameramanctrl;
	CString strCamerainit;
	CString strCameraalarm;
	CString strCamerasnap;
	CString strCameralastcleartime;
	int i = 0;
	while(TRUE)
	{
	Camera_list_Value = root_Value["camera_list"][i];
	if (Camera_list_Value.isNull())
	{
		break;
	}
	strCameraid.Format("%d",Camera_list_Value["camera_id"].asInt());//摄像机Id
	strCameraname = Camera_list_Value["camera_name"].asCString();//摄像机name
	CString strCameraname_encode = UTF8ToGB2312(strCameraname.GetBuffer(strCameraname.GetLength()));
	strCameramode.Format("%d",Camera_list_Value["camera_mode"].asInt());//摄像机视频资源提供模式
	strCameraurl=Camera_list_Value["rtsp"].asCString();//rtsp 
	strCamerastate.Format("%d",Camera_list_Value["camera_state"].asInt());//
	strCamerapolicyid.Format("%d",Camera_list_Value["mt_policy_id"].asInt());
	strCameraidlist = Camera_list_Value["db_id_list"].asCString();//
	strCameranodeid.Format("%d",Camera_list_Value["node_id"].asInt());
	strCamerafixedhost.Format("%d",Camera_list_Value["camera_state"].asInt());
	strCamerabelonghost.Format("%d",Camera_list_Value["fixed_host"].asInt());//当前摄像机挂载服务器,运行时信息,不可修改
	strCameramanctrl.Format("%d",Camera_list_Value["man_ctrl"].asInt()); //0:自动模式1:手动模式,运行时信息,不可修改
	strCamerainit.Format("%d",Camera_list_Value["man_init_state"].asInt());//手动初始状态，含义同camera_state,运行时信息,不可修改
	strCameraalarm.Format("%d",Camera_list_Value["alarm_count"].asInt());//相机自上次清零后的告警统计数
    strCamerasnap.Format("%d",Camera_list_Value["snap_count"].asInt());//相机自上次清零后的抓拍数量
	strCameralastcleartime = Camera_list_Value["last_clear_time"].asCString();//2012/07/12 09:02:00" 上次清零时间
	strcpy(pStructCameralist[i].acamera_id,strCameraid);
	strcpy(pStructCameralist[i].aname,strCameraname_encode);
	strcpy(pStructCameralist[i].acameramode,strCameramode);
	strcpy(pStructCameralist[i].aurl,strCameraurl);
	strcpy(pStructCameralist[i].acamerastate,strCamerastate);
	strcpy(pStructCameralist[i].acamerapolicyid,strCamerapolicyid);
	strcpy(pStructCameralist[i].acameraidlist,strCameraidlist);
	strcpy(pStructCameralist[i].acameranodeid,strCameranodeid);
	strcpy(pStructCameralist[i].acamerafixedhost,strCamerafixedhost);
	strcpy(pStructCameralist[i].acamerabelonghost,strCamerabelonghost);
	strcpy(pStructCameralist[i].acameramanctrl,strCameramanctrl);
	strcpy(pStructCameralist[i].acamerainit,strCamerainit);
	strcpy(pStructCameralist[i].acameraalarm,strCameraalarm);
	strcpy(pStructCameralist[i].acamerasnap,strCamerasnap);
	strcpy(pStructCameralist[i].acameralastcleartime,strCameralastcleartime);
 
// 	strcpy(pStructCameralist->acamera_id,strCameraid);
// 	strcpy(pStructCameralist->aname,strCameraname_encode);
// 	strcpy(pStructCameralist->acameramode,strCameramode);
// 	strcpy(pStructCameralist->aurl,strCameraurl);
// 	strcpy(pStructCameralist->acamerastate,strCamerastate);
// 	strcpy(pStructCameralist->acamerapolicyid,strCamerapolicyid);
// 	strcpy(pStructCameralist->acameraidlist,strCameraidlist);
// 	strcpy(pStructCameralist->acameranodeid,strCameranodeid);
// 	strcpy(pStructCameralist->acamerafixedhost,strCamerafixedhost);
// 	strcpy(pStructCameralist->acamerabelonghost,strCamerabelonghost);
// 	strcpy(pStructCameralist->acameramanctrl,strCameramanctrl);
// 	strcpy(pStructCameralist->acamerainit,strCamerainit);
// 	strcpy(pStructCameralist->acameraalarm,strCameraalarm);
// 	strcpy(pStructCameralist->acamerasnap,strCamerasnap);
// 	strcpy(pStructCameralist->acameralastcleartime,strCameralastcleartime);
	i++;
	}
	return 0;
}
//添加一个摄像机
int CameraAdd(CString strIP,CString strSession,CString strCName,int intCMode,CString strRtsp,CString strDbid,int intNodeid,int intFixedhost,int intPolicyid)
{	
	CString strCName_encode = GB2312ToUTF8(strCName.GetBuffer(strCName.GetLength()));
	CString strRtsp_encode = GB2312ToUTF8(strRtsp.GetBuffer(strRtsp.GetLength()));
	CString	strDbid_encode = GB2312ToUTF8(strDbid.GetBuffer(strDbid.GetLength()));
	CString strContent;
	strContent.Format("{\"camera_name\":\"%s\",\n \"camera_mode\":%d,\n \"url\":\"%s\",\n \"db_id_list\":\"%s\",\n \"node_id\":%d,\n \"fixed_host\":%d,\n \"mt_policy_id\":%d}",strCName_encode,intCMode,strRtsp_encode,strDbid_encode,intNodeid,intFixedhost,intPolicyid);
	int iContentLen = strContent.GetLength();
	CString strContentLen;
	strContentLen.Format("%d",iContentLen);
	CString strSend;
		strSend.Format("%s%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s%s",	
		"PUT /rtmonitor/manage HTTP/1.1\n",
		"Date: Mon, 21 Mar 2016 11:30:17 GMT\n",
		"Accept: */*\n",
		"Host: ",strIP,"\n",
		"Content-Length: ",iContentLen,"\n",
		"Accept-Language: zh-cn\n",
		"User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1)\n",
		"Pragma: no-cache\n",
		"Cookie: facecloud_session=",strSession,":root\n",
		"Connection: close\n\n",
		strContent
		);
	SOCKET sockClient=socket(AF_INET,SOCK_STREAM,0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr=inet_addr(strIP);//该处地址应为空闲一体机地址
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(8000);
	connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
	send(sockClient,strSend.GetBuffer(strSend.GetLength()),strSend.GetLength(),0);
	char revbuf[8192];
	memset(revbuf,'\0',8192);
	int revlen = recv(sockClient,revbuf,8192,0);
	CString strRecv = revbuf;
	closesocket(sockClient);
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

//删除一个摄像机
int CameraDelete(CString strIP,CString strSession,CString strCamereId)
{
	CString strContent ="{}";
	int iContentLen = strContent.GetLength();
	CString strContentLen;
	strContentLen.Format("%d",iContentLen);
	CString strSend;
	strSend.Format("%s%s%s%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s%s%s",
		"DELETE /rtmonitor/manage/",strCamereId," HTTP/1.1\n",
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
	send(sockClient,strSend.GetBuffer(strSend.GetLength()),strSend.GetLength(),0);
	char revbuf[8192];
	memset(revbuf,'\0',8192);
	int revlen = recv(sockClient,revbuf,8192,0);
	CString strRecv = revbuf;
	closesocket(sockClient);
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

//配置一个摄像机
int CameraConfig(CString strIP,CString strSession,CString strCamereId,CString strCName,int intCMode,CString strRtsp,CString strDbid,int intNodeid,int intFixedhost,int intPolicyid)
{

	CString strCName_encode = GB2312ToUTF8(strCName.GetBuffer(strCName.GetLength()));
	CString strRtsp_encode = GB2312ToUTF8(strRtsp.GetBuffer(strRtsp.GetLength()));
	CString	strDbid_encode = GB2312ToUTF8(strDbid.GetBuffer(strDbid.GetLength()));
	CString strContent;
	strContent.Format("{\"camera_name\":\"%s\",\n \"camera_mode\":%d,\n \"url\":\"%s\",\n \"db_id_list\":\"%s\",\n \"node_id\":%d,\n \"fixed_host\":%d,\n \"mt_policy_id\":%d}",strCName_encode,intCMode,strRtsp_encode,strDbid_encode,intNodeid,intFixedhost,intPolicyid);
	int iContentLen = strContent.GetLength();
	CString strContentLen;
	strContentLen.Format("%d",iContentLen);
	CString strSend;
	strSend.Format("%s%s%s%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s%s%s",
		"POST /rtmonitor/manage/",strCamereId," HTTP/1.1\n",
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
	send(sockClient,strSend.GetBuffer(strSend.GetLength()),strSend.GetLength(),0);
	char revbuf[8192];
	memset(revbuf,'\0',8192);
	int revlen = recv(sockClient,revbuf,8192,0);
	CString strRecv = revbuf;
	closesocket(sockClient);
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
//开始人脸布控
int CameraRecognitionStart(CString strIP,CString strSession,CString strCamereId)
{
	CString strContent ="{}";
	int iContentLen = strContent.GetLength();
	CString strContentLen;
	strContentLen.Format("%d",iContentLen);
	CString strSend;
	strSend.Format("%s%s%s%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s%s%s",
		"POST /rtmonitor/manage/video_recognition/",strCamereId," HTTP/1.1\n",
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
	send(sockClient,strSend.GetBuffer(strSend.GetLength()),strSend.GetLength(),0);
	char revbuf[8192];
	memset(revbuf,'\0',8192);
	int revlen = recv(sockClient,revbuf,8192,0);
	CString strRecv = revbuf;
	closesocket(sockClient);
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

//停止人脸布控
int CameraRecognitionStop(CString strIP,CString strSession,CString strCamereId)
{
	CString strContent ="{}";
	int iContentLen = strContent.GetLength();
	CString strContentLen;
	strContentLen.Format("%d",iContentLen);
	CString strSend;
	strSend.Format("%s%s%s%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s%s%s",
		"POST /rtmonitor/manage/video_recognition_stop/",strCamereId," HTTP/1.1\n",
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
	send(sockClient,strSend.GetBuffer(strSend.GetLength()),strSend.GetLength(),0);
	char revbuf[8192];
	memset(revbuf,'\0',8192);
	int revlen = recv(sockClient,revbuf,8192,0);
	CString strRecv = revbuf;
	closesocket(sockClient);
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