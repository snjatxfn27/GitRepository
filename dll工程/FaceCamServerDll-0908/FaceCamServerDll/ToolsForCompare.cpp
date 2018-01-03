#include "stdafx.h"
#include "ToolsForCompare.h"

//特征比对特征
int FeatureMatch(CString strIP, CString strSession,CString strFeature1, CString strFeature2, char*chJson, int jsonlen)
{
	USES_CONVERSION;
	CString strContent = L"feature1=" + strFeature1 + L"&feature2=" + strFeature2;
	strContent.Replace(L"+", L"%2B");
	int iContentLen = strContent.GetLength();
	CString strSend;
	strSend.Format(L"%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s",
		L"POST /api/v1/tool/featureMatch HTTP/1.1\n",
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

	int tv_out_ms=1000;
	int intsocketststus;
	intsocketststus=setsockopt(sockClient, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv_out_ms, sizeof(tv_out_ms));
	int m_connectstatus=connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	if (SOCKET_ERROR == m_connectstatus)
	{
		return 10;//socket connect failed
	}
	intsocketststus=setsockopt(sockClient, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv_out_ms, sizeof(tv_out_ms));
	if (intsocketststus == SOCKET_ERROR)
		return 10;
	int l = strSend.GetLength();
	int sendlen = send(sockClient, T2A(strSend), strSend.GetLength(), 0);
	if (sendlen == 0 || sendlen == SOCKET_ERROR)
		return 10;
	char * revbuf = new char[1024];
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
	s=s + 12;
	e = strRecv.Find(_T(","), s);
	CString strRet = strRecv.Mid(s, e - s);
	if (strRet != "0")
	{
		int intRet = atoi(T2A(strRet));
		return intRet;
	}
	strcpy_s(chJson, jsonlen, T2A(strRecv));
	delete[] revbuf;
	return 0;
}

//特征比对人脸库
int MatchPerson(CString strIP, CString strSession,CString strDBs, double sim, CString strFeature, char*chJson, int jsonlen)
{
	USES_CONVERSION;
	CString strSim;
	strSim.Format(L"%f", sim);
	CString strContent = L"dbIds=" + strDBs + L"&similarity=" + strSim + L"&feature=" + strFeature;
	strContent.Replace(L"+", L"%2B");
	int iContentLen = strContent.GetLength();
	CString strSend;
	strSend.Format(L"%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s",
		L"POST /api/v1/person/match HTTP/1.1\n",
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
	char *revbuf = new char[1024 * 1024];
	memset(revbuf, '\0', 1024 * 1024);
	recv(sockClient, revbuf, 1024 * 1024, 0);
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
	s=s + 12;
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
