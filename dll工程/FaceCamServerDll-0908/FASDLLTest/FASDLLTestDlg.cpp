
// FASDLLTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FASDLLTest.h"
#include "FASDLLTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFASDLLTestDlg �Ի���



CFASDLLTestDlg::CFASDLLTestDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CFASDLLTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFASDLLTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFASDLLTestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_LOGIN, &CFASDLLTestDlg::OnBnClickedBtnLogin)
	ON_BN_CLICKED(IDC_BTN_FEATUREDETECT, &CFASDLLTestDlg::OnBnClickedBtnFeaturedetect)
	ON_BN_CLICKED(IDC_BTN_FEATUREDETECTFORJPG, &CFASDLLTestDlg::OnBnClickedBtnFeaturedetectforjpg)
	ON_BN_CLICKED(IDC_BTN_GETPERSONFACE, &CFASDLLTestDlg::OnBnClickedBtnGetpersonface)
	ON_BN_CLICKED(IDC_BTN_ADDPERSONINFO, &CFASDLLTestDlg::OnBnClickedBtnAddpersoninfo)
	ON_BN_CLICKED(IDC_BTN_FEATURECOMPARE, &CFASDLLTestDlg::OnBnClickedBtnFeaturecompare)
	ON_BN_CLICKED(IDC_BTN_CREATEBLACKDB, &CFASDLLTestDlg::OnBnClickedBtnCreateblackdb)
	ON_BN_CLICKED(IDC_BTN_BLACKDBQUERY, &CFASDLLTestDlg::OnBnClickedBtnBlackdbquery)
	ON_BN_CLICKED(IDC_BTN_UPDATEBLACKDB, &CFASDLLTestDlg::OnBnClickedBtnUpdateblackdb)
	ON_BN_CLICKED(IDC_BTN_DELBLACKDB, &CFASDLLTestDlg::OnBnClickedBtnDelblackdb)
	ON_BN_CLICKED(IDC_BTN_BATCHADDPERSON, &CFASDLLTestDlg::OnBnClickedBtnBatchaddperson)
	ON_BN_CLICKED(IDC_BTN_UPDATEPERSON, &CFASDLLTestDlg::OnBnClickedBtnUpdateperson)
	ON_BN_CLICKED(IDC_BTN_QUERYPERSON, &CFASDLLTestDlg::OnBnClickedBtnQueryperson)
	ON_BN_CLICKED(IDC_BTN_DELETEPERSON, &CFASDLLTestDlg::OnBnClickedBtnDeleteperson)
	ON_BN_CLICKED(IDC_BTN_ASPNETGETFEATURE, &CFASDLLTestDlg::OnBnClickedBtnAspnetgetfeature)
	ON_BN_CLICKED(IDC_BTN_IMAGEFILECOMPARE, &CFASDLLTestDlg::OnBnClickedBtnImagefilecompare)
	ON_BN_CLICKED(IDC_BTN_IMAGEFILECOMPARE2, &CFASDLLTestDlg::OnBnClickedBtnImagefilecompare2)
	ON_BN_CLICKED(IDC_BUTTON3, &CFASDLLTestDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BTN_CREATEWDB, &CFASDLLTestDlg::OnBnClickedBtnCreatewdb)
END_MESSAGE_MAP()


// CFASDLLTestDlg ��Ϣ�������

BOOL CFASDLLTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFASDLLTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CFASDLLTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//base64����
int CFASDLLTestDlg::EncodeBase64(const unsigned char* pSrc, char* pDst, int nSrcLen, int nMaxLineLen)
{
	char EnBase64Tab[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	unsigned char c1, c2, c3;    // ���뻺��������3���ֽ�
	int nDstLen = 0;             // ������ַ�����
	int nLineLen = 0;            // ������г��ȼ���
	int nDiv = nSrcLen / 3;      // �������ݳ��ȳ���3�õ��ı���
	int nMod = nSrcLen % 3;      // �������ݳ��ȳ���3�õ�������

	// ÿ��ȡ3���ֽڣ������4���ַ�
	for (int i = 0; i < nDiv; i++)
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
int CFASDLLTestDlg::DecodeBase64(const char* pSrc, unsigned char* pDst, int nSrcLen)
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
		if (*pSrc != '\r' && *pSrc != '\n')
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
					*pDst++ = nValue & 0x000000ff;
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

CString CFASDLLTestDlg::UrlEncode(CString szToEncode)
{
	CString src = szToEncode;
	char hex[] = "0123456789ABCDEF";
	CString dst;

	for (size_t i = 0; i < src.GetLength(); ++i)
	{
		unsigned char cc = src[i];
		if (isascii(cc))
		{
			if (cc == ' ')
			{
				dst += "%20";
			}
			else
				dst += cc;
		}
		else
		{
			unsigned char c = static_cast<unsigned char>(src[i]);
			dst += '%';
			dst += hex[c / 16];
			dst += hex[c % 16];
		}
	}
	return dst;
}

void CFASDLLTestDlg::OnBnClickedBtnLogin()
{
	char pSession[37];
	const char * pIp = "192.168.2.30";
	int m_result = DeviceLogin(pIp, pSession);
	m_result = DeviceLogout(pIp, pSession);
}


void CFASDLLTestDlg::OnBnClickedBtnFeaturedetect()
{
	USES_CONVERSION;
	char pSession[37];
	const char * pIp = "192.168.2.30";
	int m_result = DeviceLogin(pIp, pSession);
	//CString strFileAllName = L"../img/3.jpg"; 
	//CString strFileAllName = L"../img/�Գ�.jpg";
	CString strFileAllName = L"../img/�����.jpg";
	CFile file;
	file.Open(strFileAllName, CFile::modeRead);
	int filelen = file.GetLength();
	char *imgbuf = new char[filelen + 1];
	memset(imgbuf, 0, filelen + 1);
	file.Read(imgbuf, filelen);
	file.Close();
	char *imgb64buf = new char[filelen * 4];
	memset(imgb64buf, 0, filelen * 4);
	int b64len = EncodeBase64((unsigned char*)imgbuf, imgb64buf, filelen, filelen * 4);
	file.Open(L"imgb64.txt", CFile::modeCreate | CFile::modeWrite);
	file.Write(imgb64buf, b64len);
	file.Close();

	//����jpg�ļ�
	char *imgsave = new char[filelen * 4];
	memset(imgsave, 0, filelen * 4);
	file.Open(L"fc.jpg", CFile::modeCreate | CFile::modeWrite);
	int fclen = DecodeBase64(imgb64buf, (unsigned char*)imgsave, b64len);
	file.Write(imgsave, fclen);
	file.Close();

	char *chJson = new char[1024 * 1024];
	FASFeatureDetect(pIp, pSession, imgb64buf, chJson, 1024 * 1024);
	delete[] imgbuf;
	delete[] imgb64buf;
}

void CFASDLLTestDlg::OnBnClickedBtnFeaturedetectforjpg()
{
	USES_CONVERSION;
	char pSession[37];
	const char * pIp = "192.168.2.31";
	int m_result = DeviceLogin(pIp, pSession);
	CString strFilePath = L"../img/�Գ�.jpg";
	//CString strFilePath = L"../img/�����.jpg";
	//CString strFilePath = L"../img/QQ��ͼ20170811163526.jpg";
	//CString strFilePath = L"../img/u=2193818201,1898788008&fm=26&gp=0.jpg";
	CFile file;
	int filereadstatus = file.Open(strFilePath, CFile::modeRead);
	int filelen = file.GetLength();
	int jsonlen = filelen * 2;
	file.Close();
	char *chJson = new char[jsonlen];
	memset(chJson, '\0', jsonlen);
	int b = FASFeatureDetectforImageFile(pIp, pSession, T2A(strFilePath), chJson, jsonlen);
	file.Open(L"feature.txt", CFile::modeCreate | CFile::modeWrite);
	file.Write(chJson, jsonlen);
	file.Close();
	delete chJson;

}
void CFASDLLTestDlg::OnBnClickedBtnAspnetgetfeature()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	USES_CONVERSION;
	// 	char pSession[37];
	// 	const char * pIp = "192.168.2.30";
	// 	int m_result = DeviceLogin(pIp, pSession);
	// 	//CString strFilePath = L"../img/3.jpg";
	// 	CString strFilePathIn = L"../img/QQ��ͼ20170811163526.jpg";
	// 	CString strFilePathOut = L"../img/QQ��ͼ20170811163526-photo.jpg";
	// 	//CString strFilePath = L"../img/u=2193818201,1898788008&fm=26&gp=0.jpg";
	// 	CFile file;
	// 	file.Open(strFilePathIn, CFile::modeRead);
	// 	int filelen = file.GetLength();
	// 	int jsonlen = filelen * 16;
	// 	file.Close();
	// 	char *chJson = new char[jsonlen];
	// 	memset(chJson, '\0', jsonlen);
	// 	int b = FASFeatureDetectforImageFileForNet(pIp, pSession, T2A(strFilePathIn), T2A(strFilePathOut), chJson, jsonlen);
	// 	file.Open(L"feature.txt", CFile::modeCreate | CFile::modeWrite);
	// 	file.Write(chJson, jsonlen);
	// 	file.Close();
	// 	delete chJson;
}



void CFASDLLTestDlg::OnBnClickedBtnGetpersonface()
{
	USES_CONVERSION;
	char pSession[37];
	const char * pIp = "192.168.2.30";
	int m_result = DeviceLogin(pIp, pSession);
	const char * pPersonid = "100052";
	char chJson[800 * 1024];
	int result = FASGetFaceInDb(pIp, pSession, pPersonid, chJson, 800 * 1024);
}


void CFASDLLTestDlg::OnBnClickedBtnAddpersoninfo()
{
	USES_CONVERSION;
	char pSession[37];
	const char * pIp = "192.168.2.31";
	int m_result = DeviceLogin(pIp, pSession);
	CString strName = L"ss";
	CString IdCard = L"1234567890";
	int intGender = 0;
	long longFaceDbId = 10328;
	CString strBirthday = L"1990-01-01";
	CString strValidTo = L"2090-01-01";

	//CString strFileAllName = L"../img/����.jpg"; 
	CString strFilePath = L"../img/21764.jpg";
	//CString strFileAllName = L"../img/�����.jpg";
	int jsonlen = 1024 * 1024 * 2;
	char *chJson = new char[jsonlen * 2];
	memset(chJson, '\0', jsonlen * 2);
	FASFeatureDetectforImageFile(pIp, pSession, T2A(strFilePath), chJson, jsonlen);
	int s(0), e(0);
	CString strRecv = A2T(chJson);
	s = strRecv.Find(_T("\"feature\":"));
	s = s + 11;
	e = strRecv.Find(_T(","), s);
	CString strFeatures = strRecv.Mid(s, e - 1 - s);
	s = strRecv.Find(_T("\"face_image\":"));
	s = s + 14;
	e = strRecv.Find(_T(","), s);
	CString ImageData = strRecv.Mid(s, e - 1 - s);
	CFile file;
	file.Open(L"imgb641.txt", CFile::modeCreate | CFile::modeWrite);
	file.Write(strFeatures, strFeatures.GetLength());
	file.Close();
	char possoi[128];
	int  nretur = FASAddPersoninfo(pIp, pSession, T2A(strName), T2A(IdCard), intGender, longFaceDbId, T2A(strFeatures), T2A(ImageData), T2A(strBirthday), T2A(strValidTo), possoi, 128);
	int t = 0;
}

void CFASDLLTestDlg::OnBnClickedBtnUpdateperson()
{
	USES_CONVERSION;
	char pSession[37];
	const char * pIp = "192.168.2.30";
	int m_result = DeviceLogin(pIp, pSession);
	CString strName = L"zhangsan";
	CString IdCard = L"1234567890";
	int intGender = 0;
	long longFaceDbId = 10052;
	CString strBirthday = L"1990-01-01";
	CString strValidTo = L"2090-01-01";
	char *pPersonid = "104845";
	CString strFeatures = L"";
	CString ImageData = L"";

	FASUpdatePersoninfo(pIp, pSession, pPersonid, T2A(IdCard), longFaceDbId);
}



void CFASDLLTestDlg::OnBnClickedBtnFeaturecompare()
{
	USES_CONVERSION;
	char pSession[37];
	const char * pIp = "192.168.2.30";
	int m_result = DeviceLogin(pIp, pSession);
	//CString strFileAllName = L"../img/3.jpg";
	//CString strFileAllName = L"../img/00a990969fd5d581d74b74347f866ef3.jpg";
	CString strFileAllName = L"../img/21764.jpg";
	CFile file;
	file.Open(strFileAllName, CFile::modeRead);
	int filelen = file.GetLength();
	char *imgbuf = new char[filelen + 1];
	memset(imgbuf, 0, filelen + 1);
	file.Read(imgbuf, filelen);
	file.Close();
	char *imgb64buf = new char[filelen * 2];
	memset(imgb64buf, 0, filelen * 2);
	int b64len = EncodeBase64((unsigned char*)imgbuf, imgb64buf, filelen, filelen * 2);
	file.Open(L"imgb64.txt", CFile::modeCreate | CFile::modeWrite);
	file.Write(imgb64buf, b64len);
	file.Close();

	//����jpg�ļ�
	//char *imgsave = new char[filelen * 2];
	//memset(imgsave, 0, filelen * 2);
	//file.Open(L"fc.jpg", CFile::modeCreate | CFile::modeWrite);
	//int fclen = DecodeBase64(imgb64buf, (unsigned char*)imgsave, b64len);
	//file.Write(imgsave, fclen);
	//file.Close();

	char chJson[1024 * 1024 * 2];
	FASFeatureDetect(pIp, pSession, imgb64buf, chJson, 1024 * 1024 * 2);
	int s(0), e(0);
	CString strRecv = A2T(chJson);
	s = strRecv.Find(_T("\"feature\":"));
	s = s + 11;
	e = strRecv.Find(_T(","), s);
	CString strFeatures = strRecv.Mid(s, e - 1 - s);
	// strFeatures = L"cG9wPApClrq5FcI7s2TMPQToXD05pVc+onqZPByoD72zuCs9pulMPei3A72fsMC8+IGBvfdO3L3IAXC9ZOkpPWLc6z1woBG9wThjvoI1Rb7tyQg93d0pvUy9mT1cBKc9KvWrPUbafT06O1Y+khyPPCVMNLqTD9i91TkqvrPibz0lcrq9ZRYlPnZyb73/sty9nGeLvKTu8z1SQhi+Y3StO5SyF72gic07LL6lvXkjUD0nHPM9Q5sQvcGSt7zvxmA8wlydvPY6D74cr649NiAZPQNTGL5Cth29jmI6vrhSGjtlbYK+ivquvRcWB71pZL28Cd3WPZFSJD2jhyU9Z5b4u7sQJr3+fZC9XY+/vc4UZT0WPpK9YwpZPf70Pr2bTbC87MuJvfs1aD4NpsU9m3K5PDG7LL3mW7I9Vcq/vcU7CT6yACI9eX3UPUozLjw5Rjs8dBCEPWAqlr1vE469fKXyvASJNr62kRG9slOFvRv6Qr3XBTc9nforPgaM471aqB8+mOAzPcZHGj6eAnU9dNxCvYxYBj0mt2A9hlsOPQ2pSr05x209+u6NPZpDbD1a3eG9BSxGO+4FDT4Bywe+ZzLaPcWpHD05qDC93r6DvfJPgT0rE5o9efWEvDhvpTuvl4e9WqILPlYkW0MAAIA/";

	char * pDBs = "10052";
	double douSim = 51.1;
	char chJsonMatch[1024 * 1024];
	int result = FASMatchPerson(pIp, pSession, pDBs, douSim, T2A(strFeatures), chJsonMatch, 1024 * 1024);
	DeviceLogout(pIp, pSession);

	delete[] imgbuf;
	delete[] imgb64buf;
}



void CFASDLLTestDlg::OnBnClickedBtnBlackdbquery()
{
	USES_CONVERSION;
	char pSession[37];
	const char * pIP = "192.168.2.30";
	int m_result = DeviceLogin(pIP, pSession);
	CString  strName = L"167";
	char chJson[800 * 1024];
	int result = FASCheckBlack(pIP, pSession, T2A(strName), chJson, 800 * 1024);
	int ttt = 0;
}

void CFASDLLTestDlg::OnBnClickedBtnCreateblackdb()
{
	USES_CONVERSION;
	char pSession[37];
	const char * pIP = "192.168.2.30";
	int m_result = DeviceLogin(pIP, pSession);
	CString  strName = L"��ã�����������4��";
	char chJson[800 * 1024];
	int result = FASAddBlack(pIP, pSession, T2A(strName), chJson, 800 * 1024);
}

void CFASDLLTestDlg::OnBnClickedBtnUpdateblackdb()
{
	USES_CONVERSION;
	char pSession[37];
	const char * pIP = "192.168.2.30";
	const char * pID = "10306";
	int m_result = DeviceLogin(pIP, pSession);
	CString  strName = L"��ã�����������6��";
	char chJson[800 * 1024];
	int result = FASEditBlack(pIP, pSession, pID, T2A(strName), chJson, 800 * 1024);
}


void CFASDLLTestDlg::OnBnClickedBtnDelblackdb()
{
	USES_CONVERSION;
	char pSession[37];
	const char * pIP = "192.168.2.30";
	const char * pID = "10308";
	int m_result = DeviceLogin(pIP, pSession);
	char chJson[800 * 1024];
	int result = FASDeleteBlack(pIP, pSession, pID, chJson, 800 * 1024);
}


void CFASDLLTestDlg::OnBnClickedBtnBatchaddperson()
{
	USES_CONVERSION;
	char pSession[37];
	const char * pIP = "192.168.2.30";
	int m_result = DeviceLogin(pIP, pSession);
	batchPerson1 sbatchPersonlist[10];

	CString strFileAllName = L"../img/144886.txt";
	CString strFileAllName1 = L"../img/144887.txt";
	CString strFileAllName2 = L"../img/144888.txt";
	CFile file;

	CString strName = L"zhangsan";
	CString IdCard = L"1234567890";
	int intGender = 0;
	long longFaceDbId = 10352;
	CString strBirthday = L"1990-01-01";
	CString strValidTo = L"2090-01-01";


	// 	file.Open(strFileAllName, CFile::modeRead);
	// 	int filelen = file.GetLength();
	// 	char *imgbuf = new char[filelen + 1];
	// 	memset(imgbuf, 0, filelen + 1);
	// 	file.Read(imgbuf, filelen);
	// 	file.Close();
	// 	char chImage[2] = { '1', '\0' };
	// 	(sbatchPersonlist[0].pName= T2A(strName));
	// 	(sbatchPersonlist[0].pIdCard= T2A(IdCard));
	//  	sbatchPersonlist[0].intGender = intGender;
	// 	sbatchPersonlist[0].longFaceDbId = longFaceDbId;
	// 	(sbatchPersonlist[0].pFeatures= imgbuf);
	// 	(sbatchPersonlist[0].pImageData = chImage);
	// 	(sbatchPersonlist[0].pBirthday= T2A(strBirthday));
	// 	(sbatchPersonlist[0].pValidTo= T2A(strValidTo));
	// 
	// 
	// 	file.Open(strFileAllName1, CFile::modeRead);
	// 	int filelen1 = file.GetLength();
	// 	char *imgbuf1 = new char[filelen1 + 1];
	// 	memset(imgbuf1, 0, filelen1 + 1);
	// 	file.Read(imgbuf1, filelen1);
	// 	file.Close();
	// 
	// 
	// 	(sbatchPersonlist[1].pName = T2A(strName));
	// 	(sbatchPersonlist[1].pIdCard = T2A(IdCard));
	// 	sbatchPersonlist[1].intGender = intGender;
	// 	sbatchPersonlist[1].longFaceDbId = longFaceDbId;
	// 	(sbatchPersonlist[1].pFeatures = imgbuf1);
	// 	(sbatchPersonlist[1].pImageData = chImage);
	// 	(sbatchPersonlist[1].pBirthday = T2A(strBirthday));
	// 	(sbatchPersonlist[1].pValidTo = T2A(strValidTo));

	CString strFilePath;// = L"../img/21758.jpg";
	CStringArray arCsFilePath;
	arCsFilePath.Add(_T("../img/21758.jpg"));
	arCsFilePath.Add(_T("../img/21668.jpg"));
	arCsFilePath.Add(_T("../img/21669.jpg"));
	arCsFilePath.Add(_T("../img/21670.jpg"));
	arCsFilePath.Add(_T("../img/21671.jpg"));
	int jsonlen = 1024 * 1024 * 2;
	char *chJson = new char[jsonlen * 2];
	int nLen = arCsFilePath.GetSize();
	for (int i = 0; i < nLen; i++)
	{
		strFilePath = arCsFilePath.GetAt(i);
		memset(chJson, '\0', jsonlen * 2);
		FASFeatureDetectforImageFile(pIP, pSession, T2A(strFilePath), chJson, jsonlen);
		int s(0), e(0);
		CString strRecv = A2T(chJson);
		s = strRecv.Find(_T("\"feature\":"));
		s = s + 11;
		e = strRecv.Find(_T(","), s);
		CString strFeatures = strRecv.Mid(s, e - 1 - s);
		s = strRecv.Find(_T("\"face_image\":"));
		s = s + 14;
		e = strRecv.Find(_T(","), s);
		CString ImageData = strRecv.Mid(s, e - 1 - s);
		IdCard.Format(_T("%d"), i);
		(sbatchPersonlist[i].pName = T2A(strName));
		(sbatchPersonlist[i].pIdCard = T2A(IdCard));
		sbatchPersonlist[i].intGender = intGender;
		sbatchPersonlist[i].longFaceDbId = longFaceDbId;
		(sbatchPersonlist[i].pFeatures = T2A(strFeatures));
		(sbatchPersonlist[i].pImageData = T2A(ImageData));
		(sbatchPersonlist[i].pBirthday = T2A(strBirthday));
		(sbatchPersonlist[i].pValidTo = T2A(strValidTo));

	}



	int result = FASBatchAddPersoninfo(pIP, pSession, sbatchPersonlist, nLen);
	int ttt = 0;
}




void CFASDLLTestDlg::OnBnClickedBtnQueryperson()
{
	USES_CONVERSION;
	char pSession[37];
	const char * pIP = "192.168.2.30";
	int m_result = DeviceLogin(pIP, pSession);
	char * pFaceDbId = "10364";
	char * pName = "zhangsan";
	char * pIdCard = "130626199305189962";
	int intMaxAge = 100;
	int MinAge = 0;
	int intGender = 0;
	int intPage = 1;
	int intSize = 10;
	char chJson[1024 * 128];
	int result = FASGetFaceListPageInDb(pIP, pSession, pFaceDbId, "", chJson, 1024 * 128);
	int ttt = 0;
}


void CFASDLLTestDlg::OnBnClickedBtnDeleteperson()
{
	USES_CONVERSION;
	char pSession[37];
	const char * pIP = "192.168.2.30";
	int m_result = DeviceLogin(pIP, pSession);
	const char *pPsersonid = "100111";
	int result = FASDELETEPersoninfo(pIP, pSession, pPsersonid);
}


void CFASDLLTestDlg::sssss(CString csip, char pSession[37])
{
	CString csPath1 = L"../img/21752.txt";
	CString csPath = L"../img/21866.txt";
	CFile file;


	if (!file.Open(csPath1, CFile::modeRead))
	{
	}
	int iFileLen = file.GetLength();
	char *featuresbuf1 = new char[iFileLen + 1];
	memset(featuresbuf1, 0, iFileLen + 1);
	file.Read(featuresbuf1, iFileLen);
	file.Close();

	if (!file.Open(csPath, CFile::modeRead))
	{
	}
	iFileLen = file.GetLength();
	char *featuresbuf2 = new char[iFileLen + 1];
	memset(featuresbuf2, 0, iFileLen + 1);
	file.Read(featuresbuf2, iFileLen);
	file.Close();
	USES_CONVERSION;

	char  json[1024 * 1024];
	int nRe = FASFeatureMatch(T2A(csip), pSession, featuresbuf2, featuresbuf1, json, 1024 * 1024);
	int s(0), e(0);
	CString strRecv = A2T(json);
	s = strRecv.Find(_T("\"similarity\":"));
	s = s + 13;
	e = strRecv.Find(_T("}"), s);
	CString strFeatures = strRecv.Mid(s, e - s);
	int ttt = 0;
}
void CFASDLLTestDlg::OnBnClickedBtnImagefilecompare()
{
	char pSession[37];
	const char * pIP = "192.168.2.30";
	CString csip = L"192.168.2.30";
	int m_result = DeviceLogin(pIP, pSession);
	sssss(csip, pSession);
	int t = 0;
}


void CFASDLLTestDlg::OnBnClickedBtnImagefilecompare2()
{
	char pSession[37];
	const char * pIP = "192.168.2.30";
	CString csip = L"192.168.2.30";
	int m_result = DeviceLogin(pIP, pSession);

	CFile file;

	CString strFileAllName = L"../img/21669.jpg";
	CString strFileAllName1 = L"../img/21819.jpg";
	USES_CONVERSION;

	file.Open(strFileAllName, CFile::modeRead);
	int filelen = file.GetLength();
	char *imgbuf = new char[filelen + 1];
	memset(imgbuf, 0, filelen + 1);
	file.Read(imgbuf, filelen);
	file.Close();
	char *imgb64buf = new char[filelen * 2];
	memset(imgb64buf, 0, filelen * 2);
	int b64len = EncodeBase64((unsigned char*)imgbuf, imgb64buf, filelen, filelen * 2);


	file.Open(strFileAllName1, CFile::modeRead);
	filelen = file.GetLength();
	char *imgbuf1 = new char[filelen + 1];
	memset(imgbuf1, 0, filelen + 1);
	file.Read(imgbuf1, filelen);
	file.Close();
	char *imgb64buf1 = new char[filelen * 2];
	memset(imgb64buf1, 0, filelen * 2);
	b64len = EncodeBase64((unsigned char*)imgbuf1, imgb64buf1, filelen, filelen * 2);

	char  json[1024 * 1024];
	int nRe = FASFeatureMatch(T2A(csip), pSession, imgb64buf1, imgb64buf, json, 1024 * 1024);
	int s(0), e(0);
	CString strRecv = A2T(json);
	s = strRecv.Find(_T("\"similarity\":"));
	s = s + 13;
	e = strRecv.Find(_T("}"), s);
	CString strFeatures = strRecv.Mid(s, e - s);
	int ttt = 0;
}


void CFASDLLTestDlg::OnBnClickedButton3()
{
	USES_CONVERSION;
	char pSession[37];
	const char * pIP = "192.168.2.30";
	CString cstemp, cstemp1;
	cstemp1 = A2T(pIP);
	cstemp.Format(_T("%s"), cstemp1);
	int ttt = 0;
}


void CFASDLLTestDlg::OnBnClickedBtnCreatewdb()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
