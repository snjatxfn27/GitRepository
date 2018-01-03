
// GuardServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GuardServer.h"
#include "GuardServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define  PROGRAM_CONTRAST_NAME  L"多路识别"
#define  PROGRAM_CHATTER_NAME  L"心跳"
#define  PROGRAM_SNAPSHOT_NAME  L"抓拍"

#define  PROGRAM_CONTRAST_PATH L"\\..\\CollectionAndContrast\\CollectionAnfContrast.exe"
#define  PROGRAM_CHATTER_PATH   L"\\..\\CollectionAndContrast\\WinChatterServer.exe"
#define  PROGRAM_SNAPSHOT_PATH  L"\\..\\CollectionAndContrast\\WinSnapshotServer.exe"


#define UDP_RECV_PORT 30000

#define CHECK_TIME 30000

#define TIME_MAX   30

#define COLLECTION_TIME		1
#define CHATTER_TIME		2
#define SNAPSHOT_TIME		3


UINT OpenUdpServer(LPVOID lpam);
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


// CGuardServerDlg 对话框



CGuardServerDlg::CGuardServerDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CGuardServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGuardServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGuardServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CGuardServerDlg::OnBnClickedOk)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CGuardServerDlg 消息处理程序

BOOL CGuardServerDlg::OnInitDialog()
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

	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
	{
		AfxMessageBox(L"Sock init fail!");
		return false;
	}

	InitProgram();

	AfxBeginThread(&OpenUdpServer, 0);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGuardServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGuardServerDlg::OnPaint()
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
HCURSOR CGuardServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

UINT OpenUdpServer(LPVOID lpam)
{
	CGuardServerDlg *dlg = (CGuardServerDlg *)AfxGetApp()->GetMainWnd();

	SOCKET sock;
	SOCKADDR_IN sin;
	BOOL bWhile = TRUE;
	BOOL bWhileRecv = TRUE;

	while (bWhile)
	{
		try
		{
			sock = socket(AF_INET, SOCK_DGRAM, 0);
			if (sock == SOCKET_ERROR)
			{
				dlg->Log_Error(L"Sock create fail!");
				continue;
			}

			sin.sin_family = AF_INET;
			sin.sin_addr.s_addr = htonl(INADDR_ANY);
			sin.sin_port = htons(UDP_RECV_PORT);

			if (bind(sock, (SOCKADDR *)&sin, sizeof(sin)) == SOCKET_ERROR)
			{
				dlg->Log_Error(L"sock bind error code!");
				closesocket(sock);
				continue;
			}

			while (bWhileRecv)
			{
				char szBuffer[4096];
				DWORD dwRead = 0;
				SOCKADDR sa;
				int nLen;
				int nSaLen = sizeof(sa);

				nLen = recvfrom(sock, szBuffer, sizeof(szBuffer), 0, &sa, &nSaLen);

				CString csTemp(szBuffer);
				csTemp = csTemp.Mid(0, nLen);
				if (nLen <= 0)
				{
					dlg->Log_Error(L"Data receive fail!");
					closesocket(sock);
					continue;
				}
				else
				{
					//收到信息
					//CString strIPAddress(inet_ntoa(((SOCKADDR_IN *)&sa)->sin_addr));  //取IP地址
					//WORD wPort = ((SOCKADDR_IN *)&sa)->sin_port;                       //取端口号


				}

			}
		}
		catch (CException* e)
		{

		}
	}
	closesocket(sock);
	WSACleanup();
	AfxEndThread(0);
	return 0;
}




void CGuardServerDlg::InitProgram()
{

	CString stTemp;
	TCHAR pFileName[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, pFileName);
	CString csFullPath(pFileName);

	if (!IsFindProgram(PROGRAM_CONTRAST_NAME, 0))
	{
		stTemp = csFullPath + PROGRAM_CONTRAST_PATH;

		HINSTANCE hShell = ShellExecute(NULL, L"open", stTemp, NULL, NULL, SW_SHOWNORMAL);
		if ((long)hShell > 32)
		{
			m_bCollection = TRUE;
			m_cTimeOldChatter = CTime::GetCurrentTime();
			SetTimer(COLLECTION_TIME, CHECK_TIME, NULL);
		}
		else
		{
			m_bCollection = FALSE;

		}
	}


	if (!IsFindProgram(PROGRAM_CHATTER_NAME, 0))
	{
		HINSTANCE hShell = ShellExecute(NULL, L"open", PROGRAM_CHATTER_NAME, NULL, NULL, SW_SHOWNORMAL);
		if ((long)hShell > 32)
		{
			m_bChatter = TRUE;
			m_cTimeOldSnapshot = CTime::GetCurrentTime();
			SetTimer(CHATTER_TIME, CHECK_TIME, NULL);
		}
		else
		{
			m_bChatter = FALSE;

		}
	}


	if (!IsFindProgram(PROGRAM_SNAPSHOT_NAME, 0))
	{
		HINSTANCE hShell = ShellExecute(NULL, L"open", PROGRAM_SNAPSHOT_NAME, NULL, NULL, SW_SHOWNORMAL);
		if ((long)hShell > 32)
		{
			m_bSnapsho = TRUE;
			m_cTimeOldCollection = CTime::GetCurrentTime();
			SetTimer(SNAPSHOT_TIME, CHECK_TIME, NULL);
		}
		else
		{
			m_bSnapsho = FALSE;
		}
	}

}


BOOL CGuardServerDlg::IsFindProgram(CString strTitle, int nType)
{
	CWnd*pWnd;
	pWnd = CWnd::FindWindowW(NULL, strTitle);
	if (pWnd != NULL)
	{
		if (nType == 0)
		{
			return TRUE;
		}
		else if (nType == 1)
		{
			pWnd->SendMessage(WM_CLOSE);
			return TRUE;
		}
	}
	return FALSE;
}
BOOL CGuardServerDlg::CompareTime(CTime StartTime)
{
	CTimeSpan delta;
	CTime CurrentTime;
	CurrentTime = CTime::GetCurrentTime();

	delta = CurrentTime - StartTime;

	double dTimeInterval = delta.GetTotalSeconds();
	if (dTimeInterval > TIME_MAX)
	{
		return FALSE;
	}
	return TRUE;
}
void CGuardServerDlg::Log_Error(CString csLog)
{
	//wchar_t 转为 char
	int iSize;
	char* pszMultiByte;
	//返回接受字符串所需缓冲区的大小，已经包含字符结尾符'\0'
	iSize = WideCharToMultiByte(CP_ACP, 0, csLog, -1, NULL, 0, NULL, NULL); //iSize =wcslen(pwsUnicode)+1=6
	pszMultiByte = (char*)malloc(iSize*sizeof(char)); //不需要 pszMultiByte = (char*)malloc(iSize*sizeof(char)+1);
	WideCharToMultiByte(CP_ACP, 0, csLog, -1, pszMultiByte, iSize, NULL, NULL);

	DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, pszMultiByte);
	free(pszMultiByte);
}
void CGuardServerDlg::OnBnClickedOk()
{
	CString strIp;
	TCHAR pFileName[MAX_PATH];
	int nPos = GetCurrentDirectory(MAX_PATH, pFileName);

	CString csFullPath(pFileName);
	//ShellExecute(NULL, L"open", csFullPath, NULL, NULL, SW_SHOWNORMAL);
	m_cTimeOldSnapshot = CTime::GetCurrentTime();
	Sleep(2000);
	m_cTimeOldChatter = CTime::GetCurrentTime();
	BOOL ssssss = CompareTime(m_cTimeOldSnapshot);

	int t = 0;
	//CDialogEx::OnOK();
}


void CGuardServerDlg::OnTimer(UINT_PTR nIDEvent)
{
	CString stTemp;
	TCHAR pFileName[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, pFileName);
	CString csFullPath(pFileName);

	if (nIDEvent == COLLECTION_TIME)
	{
		if (!CompareTime(m_cTimeOldCollection))
		{
			KillTimer(COLLECTION_TIME);
			IsFindProgram(PROGRAM_CONTRAST_NAME, 1);
			m_bCollection = FALSE;

			stTemp = csFullPath + PROGRAM_CONTRAST_PATH;
			HINSTANCE hShell = ShellExecute(NULL, L"open", stTemp, NULL, NULL, SW_SHOWNORMAL);
			if ((long)hShell > 32)
			{
				m_bCollection = TRUE;
				m_cTimeOldChatter = CTime::GetCurrentTime();
				SetTimer(COLLECTION_TIME, CHECK_TIME, NULL);
			}
		}
	}
	else if (nIDEvent == CHATTER_TIME)
	{
		if (!CompareTime(m_cTimeOldChatter))
		{
			KillTimer(CHATTER_TIME);
			m_bChatter = FALSE;

			IsFindProgram(PROGRAM_CHATTER_NAME, 1);
			stTemp = csFullPath + PROGRAM_CHATTER_PATH;
			HINSTANCE hShell = ShellExecute(NULL, L"open", stTemp, NULL, NULL, SW_SHOWNORMAL);
			if ((long)hShell > 32)
			{
				m_bChatter = TRUE;
				m_cTimeOldChatter = CTime::GetCurrentTime();
				SetTimer(CHATTER_TIME, CHECK_TIME, NULL);
			}
		}
	}
	else if (nIDEvent == SNAPSHOT_TIME)
	{
		if (!CompareTime(m_cTimeOldSnapshot))
		{
			KillTimer(SNAPSHOT_TIME);
			m_bSnapsho = FALSE;

			IsFindProgram(PROGRAM_SNAPSHOT_NAME, 1);
			stTemp = csFullPath + PROGRAM_SNAPSHOT_PATH;
			HINSTANCE hShell = ShellExecute(NULL, L"open", stTemp, NULL, NULL, SW_SHOWNORMAL);
			if ((long)hShell > 32)
			{
				m_bSnapsho = TRUE;
				m_cTimeOldSnapshot = CTime::GetCurrentTime();
				SetTimer(SNAPSHOT_TIME, CHECK_TIME, NULL);
			}
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}
