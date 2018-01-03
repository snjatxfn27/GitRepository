#include "stdafx.h"
#include "ToolsForBlackDB.h"
#include "Tools.h"
//黑名单人脸库查询
int CheckBlack(CString strIP,CString strSession, CString strName, char*chJson, int jsonlen)
{
	USES_CONVERSION;
	CString strSend;
	strName = UrlEncode(strName);
	CString src = L"/api/v1/facedb/black?name=" + strName;
	strSend.Format(_T("%s%s%s%s%s%s%s%s%s%s%s"),
		L"GET ", src, L" HTTP/1.1\n",
		L"Accept-Language: zh-CN\n",
		L"Host: ", strIP, L":8080\n",
		L"Cookie: SESSION=", strSession, L"\r\n",
		L"Connection: close\n\n"
		);

	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(T2A(strIP.GetBuffer(0)));//该处地址应为空闲一体机地址
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8080);
	int conre = connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	int len1 = strSend.GetLength();
	int sendre = send(sockClient, T2A(strSend.GetBuffer(0)), len1, 0);
	char *revbuf = new char[128 * 1024];
	memset(revbuf, '\0', 128 * 1024);
	int iCurPos = 0;
	while (1)
	{
		int revlen = recv(sockClient, revbuf + iCurPos, 2 * 1024, 0);
		if (revlen == 0 || revlen == -1)
			break;
		iCurPos += revlen;
		Sleep(1);
	}
	closesocket(sockClient);
	//整理json结构
	CString strRecv;
	strRecv = revbuf;
	int s(0), e(0);
	int strJsonstart = strRecv.Find(L"{", 0);
	if (strJsonstart == -1)
	{
		delete[] revbuf;
		if (strRecv.Find(L"403", 0) >= 0)
		{
			return 403;
		}
		else if (strRecv.Find(L"40", 0) >= 0)
		{
			return 400;
		}
		return 3;
	}
	strRecv.Delete(0, strJsonstart);
	s = strRecv.Find(_T("\"errorCode\":"));
	if (s == -1)
	{
		delete[] revbuf;
		return 4;
	}
	s = s + 12;
	e = strRecv.Find(_T(","), s);
	CString strRet = strRecv.Mid(s, e - s);
	if (strRet != "0")
	{
		delete[] revbuf;
		int intRet = atoi(T2A(strRet));
		return intRet;
	}
	//strRecv = Utf8ToStringT(T2A(strRecv));
	strcpy_s(chJson, jsonlen, T2A(strRecv));
	delete[] revbuf;
	return 0;
}
//黑名单人脸库添加
int AddBlack(CString strIP, CString strSession, CString strName, char*chJson, int jsonlen)
{
	USES_CONVERSION;
	strName = UrlEncode(strName);
	CString strContent = L"name=" + strName;
	int iContentLen = strContent.GetLength();
	CString strSend;
	strSend.Format(L"%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s",
		L"POST /api/v1/facedb/black HTTP/1.1\n",
		L"Accept: */*\n",
		L"Host: ", strIP, L"\n",
		L"Content-Length: ", iContentLen, L"\n",
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
	int l = strSend.GetLength();
	int sendlen = send(sockClient, T2A(strSend), strSend.GetLength(), 0);
	if (sendlen == 0 || sendlen == SOCKET_ERROR)
		return 10;
	char *revbuf = new char[1024];
	memset(revbuf, '\0', 1024);
	recv(sockClient, revbuf, 1024, 0);
	closesocket(sockClient);
	//整理json结构
	CString strRecv;
	strRecv = revbuf;
	int s(0), e(0);
	int strJsonstart = strRecv.Find(L"{", 0);
	if (strJsonstart == -1)
	{
		delete[] revbuf;
		if (strRecv.Find(L"403", 0) >= 0)
		{
			return 403;
		}
		else if (strRecv.Find(L"40", 0) >= 0)
		{
			return 400;
		}
		return 3;
	}
	strRecv.Delete(0, strJsonstart);
	s = strRecv.Find(_T("\"errorCode\":"));
	if (s == -1)
	{
		delete[] revbuf;
		return 4;
	}
	s = s + 12;
	e = strRecv.Find(_T(","), s);
	CString strRet = strRecv.Mid(s, e - s);
	if (strRet != "0")
	{
		delete[] revbuf;
		int intRet = atoi(T2A(strRet));
		return intRet;
	}
	strcpy_s(chJson, jsonlen, T2A(strRecv));
	delete[] revbuf;
	return 0;
}
//黑名单人脸库修改
int EditBlack(CString strIP, CString strSession, CString strID, CString strName, char*chJson, int jsonlen)
{
	USES_CONVERSION;
	strName = UrlEncode(strName+L" ");
	CString strPut = L"PUT /api/v1/facedb/black/" + strID + L" HTTP/1.1\n";
	CString strContent = L"name=" + strName;
	int iContentLen = strContent.GetLength();
	CString strSend;
	strSend.Format(L"%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s",
		strPut,
		L"Accept: */*\n",
		L"Host: ", strIP, L"\n",
		L"Content-Length: ", iContentLen, L"\n",
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
	int l = strSend.GetLength();
	int sendlen = send(sockClient, T2A(strSend), strSend.GetLength(), 0);
	if (sendlen == 0 || sendlen == SOCKET_ERROR)
		return 10;
	char *revbuf = new char[1024];
	memset(revbuf, '\0', 1024);
	recv(sockClient, revbuf, 1024, 0);
	closesocket(sockClient);
	//整理json结构
	CString strRecv;
	strRecv = revbuf;
	int s(0), e(0);
	int strJsonstart = strRecv.Find(L"{", 0);
	if (strJsonstart == -1)
	{
		delete[] revbuf;
		if (strRecv.Find(L"403", 0) >= 0)
		{
			return 403;
		}
		else if (strRecv.Find(L"40", 0) >= 0)
		{
			return 400;
		}
		return 3;
	}
	strRecv.Delete(0, strJsonstart);
	s = strRecv.Find(_T("\"errorCode\":"));
	if (s == -1)
	{
		delete[] revbuf;
		return 4;
	}
	s = s + 12;
	e = strRecv.Find(_T(","), s);
	CString strRet = strRecv.Mid(s, e - s);
	if (strRet != "0")
	{
		delete[] revbuf;
		int intRet = atoi(T2A(strRet));
		return intRet;
	}
	strcpy_s(chJson, jsonlen, T2A(strRecv));
	delete[] revbuf;
	return 0;
}
//黑名单人脸库删除
int DeleteBlack(CString strIP, CString strSession, CString strID, char*chJson, int jsonlen)
{
	USES_CONVERSION;

	CString strSend;
	CString strDelete = L"DELETE /api/v1/facedb/black/" + strID + L" HTTP/1.1\n";
	strSend.Format(_T("%s%s%s%s%s%s%s%s%s"),
		strDelete,
		L"Accept-Language: zh-CN\n",
		L"Host: ", strIP, L":8080\n",
		L"Cookie: SESSION=", strSession, L"\r\n",
		L"Connection: close\n\n"
		);

	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(T2A(strIP.GetBuffer(0)));//该处地址应为空闲一体机地址
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8080);
	int conre = connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	int len1 = strSend.GetLength();
	int sendre = send(sockClient, T2A(strSend.GetBuffer(0)), len1, 0);
	char *revbuf = new char[1024];
	memset(revbuf, '\0', 1024);
	recv(sockClient, revbuf, 1024, 0);
	closesocket(sockClient);
	//整理json结构
	CString strRecv;
	strRecv = revbuf;
	int s(0), e(0);
	int strJsonstart = strRecv.Find(L"{", 0);
	if (strJsonstart == -1)
	{
		delete[] revbuf;
		if (strRecv.Find(L"403", 0) >= 0)
		{
			return 403;
		}
		else if (strRecv.Find(L"40", 0) >= 0)
		{
			return 400;
		}
		return 3;
	}
	strRecv.Delete(0, strJsonstart);
	s = strRecv.Find(_T("\"errorCode\":"));
	if (s == -1)
	{
		delete[] revbuf;
		return 4;
	}
	s = s + 12;
	e = strRecv.Find(_T(","), s);
	CString strRet = strRecv.Mid(s, e - s);
	if (strRet != "0")
	{
		delete[] revbuf;
		int intRet = atoi(T2A(strRet));
		return intRet;
	}
	strcpy_s(chJson, jsonlen, T2A(strRecv));
	delete[] revbuf;
	return 0;
}