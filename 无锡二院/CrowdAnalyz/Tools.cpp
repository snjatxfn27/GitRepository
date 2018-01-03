// Tools.cpp : implementation file
//

#include "stdafx.h"
#include "Tools.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTools

CTools::CTools()
{
}

CTools::~CTools()
{
}


BEGIN_MESSAGE_MAP(CTools, CWnd)
	//{{AFX_MSG_MAP(CTools)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTools message handlers
int CTools::EncodeBase64(const unsigned char* pSrc, char* pDst, int nSrcLen, int nMaxLineLen)
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

int CTools::DecodeBase64(const char* pSrc, unsigned char* pDst, int nSrcLen)
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

int CTools::DownloadImageFromFileServer(CString IP, int port, CString src, CString dst)
{
	try
	{
		USES_CONVERSION;
		CString strSend;
		strSend.Format(_T("%s%s%s%s%s%s%s%d%s%s"),
			L"GET ", src, L" HTTP/1.1\n",
			//		L"Accept: text/html, application/xhtml+xml, */*\n",
			L"Accept-Language: zh-CN\n",
			//		L"User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; rv:11.0) like Gecko\n",
			//		L"Accept-Encoding: gzip, deflate\n",
			L"Host: ", IP, L":", port, L"\n",
			//		L"DNT: 1\n",
			//		L"Connection: Keep - Alive\n\n"
			L"Connection: close\n\n"
			);

		SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);
		SOCKADDR_IN addrSrv;
		addrSrv.sin_addr.S_un.S_addr = inet_addr(T2A(IP));//该处地址应为空闲一体机地址
		addrSrv.sin_family = AF_INET;
		addrSrv.sin_port = htons(port);
		int conre = connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
		int len1 = strSend.GetLength();
		int sendre = send(sockClient, T2A(strSend.GetBuffer(0)), len1 * 2 + 2, 0);
		char revbuf[800 * 1024];
		memset(revbuf, '\0', 800 * 1024);
		int iCurPos = 0;
		SYSTEMTIME tm;
		GetLocalTime(&tm);
		BOOL bReturn = FALSE;
		TRACE(_T("%02d:%02d:%02d.%03d\n"), tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds);
		while (1)
		{
			int revlen = recv(sockClient, revbuf + iCurPos, 50 * 1024, 0);
			TRACE(_T("revlen = %d\n"), revlen);
			if (revlen == 0 || revlen == -1)
			{
				break;
			}
			iCurPos += revlen;
			Sleep(1);
		}
		
		TRACE("iCurPos = %d\n", iCurPos);
		GetLocalTime(&tm);
		TRACE(_T("%02d:%02d:%02d.%03d\n"), tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds);
		closesocket(sockClient);
		CString strRev;
		strRev = revbuf;

		if (strRev.IsEmpty())
		{
			return -1;
		}

		int clPos = strRev.Find(L"Content-Length: ");
		int nPos = strRev.Find(L"\r\n", clPos);
		CString strLen = strRev.Mid(clPos + 16, nPos - clPos - 16);
		int imgLen = _wtoi(strLen);

		CFile file;
// 		if (file.Open(L"http_response.txt", CFile::modeCreate | CFile::modeWrite))
// 		{
// 			file.Write(revbuf, iCurPos);
// 			file.Close();
// 		}
		
		if (file.Open(dst, CFile::modeCreate | CFile::modeWrite))
		{
			file.Write(&revbuf[iCurPos - imgLen], imgLen);
			file.Close();
		}

		

		TRACE(_T("imglen = %d, iCurpos = %d\n"), imgLen, iCurPos);
	}
	catch (CException* e)
	{
		return -1;

	}
	_CrtDumpMemoryLeaks();
	CFileStatus fs;
	return CFile::GetStatus(dst, fs);
}

int CTools::GetJPGFromDB(CString csDBIp, CString csDBName, CString csDBUser,CString csDBPwd,CString strIDCard, CString dst)
{
	USES_CONVERSION;
	_ConnectionPtr conn;
	conn.CreateInstance(__uuidof(Connection));
	CString strConn;
	strConn.Format(_T("Driver={MySQL ODBC 5.1 Driver}; Server = %s; Database = %s;uid=%s;pwd=%s;"), csDBIp, csDBName, csDBUser, csDBPwd);
	try
	{
		conn->Open(_bstr_t(strConn), "", "", adModeUnknown);
	}
	catch (_com_error e)
	{
		return false;
	}
	_RecordsetPtr pSet;
	pSet.CreateInstance(__uuidof(Recordset));
	CString strSQL;
	strSQL.Format(_T("select img from imginfo where idcard='%s'"), strIDCard);
	pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	if (!pSet->adoEOF)
	{
		CString strImg;
		//strImg = getsqlstr("img");
		strImg = (TCHAR *)(_bstr_t)pSet->GetFields()->GetItem("img")->Value;

		int strlen = strImg.GetLength() * 2 + 2;
		unsigned char * jpgchar = new unsigned char[strlen];
		memset(jpgchar, 0, strlen);
		int flen = DecodeBase64(T2A(strImg.GetBuffer(0)), jpgchar, strlen);
		CFile file;
		file.Open(dst, CFile::modeCreate | CFile::modeWrite);
		file.Write(jpgchar, flen);
		file.Close();
		delete[] jpgchar;
	}
	else
	{
		pSet->Close();
		conn->Close();
		return 1;
	}
	pSet->Close();
	conn->Close();
	return 0;
}