#include "stdafx.h"
#include "ToolsForFaceServerLogin.h"

int Login(CString strIP, char *pSession)
{
	USES_CONVERSION;
	CString strContent = L"username=admin&password=123456"; 
	int iContentLen = strContent.GetLength();
	CString strSend;
	strSend.Format(L"%s%s%s%s%s%s%d%s%s%s%s%s",
		L"POST /login HTTP/1.1\n",
		L"Accept: */*\n",
		L"Host: ", strIP, L"\n",
		L"Content-Length: ", iContentLen, L"\n",
		L"Content-Type: application/x-www-form-urlencoded\r\n",
		L"Connection: close\n\n",
		strContent, L"\n");
	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);
	if (sockClient == INVALID_SOCKET)
	{
		WSACleanup();
		return 10;//socket create failed
	}
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(T2A(strIP));//该处地址应为空闲一体机地址
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8080);

	int tv_out_ms = 1000;
	int intsocketststus;
	intsocketststus = setsockopt(sockClient, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv_out_ms, sizeof(tv_out_ms));
	int m_connectstatus = connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	if (SOCKET_ERROR == m_connectstatus)
	{
		return 10;//socket connect failed
	}
	intsocketststus = setsockopt(sockClient, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv_out_ms, sizeof(tv_out_ms));
	if (intsocketststus == SOCKET_ERROR)
		return 10;
	int sendlen = send(sockClient, T2A(strSend), strSend.GetLength(), 0);
	if (sendlen == 0 || sendlen == SOCKET_ERROR)
		return 10;
	char *revbuf = new char[1024];
	memset(revbuf, '\0', 1024);
	recv(sockClient, revbuf, 1024, 0);
	CString strRecv = A2T(revbuf);
	closesocket(sockClient);
	int s(0), e(0);
	s = strRecv.Find(_T("SESSION="));
	if (s == -1)
	{
		delete[] revbuf;
		return 1;
	}
    s =	s + 8;
	e = strRecv.Find(_T(";"), s);
	if (e == -1)
	{
		delete[] revbuf;
		return 1;
	}
	CString strSession = strRecv.Mid(s, e - s);
	memset(pSession, '\0', 37);
	strcpy_s(pSession, 37,T2A(strSession));
	delete[] revbuf;
	return 0;
}

int Logout(CString strIP, CString strSession)
{
	USES_CONVERSION;
	CString strContent = L"";
	int iContentLen = strContent.GetLength();
	CString strSend;
	strSend.Format(L"%s%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s%s%s%s",
		L"POST /logout HTTP/1.1\n",
		L"Date: Mon, 21 Mar 2016 11:30:17 GMT\n",
		L"Accept: */*\n",
		L"Host: ", strIP, L"\n",
		L"Content-Length: ", iContentLen, L"\n",
		L"Accept-Language: zh-cn\n",
		L"User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1)\n",
		L"Pragma: no-cache\n",
		L"Content-Type: application/x-www-form-urlencoded\r\n",
		L"Cookie: SESSION=", strSession, L"\r\n",
		L"Connection: close\n\n",
		strContent, L"\n");
	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);
	if (sockClient == INVALID_SOCKET)
	{
		WSACleanup();
		return 10;//socket create failed
	}

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(T2A(strIP));//该处地址应为空闲一体机地址
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8080);

	int tv_out_ms = 1000;
	int intsocketststus;
	intsocketststus = setsockopt(sockClient, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv_out_ms, sizeof(tv_out_ms));
	int m_connectstatus = connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	if (SOCKET_ERROR == m_connectstatus)
	{
		return 10;//socket connect failed
	}
	intsocketststus = setsockopt(sockClient, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv_out_ms, sizeof(tv_out_ms));
	if (intsocketststus == SOCKET_ERROR)
		return 10;
	int sendlen = send(sockClient, T2A(strSend), strSend.GetLength() * 2, 0);
	if (sendlen == 0 || sendlen == SOCKET_ERROR)
	{
		return 10;
	}	
	char *revbuf = new char[1024*8];
	memset(revbuf, '\0', 1024*8);
	recv(sockClient, revbuf, 1024*8, 0);
	CString strRecv = A2T(revbuf);
	closesocket(sockClient);
	int s(0);
	strRecv.Find(_T("SESSION="));
	if (s == -1)
	{
		delete[] revbuf;
		return 2;
	}
	delete[] revbuf;
	return 0;
}