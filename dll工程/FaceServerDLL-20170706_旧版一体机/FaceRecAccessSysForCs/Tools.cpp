// Tools.cpp : implementation file
//

#include "stdafx.h"
#include "FaceRecAccessSys.h"
#include "Tools.h"
#include "json/json.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Tools

/////////////////////////////////////////////////////////////////////////////
// Tools message handlers
//登录一体机

int Login(CString strIP,char * pSession)
{
	CString strContent = "{\n	\"user_name\":	\"root\",\n	\"user_pwd\":	\"6D3D66861E2E1AA9BAFBB09B7E5AE651\",\n	\"mode\":	\"force_login\"\n}";

	int iContentLen = strContent.GetLength();
	CString strContentLen;
	strContentLen.Format("%d",iContentLen);
 	CString strSend;
	strSend.Format("%s%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s",
		"POST /base/auth/login HTTP/1.1\n",
		"Date: Mon, 21 Mar 2016 11:30:17 GMT\n",
		"Accept: */*\n",
		"Host: ",strIP,"\n",
		"Content-Length: ",iContentLen,"\n",
		"Accept-Language: zh-cn\n",
		"User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1)\n",
		"Pragma: no-cache\n",
		"Cookie: facecloud_session=160315175150000005:root\n",
		"Connection: close\n\n",
		strContent,"\n");
	SOCKET sockClient=socket(AF_INET,SOCK_STREAM,0);
	if (sockClient == INVALID_SOCKET)
	{ 
 		WSACleanup(); 
 		return 10;//socket create failed
 	}

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr=inet_addr(strIP);//该处地址应为空闲一体机地址
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(8000);

    int tv_out_ms =1000;
	int intsocketststus;
	intsocketststus = setsockopt(sockClient, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv_out_ms, sizeof(tv_out_ms));
	int m_connectstatus = connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
	if (SOCKET_ERROR == m_connectstatus )
	{
		return 10;//socket connect failed
	}
	intsocketststus = setsockopt(sockClient, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv_out_ms, sizeof(tv_out_ms));
	if (intsocketststus==SOCKET_ERROR)
	return -1;
	int sendlen = send(sockClient,strSend.GetBuffer(strSend.GetLength()),strSend.GetLength(),0);
 	if(sendlen == 0||sendlen==SOCKET_ERROR)
	return 10;
	char revbuf[8192];
	memset(revbuf,'\0',8192);
	recv(sockClient,revbuf,8192,0);
	CString strRecv = revbuf;
	closesocket(sockClient);
	int s(0),e(0);
	s = strRecv.Find("\"ret\" : ");
	if (s == -1)
	{
		return -1;
	}
	s = s + 8;
	e = strRecv.Find(",",s);
	CString strRet = strRecv.Mid(s,e-s);
	if(strRet != "0")
	{
	int intRet = atoi(strRet);
	return intRet;
	}
	s = strRecv.Find("facecloud_session=");
	s = s + 18;
	e = strRecv.Find(":",s);
	CString strSession = strRecv.Mid(s,e-s);
	strcpy(pSession, strSession);
	return 0;
}

//登出一体机
int Logout(CString strIP,char * pSession)
{
	CString strContent;
	strContent.Format("{}");
	int iContentLen = strContent.GetLength();
	CString strContentLen;
	strContentLen.Format("%d",iContentLen);
	CString strSend;
	CString strSession = pSession;
	strSend.Format("%s%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s%s",
		"POST /base/auth/logout HTTP/1.1\n",
		"Date: Mon, 21 Mar 2016 11:30:17 GMT\n",
		"Accept: */*\n",
		"Host: ",strIP,"\n",
		"Content-Length: ",iContentLen,"\n",
		"Accept-Language: zh-cn\n",
		"User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1)\n",
		"Pragma: no-cache\n",
		"Cookie: facecloud_session=",strSession,":root\n",
		"Connection: close\n\n",
		strContent);
//	TRACE(strSend);
	SOCKET sockClient=socket(AF_INET,SOCK_STREAM,0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr=inet_addr(strIP);//该处地址应为空闲一体机地址
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(8000);
	int m_connectstatus = connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
	if (SOCKET_ERROR == m_connectstatus )
	{
		return 10;//socket connect failed
	}
	send(sockClient,strSend.GetBuffer(strSend.GetLength()),strSend.GetLength(),0);
	char revbuf[8192];
	memset(revbuf,'\0',8192);
	int revlen = recv(sockClient,revbuf,8192,0);
	CString strRecv = revbuf;
	TRACE(strRecv);
	closesocket(sockClient);
	int s(0),e(0);
	s = strRecv.Find("\"ret\" : ");
	if (s == -1)
	{
		return -1;
	}
	s = s + 8;
	e = strRecv.Find(",",s);
	CString strRet = strRecv.Mid(s,e-s);
	if(strRet != "0")
	{
		int intRet = atoi(strRet);
		return intRet;
	}
	return 0;
}

//base64编码
int EncodeBase64(const unsigned char* pSrc, char* pDst, int nSrcLen, int nMaxLineLen)
{  
	char EnBase64Tab[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	unsigned char c1, c2, c3;    // 输入缓冲区读出3个字节
	int nDstLen = 0;             // 输出的字符计数
	int nLineLen = 0;            // 输出的行长度计数
	int nDiv = nSrcLen / 3;      // 输入数据长度除以3得到的倍数
	int nMod = nSrcLen % 3;      // 输入数据长度除以3得到的余数
	   
	// 每次取3个字节，编码成4个字符
	for (int i = 0; i < nDiv; i ++)
	{
		// 取3个字节
		c1 = *pSrc++;
		c2 = *pSrc++;
		c3 = *pSrc++;

	   // 编码成4个字符
		*pDst++ = EnBase64Tab[c1 >> 2];
		*pDst++ = EnBase64Tab[((c1 << 4) | (c2 >> 4)) & 0x3f];
		*pDst++ = EnBase64Tab[((c2 << 2) | (c3 >> 6)) & 0x3f];
		*pDst++ = EnBase64Tab[c3 & 0x3f];
		nLineLen += 4;
		nDstLen += 4;
   
	   // 输出换行？
		if (nLineLen > nMaxLineLen - 4)
		{
		   *pDst++ = '\r';
		   *pDst++ = '\n';
		   nLineLen = 0;
		   nDstLen += 2;
		}
	}

	   // 编码余下的字节
	if (nMod == 1)
	{
		c1 = *pSrc++;
		*pDst++ = EnBase64Tab[(c1 & 0xfc) >> 2];
		*pDst++ = EnBase64Tab[((c1 & 0x03) << 4)];
		*pDst++ = '=';
		*pDst++ = '=';
		nLineLen += 4;
		nDstLen += 4;
	}
	else if (nMod == 2)
	{
		c1 = *pSrc++;
		c2 = *pSrc++;
		*pDst++ = EnBase64Tab[(c1 & 0xfc) >> 2];
		*pDst++ = EnBase64Tab[((c1 & 0x03) << 4) | ((c2 & 0xf0) >> 4)];
		*pDst++ = EnBase64Tab[((c2 & 0x0f) << 2)];
		*pDst++ = '=';
		nDstLen += 4;
	}
	// 输出加个结束符
	*pDst = '\0';

	return nDstLen;
}
//base64解码
int DecodeBase64(const char* pSrc, unsigned char* pDst, int nSrcLen)
{
	char DeBase64Tab[] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			62,        // '+'
			0, 0, 0,
			63,        // '/'
			52, 53, 54, 55, 56, 57, 58, 59, 60, 61,        // '0'-'9'
			0, 0, 0, 0, 0, 0, 0,
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
			13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,        // 'A'-'Z'
			0, 0, 0, 0, 0, 0,
			26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
			39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,        // 'a'-'z'
	};
	
    int nDstLen;            // 输出的字符计数
    int nValue;             // 解码用到的长整数
    int i;
	
    i = 0;
    nDstLen = 0;
	
    // 取4个字符，解码到一个长整数，再经过移位得到3个字节
    while (i < nSrcLen)
    {
        if (*pSrc != '\r' && *pSrc!='\n')
        {
            nValue = DeBase64Tab[*pSrc++] << 18;
            nValue += DeBase64Tab[*pSrc++] << 12;
            *pDst++ = (nValue & 0x00ff0000) >> 16;
            nDstLen++;
			
            if (*pSrc != '=')
            {
                nValue += DeBase64Tab[*pSrc++] << 6;
                *pDst++ = (nValue & 0x0000ff00) >> 8;
                nDstLen++;
				
                if (*pSrc != '=')
                {
                    nValue += DeBase64Tab[*pSrc++];
                    *pDst++ =nValue & 0x000000ff;
                    nDstLen++;
                }
            }
			
            i += 4;
        }
        else        // 回车换行，跳过
        {
            pSrc++;
            i++;
        }
	}
    // 输出加个结束符
    *pDst = '\0';
	
    return nDstLen;
}
//字符集转换GBKtoUTF-8
CString GB2312ToUTF8(const char *str)
{
	CString result;
	WCHAR *strSrc;
	TCHAR *szRes;
	//获得临时变量的大小
	int i = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	strSrc = new WCHAR[i+1];
	MultiByteToWideChar(CP_ACP, 0, str, -1, strSrc, i);
	//获得临时变量的大小
	i = WideCharToMultiByte(CP_UTF8, 0, strSrc, -1, NULL, 0, NULL, NULL);
	szRes = new TCHAR[i+1];
	int j=WideCharToMultiByte(CP_UTF8, 0, strSrc, -1, szRes, i, NULL, NULL);
	result = szRes;
	delete []strSrc;
	delete []szRes;
	return result;
 }
//字符集转换UTF-8toGBK
CString UTF8ToGB2312(const char* str)
{
	CString result;
	WCHAR *strSrc;
	TCHAR *szRes;
	//获得临时变量的大小
	int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	strSrc = new WCHAR[i+1];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);
	//获得临时变量的大小
	i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
	szRes = new TCHAR[i+1];
	WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);
	result = szRes;
	delete []strSrc;
	delete []szRes;
	return result;
}