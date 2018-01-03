#include "stdafx.h"
#include "ToolsForRecognition.h"
#include "Tools.h"
#include <exception>
//#include "include/json/json.h"
//#pragma comment(lib,"../FaceCamServerDll/json_vc71_libmtd.lib")

//��ȡ��������base64
int FeatureDetect(CString strIP, CString strSession, CString strImageb64, char * chJson, int jsonlen)
{
	USES_CONVERSION;
	CString strContent = L"image=" + strImageb64;
	strContent.Replace(L"+", L"%2B");
	int iContentLen = strContent.GetLength();
	CString strSend;
	strSend.Format(L"%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s",
		L"POST /api/v1/tool/detectFace HTTP/1.1\n",
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
	addrSrv.sin_addr.S_un.S_addr = inet_addr(T2A(strIP));//�ô���ַӦΪ����һ�����ַ
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
	int sendlen = send(sockClient, T2A(strSend), strSend.GetLength(), 0);
	if (sendlen == 0 || sendlen == SOCKET_ERROR)
		return 10;
	memset(chJson, '\0', jsonlen);
	int iAllRevLen = 0;
	while (1)
	{
		int revlen = recv(sockClient, chJson + iAllRevLen, 1024 * 80, 0);
		if (revlen == 0)
			break;
		else
			iAllRevLen += revlen;
		Sleep(1);
	}
	closesocket(sockClient);
	//����json�ṹ
	CString strRecv;
	strRecv = chJson;
	int s(0), e(0);
	int strJsonstart = strRecv.Find(L"{", 0);
	if (strJsonstart == -1)
	{
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
		return 4;
	}
    s =	s + 12;
	e = strRecv.Find(_T(","), s);
	CString strRet = strRecv.Mid(s, e - s);
	if (strRet != "0")
	{
		int intRet = atoi(T2A(strRet));
		return intRet;
	}
	strcpy_s(chJson, jsonlen, T2A(strRecv));
	return 0;
}

//��ȡ����������ͼƬ(VC++ʹ��)
int FeatureDetectforImageFile(CString strIP, CString strSession ,CString strFilePath, char*chJson, int jsonlen)
{
	
	USES_CONVERSION;
	//��ȡ�ļ�
	CFile file;
	int filereadstatus=file.Open(strFilePath, CFile::modeRead);
	if (filereadstatus == 0)
	{
		return 5;//��Ƭ������
	}
	int filelen = file.GetLength();
	char *imagebuf = new char[filelen + 1];
	memset(imagebuf, 0, filelen + 1);
	file.Read(imagebuf, filelen);
	file.Close();
	filelen += 1;
	CString strHeader;
	strHeader.Format(L"%s%s%s%s%s%s%s%s%s%s%s",
		L"POST /api/v1/tool/detectFaceInFile HTTP/1.1\r\n",
		L"Host: ", strIP, L":8080\r\n",
		L"Accept: */*\r\n",
		//		L"Pragma: no-cache\r\n",
		L"Content-Type: multipart/form-data; boundary=----WebKitFormBoundaryA4IcDySIFFEKMEXb\r\n",
		L"Cookie: SESSION=", strSession, L"\r\n",
		L"X-Requested-With: XMLHttpRequest\r\n",
		L"Connection: close\r\n");
	CString strContect;
	strContect = L"------WebKitFormBoundaryA4IcDySIFFEKMEXb\r\n";
	strContect += L"Content-Disposition: form-data; name=\"file\"; filename=\"li1-4.jpg\"\r\n";
	strContect += L"Content-Type: image/jpeg \r\n\r\n";

	int la = strContect.GetLength();
	CString strNex = L"\r\n------WebKitFormBoundaryA4IcDySIFFEKMEXb--";
	int iContentLen = filelen + strContect.GetLength() + strNex.GetLength();
	CString strAdd;
	strAdd.Format(L"Content-Length: %d\r\n\r\n", iContentLen);
	strHeader += strAdd;

	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(T2A(strIP));//�ô���ַӦΪ����һ�����ַ
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8080);
	connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));

	int re = 0; int sendlen = 0;
	int headLen = strHeader.GetLength();
	int contentLen = strContect.GetLength();
	int nexLen = strNex.GetLength();
	sendlen = headLen + contentLen + filelen + nexLen;
	char *sendbuf = new char[sendlen];
	memset(sendbuf, '\0', sendlen);
	memcpy_s(sendbuf, sendlen, T2A(strHeader.GetBuffer(0)), headLen);
	memcpy_s(sendbuf + headLen, sendlen, T2A(strContect.GetBuffer(0)), contentLen);
	memcpy_s(sendbuf + headLen + contentLen, sendlen, imagebuf, filelen);
	memcpy_s(sendbuf + headLen + contentLen + filelen, sendlen, T2A(strNex.GetBuffer(0)), nexLen);
	re = send(sockClient, sendbuf, sendlen, 0);
	memset(chJson, '\0', jsonlen);
	int index = 0;
	int revlen = 0;
	int i = 0;
	while (jsonlen != 0)
	{
		//revlen = recv(sockClient, chJson + iAllRevLen, 1024 * 80, 0);
		//if (revlen == 0)
		//	break;
		//else
		//	iAllRevLen += revlen;
		//Sleep(1);
		revlen = recv(sockClient, (char*)chJson + index, jsonlen, 0);
		if (revlen == SOCKET_ERROR) break;
		else if (revlen == 0) break;
		jsonlen -= revlen;
		index += revlen;
	}

	closesocket(sockClient);
	CString strRecv = A2T(chJson);
	int s(0), e(0);
	int strJsonstart = strRecv.Find(L"{", 0);
	if (strJsonstart == -1)
	{
		delete[] imagebuf;
		delete[] sendbuf;
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
		delete[] imagebuf;
		delete[] sendbuf;
		return 4;
	}
	s=s + 12;
	e = strRecv.Find(_T(","), s);
	CString strRet = strRecv.Mid(s, e - s);
	if (strRet != "0")
	{
		delete[] imagebuf;
		delete[] sendbuf;
		int intRet = atoi(T2A(strRet));
		return intRet;
	}
	strcpy_s(chJson, jsonlen,T2A(strRecv));
	delete[] imagebuf;
	delete[] sendbuf;
	return 0;
}

//��ȡ����������ͼƬ(ASP.netʹ�ã�����ֵ��󲻳���100kb,dll�����еľֲ�����Ҫ��new��delete)
int FeatureDetectforImageFileForNet(CString strIP, CString strSession, CString strFilePath, char*chJson, int jsonlen)
{

	USES_CONVERSION;
	//��ȡ�ļ�
	CFile file;
	int filereadstatus=file.Open(strFilePath, CFile::modeRead);
	if (filereadstatus == 0)
	{
		return 5;//��Ƭ������
	}
	int filelen = file.GetLength();
	char *imagebuf = new char[filelen + 1];
	memset(imagebuf, 0, filelen + 1);
	file.Read(imagebuf, filelen);
	file.Close();
	filelen += 1;
	CString strHeader;
	strHeader.Format(L"%s%s%s%s%s%s%s%s%s%s%s",
		L"POST /api/v1/tool/detectFaceInFile HTTP/1.1\r\n",
		L"Host: ", strIP, L":8080\r\n",
		L"Accept: */*\r\n",
		//		L"Pragma: no-cache\r\n",
		L"Content-Type: multipart/form-data; boundary=----WebKitFormBoundaryA4IcDySIFFEKMEXb\r\n",
		L"Cookie: SESSION=", strSession, L"\r\n",
		L"X-Requested-With: XMLHttpRequest\r\n",
		L"Connection: close\r\n");
	CString strContect;
	strContect = L"------WebKitFormBoundaryA4IcDySIFFEKMEXb\r\n";
	strContect += L"Content-Disposition: form-data; name=\"file\"; filename=\"li1-4.jpg\"\r\n";
	strContect += L"Content-Type: image/jpeg \r\n\r\n";

	int la = strContect.GetLength();
	CString strNex = L"\r\n------WebKitFormBoundaryA4IcDySIFFEKMEXb--";
	int iContentLen = filelen + strContect.GetLength() + strNex.GetLength();
	CString strAdd;
	strAdd.Format(L"Content-Length: %d\r\n\r\n", iContentLen);
	strHeader += strAdd;

	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(T2A(strIP));//�ô���ַӦΪ����һ�����ַ
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8080);
	connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));

	int re = 0; int sendlen = 0;
	int headLen = strHeader.GetLength();
	int contentLen = strContect.GetLength();
	int nexLen = strNex.GetLength();
	sendlen = headLen + contentLen + filelen + nexLen;
	char *sendbuf = new char[sendlen];
	memset(sendbuf, '\0', sendlen);
	memcpy_s(sendbuf, sendlen, T2A(strHeader.GetBuffer(0)), headLen);
	memcpy_s(sendbuf + headLen, sendlen, T2A(strContect.GetBuffer(0)), contentLen);
	memcpy_s(sendbuf + headLen + contentLen, sendlen, imagebuf, filelen);
	memcpy_s(sendbuf + headLen + contentLen + filelen, sendlen, T2A(strNex.GetBuffer(0)), nexLen);
	re = send(sockClient, sendbuf, sendlen, 0);
	memset(chJson, '\0', jsonlen);
	int index = 0;
	int revlen = 0;
	int i = 0;
	while (jsonlen != 0)
	{
		//revlen = recv(sockClient, chJson + iAllRevLen, 1024 * 80, 0);
		//if (revlen == 0)
		//	break;
		//else
		//	iAllRevLen += revlen;
		//Sleep(1);
		revlen = recv(sockClient, (char*)chJson + index, jsonlen, 0);
		if (revlen == SOCKET_ERROR) break;
		else if (revlen == 0) break;
		jsonlen -= revlen;
		index += revlen;
	}

	closesocket(sockClient);
	//CString strRecv = A2W(chJson);
	//int a = WideCharToMultiByte(CP_ACP, 0, strRecv, -1, NULL, 0, NULL, NULL);
	//����char *�����С�����ֽ�Ϊ��λ��һ������ռ�����ֽ�
	int charLen = strlen(chJson);
	//������ֽ��ַ��Ĵ�С�����ַ����㡣
	int len = MultiByteToWideChar(CP_ACP, 0, chJson, charLen, NULL, 0);
	//Ϊ���ֽ��ַ���������ռ䣬�����СΪ���ֽڼ���Ķ��ֽ��ַ���С
	TCHAR *buf = new TCHAR[len + 1];
	//���ֽڱ���ת���ɿ��ֽڱ���
	MultiByteToWideChar(CP_ACP, 0, chJson, charLen, buf, len);
	buf[len] = '\0';  //����ַ�����β��ע�ⲻ��len+1
	//��TCHAR����ת��ΪCString
	CString strRecv;
	strRecv.Append(buf);
	//ɾ��������
	delete[]buf;
	int s(0), e(0);
	int strJsonstart = strRecv.Find(L"{", 0);
	if (strJsonstart == -1)
	{
		delete[] imagebuf;
		delete[] sendbuf;
		if (strRecv.Find(L"403", 0)>=0)
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
	s=strRecv.Find(_T("\"errorCode\":"));
	if (s == -1)
	{
		delete[] imagebuf;
		delete[] sendbuf;
		return 4;
	}
	s=s + 12;
	e = strRecv.Find(_T(","), s);
	CString strRet = strRecv.Mid(s, e - s);
	if (strRet != "0")
	{
		delete[] imagebuf;
		delete[] sendbuf;
		int intRet = atoi(T2A(strRet));
		return intRet;
	}
	int getlength = strRecv.GetLength();
	strcpy_s(chJson, jsonlen, T2A(strRecv));
	delete[] imagebuf;
	delete[] sendbuf;
	return 0;
}