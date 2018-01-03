
// VideoRecordServerDlg.cpp : 实现文件
//



#include "stdafx.h"
#include "VideoRecordServer.h"
#include "VideoRecordServerDlg.h"
#include "afxdialogex.h"
#include "lib/Log.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define  DBINI_PATH						  "\\Parameter.ini"
// #define  VIDEO_ROOTPATH					 "\\..\\..\\Data\\"
#define  VIDEO_ROOTPATH					 "\\..\\Data\\"
#define  VIDEO_PATH					     "\\PlaybackVideo\\"
#define  TCPSERVER_PORT					49999

VideoPlay VideoPlaydlg;

static ThandScheduling * g_ThandSchedulingArr;
CWinThread * g_SchedulingThread;
_ConnectionPtr  g_pConnect;

UINT ThreadScheduling(LPVOID lparam);
UINT ThreadDownLoadVideo(LPVOID lparam);
UINT ThreadTCPServer(LPVOID lparam);

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
/******************************************
拆分字符串
******************************************/
int __BreakCString(CString csData, CStringArray &csOutTime)
{
	csData += ":";
	csData.Replace(" ", "/");
	int iPos = -1;
	CString csTemp, csTemp1;
	while ((iPos = csData.Find('/')) != -1)
	{
		csTemp = csData.Left(iPos);
		csData.Delete(0, iPos + 1);
		csOutTime.Add(csTemp);
	}

	while ((iPos = csData.Find(':')) != -1)
	{
		csTemp = csData.Left(iPos);
		csData.Delete(0, iPos + 1);
		csOutTime.Add(csTemp);
	}

	return csOutTime.GetSize();
}
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


// CVideoRecordServerDlg 对话框



CVideoRecordServerDlg::CVideoRecordServerDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CVideoRecordServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVideoRecordServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CVideoRecordServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(DOWNLOADEND, OnDownLoadMessageEnd)
	ON_MESSAGE(DOWNLOADSTORP, OnDownLoadMessageStorp)
	ON_MESSAGE(DOWNLOADERROR, OnDownLoadMessageError)
	ON_MESSAGE(WM_SHOW_TASK, OnShowNotify)
	ON_BN_CLICKED(IDC_BUT_START, &CVideoRecordServerDlg::OnBnClickedButStart)
	ON_BN_CLICKED(IDC_BUT_START2, &CVideoRecordServerDlg::OnBnClickedButStart2)
	ON_COMMAND(ID_Exit, &CVideoRecordServerDlg::OnExit)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUT_TESTING, &CVideoRecordServerDlg::OnBnClickedButTesting)
END_MESSAGE_MAP()


BOOL CVideoRecordServerDlg::OnInitDialog()
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

	// TODO:  在此添加额外的初始化代码
	//初始化com
	AfxOleInit();
	InitializeCriticalSection(&cSECTION);

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (InitDbParameter() && GetDBParameterini())
	{
		GetDlgItem(IDC_BUT_TESTING)->EnableWindow(FALSE);
		VideoPlaydlg.InitVideoPlay(this->m_hWnd);
		OnBnClickedButStart();
		SetTimer(1, 500, NULL);
	}
	else
	{
		MessageBox("启动失败请检查配置");
	}

	AfxBeginThread(&ThreadTCPServer, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


LRESULT CVideoRecordServerDlg::OnShowNotify(WPARAM wParam, LPARAM lParam)
{
	return m_tray.OnShowTask(wParam, lParam);
}

LRESULT CVideoRecordServerDlg::OnDownLoadMessageEnd(WPARAM wParam, LPARAM lParam)
{
	CString csSql, csTemp;
	csSql.Format("UPDATE warninginfo SET isstorage = '2' where id = '%s'", g_ThandSchedulingArr[wParam].csWarningid);
	g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
	csTemp.Format("OnDownLoadMessageEnd_%d_%s_%s", wParam, g_ThandSchedulingArr[wParam].csWarningid, g_ThandSchedulingArr[wParam].csWarningid);
	DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csTemp);

	g_ThandSchedulingArr[wParam].bWorkState = FALSE;
	return 0;
}

LRESULT CVideoRecordServerDlg::OnDownLoadMessageError(WPARAM wParam, LPARAM lParam)
{
	CString csSql;
	csSql.Format("UPDATE warninginfo SET isstorage = '3' where id = '%s' ", g_ThandSchedulingArr[wParam].csWarningid);

	g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
	DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "OnDownLoadMessageError" + csSql);


	g_ThandSchedulingArr[wParam].bWorkState = FALSE;
	return 0;
}

LRESULT CVideoRecordServerDlg::OnDownLoadMessageStorp(WPARAM wParam, LPARAM lParam)
{

	CString csSql;
	csSql.Format("UPDATE warninginfo SET isstorage = '0' where id = '%s' ", g_ThandSchedulingArr[wParam].csWarningid);
//	g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
	DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "OnDownLoadMessageStorp");

	g_ThandSchedulingArr[wParam].bWorkState = FALSE;
	return 0;
}

void CVideoRecordServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CVideoRecordServerDlg::OnPaint()
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
HCURSOR CVideoRecordServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//获取INI 文件路径
CString CVideoRecordServerDlg::GetiniPath()
{
	CString strIp;
	TCHAR pFileName[MAX_PATH];
	int nPos = GetCurrentDirectory(MAX_PATH, pFileName);

	CString csFullPath(pFileName);
	csFullPath += DBINI_PATH;

	return csFullPath;
}


CString CVideoRecordServerDlg::GetVideoPath()
{
	CString strIp;
	TCHAR pFileName[MAX_PATH];
	int nPos = GetCurrentDirectory(MAX_PATH, pFileName);

	CString csFullPath(pFileName);
	
	return csFullPath;
}
//初始化 数据库
BOOL CVideoRecordServerDlg::InitDbParameter()
{
	CString csFullPath = GetiniPath();
	CString csSqlIP, csSqlDatabase;
	GetPrivateProfileString(("SqlConfiguration"), "Sql_ServerAdd", (""), csSqlIP.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);
	GetPrivateProfileString(("SqlConfiguration"), "Sql_Database", (""), csSqlDatabase.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);
	CString csSqlConnect;
	csSqlConnect.Format("Driver={MySQL ODBC 5.1 Driver};Server=%s;Database=%s;uid=root;pwd=cr;", csSqlIP, csSqlDatabase);
	try
	{
		g_pConnect.CreateInstance(__uuidof(Connection));
		_bstr_t strConnect = csSqlConnect;
		g_pConnect->Open(strConnect, "", "", adModeUnknown);
	}
	catch (_com_error e)
	{
		return FALSE;
	}
	return  TRUE;
}

BOOL  CVideoRecordServerDlg::GetDBParameterini()
{
	CString csSql;
	int nThreadIdex = 0;
	_RecordsetPtr pRecordset;
	try
	{
		csSql = "select * from ParameterIni where Parameter = 'videorecord' ";
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		if (!pRecordset->adoEOF)
		{
			m_csDifferenceTime = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("DifferenceTime")->Value;
			m_csThreadSum = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("MaxThreadSum")->Value;
			m_cslDevIp = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("lDevIp")->Value;
			m_cslDevPort = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("lDevPort")->Value;
			m_cslDevUser = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("lDevUser")->Value;
			m_cslDevPwd = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("lDevPwd")->Value;

			m_nThreadSum = atoi(m_csThreadSum);

			return TRUE;
		}
		else
			return FALSE;
	}
	catch (CException* e)
	{
		return FALSE;
	}
}


int CVideoRecordServerDlg::InitThreadToSum()
{
	int nThreadIdex = 0;
	try
	{
		if (m_nThreadSum > 0)
		{
			g_ThandSchedulingArr = new ThandScheduling[m_nThreadSum];

			for (; nThreadIdex < m_nThreadSum; nThreadIdex++)
			{
				g_ThandSchedulingArr[nThreadIdex].bWorkState = FALSE;
				g_ThandSchedulingArr[nThreadIdex].nDifferenceTime = atoi(m_csDifferenceTime);
				g_ThandSchedulingArr[nThreadIdex].HEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
				g_ThandSchedulingArr[nThreadIdex].HThand = AfxBeginThread(&ThreadDownLoadVideo, (LPVOID)nThreadIdex);
			}
		}
	}
	catch (CException* e)
	{
		return -1;
	}

	return nThreadIdex;
}

BOOL CVideoRecordServerDlg::GetVideoTime(CString csTime, CString &csStartTime, CString &csEndTime, int nTime)
{
	CStringArray arTime;
	CTimeSpan delta(0, 0, 0, 15);
	if (__BreakCString(csTime, arTime) == 6)
	{
		CTime now(atoi(arTime.GetAt(0)), atoi(arTime.GetAt(1)), atoi(arTime.GetAt(2)),
			atoi(arTime.GetAt(3)), atoi(arTime.GetAt(4)), atoi(arTime.GetAt(5)));

		now -= delta;
		csStartTime.Format(_T("%04d%02d%02d%02d%02d%02d"),
			now.GetYear(),
			now.GetMonth(),
			now.GetDay(),
			now.GetHour(),
			now.GetMinute(),
			now.GetSecond()
			);

		now += delta;
		now += delta;
		csEndTime.Format(_T("%04d%02d%02d%02d%02d%02d"),
			now.GetYear(),
			now.GetMonth(),
			now.GetDay(),
			now.GetHour(),
			now.GetMinute(),
			now.GetSecond()
			);
		return TRUE;
	}
	return FALSE;
}


//调度
UINT ThreadScheduling(LPVOID lparam)
{

	CVideoRecordServerDlg *pWnd = (CVideoRecordServerDlg *)AfxGetApp()->GetMainWnd();

	BOOL bWhile = TRUE;
	BOOL bSetEvent = FALSE;
	_RecordsetPtr pRecordset;

	CString csSql;
	int  nIdex = 0;

	while (bWhile)
	{
		try
		{
			CString csTemp;
			csTemp.Format("ThreadScheduling_%d", pWnd->m_nThreadSum);
			//DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csTemp);

			csSql = "select warninginfo.id , warninginfo.snapshotid, snapshotinfo.inserttime , snapshotinfo.imgpath from warninginfo JOIN snapshotinfo  on  warninginfo.isstorage =0  and warninginfo.snapshotid =snapshotinfo.id ";
			pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
			while (!pRecordset->adoEOF)
			{
				bSetEvent = FALSE;

				if (nIdex >= pWnd->m_nThreadSum)
				{
					nIdex = 0;
					Sleep(2000);
				}

				//循环判断在线一体机是否在工作
				for (; nIdex < pWnd->m_nThreadSum; nIdex++)
				{
					if (!g_ThandSchedulingArr[nIdex].bWorkState)
					{
						//Sleep(2000);

						EnterCriticalSection(&cSECTION);
						g_ThandSchedulingArr[nIdex].bWorkState = TRUE;
						g_ThandSchedulingArr[nIdex].csSnapshotid = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("snapshotid")->Value;
						g_ThandSchedulingArr[nIdex].csWarningid = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("id")->Value;
						g_ThandSchedulingArr[nIdex].csVideoTime = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("inserttime")->Value;
						g_ThandSchedulingArr[nIdex].csFile = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("imgpath")->Value;
						LeaveCriticalSection(&cSECTION);
						csTemp.Format("nIdex_%d_%s_%s", nIdex, g_ThandSchedulingArr[nIdex].csWarningid, g_ThandSchedulingArr[nIdex].csSnapshotid);

						DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csTemp);

						csSql.Format("UPDATE warninginfo SET isstorage = '1' where id = '%s'", g_ThandSchedulingArr[nIdex].csWarningid);
						g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);

						if (!SetEvent(g_ThandSchedulingArr[nIdex].HEvent))
						{
							g_ThandSchedulingArr[nIdex].bWorkState = FALSE;
							DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "SetEventerrer");

							continue;
						}
						//因为要跳出nIdex 不回自加
						nIdex++;
						bSetEvent = TRUE;
						break;
					}
				}

				if (bSetEvent)
					pRecordset->MoveNext();
			}
			//Sleep(60000);
		}
		catch (...)
		{

		}
	}

	return 0;
}

//处理
UINT ThreadDownLoadVideo(LPVOID lparam)
{
	int nIdex = (int)lparam;
	CVideoRecordServerDlg *pWnd = (CVideoRecordServerDlg *)AfxGetApp()->GetMainWnd();

	DWORD dReturn;
	CString csSql, csIp, csPath, csName, csFile, csStartTime, csEndTime, csTime, csSnapshotid, csWarningid;
	BOOL bWhile = TRUE;
	int  nDifferenceTime = 15;
	int nVideoIndex = -1;
	_RecordsetPtr pRecordset;
	BOOL bOnLine = FALSE;
	CString csTemp;
	while (bWhile)
	{
		try
		{
			dReturn = WaitForSingleObject(g_ThandSchedulingArr[nIdex].HEvent, INFINITE);
			if (WAIT_OBJECT_0 == dReturn)
			{
				EnterCriticalSection(&cSECTION);
				csPath = pWnd->GetVideoPath();
				csTemp.Format("%s%s%s%s", csPath, VIDEO_ROOTPATH, g_ThandSchedulingArr[nIdex].csFile, VIDEO_PATH);
				csPath = csTemp;
				csSnapshotid = g_ThandSchedulingArr[nIdex].csSnapshotid;
				csWarningid = g_ThandSchedulingArr[nIdex].csWarningid;
				nDifferenceTime = g_ThandSchedulingArr[nIdex].nDifferenceTime;
				csTime = g_ThandSchedulingArr[nIdex].csVideoTime;
				LeaveCriticalSection(&cSECTION);

				csSql.Format("SELECT camera.ip from snapshotinfo left join device on snapshotinfo.clientip = device.ip LEFT JOIN camera on device.id=camera.deviceid and snapshotinfo.cameraid = camera.cameraid WHERE snapshotinfo.id =%s;", csSnapshotid);
				pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);

				if (!pRecordset->adoEOF)
				{
					csIp = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("ip")->Value;

					if (pWnd->GetVideoTime(csTime, csStartTime, csEndTime, nDifferenceTime))
					{
						bOnLine = FALSE;
						VideoPlaydlg.GetDevCameraNumDataToIndex(nVideoIndex, csIp, bOnLine);

						CString csTemp;
						csTemp.Format("ThreadDownLoadVideo_%d_%s_%d_%s_%s", nIdex, csWarningid, nVideoIndex, csStartTime, csEndTime);
						DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csTemp);

						if (bOnLine)
						{
							//Sleep(15000);
							VideoPlaydlg.GetDownloadPos(nVideoIndex, nIdex, csStartTime, csEndTime, csWarningid, csPath);
							csTemp.Format("%s_%s_end", csWarningid, csPath);
							DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csTemp);
						}
					}
				}
				else
				{
					DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "SELECT_NO");
					g_ThandSchedulingArr[nIdex].bWorkState = FALSE;
				}
			}
		}
		catch (...)
		{

		}
	}

	return 0;
}


UINT ThreadTCPServer(LPVOID lparam)
{
	CVideoRecordServerDlg *pWnd = (CVideoRecordServerDlg *)AfxGetApp()->GetMainWnd();

	_RecordsetPtr pRecordset;
	CString csSql;
	SOCKET            sServer;
	SOCKET            sClient;
	SOCKADDR_IN        addrServ;;        //服务器地址
	int retVal;
	BOOL bWhile = TRUE;
	sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//服务器套接字地址 
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(TCPSERVER_PORT);
	addrServ.sin_addr.s_addr = INADDR_ANY;

	int	TimeOut = 6000; //设置发送超时6秒

	if (setsockopt(sServer, SOL_SOCKET, SO_SNDTIMEO, (char*)&TimeOut, sizeof(TimeOut)) == SOCKET_ERROR)
	{
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "设置发送超时失败");
		return -1;
	}


	if (setsockopt(sServer, SOL_SOCKET, SO_RCVTIMEO, (char*)&TimeOut, sizeof(TimeOut)) == SOCKET_ERROR)
	{
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "设置接收超时失败");
		return -1;
	}

	retVal = bind(sServer, (LPSOCKADDR)&addrServ, sizeof(SOCKADDR_IN));
	//开始监听 
	retVal = listen(sServer, 1);

	char buf[500];
	ZeroMemory(buf, sizeof(buf));

	while (bWhile)
	{
		sockaddr_in addrClient;
		int addrClientlen = sizeof(addrClient);
		sClient = accept(sServer, (sockaddr FAR*)&addrClient, &addrClientlen);

		retVal = recv(sClient, buf, sizeof(buf), 0);

		if (retVal != -1 && retVal != 0)
		{
			CString csTemp = buf;
			CString csFileFPath,csVoidName;
			CString csFile = csTemp.Mid(csTemp.GetLength() - 8, 8);
			CString csName = csTemp.Mid(0, csTemp.GetLength() - 8);
			csFileFPath = pWnd->GetVideoPath();
		
			csTemp.Format("%s%s%s%s", csFileFPath, VIDEO_ROOTPATH, csFile, VIDEO_PATH);

			csVoidName.Format("%s%s.avi", csTemp, csName);

			if (_access(csVoidName, 0) != -1)
			{
				CFile fileJPG;
				fileJPG.Open(csVoidName, CFile::modeRead);
				int iFileLen = fileJPG.GetLength();
				char *imagebuf = new char[iFileLen];
				fileJPG.Read(imagebuf, iFileLen);
				fileJPG.Close();

				retVal = send(sClient, imagebuf, iFileLen, 0);
			}
		}
	}
	return 0;
}

//暂时不用
UINT ThreadTCPSend(LPVOID lparam)
{
	CVideoRecordServerDlg *pWnd = (CVideoRecordServerDlg *)AfxGetApp()->GetMainWnd();

	_RecordsetPtr pRecordset;
	CString csSql;

	return 0;
}

void CVideoRecordServerDlg::OnBnClickedButStart2()
{
	m_tray.CreateTray(IDR_MENU1, this, WM_SHOW_TASK, "VideoRecordServer", this->m_hIcon, true);
	ShowWindow(SW_HIDE);
}

void CVideoRecordServerDlg::OnExit()
{
	m_tray.DelTray();
	OnOK();
}

void CVideoRecordServerDlg::OnBnClickedButStart()
{
	if (VideoPlaydlg.LoginVideoPlay(m_cslDevIp, m_cslDevPort, m_cslDevUser, m_cslDevPwd))
	{
		VideoPlaydlg.DoGetDeviceResoureCfg();
		InitThreadToSum();
		m_bIsLogIn = TRUE;
		g_SchedulingThread = AfxBeginThread(&ThreadScheduling, NULL);
		GetDlgItem(IDC_BUT_START)->EnableWindow(FALSE);
	}
	else
	{
		m_bIsLogIn = FALSE;
		MessageBox("硬盘录像机登录失败", "提示");
	}
}

void CVideoRecordServerDlg::OnBnClickedButTesting()
{
	if (InitDbParameter() && GetDBParameterini())
	{
		VideoPlaydlg.InitVideoPlay(this->m_hWnd);
		GetDlgItem(IDC_BUT_TESTING)->EnableWindow(FALSE);
	}
}

void CVideoRecordServerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1)
	{
		KillTimer(1);
		OnBnClickedButStart2();
	}
	CDialogEx::OnTimer(nIDEvent);
}