
// WinSnapshotServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WinSnapshotServer.h"
#include "WinSnapshotServerDlg.h"
#include "afxdialogex.h"
#include "Tools.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define GETSNAPPORT 28003
#define UDP_SEND_PORT 30000  //udp端口
#define  PROGRAM_INDEX   2		//程序ID
#define  SLEEP_TIME   30000		//心跳间隔

UINT GetSnapInfoThread(LPVOID pParam);
UINT GetSnapImgThread(LPVOID pParam);
UINT HeartbeatThread(LPVOID lpam);


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CWinSnapshotServerDlg 对话框



CWinSnapshotServerDlg::CWinSnapshotServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWinSnapshotServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWinSnapshotServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWinSnapshotServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_TEST, &CWinSnapshotServerDlg::OnBnClickedTest)
	ON_WM_TIMER()
	ON_COMMAND(ID_EXIT, &CWinSnapshotServerDlg::OnExit)
	ON_MESSAGE(WM_SHOW_TASK, OnShowNotify)

END_MESSAGE_MAP()


// CWinSnapshotServerDlg 消息处理程序

BOOL CWinSnapshotServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	GetDlgItem(IDC_TEST)->ShowWindow(false);

	AfxBeginThread((AFX_THREADPROC)GetSnapInfoThread, (void*)this);
	AfxBeginThread((AFX_THREADPROC)GetSnapImgThread, (void*)this); 
	AfxBeginThread((AFX_THREADPROC)HeartbeatThread, (void*)this);
	SetTimer(1, 1, NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CWinSnapshotServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CWinSnapshotServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CWinSnapshotServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

UINT GetSnapInfoThread(LPVOID pParam)
{
	AfxOleInit();
	CWinSnapshotServerDlg *pWnd = (CWinSnapshotServerDlg*)pParam;
	int re;
	SOCKET sockSrv = socket(AF_INET, SOCK_DGRAM, 0);
	SOCKADDR_IN addrJump;
	addrJump.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrJump.sin_family = AF_INET;
	addrJump.sin_port = htons(GETSNAPPORT);
	re = bind(sockSrv, (SOCKADDR*)&addrJump, sizeof(SOCKADDR));
	TRACE(_T("\nbind re = %d\n"), re);

	char ucRevData[1000];
	memset(ucRevData, '\0', 1000);
	SOCKADDR_IN Client_sa;
	int nAddrSize = sizeof(SOCKADDR_IN);
	while (true)
	{
		int iRecvCount = recvfrom(sockSrv, ucRevData, 1000, 0, (sockaddr *)(&Client_sa), &nAddrSize);
		CString strClientip;
		strClientip = inet_ntoa(Client_sa.sin_addr);
/*		CFile file;
		file.Open(_T("json_msg.txt"), CFile::modeCreate | CFile::modeWrite);
		file.Write(ucRevData, iRecvCount);
		file.Close();*/
		pWnd->PutSnapInfoToDB(strClientip, ucRevData);

/*		pWnd->GetCurFaceImage(_T("192.168.2.10"), 1, face_name, 1);
		pWnd->m_bRedrawCompare = pWnd->m_bRedrawCard = true;
		pWnd->InvalidateRect(pWnd->m_rCompare);
		pWnd->InvalidateRect(pWnd->m_rCard);*/
		TRACE(_T("recv one json!\n"));
	}
	return 1;
}

UINT GetSnapImgThread(LPVOID pParam)
{
	AfxOleInit();
	CWinSnapshotServerDlg *pWnd = (CWinSnapshotServerDlg*)pParam;
	_ConnectionPtr conn;
	conn.CreateInstance(__uuidof(Connection));
	CString strConn;
	strConn.Format(_T("Driver={MySQL ODBC 5.1 Driver}; Server = %s; Database = %s;uid=%s;pwd=%s;"), theApp.m_strDBIP, theApp.m_strDBName, theApp.m_strDBUsername, theApp.m_strDBPassword);
	try
	{
		conn->Open(_bstr_t(strConn), "", "", adModeUnknown);
	}
	catch (_com_error e)
	{
		return false;
	}
	CString strIP, strFacename,strPhotoname,strImgpath;
	int iID, iHostid;
	_RecordsetPtr pSet;
	pSet.CreateInstance(__uuidof(Recordset));
	CString strSQL;
	while (true)
	{
		strSQL.Format(_T("select * from snapshotinfo where imgisexist=0 limit 0,5"));
		pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
		while (!pSet->adoEOF)
		{
			strIP = getsqlstr("clientip");
			strFacename = getsqlstr("facename");
			strPhotoname = getsqlstr("photoname");
			strImgpath = getsqlstr("imgpath");
			iID = getsqldouble("id");
			iHostid = getsqlint("hostid");
			if (pWnd->GetSnapshotFaceImage(strIP, iHostid, strFacename, iID, strImgpath))
			{
				pSet->put_Collect(_variant_t("imgisexist"), _variant_t("1"));
				pSet->put_Collect(_variant_t("process_status"), _variant_t("1"));
			}
			else
			{
				pSet->put_Collect(_variant_t("imgisexist"), _variant_t("2"));
			}
			pWnd->GetSnapshotPhotoImage(strIP, iHostid, strPhotoname, iID, strImgpath);
			pSet->Update();
			pSet->MoveNext();
		}
		pSet->Close();
		Sleep(100);
	}
	conn->Close();
	return true;
}

void CWinSnapshotServerDlg::PutSnapInfoToDB(CString strIP, char*cJson)
{
	Json::Reader reader;
	Json::Value root;
	BOOL readre = reader.parse(cJson, root);
	if (!readre)
		return;
	CString strAlarmid,strCameraid,strAlarmtime,strPhotoname,strFacename,strHostid,strImgpath;
	int iCameraid, iHostid;

	strAlarmid = root["alarm_id"].asCString();
	iCameraid = root["camera_id"].asInt();
	strCameraid.Format(_T("%d"), iCameraid);
	strAlarmtime = root["alarm_time"].asCString();
	strPhotoname = root["photo_name"].asCString();
	strFacename = root["face_name"].asCString();
	iHostid = root["photo_host_id"].asInt();
	strHostid.Format(_T("%d"), iHostid);
	strImgpath = strAlarmtime.Mid(0, 8);
	strAlarmtime.Insert(12, L":");
	strAlarmtime.Insert(10, L":");
	strAlarmtime.Insert(8, L" ");
	strAlarmtime.Insert(6, L"-");
	strAlarmtime.Insert(4, L"-");
	
	CString strPath;
	strPath.Format(_T("../../Data/%s"), strImgpath);
	BOOL bRe = PathIsDirectoryW(strPath);
	if (!bRe)
	{
		CreateDirectoryW(strPath, NULL);
		CString strCreatePath;
		strCreatePath.Format(_T("%s/imgCurFaceBase64"), strPath);
		CreateDirectoryW(strCreatePath, NULL);
		strCreatePath.Format(_T("%s/imgCurFaceJpg"), strPath);
		CreateDirectoryW(strCreatePath, NULL);
		strCreatePath.Format(_T("%s/imgCurPhotoBase64"), strPath);
		CreateDirectoryW(strCreatePath, NULL);
		strCreatePath.Format(_T("%s/imgCurPhotoJpg"), strPath);
		CreateDirectoryW(strCreatePath, NULL);
		strCreatePath.Format(_T("%s/PlaybackVideo"), strPath);
		CreateDirectoryW(strCreatePath, NULL);
	}

	_ConnectionPtr conn;
	conn.CreateInstance(__uuidof(Connection));
	CString strConn;
	strConn.Format(_T("Driver={MySQL ODBC 5.1 Driver}; Server = %s; Database = %s;uid=%s;pwd=%s;"), theApp.m_strDBIP, theApp.m_strDBName, theApp.m_strDBUsername, theApp.m_strDBPassword);
	try
	{
		conn->Open(_bstr_t(strConn), "", "", adModeUnknown);
	}
	catch (_com_error e)
	{

		return;
	}
	_RecordsetPtr pSet;
	pSet.CreateInstance(__uuidof(Recordset));
	CString strSQL;
	strSQL.Format(_T("select now() as curtime"));
	pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	CString strCurDBTime;
	strCurDBTime = getsqlstr("curtime");
	pSet->Close();

	strSQL.Format(_T("select * from snapshotinfo limit 0,1"));
	pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	pSet->AddNew();
	pSet->put_Collect(_variant_t("clientip"), _variant_t(strIP));
	pSet->put_Collect(_variant_t("cameraid"), _variant_t(strCameraid));
	pSet->put_Collect(_variant_t("alarmtime"), _variant_t(strAlarmtime));
	pSet->put_Collect(_variant_t("photoname"), _variant_t(strPhotoname));
	pSet->put_Collect(_variant_t("facename"), _variant_t(strFacename));
	pSet->put_Collect(_variant_t("hostid"), _variant_t(strHostid));
	pSet->put_Collect(_variant_t("imgpath"), _variant_t(strImgpath));
	pSet->put_Collect(_variant_t("inserttime"), _variant_t(strCurDBTime));
	pSet->Update();
	pSet->Close();
	conn->Close();
}

BOOL CWinSnapshotServerDlg::GetSnapshotFaceImage(CString strIP, int iHostID, CString strFaceName, int iID, CString strImgpath)
{
	CString strSession;
	int logre = Login(strIP, strSession);
	CString strSend;
	strSend.Format(_T("%s%d%s%s%s%s%s%s%s%s%s%s%s"),
		L"GET /rtmonitor/alarm/cface/", iHostID, L"?filename=", strFaceName, L" HTTP/1.1\n",
		L"User-Agent: curl/7.19.7 (x86_64-redhat-linux-gnu) libcurl/7.19.7 NSS/3.13.1.0 zlib/1.2.3 libidn/1.18 libssh2/1.2.2\n",
		L"Host: ", strIP, L"\n",
		//		"Connection: close\n",
		L"Accept: */*\n\n",
		L"Cookie: facecloud_session=",strSession,L":root\n"
		);
	const size_t ipsize = (strIP.GetLength() + 1) * 2; // 宽字符的长度;
	char * ipchar = new char[ipsize]; //分配空间;
	size_t sz = 0;
	wcstombs_s(&sz, ipchar, ipsize, strIP, _TRUNCATE);

	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(ipchar);//该处地址应为空闲一体机地址
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8000);
	int timeout = 1000;
	setsockopt(sockClient, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
	connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	delete[] ipchar;
	const size_t sendsize = (strSend.GetLength() + 1) * 2; // 宽字符的长度;
	char * sendchar = new char[sendsize]; //分配空间;
	wcstombs_s(&sz, sendchar, sendsize, strSend, _TRUNCATE);

	send(sockClient, sendchar, sz, 0);
	char revbuf[2000 * 1024];
	memset(revbuf, '\0', 2000 * 1024);
	int iCurPos = 0;
	while (1)
	{
		int revlen = recv(sockClient, revbuf + iCurPos, 500 * 1024, 0);
		TRACE("revlen = %d\n", revlen);
		if (revlen == 0 || revlen == -1)
			break;
		iCurPos += revlen;
		Sleep(1);
	}
	closesocket(sockClient);
	delete[] sendchar;

	CString strRecv;
	strRecv = revbuf;
	int js = strRecv.Find(_T("{"), 0);
	//	strRecv.Delete(0, js);

	Json::Reader reader;
	Json::Value root;
	BOOL readre = reader.parse(revbuf + js, root);
	if (!readre)
		return false;
	int ret = root["ret"].asInt();
	if (ret != 0)
		return false;
	CString strBase64;
	strBase64 = root["image_data"]["content"].asCString();
/*	CFile file;
	CString strFilename;
	strFilename.Format(_T("curface_%d.b64"), iID);
	file.Open(strFilename, CFile::modeCreate | CFile::modeWrite);
	file.Write(strBase64.GetBuffer(strBase64.GetLength()*2+2), strBase64.GetLength()*2+2);
	file.Close();*/

	CTools tl;
	int bLen = strBase64.GetLength();
	unsigned char *cImg = new unsigned char[bLen];
	memset(cImg, 0, bLen);

	const size_t b64size = (strBase64.GetLength() + 1) * 2; // 宽字符的长度;
	char * b64char = new char[b64size]; //分配空间;
	wcstombs_s(&sz, b64char, b64size, strBase64, _TRUNCATE);

	CFile file;
	CString strFilename;
	strFilename.Format(_T("../../Data/%s/imgCurFaceBase64/curface_%d.b64"), strImgpath,iID);
	file.Open(strFilename, CFile::modeCreate | CFile::modeWrite);
	file.Write(b64char, sz);
	file.Close();

	int tre = tl.DecodeBase64(b64char, cImg, bLen);
	CString strImgFileName;
	strImgFileName.Format(_T("../../Data/%s/imgCurFaceJpg/curface_%d.jpg"), strImgpath,iID);
	CFile fileJPG;
	fileJPG.Open(strImgFileName, CFile::modeCreate | CFile::modeWrite);
	fileJPG.Write(cImg, tre);
	fileJPG.Close();
	delete[] b64char;
	delete[] cImg;
	return true;
}

BOOL CWinSnapshotServerDlg::GetSnapshotPhotoImage(CString strIP, int iHostID, CString strPhotoName, int iID, CString strImgpath)
{
	CString strSend;
	strSend.Format(_T("%s%d%s%s%s%s%s%s%s%s%s"),
		L"GET /rtmonitor/alarm/image/", iHostID, L"?filename=", strPhotoName, L" HTTP/1.1\n",
		L"User-Agent: curl/7.19.7 (x86_64-redhat-linux-gnu) libcurl/7.19.7 NSS/3.13.1.0 zlib/1.2.3 libidn/1.18 libssh2/1.2.2\n",
		L"Host: ", strIP, L"\n",
		L"Connection: close\n",
		L"Accept: */*\n\n"
		);
	const size_t ipsize = (strIP.GetLength() + 1) * 2; // 宽字符的长度;
	char * ipchar = new char[ipsize]; //分配空间;
	size_t sz = 0;
	wcstombs_s(&sz, ipchar, ipsize, strIP, _TRUNCATE);

	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(ipchar);//该处地址应为空闲一体机地址
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8000);
	int timeout = 1000;
	setsockopt(sockClient, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
	connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	delete[] ipchar;
	const size_t sendsize = (strSend.GetLength() + 1) * 2; // 宽字符的长度;
	char * sendchar = new char[sendsize]; //分配空间;
	wcstombs_s(&sz, sendchar, sendsize, strSend, _TRUNCATE);

	send(sockClient, sendchar, sz, 0);
	char revbuf[8000 * 1024];
	memset(revbuf, '\0', 8000 * 1024);
	int iCurPos = 0;
	while (1)
	{
		int revlen = recv(sockClient, revbuf + iCurPos, 500 * 1024, 0);
		TRACE("revlen = %d\n", revlen);
		if (revlen == 0 || revlen == -1)
			break;
		iCurPos += revlen;
		Sleep(1);
	}
	closesocket(sockClient);
	delete[] sendchar;

	CString strRecv;
	strRecv = revbuf;
	int js = strRecv.Find(_T("{"), 0);
	//	strRecv.Delete(0, js);

	Json::Reader reader;
	Json::Value root;
	BOOL readre = reader.parse(revbuf + js, root);
	if (!readre)
		return false;
	int ret = root["ret"].asInt();
	if (ret != 0)
		return false;
	CString strBase64;
	strBase64 = root["image_data"]["content"].asCString();
/*	CFile file;
	CString strFilename;
	strFilename.Format(_T("curphoto_%d.b64"), iID);
	file.Open(strFilename, CFile::modeCreate | CFile::modeWrite);
	file.Write(strBase64.GetBuffer(strBase64.GetLength() * 2 + 2), strBase64.GetLength() * 2 + 2);
	file.Close();*/

	CTools tl;
	int bLen = strBase64.GetLength();
	unsigned char *cImg = new unsigned char[bLen];
	memset(cImg, 0, bLen);

	const size_t b64size = (strBase64.GetLength() + 1) * 2; // 宽字符的长度;
	char * b64char = new char[b64size]; //分配空间;
	wcstombs_s(&sz, b64char, b64size, strBase64, _TRUNCATE);

	CFile file;
	CString strFilename;
	strFilename.Format(_T("../../Data/%s/imgCurPhotoBase64/curphoto_%d.b64"), strImgpath, iID);
	file.Open(strFilename, CFile::modeCreate | CFile::modeWrite);
	file.Write(b64char, sz);
	file.Close();

	int tre = tl.DecodeBase64(b64char, cImg, bLen);
	CString strImgFileName;
	strImgFileName.Format(_T("../../Data/%s/imgCurPhotoJpg/curphoto_%d.jpg"), strImgpath, iID);
	CFile fileJPG;
	fileJPG.Open(strImgFileName, CFile::modeCreate | CFile::modeWrite);
	fileJPG.Write(cImg, tre);
	fileJPG.Close();
	delete[] b64char;
	delete[] cImg;
	return true;
}

void CWinSnapshotServerDlg::OnBnClickedTest()
{
	CFileDialog getfilepathdlg(true, NULL, NULL, OFN_HIDEREADONLY, _T("All Files(*.jpg)|*.jpg"), NULL);
	if (getfilepathdlg.DoModal() == IDOK)
	{
		int a = 0;
	}
	else
	{
		int b = 0;
	}
}

int CWinSnapshotServerDlg::Login(CString strIP, CString &strSession)
{
	CString strContent = L"{\n	\"user_name\":	\"root\",\n	\"user_pwd\":	\"6D3D66861E2E1AA9BAFBB09B7E5AE651\",\n	\"mode\":	\"force_login\"\n}";

	int iContentLen = strContent.GetLength();
	CString strContentLen;
	strContentLen.Format(_T("%d"), iContentLen);
	CString strSend;
	strSend.Format(_T("%s%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s"),
		L"POST /base/auth/login HTTP/1.1\n",
		L"Date: Mon, 21 Mar 2016 11:30:17 GMT\n",
		L"Accept: */*\n",
		L"Host: ", strIP, L"\n",
		L"Content-Length: ", iContentLen, L"\n",
		L"Accept-Language: zh-cn\n",
		L"User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1)\n",
		L"Pragma: no-cache\n",
		L"Cookie: facecloud_session=160315175150000005:root\n",
		L"Connection: close\n\n",
		strContent,L"\n");
	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);
	if (sockClient == INVALID_SOCKET)
	{
		WSACleanup();
		return 10;//socket create failed
	}

	const size_t ipsize = (strIP.GetLength() + 1) * 2; // 宽字符的长度;
	char * ipchar = new char[ipsize]; //分配空间;
	size_t sz = 0;
	wcstombs_s(&sz, ipchar, ipsize, strIP, _TRUNCATE);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(ipchar);//该处地址应为空闲一体机地址
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8000);

	int tv_out_ms = 1000;
	int intsocketststus;
	intsocketststus = setsockopt(sockClient, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv_out_ms, sizeof(tv_out_ms));
	int m_connectstatus = connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	delete[] ipchar;
	if (SOCKET_ERROR == m_connectstatus)
	{
		return 10;//socket connect failed
	}
	intsocketststus = setsockopt(sockClient, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv_out_ms, sizeof(tv_out_ms));
	if (intsocketststus == SOCKET_ERROR)
		return -1;

	const size_t sendsize = (strSend.GetLength() + 1) * 2; // 宽字符的长度;
	char * sendchar = new char[sendsize]; //分配空间;
	wcstombs_s(&sz, sendchar, sendsize, strSend, _TRUNCATE);

	int sendlen = send(sockClient, sendchar, sendsize, 0);
	delete[] sendchar;
	if (sendlen == 0 || sendlen == SOCKET_ERROR)
		return 10;
	char revbuf[8192];
	memset(revbuf, '\0', 8192);
	recv(sockClient, revbuf, 8192, 0);
	CString strRecv;
	strRecv = revbuf;
	closesocket(sockClient);
	int s(0), e(0);
	s = strRecv.Find(L"\"ret\" : ");
	if (s == -1)
	{
		return -1;
	}
	s = s + 8;
	e = strRecv.Find(L",", s);
	CString strRet = strRecv.Mid(s, e - s);
	if (strRet != "0")
	{
		int intRet = _wtoi(strRet);
		return intRet;
	}
	s = strRecv.Find(L"facecloud_session=");
	s = s + 18;
	e = strRecv.Find(L":", s);
	strSession = strRecv.Mid(s, e - s);
	return 0;
}
void CWinSnapshotServerDlg::ToTray()
{
	m_tray.CreateTray(IDR_MENU1, this, WM_SHOW_TASK, L"WinSnapshotServer服务端", this->m_hIcon, true);
	ShowWindow(SW_HIDE);
}

void CWinSnapshotServerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1)
	{
		KillTimer(1);
		ToTray();
	}
	CDialogEx::OnTimer(nIDEvent);
}

LRESULT CWinSnapshotServerDlg::OnShowNotify(WPARAM wParam, LPARAM lParam)
{
	return m_tray.OnShowTask(wParam, lParam);
}
void CWinSnapshotServerDlg::OnExit()
{
	// TODO:  在此添加命令处理程序代码
	m_tray.DelTray();
	OnOK();
}
UINT HeartbeatThread(LPVOID lpam)
{
	USES_CONVERSION;
	while (true)
	{
		try
		{
			SOCKET sock;
			SOCKADDR_IN sin;
			sock = socket(AF_INET, SOCK_DGRAM, 0);
			if (sock == SOCKET_ERROR)
			{
				continue;;
			}
			sin.sin_family = AF_INET;
			sin.sin_addr.s_addr = inet_addr("127.0.0.1");
			sin.sin_port = htons(UDP_SEND_PORT);

			CString strSend;
			strSend.Format(L"Heartbeat_%d", PROGRAM_INDEX);
			int length;
			length = sendto(sock, T2A(strSend.GetBuffer(0)), strSend.GetLength(), 0, (SOCKADDR*)&sin, sizeof(sin));
			Sleep(SLEEP_TIME);
		}
		catch (CException* e)
		{
		}
	}
	return 0;
}