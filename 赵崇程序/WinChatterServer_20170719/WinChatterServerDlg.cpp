
// WinChatterServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WinChatterServer.h"
#include "WinChatterServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define CHATTERSERVERPORT 28002
#define ONLINE_TIMEOUT 10
#define UDP_SEND_PORT 30000  //udp端口
#define  PROGRAM_INDEX   1		//程序ID
#define  SLEEP_TIME   30000		//心跳间隔

UINT GetChatterServer(LPVOID pParam);
UINT CheckOnlineStatus(LPVOID pParam);
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


// CWinChatterServerDlg 对话框



CWinChatterServerDlg::CWinChatterServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWinChatterServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWinChatterServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWinChatterServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_COMMAND(ID_Exit, &CWinChatterServerDlg::OnExit)
	ON_MESSAGE(WM_SHOW_TASK, OnShowNotify)

END_MESSAGE_MAP()


// CWinChatterServerDlg 消息处理程序

BOOL CWinChatterServerDlg::OnInitDialog()
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

/*	CString strNowTime = L"2017-03-06 13:54:23";
	CString strOldTime = L"2017-03-06 13:53:41";

	CompareTime(strNowTime,strOldTime);*/
	AfxBeginThread((AFX_THREADPROC)GetChatterServer, (void*)this);
	AfxBeginThread((AFX_THREADPROC)CheckOnlineStatus, (void*)this);
	AfxBeginThread((AFX_THREADPROC)HeartbeatThread, (void*)this);
	SetTimer(1, 1, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CWinChatterServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CWinChatterServerDlg::OnPaint()
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
HCURSOR CWinChatterServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

UINT GetChatterServer(LPVOID pParam)
{
	CWinChatterServerDlg *pWnd = (CWinChatterServerDlg*)pParam;
	int re;
	SOCKET sockSrv = socket(AF_INET, SOCK_DGRAM, 0);
	SOCKADDR_IN addrJump;
	addrJump.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrJump.sin_family = AF_INET;
	addrJump.sin_port = htons(CHATTERSERVERPORT);
	re = bind(sockSrv, (SOCKADDR*)&addrJump, sizeof(SOCKADDR));
	TRACE(_T("\nbind re = %d\n"), re);

	char ucRevData[100];
	memset(ucRevData, '\0', 100);
	CString strOnlyID, strTime;
	SYSTEMTIME nowtime;
	while (true)
	{
		recvfrom(sockSrv, ucRevData, 100, 0, NULL, NULL);

		strOnlyID = ucRevData;
		GetLocalTime(&nowtime);
		strTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), nowtime.wYear, nowtime.wMonth, nowtime.wDay, nowtime.wHour, nowtime.wMinute, nowtime.wSecond);
//		AfxMessageBox(strOnlyID);
		pWnd->PutJumpTimeToDB(strOnlyID, strTime);
		TRACE(_T("%s %s\n"),strOnlyID,strTime);
	}
	return 1;
}

BOOL CWinChatterServerDlg::CompareTime(CString strNowTime, CString strLastTime)
{
	/*
	const size_t strsize1 = (strNowTime.GetLength() + 1) * 2; // 宽字符的长度;
	char * pstr1 = new char[strsize1]; //分配空间;
	size_t sz = 0;
	wcstombs_s(&sz, pstr1, strsize1, strNowTime, _TRUNCATE);
	const size_t strsize2 = (strLastTime.GetLength() + 1) * 2; // 宽字符的长度;
	char * pstr2 = new char[strsize2]; //分配空间;
	sz = 0;
	wcstombs_s(&sz, pstr2, strsize2, strLastTime, _TRUNCATE);
	*/
	COleDateTime oleNowTime, oleLastTime;
	COleDateTimeSpan oleSpan;
	int y, m, d, h, u, s;
	swscanf_s(strNowTime, _T("%d-%d-%d %d:%d:%d"), &y, &m, &d, &h, &u, &s);
	oleNowTime.SetDateTime(y, m, d, h, u, s);
	swscanf_s(strLastTime, _T("%d-%d-%d %d:%d:%d"), &y, &m, &d, &h, &u, &s);
	oleLastTime.SetDateTime(y, m, d, h, u, s);

/*	delete[] pstr1;
	delete[] pstr2;*/
	oleSpan = oleNowTime - oleLastTime;
	double cs = oleSpan.GetTotalSeconds();

	if (cs <= ONLINE_TIMEOUT)
		return TRUE;
	else
		return FALSE;
}

UINT CheckOnlineStatus(LPVOID pParam)
{
	AfxOleInit();
	CWinChatterServerDlg *pWnd = (CWinChatterServerDlg*)pParam;
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
		//AfxMessageBox(L"Are you sure?", MB_YESNO | MB_ICONQUESTION);
		return 1;
	}
	_RecordsetPtr pSet;
	pSet.CreateInstance(__uuidof(Recordset));
	while (1)
	{
		pSet->Open(_variant_t("select * from device"), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
		while (!pSet->adoEOF)
		{
			CString strNowTime, strLastTime;
			//获取当前时间字符串
			SYSTEMTIME nowtime;
			GetLocalTime(&nowtime);
			strNowTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), nowtime.wYear, nowtime.wMonth, nowtime.wDay, nowtime.wHour, nowtime.wMinute, nowtime.wSecond);
			//获取当条记录lasttime字符串
			strLastTime = getsqlstr("lasttime");
			//比对返回结果
			BOOL bOnline = pWnd->CompareTime(strNowTime, strLastTime);
			//修改onoff字段
			if (bOnline)
				pSet->put_Collect(_variant_t("isonline"), _variant_t("1"));
			else
				pSet->put_Collect(_variant_t("isonline"), _variant_t("0"));
			pSet->Update();
			//继续下一条记录
			pSet->MoveNext();
		}
		pSet->Close();
		Sleep(1000);
	}
	conn->Close();

	return 1;
}

void CWinChatterServerDlg::PutJumpTimeToDB(CString strOnlyID, CString strJumpTime)
{
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
		MessageBox(L"数据库连接失败",L"失败",MB_OK);
		return;
	}
	_RecordsetPtr pSet;
	pSet.CreateInstance(__uuidof(Recordset));
	CString strSQL;
	strSQL.Format(_T("select * from device where onlyid='%s'"), strOnlyID);
	pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	if (!pSet->adoEOF)
	{
		pSet->put_Collect(_variant_t("lasttime"), _variant_t(strJumpTime));
		pSet->Update();
	}
	pSet->Close();
	conn->Close();
}

void CWinChatterServerDlg::ToTray()
{
	m_tray.CreateTray(IDR_MENU2, this, WM_SHOW_TASK, L"WinChatterServer服务端", this->m_hIcon, true);
	ShowWindow(SW_HIDE);
}
void CWinChatterServerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1)
	{
		KillTimer(1);
		ToTray();
	}
	CDialogEx::OnTimer(nIDEvent);
}
LRESULT CWinChatterServerDlg::OnShowNotify(WPARAM wParam, LPARAM lParam)
{
	return m_tray.OnShowTask(wParam, lParam);
}

void CWinChatterServerDlg::OnExit()
{
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