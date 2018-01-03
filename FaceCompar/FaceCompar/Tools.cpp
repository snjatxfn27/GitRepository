// Tools.cpp: implementation of the Tools class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Tools.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


void __SetFontSize(CWnd &m_tree)
{
	LOGFONT   logfont;
	CFont   *pfont1   =   m_tree.GetFont();  
	pfont1->GetLogFont(   &logfont   );  
	logfont.lfHeight   =logfont.lfHeight   *   2;   //��������޸�����ĸ߱���
	logfont.lfWidth     =   logfont.lfWidth   *   2;   //��������޸�����Ŀ����
	static   CFont   font1;  
	font1.CreateFontIndirect(&logfont);  
	m_tree.SetFont(&font1);  
	font1.Detach();
}
int __BreakCString(CString csData,CStringArray &arOutCString,CString csFindFlag)
{
	int iPos = -1;
	CString csTemp;
	while ((iPos = csData.Find(csFindFlag)) != -1)
	{
		csTemp = csData.Left(iPos);
		csData.Delete(0, iPos + 1);
		arOutCString.Add(csTemp);
	}
	arOutCString.Add(csData);

	return arOutCString.GetCount();
}
void __ExpandAllNode(HTREEITEM hItem, CTreeCtrl& m_treeShow)
{

	HTREEITEM hChild = m_treeShow.GetChildItem(hItem); 

	while (hChild)
	{
		m_treeShow.Expand(hItem, TVE_EXPAND);

		__ExpandAllNode(hChild, m_treeShow);

		hChild = m_treeShow.GetNextSiblingItem(hChild); 
	}

}


BOOL __IsDigital(CString strItem)
{
	for(int i= 0; i<strItem.GetLength(); i++)
	{
		// check"+"��"-"
		if (i== 0 && (strItem.GetAt(i)== 0x2B || strItem.GetAt(i)== 0x2D))
			continue;

		// check char
		if ( !isdigit(strItem.GetAt(i)) && strItem.GetAt(i)!= '.')
			return FALSE;
	}

	// check С����
	//if (strItem.Find('.')!= strItem.ReverseFind('.'))
	//	return FALSE;

	return TRUE;
}




CString Login(CString strIP)
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
//	TRACE(strSend);
	SOCKET sockClient=socket(AF_INET,SOCK_STREAM,0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr=inet_addr(strIP);//�ô���ַӦΪ����һ�����ַ
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(8000);
	connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
	send(sockClient,strSend.GetBuffer(strSend.GetLength()),strSend.GetLength(),0);
	char revbuf[8192];
	memset(revbuf,'\0',8192);
	int revlen = recv(sockClient,revbuf,8192,0);
	CString strRecv = revbuf;
//	TRACE(strRecv);
	closesocket(sockClient);

	int s(0),e(0);
	s = strRecv.Find("\"ret\" : ");
	s = s + 8;
	e = strRecv.Find(",",s);
	CString strRet = strRecv.Mid(s,e-s);
//	TRACE("strRet = %s\n",strRet);
	if(strRet != "0")
		return "FAULT";

	s = strRecv.Find("facecloud_session=");
	s = s + 18;
	e = strRecv.Find(":",s);
	CString strSession = strRecv.Mid(s,e-s);
	TRACE("Session = %s\n",strSession);

	return strSession;
}

//��ȡ��������
void FeatureDetect(CString strIP,CString strSession,CString strEImage,char *EFeature)
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
		strBody//,"\n"
		);
	int nNetTimeout=1000;//1�룬
	SOCKET sockClient=socket(AF_INET,SOCK_STREAM,0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr=inet_addr(strIP);//�ô���ַӦΪ����һ�����ַ
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(8000);
	connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
	//setsockopt(sockClient,SOL_SOCKET,SO_RCVTIMEO,(char *)&nNetTimeout,sizeof(int));
	int sendlen = strSend.GetLength();
	int re = send(sockClient,strSend.GetBuffer(strSend.GetLength()),strSend.GetLength(),0);
	TRACE("send %d / %d Byte\n",re,sendlen);
	int iAllRevLen = 0;
	while(1)
	{
		int revlen = recv(sockClient,EFeature + iAllRevLen,8192,MSG_WAITALL);
		if(revlen == 0)
			break;
		else
			iAllRevLen += revlen;
	}
	CString strRecv = EFeature;
	TRACE("recv %d finished!\n\n",iAllRevLen);
	
	closesocket(sockClient);

}
//base64����
int __EncodeBase64(const unsigned char* pSrc, char* pDst, int nSrcLen, int nMaxLineLen)
{  
	char EnBase64Tab[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	unsigned char c1, c2, c3;    // ���뻺��������3���ֽ�
	int nDstLen = 0;             // ������ַ�����
	int nLineLen = 0;            // ������г��ȼ���
	int nDiv = nSrcLen / 3;      // �������ݳ��ȳ���3�õ��ı���
	int nMod = nSrcLen % 3;      // �������ݳ��ȳ���3�õ�������
	   
	// ÿ��ȡ3���ֽڣ������4���ַ�
	for (int i = 0; i < nDiv; i ++)
	{
		// ȡ3���ֽ�
		c1 = *pSrc++;
		c2 = *pSrc++;
		c3 = *pSrc++;

	   // �����4���ַ�
		*pDst++ = EnBase64Tab[c1 >> 2];
		*pDst++ = EnBase64Tab[((c1 << 4) | (c2 >> 4)) & 0x3f];
		*pDst++ = EnBase64Tab[((c2 << 2) | (c3 >> 6)) & 0x3f];
		*pDst++ = EnBase64Tab[c3 & 0x3f];
		nLineLen += 4;
		nDstLen += 4;
   
	   // ������У�
		if (nLineLen > nMaxLineLen - 4)
		{
		   *pDst++ = '\r';
		   *pDst++ = '\n';
		   nLineLen = 0;
		   nDstLen += 2;
		}
	}

	   // �������µ��ֽ�
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
	// ����Ӹ�������
	*pDst = '\0';

	return nDstLen;
}
//base64����
int __DecodeBase64(const char* pSrc, unsigned char* pDst, int nSrcLen)
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
	
    int nDstLen;            // ������ַ�����
    int nValue;             // �����õ��ĳ�����
    int i;
	
    i = 0;
    nDstLen = 0;
	
    // ȡ4���ַ������뵽һ�����������پ�����λ�õ�3���ֽ�
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
        else        // �س����У�����
        {
            pSrc++;
            i++;
        }
	}
    // ����Ӹ�������
    *pDst = '\0';
	
    return nDstLen;
}
//�ַ���ת��GBKtoUTF-8
CString GB2312ToUTF8(const char *str)
{
	CString result;
	WCHAR *strSrc;
	TCHAR *szRes;
	
	//�����ʱ�����Ĵ�С
	int i = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	strSrc = new WCHAR[i+1];
	MultiByteToWideChar(CP_ACP, 0, str, -1, strSrc, i);
	
	//�����ʱ�����Ĵ�С
	i = WideCharToMultiByte(CP_UTF8, 0, strSrc, -1, NULL, 0, NULL, NULL);
	szRes = new TCHAR[i+1];
	int j=WideCharToMultiByte(CP_UTF8, 0, strSrc, -1, szRes, i, NULL, NULL);
	
	result = szRes;
	delete []strSrc;
	delete []szRes;
	return result;
 }
