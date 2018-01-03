// ToolsForRecognition.cpp: implementation of the ToolsForRecognition class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FaceRecAccessSys.h"
#include "ToolsForRecognition.h"
#include "json/json.h"
#include "FASRecognition.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
//获取人脸特征
int FeatureDetect(CString strIP,CString strSession,CString strEImage,char * pEImage,char *pEFeature)
{
	CString strBody;
	strBody.Format("{\"image_data\":{\"type\":\"jpg\",\"content\":\"%s\"}}",strEImage);
	int iBodylen = strBody.GetLength();

	CString strSend;
	strSend.Format("%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s%s",
		"POST /faceops/image_detection HTTP/1.1\n",
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
	char revbuf[1024*1024*6];
	memset(revbuf,'\0',1024*1024*6);
// 	struct timeval
//     {
// 		time_t tv_sec;
// 		time_t tv_usec;
//     };
// 	struct timeval tv_out;
//     tv_out.tv_sec = 6;
//     tv_out.tv_usec = 0;
// 	int intsocketststus;
// 	intsocketststus = setsockopt(sockClient, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv_out, sizeof(tv_out));
// 	if (intsocketststus==SOCKET_ERROR)
// 		return -1;
// 	int sendlen = send(sockClient,strSend.GetBuffer(strSend.GetLength()),strSend.GetLength(),0);
// 	if(sendlen == 0||sendlen==SOCKET_ERROR)
// 		return -1;
//  	intsocketststus = setsockopt(sockClient, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv_out, sizeof(tv_out));
//  	if (intsocketststus==SOCKET_ERROR)
//  		return -1;
// 	int revlen = recv(sockClient,revbuf,1024*1024*6,0);
// 	if(revlen == 0||revlen==SOCKET_ERROR)
// 		return -1;
	int sendlen = strSend.GetLength();
	int re = send(sockClient,strSend.GetBuffer(strSend.GetLength()),strSend.GetLength(),0);
	TRACE("send %d / %d Byte\n",re,sendlen);
	int iAllRevLen = 0;
	while(1)
	{
		int revlen = recv(sockClient,pEFeature + iAllRevLen,8192,0);
		if(revlen == 0)
			break;
		else
			iAllRevLen += revlen;
	}
	CString strRevbuf = pEFeature;
	closesocket(sockClient);
	//解析json结构
	int strJsonstart = strRevbuf.Find("{",0);
	strRevbuf.Delete(0,strJsonstart);
	Json::Reader reader;
	Json::Value root_Value,dection_list_Value;
	BOOL readresult = reader.parse(strRevbuf.GetBuffer(strRevbuf.GetLength()),root_Value);
	if (!readresult)
	{
		return -1;
	}
	int ret = root_Value["ret"].asInt();
	if (ret!=0)
	{
		return ret;
	}
	int listsize = root_Value["list_size"].asInt();//如果listsize大于1，说明图片中存在多张人脸。
	if (listsize>1)
	{
		return 2020;
	}
	CString m_strEImage="";
	CString m_strFeature="";
	int i = 0;
	while(TRUE)
	{
		dection_list_Value = root_Value["dection_list"][i];
		if (dection_list_Value.isNull())
		{
			break;
		}
		m_strEImage = dection_list_Value["image_data"]["content"].asCString();
		m_strFeature = dection_list_Value["feature_data"]["content"].asCString();
		strcpy(pEImage,m_strEImage);
		strcpy(pEFeature,m_strFeature);
		//i++; //一个图片中提取多个人脸时使用。
		break;
	}
	return 0;
}
//图片特征对比
int ImgRecognition(CString strIP,CString strSession,CString strFacedb,CString strImage,CString strTop,CString strSimilarity, int * plistsize,struct StructRecognitionList * pStructRecognitionlist )
{
	CString strBody;
	strBody.Format("{\"image_data\":{\"type\":\"jpg\",\"content\":\"%s\"}}",strImage);
	int iBodylen = strBody.GetLength();
	CString strSend;
	strSend.Format("%s%s%s%s%s%s%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s%s",
		"POST /faceops/image_recognition?face_db=",strFacedb,"&top=",strTop,"&similarity=",strSimilarity," HTTP/1.1\n",//top指最多返回多少条，similar是返回相似度范围
		"Accept: */*\n",
		"Host:",strIP,"\n",
		"Content-Length: ",iBodylen,"\n",
		"Accept-Language: zh-cn\n",
		"User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1)\n",
		"Pragma: no-cache\n",
		"Cookie: facecloud_session=",strSession,":root\n",
		"Connection: close\n\n",
		strBody//,"\n"
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
	Json::Value root_Value,recognition_list_Value,matching_list_Value;
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
	int j = 0;
	int i = 0;
	CString m_strIDcard;
	CString m_strSimilarity="00.00";
	CString m_strSimilarity1="00.00";
	int dou_sim;
	while(TRUE)
	{
		recognition_list_Value = root_Value["recognition_list"][i];
		if (recognition_list_Value.isNull())
		{
			break;
		}
		int k=0;
		while(TRUE)
		{
			matching_list_Value = recognition_list_Value["matching_list"][k];
			if (matching_list_Value.isNull())
			{
				break;
			}
			//m_strIdcard.Format("%s",matching_list_Value["id_card"].asCString());
			m_strIDcard=matching_list_Value["id_card"].asCString();
			dou_sim=matching_list_Value["similarity"].asInt();
			m_strSimilarity.Format("%d",dou_sim);
			memset(pStructRecognitionlist[j].aSimilarity,0,strlen(pStructRecognitionlist[j].aSimilarity)); 
			memset(pStructRecognitionlist[j].aId_card,0,strlen(pStructRecognitionlist[j].aId_card));
			strcpy(pStructRecognitionlist[j].aId_card,m_strIDcard);
			strcpy(pStructRecognitionlist[j].aSimilarity,m_strSimilarity);
			j++;
			k++;
		}
		i++;
	}
	* plistsize = j;
	return 0;
}

//图片特征对比条件查询
int ImgRecognitionForSelect(CString strIP,CString strSession,CString strFacedb,CString strImage,CString strTop,CString strSimilarity, CString strAge,CString strSex,CString strNative_place,CString strAddr,CString strName,CString strPhone,CString strId_card,int * plistsize,struct StructRecognitionList * pStructRecognitionlist )
{
	CString strNative_place_encode = GB2312ToUTF8(strNative_place.GetBuffer(strNative_place.GetLength()));
	CString strAddr_encode = GB2312ToUTF8(strAddr.GetBuffer(strAddr.GetLength()));
	CString strName_place_encode = GB2312ToUTF8(strName.GetBuffer(strName.GetLength()));
	CString strBody;
	strBody.Format("{\"image_data\":{\"type\":\"jpg\",\"content\":\"%s\"}}",strImage);
	int iBodylen = strBody.GetLength();
	CString strSend;
	strSend.Format("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s%s",
		"POST /faceops/image_recognition?face_db=",strFacedb,"&top=",strTop,"&similarity=",strSimilarity,"&age=",strAge,"&sex=",strSex,"&native_place=",strNative_place_encode,"&addr=",strAddr_encode,"&name=",strName_place_encode,"&phone=",strPhone,"&id_card=",strId_card," HTTP/1.1\n",//top指最多返回多少条，similar是返回相似度范围
		"Accept: */*\n",
		"Host:",strIP,"\n",
		"Content-Length: ",iBodylen,"\n",
		"Accept-Language: zh-cn\n",
		"User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1)\n",
		"Pragma: no-cache\n",
		"Cookie: facecloud_session=",strSession,":root\n",
		"Connection: close\n\n",
		strBody//,"\n" 
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
	Json::Value root_Value,recognition_list_Value,matching_list_Value;
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
	int j = 0;
	int i = 0;
	CString m_strIDcard;
	CString m_strSimilarity="00.00";
	CString m_strSimilarity1="00.00";
	int dou_sim;
	while(TRUE)
	{
		recognition_list_Value = root_Value["recognition_list"][i];
		if (recognition_list_Value.isNull())
		{
			break;
		}
		int k=0;
		while(TRUE)
		{
			matching_list_Value = recognition_list_Value["matching_list"][k];
			if (matching_list_Value.isNull())
			{
				break;
			}
			//m_strIdcard.Format("%s",matching_list_Value["id_card"].asCString());
			m_strIDcard=matching_list_Value["id_card"].asCString();
			dou_sim=matching_list_Value["similarity"].asInt();
			m_strSimilarity.Format("%d",dou_sim);
			memset(pStructRecognitionlist[j].aSimilarity,0,strlen(pStructRecognitionlist[j].aSimilarity)); 
			memset(pStructRecognitionlist[j].aId_card,0,strlen(pStructRecognitionlist[j].aId_card));
			strcpy(pStructRecognitionlist[j].aId_card,m_strIDcard);
			strcpy(pStructRecognitionlist[j].aSimilarity,m_strSimilarity);
			j++;
			k++;
		}
		i++;
	}
	* plistsize = j;
	return 0;
}

//特征对比
int FeatureMatching(CString strIP,CString strSession,CString strFacedb,CString strFeature,CString strTop,CString strSimilarity, int * plistsize,struct StructRecognitionList * pStructRecognitionlist )
{
	CString strBody;
	strBody.Format("{\"feature_list\":[{\"content\":\"%s\"}]}",strFeature);
	int iBodylen = strBody.GetLength();
	CString strSend;
	strSend.Format("%s%s%s%s%s%s%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s%s",
		"POST /faceops/feature_matching?face_db=",strFacedb,"&top=",strTop,"&similarity=",strSimilarity," HTTP/1.1\n",//top指最多返回多少条，similar是返回相似度范围
		"Accept: */*\n",
		"Host:",strIP,"\n",
		"Content-Length: ",iBodylen,"\n",
		"Accept-Language: zh-cn\n",
		"User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1)\n",
		"Pragma: no-cache\n",
		"Cookie: facecloud_session=",strSession,":root\n",
		"Connection: close\n\n",
		strBody//,"\n"
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
	Json::Value root_Value,recognition_list_Value,matching_list_Value;
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
	int j = 0;
	int i = 0;
	CString m_strIDcard;
	CString m_strSimilarity="00.00";
	CString m_strSimilarity1="00.00";
	int dou_sim;
		while(TRUE)
		{
			matching_list_Value = root_Value["matching_list"][i];
			if (matching_list_Value.isNull())
			{
				break;
			}
			m_strIDcard=matching_list_Value["id_card"].asCString();
			dou_sim=matching_list_Value["similarity"].asInt();
			m_strSimilarity.Format("%d",dou_sim);
			memset(pStructRecognitionlist[j].aSimilarity,0,strlen(pStructRecognitionlist[j].aSimilarity)); 
			memset(pStructRecognitionlist[j].aId_card,0,strlen(pStructRecognitionlist[j].aId_card));
			strcpy(pStructRecognitionlist[j].aId_card,m_strIDcard);
			strcpy(pStructRecognitionlist[j].aSimilarity,m_strSimilarity);
			j++;
			i++;
		}
	* plistsize = j;
	return 0;
}



//性别识别
int ImgAgeSexAnalysis(CString strIP,CString strSession,struct StructRecognitionList * pStructRecognitionlist,CString strImage)
{
	CString strBody;
	strBody.Format("{\"face_image\":\"%s\"}",strImage);
	int iBodylen = strBody.GetLength();
	CString strSend;
	strSend.Format("%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s%s",
		"POST /faceops/age_sex_analysis HTTP/1.1\n",
		"Accept: */*\n",
		"Host:",strIP,"\n",
		"Content-Length: ",iBodylen,"\n",
		"Accept-Language: zh-cn\n",
		"User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1)\n",
		"Pragma: no-cache\n",
		"Cookie: facecloud_session=",strSession,":root\n",
		"Connection: close\n\n",
		strBody//,"\n"
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
	Json::Value root_Value,result_list_Value;
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
	int listsize = root_Value["list_size"].asInt();//如果listsize大于1，说明图片中存在多张人脸。
	if (listsize>1)
	{
		return 2020;
	}
	int i = 0;
	while(TRUE)
	{
		result_list_Value = root_Value["result_list"][i];
		if (result_list_Value.isNull())
		{
			break;
		}
		int age = result_list_Value["age"].asInt();
		CString sex = result_list_Value["sex"].asCString();
		//i++; //一个图片中提取多个人脸时使用。
		break;
	}
	return 0;
}