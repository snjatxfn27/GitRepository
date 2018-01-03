// ToolsForPolicy.cpp: implementation of the ToolsForPolicy class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FaceRecAccessSys.h"
#include "ToolsForPolicy.h"
#include "json/json.h"
#include "FASPolicyManage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//布控策略查询
int PolicyQuery(CString strIP,CString strSession,CString strPolicy_id,struct StructPolicyList * pStructPolicylist,int * mt_policy_num)
{
	CString strContent;
	strContent.Format("{}");
	int iContentLen = strContent.GetLength();
	CString strContentLen;
	strContentLen.Format("%d",iContentLen);
	CString strSend;
	strSend.Format("%s%s%s%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s%s%s",
		"GET /rtmonitor/mt_policy?mt_policy_id=",strPolicy_id,"	HTTP/1.1\n",
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
	Json::Value root_Value,Policy_list_Value;
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
	* mt_policy_num = root_Value["mt_policy_num"].asInt();
	CString strPolicyid;
	CString strPolicyname;
	CString strPolicyname_encode;
	CString strPolicymonitor_type;
	CString strPolicyalarm_type;
	CString strPolicytime_schema_id;
	CString strPolicypolicysimilarity;
	CString strPolicymax_return;
	CString strPolicyscene_saved;
	CString strPolicymin_pixel;
	CString strPolicymax_face_num;
	CString strPolicyzoom_times;
	CString strPolicyface_storage_day;
	CString strPolicybest_face;
	CString strPolicyspeed_mode;
	CString strPolicyface_adjust;
	CString strPolicymin_point;
	CString strPolicyskip_dup;
	CString strPolicyskip_time;
	int i = 0;
	while(TRUE)
	{
	Policy_list_Value = root_Value["mt_policy_list"][i];
	if (Policy_list_Value.isNull())
	{
		break;
	}
	strPolicyid.Format("%d",Policy_list_Value["mt_policy_id"].asInt());
	strPolicyname = Policy_list_Value["mt_policy_name"].asCString();
	strPolicyname_encode = UTF8ToGB2312(strPolicyname.GetBuffer(strPolicyname.GetLength()));
	strPolicymonitor_type.Format("%d",Policy_list_Value["monitor_type"].asInt());
	strPolicyalarm_type=Policy_list_Value["alarm_type"].asCString();
	strPolicytime_schema_id.Format("%d",Policy_list_Value["time_schema_id"].asInt());
	strPolicypolicysimilarity.Format("%d",Policy_list_Value["similarity"].asInt());
	strPolicymax_return.Format("%d",Policy_list_Value["max_return"].asInt());
	strPolicyscene_saved.Format("%d",Policy_list_Value["scene_saved"].asInt());
	strPolicymin_pixel.Format("%d",Policy_list_Value["min_pixel"].asInt());
	strPolicymax_face_num.Format("%d",Policy_list_Value["max_face_num"].asInt());
	strPolicyzoom_times.Format("%.2lf",Policy_list_Value["zoom_times"].asDouble()); 
	strPolicyface_storage_day.Format("%d",Policy_list_Value["face_storage_day"].asInt());
	strPolicybest_face.Format("%d",Policy_list_Value["best_face"].asInt());
    strPolicyspeed_mode.Format("%d",Policy_list_Value["speed_mode"].asInt());
	strPolicyface_adjust = Policy_list_Value["face_adjust"].asInt();
	strPolicymin_point.Format("%d",Policy_list_Value["min_point"].asInt());
	strPolicyskip_dup.Format("%d",Policy_list_Value["skip_dup"].asInt());
	strPolicyskip_time.Format("%d",Policy_list_Value["skip_time"].asInt());
	strcpy(pStructPolicylist[i].aPolicy_id,strPolicyid);
	strcpy(pStructPolicylist[i].aname,strPolicyname_encode);
	strcpy(pStructPolicylist[i].aPolicymonitor_type,strPolicymonitor_type);
	strcpy(pStructPolicylist[i].aPolicyalarm_type,strPolicyalarm_type);
	strcpy(pStructPolicylist[i].aPolicytime_schema_id,strPolicytime_schema_id);
	strcpy(pStructPolicylist[i].aPolicypolicysimilarity,strPolicypolicysimilarity);
	strcpy(pStructPolicylist[i].aPolicymax_return,strPolicymax_return);
	strcpy(pStructPolicylist[i].aPolicyscene_saved,strPolicyscene_saved);
	strcpy(pStructPolicylist[i].aPolicymin_pixel,strPolicymin_pixel);
	strcpy(pStructPolicylist[i].aPolicymax_face_num,strPolicymax_face_num);
	strcpy(pStructPolicylist[i].aPolicyzoom_times,strPolicyzoom_times);
	strcpy(pStructPolicylist[i].aPolicyface_storage_day,strPolicyface_storage_day);
	strcpy(pStructPolicylist[i].aPolicybest_face,strPolicybest_face);
	strcpy(pStructPolicylist[i].aPolicyspeed_mode,strPolicyspeed_mode);
	strcpy(pStructPolicylist[i].aPolicyface_adjust,strPolicyface_adjust);
	strcpy(pStructPolicylist[i].aPolicymin_point,strPolicymin_point);
	strcpy(pStructPolicylist[i].aPolicyskip_dup,strPolicyskip_dup);
	strcpy(pStructPolicylist[i].aPolicyskip_time,strPolicyskip_time);
	i++;
	}
	return 0;
}

//添加一个策略
int PolicyAdd(CString strIP,CString strSession,CString strPolicyname,int intmonitor_type,CString stralarm_type,int inttime_schema_id,int intsimilarity,int intsimilarity2,int intsimilarity3,int intmax_return,int intscene_saved,int intmin_pixel,int intmax_face_num,double douzoom_times,int intstorage_day, int intbest_face,int intspeed_mode,int intface_adjust,int intmin_point,int intskip_dup,int intskip_time)
{	
	CString strPolicyname_encode = GB2312ToUTF8(strPolicyname.GetBuffer(strPolicyname.GetLength()));
	CString stralarm_type_encode = GB2312ToUTF8(stralarm_type.GetBuffer(stralarm_type.GetLength()));
	CString strContent;
	strContent.Format("{\"mt_policy_name\":\"%s\",\n \"monitor_type\":%d,\n \"alarm_type\":\"%s\",\n \"time_schema_id\":%d,\n \"similarity\":%d,\n \"similarity2\":%d,\n \"similarity3\":%d,\n \"max_return\":%d,\n \"scene_saved\":%d,\n \"min_pixel\":%d,\n \"max_face_num\":%d,\n \"zoom_times\":%.2lf,\n \"face_storage_day\":%d,\n \"best_face\":%d,\n \"speed_mode\":%d,\n \"face_adjust\":%d,\n \"min_point\":%d,\n \"skip_dup\":%d,\n \"skip_time\":%d}",strPolicyname_encode,intmonitor_type,stralarm_type_encode,inttime_schema_id,intsimilarity,intsimilarity2,intsimilarity3,intmax_return,intscene_saved,intmin_pixel,intmax_face_num,douzoom_times,intstorage_day,intbest_face,intspeed_mode,intface_adjust,intmin_point,intskip_dup,intskip_time);
	//strContent.Format("{\"mt_policy_name\":\"%s\",\n \"monitor_type\":%d,\n \"alarm_type\":\"%s\",\n \"time_schema_id\":%d,\n \"similarity\":%d,\n \"max_return\":%d,\n \"scene_saved\":%d,\n \"min_pixel\":%d,\n \"max_face_num\":%d,\n \"zoom_times\":%d,\n \"face_storage_day\":%d,\n \"best_face\":%d,\n \"speed_mode\":%d,\n \"face_adjust\":%d,\n \"min_point\":%d,\n \"skip_dup\":%d,\n \"skip_time\":%d}",strPolicyname_encode,intmonitor_type,stralarm_type_encode,inttime_schema_id,intsimilarity,intmax_return,intscene_saved,intmin_pixel,intmax_face_num,intzoom_times,intstorage_day,intbest_face,intspeed_mode,intface_adjust,intmin_point,intskip_dup,intskip_time);
	int iContentLen = strContent.GetLength();
	CString strContentLen;
	strContentLen.Format("%d",iContentLen);
	CString strSend;
		strSend.Format("%s%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s%s",	
		"PUT /rtmonitor/mt_policy HTTP/1.1\n",
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
	Json::Value root_Value,Policy_list_Value;
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

//删除一个策略
int PolicyDelete(CString strIP,CString strSession,CString strMt_policy_id)
{
	CString strContent ="{}";
	int iContentLen = strContent.GetLength();
	CString strContentLen;
	strContentLen.Format("%d",iContentLen);
	CString strSend;
	strSend.Format("%s%s%s%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s%s%s",
		"DELETE /rtmonitor/mt_policy/delete/",strMt_policy_id," HTTP/1.1\n",
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
	Json::Value root_Value,Policy_list_Value;
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

//修改一个策略
int PolicyModify(CString strIP,CString strSession,CString strMt_policy_id,CString strPolicyname,int intmonitor_type,CString stralarm_type,int inttime_schema_id,int intsimilarity,int intsimilarity2,int intsimilarity3,int intmax_return,int intscene_saved,int intmin_pixel,int intmax_face_num,double douzoom_times,int intstorage_day, int intbest_face,int intspeed_mode,int intface_adjust,int intmin_point,int intskip_dup,int intskip_time)
{	
	CString strPolicyname_encode = GB2312ToUTF8(strPolicyname.GetBuffer(strPolicyname.GetLength()));
	CString stralarm_type_encode = GB2312ToUTF8(stralarm_type.GetBuffer(stralarm_type.GetLength()));
	CString strMt_policy_id_encode = GB2312ToUTF8(strMt_policy_id.GetBuffer(strMt_policy_id.GetLength()));
	CString strContent;
	strContent.Format("{\"mt_policy_name\":\"%s\",\n \"monitor_type\":%d,\n \"alarm_type\":\"%s\",\n \"time_schema_id\":%d,\n \"similarity\":%d,\n \"similarity2\":%d,\n \"similarity3\":%d,\n \"max_return\":%d,\n \"scene_saved\":%d,\n \"min_pixel\":%d,\n \"max_face_num\":%d,\n \"zoom_times\":%.2lf,\n \"face_storage_day\":%d,\n \"best_face\":%d,\n \"speed_mode\":%d,\n \"face_adjust\":%d,\n \"min_point\":%d,\n \"skip_dup\":%d,\n \"skip_time\":%d}",strPolicyname_encode,intmonitor_type,stralarm_type_encode,inttime_schema_id,intsimilarity,intsimilarity2,intsimilarity3,intmax_return,intscene_saved,intmin_pixel,intmax_face_num,douzoom_times,intstorage_day,intbest_face,intspeed_mode,intface_adjust,intmin_point,intskip_dup,intskip_time);
	int iContentLen = strContent.GetLength();
	CString strContentLen;
	strContentLen.Format("%d",iContentLen);
	CString strSend;
		strSend.Format("%s%s%s%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s%s",	
		"POST /rtmonitor/mt_policy/",strMt_policy_id_encode," HTTP/1.1\n",
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
	Json::Value root_Value,Policy_list_Value;
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
