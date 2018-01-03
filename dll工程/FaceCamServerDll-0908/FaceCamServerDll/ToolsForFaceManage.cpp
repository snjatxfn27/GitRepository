#include "stdafx.h"
#include "ToolsForFaceManage.h"
//人脸库人员分页查询
int GetFaceListPageInDb(CString strIP, CString strSession, CString strFaceDbId, CString IdCard, char*chJson, int jsonlen)
{
	USES_CONVERSION;
	CString strContent;
	strContent.Format(L"%s%s%s%s", L"dbIds=", strFaceDbId, L"&idCard=", IdCard);
	int iContentLen = strContent.GetLength();
	CString strSend;
	strSend.Format(L"%s%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s%s%s%s",
		L"POST /api/v1/person/query HTTP/1.1\n",
		L"Date: Mon, 21 Mar 2016 11:30:17 GMT\n",
		L"Accept: */*\n",
		L"Host: ", strIP, L"\n",
		L"Content-Length: ", iContentLen, L"\n",
		L"Accept-Language: zh-cn\n",
		L"User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1)\n",
		L"Pragma: no-cache\n",
		L"Content-Type: application/x-www-form-urlencoded\r\n",
		L"Cookie: SESSION=", strSession, L"\r\n",
		L"Connection:close\n\n",
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
		return 10;
	char *revbuf = new char[1024*128];
	memset(revbuf, '\0', 1024 * 128);
	int iAllRevLen = 0;
	while (1)
	{
		int revlen = recv(sockClient, revbuf + iAllRevLen, 1024 , 0);
		if (revlen == 0)
			break;
		else
			iAllRevLen += revlen;
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
	strcpy_s(chJson, jsonlen, T2A(strRecv));
	delete[] revbuf;
	return 0;
}

//给人脸库添加一个人脸信息
int AddPersoninfo(CString strIP, CString strSession, CString strName, CString IdCard, int intGender, long longFaceDbId, CString strFeatures, CString strImageData, CString strBirthday, CString strValidTo, char * pPossonId, int nPossonLen)
{
// 	CFile file;
// 	CString csFile;
// 	csFile.Format(_T("%s.txt"), strName);
// 	file.Open(csFile, CFile::modeCreate | CFile::modeWrite);
// 	file.Write(strFeatures, strFeatures.GetLength()); //写入实际数据  
// 	file.Close();
	USES_CONVERSION;
	CString strContent;
	strContent.Format(L"%s%s%s%s%s%d%s%ld%s%s%s%s%s%s%s%s", L"name=", strName, L"&idCard=", IdCard, L"&Gender=", intGender, L"&dbId=", longFaceDbId, L"&features=", strFeatures, L"&imageData=", strImageData, L"&birthday=", strBirthday,L"&validTo=", strValidTo);
	strContent.Replace(L"+", L"%2B");
	int iContentLen = strContent.GetLength();
	CString strSend;
	strSend.Format(L"%s%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s%s%s%s",
		L"POST /api/v1/person HTTP/1.1\n",
		L"Date: Mon, 21 Mar 2016 11:30:17 GMT\n",
		L"Accept: */*\n",
		L"Host: ", strIP, L"\n",
		L"Content-Length: ", iContentLen, L"\n",
		L"Accept-Language: zh-cn\n",
		L"User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1)\n",
		L"Pragma: no-cache\n",
		L"Content-Type: application/x-www-form-urlencoded\r\n",
		L"Cookie: SESSION=", strSession, L"\r\n",
		L"Connection:  close\n\n",
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
	char * revbuf=new char[1024];
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
	int ns, ne;
	ns = strRecv.Find(_T("\"id\":"));
	if (ns == -1)
	{
		delete revbuf;
		return 400;
	}
	ns = ns + 5;
	ne = strRecv.Find(_T(","),ns);
	CString csPersonid = strRecv.Mid(ns, ne - ns);

	strcpy_s(pPossonId, nPossonLen, T2A(csPersonid));
	delete revbuf;
	return 0;
}

//人脸库编辑一个人脸信息
int UpdatePersoninfo(CString strIP, CString strSession, CString strPersonid, CString IdCard,  long longFaceDbId)
//int UpdatePersoninfo(CString strIP, CString strSession, CString strPersonid, CString strName, CString IdCard, int intGender, long longFaceDbId, CString strFeatures, CString strImageData, CString strBirthday, CString strValidTo)
{
	USES_CONVERSION;
	CString strContent;
//	strContent.Format(L"%s%s%s%s%s%d%s%ld%s%s%s%s%s%s%s%s", L"name=", strName, L"&idCard=", IdCard, L"&Gender=", intGender, L"&dbId=", longFaceDbId, L"&features=", strFeatures, L"&imageData=", strImageData, L"&birthday=", strBirthday, L"&validTo=", strValidTo);
	strContent.Format(L"%s%s%s%d", L"idCard=", IdCard, L"&dbId=", longFaceDbId);
	strContent.Replace(L"+", L"%2B");
	int iContentLen = strContent.GetLength();
	CString strSend;
	strSend.Format(L"%s%s%s%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s%s%s%s",
		L"PUT  /api/v1/person/", strPersonid, L" HTTP/1.1\n",
		L"Date: Mon, 21 Mar 2016 11:30:17 GMT\n",
		L"Accept: */*\n",
		L"Host: ", strIP, L"\n",
		L"Content-Length: ", iContentLen, L"\n",
		L"Accept-Language: zh-cn\n",
		L"User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1)\n",
		L"Pragma: no-cache\n",
		L"Content-Type: application/x-www-form-urlencoded\r\n",
		L"Cookie: SESSION=", strSession, L"\r\n",
		L"Connection:  close\n\n",
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
	s = s + 12;
	e = strRecv.Find(_T(","), s);
	CString strRet = strRecv.Mid(s, e - s);
	if (strRet != "0")
	{
		delete[] revbuf;
		int intRet = atoi(T2A(strRet));
		return intRet;
	}
	delete[] revbuf;
	return 0;
}

//通过人脸库personid删除一条Person信息
int DELETEPersoninfo(CString strIP, CString strSession, CString strPersonid)
{
	USES_CONVERSION;
	CString strContent = L"";
	int iContentLen = strContent.GetLength();
	CString strSend;
	strSend.Format(L"%s%s%s%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s%s%s%s%s",
		L"DELETE /api/v1/person/", strPersonid, L" HTTP/1.1\n",
		L"Date: Mon, 21 Mar 2016 11:30:17 GMT\n",
		L"Accept: */*\n",
		L"Host: ", strIP, L"\n",
		L"Content-Length: ", iContentLen, L"\n",
		L"Accept-Language: zh-cn\n",
		L"User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1)\n",
		L"Pragma: no-cache\n",
		L"Content-Type: application/x-www-form-urlencoded\r\n",
		L"Cookie: SESSION=", strSession, L"\r\n",
		L"Connection:close\n\n",
		L"Upgrade-Insecure-Requests: 1\r\n",
		strContent, L"\n"
		);
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
		return 10;
	char *revbuf=new char[1024 ];
	memset(revbuf, '\0', 1024 );
	recv(sockClient, revbuf, 1024 , 0);
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
	delete[] revbuf;
	return 0;
}


//通过人脸库personid获取人脸库中人员的的特征和面部照片信息
int GetFaceInDb(CString strIP, CString strSession, CString strPersonid, char*chJson, int jsonlen)
{
	USES_CONVERSION;
	CString strContent = L"";
	int iContentLen = strContent.GetLength();
	CString strSend;
	strSend.Format(L"%s%s%s%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s%s%s%s%s",
		L"GET /api/v1/person/", strPersonid, L"/features HTTP/1.1\n",
		L"Date: Mon, 21 Mar 2016 11:30:17 GMT\n",
		L"Accept: */*\n",
		L"Host: ", strIP, L"\n",
		L"Content-Length: ", iContentLen, L"\n",
		L"Accept-Language: zh-cn\n",
		L"User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1)\n",
		L"Pragma: no-cache\n",
		L"Content-Type: application/x-www-form-urlencoded\r\n",
		L"Cookie: SESSION=", strSession, L"\r\n",
		L"Connection:close\n\n",
		L"Upgrade-Insecure-Requests: 1\r\n",
		strContent, L"\n"
		);
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
//给人脸库添加多个人脸信息
int BatchAddPersoninfo(CString strIP, CString strSession, struct batchPerson1 *sbatchPersonlist, int nNumber)
{
	USES_CONVERSION;
	CString strContent;
	CString strContenttemp,csTemp;
	int i = 0;
	for (i = 0; i < nNumber; i++)
	{
		csTemp.Format(L"%s%s%s%s%s%d%s%ld%s%s%s%s%s%s%s", L"{\"name\":\"", A2T(sbatchPersonlist[i].pName),
			L"\",\"idCard\":\"", A2T(sbatchPersonlist[i].pIdCard), 
			L"\",\"gender\":", sbatchPersonlist[i].intGender,
			L",\"dbId\":",sbatchPersonlist[i].longFaceDbId, 
			L",\"birthday\":\"", A2T(sbatchPersonlist[i].pBirthday),
			L"\",\"features\":\"", A2T(sbatchPersonlist[i].pFeatures), 
			L"\",\"imageData\":\"",A2T(sbatchPersonlist[i].pImageData), 
			L"\"}");
		csTemp.Replace(L"+", L"%2B");
		csTemp = csTemp + L",";
		strContenttemp += csTemp;
	}
	strContenttemp = strContenttemp.Mid(0, strContenttemp.GetLength() - 1);
	strContent = L"[" + strContenttemp+ L"]";
	int iContentLen = strContent.GetLength();
	CString strSend;
	strSend.Format(L"%s%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s%s%s%s",
		L"POST /api/v1/person/import HTTP/1.1\n",
		L"Date: Mon, 21 Mar 2016 11:30:17 GMT\n",
		L"Accept: */*\n",
		L"Host: ", strIP, L"\n",
		L"Content-Length: ", iContentLen, L"\n",
		L"Accept-Language: zh-cn\n",
		L"User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1)\n",
		L"Pragma: no-cache\n",
		L"Content-Type: application/json\r\n",
		L"Cookie: SESSION=", strSession, L"\r\n",
		L"Connection:  close\n\n",
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
	s = s + 12;
	e = strRecv.Find(_T(","), s);
	CString strRet = strRecv.Mid(s, e - s);
	if (strRet != "0")
	{
		delete[] revbuf;
		int intRet = atoi(T2A(strRet));
		return intRet;
	}
	delete[] revbuf;
	return 0;
}