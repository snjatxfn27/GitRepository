
// WatchDogDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WatchDog.h"
#include "WatchDogDlg.h"
#include "afxdialogex.h"

//查服务
#include  <Winsvc.h>
#pragma comment(lib, "advapi32")
///////////////////ping

#include <Winsock2.h>
#include "iphlpapi.h" 

#pragma comment(lib,"Iphlpapi.lib")
#pragma comment(lib,"Ws2_32.lib")



typedef HANDLE(WINAPI* ICMPCREATEFILE)(VOID);
typedef BOOL(WINAPI* ICMPCLOSEHANDLE)(HANDLE);
typedef DWORD(WINAPI* ICMPSENDECHO)(HANDLE, DWORD, LPVOID, WORD, PIP_OPTION_INFORMATION, LPVOID, DWORD, DWORD);

//定义三个指针函数   
ICMPCREATEFILE pIcmpCreateFile;
ICMPCLOSEHANDLE pIcmpCloseHandle;
ICMPSENDECHO pIcmpSendEcho;


/////////////ping end
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define  PROGRAM_CONTRAST_NAME  "识别软件"
#define  PROGRAM_CHATTER_NAME  "心跳软件"
#define  PROGRAM_SNAPSHOT_NAME  "抓拍软件"

#define  PROGRAM_CONTRAST_PATH "\\..\\CollectionAndContrast\\CollectionAnfContrast.exe"
#define  PROGRAM_CHATTER_PATH  "\\..\\WinChatterServer\\WinChatterServer.exe"
#define  PROGRAM_SNAPSHOT_PATH  "\\..\\WinSnapshotServer\\WinSnapshotServer.exe"

#define COLLECTION_TIME		1
#define CHATTER_TIME		2
#define SNAPSHOT_TIME		3

#define PROGRAM_MAX   3		//程序个数

#define UDP_RECV_PORT 30000  //udp端口

#define CHECK_TIME 70000    //检测时间

#define TIME_MAX   30		//最大时间值

#define  HEARTBEAT  "Heartbeat";

char Progtam_Name[PROGRAM_MAX][126] =
{
	PROGRAM_CONTRAST_NAME,
	PROGRAM_CHATTER_NAME,
	PROGRAM_SNAPSHOT_NAME
};

char Progtam_Path[PROGRAM_MAX][126] =
{
	PROGRAM_CONTRAST_PATH,
	PROGRAM_CHATTER_PATH,
	PROGRAM_SNAPSHOT_PATH
};

int Progtam_Time[PROGRAM_MAX] =
{
	COLLECTION_TIME,
	CHATTER_TIME,
	SNAPSHOT_TIME
};



static SProgramData * g_SProgramArr;

UINT OpenUdpServer(LPVOID lpam);
UINT GuardServerThand(LPVOID lpam);

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


// CWatchDogDlg 对话框



CWatchDogDlg::CWatchDogDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CWatchDogDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWatchDogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOG, m_wndListLog);
}

BEGIN_MESSAGE_MAP(CWatchDogDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONUP()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CWatchDogDlg 消息处理程序

BOOL CWatchDogDlg::OnInitDialog()
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


	//界面
	MoveWindow(0, 0, WIDTH, HEIGHT);
	m_rClient.SetRect(0, 0, WIDTH, HEIGHT);
	m_rXTRJ.SetRect(44, 163, 44 + 200, 163 + 126);
	m_rZPRJ.SetRect(410, 163, 410 + 200, 163 + 126);
	m_rSBRJ.SetRect(778, 163, 778 + 200, 163 + 126);
	m_rIIS.SetRect(44, 344, 44 + 200, 344 + 126);
	m_rMYSQL.SetRect(410, 344, 410 + 200, 344 + 126);
	m_rYPLXJ.SetRect(778, 344, 778 + 200, 344 + 126);
	m_rClose.SetRect(992, 5, 1016, 27);
	m_rMin.SetRect(960, 5, 986, 27);

	m_wndListLog.SetExtendedStyle(LVS_EX_INFOTIP | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_wndListLog.InsertColumn(0, _T("名称 "), LVCFMT_LEFT, 120);
	m_wndListLog.InsertColumn(1, _T("事件"), LVCFMT_LEFT, 160);
	m_wndListLog.InsertColumn(2, _T("时间"), LVCFMT_LEFT, 170);
	m_wndListLog.GetHeaderCtrl()->EnableWindow(FALSE);
	m_wndListLog.MoveWindow(0, 513, 1023, 222);

	m_bIIS = m_bMYSQL = m_bSBRJ = m_bXTRJ = m_bYPLXJ = m_bZPRJ = false;
	m_iDiskPerUse = m_iMemPerUse = 0;

	//双缓存
	CClientDC dc(this);
	pMemDC = new CDC;
	pBitmap = new CBitmap;
	pMemDC->CreateCompatibleDC(&dc);
	pBitmap->CreateCompatibleBitmap(&dc, WIDTH, HEIGHT);

	AfxOleInit();
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
	{
		AfxMessageBox(L"Sock init fail!");
		return false;
	}
	InitIcmp();
	m_csPingIp = "192.168.2.99";

	InitProgram();

	AfxBeginThread(&OpenUdpServer, 0);
	AfxBeginThread(&GuardServerThand, 0);



	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CWatchDogDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CWatchDogDlg::OnPaint()
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
		//界面
		CPaintDC dc(this); // 用于绘制的设备上下文
		CBitmap * pOldBitmap = pMemDC->SelectObject(pBitmap);
		pMemDC->PatBlt(0, 0, WIDTH, HEIGHT, PATCOPY);
		Graphics g(pMemDC->m_hDC);
		//画总背景
		Image img_bk(L"img/bk.jpg");
		g.DrawImage(&img_bk, 0, 0, m_rClient.Width(), m_rClient.Height());
		//IIS
		if (m_bIIS)
		{
			Image img_IIS(L"img/IIS-1.jpg");
			g.DrawImage(&img_IIS, m_rIIS.left, m_rIIS.top, m_rIIS.Width(), m_rIIS.Height() - 34);
		}
		else
		{
			Image img_IIS(L"img/IIS-2.jpg");
			g.DrawImage(&img_IIS, m_rIIS.left, m_rIIS.top, m_rIIS.Width(), m_rIIS.Height() - 34);
		}
		//MYSQL
		if (m_bMYSQL)
		{
			Image img_MYSQL(L"img/MYSQL-1.jpg");
			g.DrawImage(&img_MYSQL, m_rMYSQL.left, m_rMYSQL.top, m_rMYSQL.Width(), m_rMYSQL.Height() - 34);
		}
		else
		{
			Image img_MYSQL(L"img/MYSQL-2.jpg");
			g.DrawImage(&img_MYSQL, m_rMYSQL.left, m_rMYSQL.top, m_rMYSQL.Width(), m_rMYSQL.Height() - 34);
		}
		//YPLXJ
		if (m_bYPLXJ)
		{
			Image img_YPLXJ(L"img/YPLXJ-1.jpg");
			g.DrawImage(&img_YPLXJ, m_rYPLXJ.left, m_rYPLXJ.top, m_rYPLXJ.Width(), m_rYPLXJ.Height() - 34);
		}
		else
		{
			Image img_YPLXJ(L"img/YPLXJ-2.jpg");
			g.DrawImage(&img_YPLXJ, m_rYPLXJ.left, m_rYPLXJ.top, m_rYPLXJ.Width(), m_rYPLXJ.Height() - 34);
		}
		//XTRJ
		if (m_bXTRJ)
		{
			Image img_XTRJ(L"img/XTRJ-1.jpg");
			g.DrawImage(&img_XTRJ, m_rXTRJ.left, m_rXTRJ.top, m_rXTRJ.Width(), m_rXTRJ.Height() - 34);
		}
		else
		{
			Image img_XTRJ(L"img/XTRJ-2.jpg");
			g.DrawImage(&img_XTRJ, m_rXTRJ.left, m_rXTRJ.top, m_rXTRJ.Width(), m_rXTRJ.Height() - 34);
		}
		//ZPRJ
		if (m_bZPRJ)
		{
			Image img_ZPRJ(L"img/ZPRJ-1.jpg");
			g.DrawImage(&img_ZPRJ, m_rZPRJ.left, m_rZPRJ.top, m_rZPRJ.Width(), m_rZPRJ.Height() - 34);
		}
		else
		{
			Image img_ZPRJ(L"img/ZPRJ-2.jpg");
			g.DrawImage(&img_ZPRJ, m_rZPRJ.left, m_rZPRJ.top, m_rZPRJ.Width(), m_rZPRJ.Height() - 34);
		}
		//SBRJ
		if (m_bSBRJ)
		{
			Image img_SBRJ(L"img/SBRJ-1.jpg");
			g.DrawImage(&img_SBRJ, m_rSBRJ.left, m_rSBRJ.top, m_rSBRJ.Width(), m_rSBRJ.Height() - 34);
		}
		else
		{
			Image img_SBRJ(L"img/SBRJ-2.jpg");
			g.DrawImage(&img_SBRJ, m_rSBRJ.left, m_rSBRJ.top, m_rSBRJ.Width(), m_rSBRJ.Height() - 34);
		}
		//硬盘及内存使用量
		Image img_jdt_green(L"img/jdt-green.jpg");
		Image img_jdt_yellow(L"img/jdt-yellow.jpg");
		Image img_jdt_red(L"img/jdt-red.jpg");
		if (m_iDiskPerUse < 80)
			g.DrawImage(&img_jdt_green, 503, 83, m_iDiskPerUse, 24);
		else if (m_iDiskPerUse < 90)
			g.DrawImage(&img_jdt_yellow, 503, 83, m_iDiskPerUse, 24);
		else
			g.DrawImage(&img_jdt_red, 503, 83, m_iDiskPerUse, 24);

		if (m_iMemPerUse < 60)
			g.DrawImage(&img_jdt_green, 817, 83, m_iMemPerUse, 24);
		else if (m_iMemPerUse < 80)
			g.DrawImage(&img_jdt_yellow, 817, 83, m_iMemPerUse, 24);
		else
			g.DrawImage(&img_jdt_red, 817, 83, m_iMemPerUse, 24);
		//硬盘及内存用量文字
		Gdiplus::Font fontPercent(L"微软雅黑", 20, FontStyleRegular, UnitPixel);
		SolidBrush percentBrush(Color(255, 50, 50, 50));
		PointF pointDiskPerUse(615, 82);
		PointF pointMemPerUse(928, 82);
		CString strDiskPerUse, strMemPerUse;
		strDiskPerUse.Format(L"%d％", m_iDiskPerUse);
		g.DrawString(strDiskPerUse.AllocSysString(), -1, &fontPercent, pointDiskPerUse, &percentBrush);
		strMemPerUse.Format(L"%d％", m_iMemPerUse);
		g.DrawString(strMemPerUse.AllocSysString(), -1, &fontPercent, pointMemPerUse, &percentBrush);
		//6个程序状态使用的文字
		Gdiplus::Font fontState(L"宋体", 18, FontStyleRegular, UnitPixel);
		SolidBrush stateBrush(Color(255, 50, 50, 50));
		PointF pointXT(130, 263);
		PointF pointZP(494, 263);
		PointF pointSB(860, 263);
		PointF pointIIS(130, 444);
		PointF pointMYSQL(494, 444);
		PointF pointYP(860, 444);
		CString strDis("");
		if (m_bXTRJ) strDis = "启动";
		else strDis = "终止";
		g.DrawString(strDis.AllocSysString(), -1, &fontState, pointXT, &stateBrush);
		if (m_bZPRJ) strDis = "启动";
		else strDis = "终止";
		g.DrawString(strDis.AllocSysString(), -1, &fontState, pointZP, &stateBrush);
		if (m_bSBRJ) strDis = "启动";
		else strDis = "终止";
		g.DrawString(strDis.AllocSysString(), -1, &fontState, pointSB, &stateBrush);
		if (m_bIIS) strDis = "启动";
		else strDis = "终止";
		g.DrawString(strDis.AllocSysString(), -1, &fontState, pointIIS, &stateBrush);
		if (m_bMYSQL) strDis = "启动";
		else strDis = "终止";
		g.DrawString(strDis.AllocSysString(), -1, &fontState, pointMYSQL, &stateBrush);
		if (m_bYPLXJ) strDis = "连通";
		else strDis = "断开";
		g.DrawString(strDis.AllocSysString(), -1, &fontState, pointYP, &stateBrush);

		dc.BitBlt(0, 0, WIDTH, HEIGHT, pMemDC, 0, 0, SRCCOPY);
		pMemDC->SelectObject(pOldBitmap);
		g.ReleaseHDC(pMemDC->m_hDC);

		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CWatchDogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CWatchDogDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	// 	界面
	// 		if (m_rClose.PtInRect(point))
	// 		{
	// 			/*		SetIISImage(true);
	// 					SetDiskPerUse(92);
	// 					SetMemPerUse(81);*/
	// 			PostMessage(WM_CLOSE);
	// 		}
	// 		else if (m_rMin.PtInRect(point))
	// 		{
	// 			/*		SetIISImage(false);
	// 					SetDiskPerUse(81);
	// 					SetMemPerUse(54);*/
	// 			PostMessage(WM_SYSCOMMAND, SC_MINIMIZE);
	// 		}

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CWatchDogDlg::SetIISImage(BOOL bIIS)
{
	if (m_bIIS == bIIS)
		return;
	m_bIIS = bIIS;
	InvalidateRect(m_rIIS, false);
	Sleep(500);
	//RedrawWindow(m_rIIS, false);
}
void CWatchDogDlg::SetMYSQLImage(BOOL bMYSQL)
{
	if (m_bMYSQL == bMYSQL)
		return;
	m_bMYSQL = bMYSQL;
	InvalidateRect(m_rMYSQL, false);
	Sleep(500);
	//RedrawWindow(m_rMYSQL, false);
}
void CWatchDogDlg::SetYPLXJImage(BOOL bYPLXJ)
{
	if (m_bYPLXJ == bYPLXJ)
		return;
	m_bYPLXJ = bYPLXJ;
	InvalidateRect(m_rYPLXJ, false);
	Sleep(500);
	//	RedrawWindow(m_rYPLXJ, false);
}
void CWatchDogDlg::SetXTRJImage(BOOL bXTRJ)
{
	if (m_bXTRJ == bXTRJ)
		return;
	m_bXTRJ = bXTRJ;
	InvalidateRect(m_rXTRJ, false);
	Sleep(500);
	//RedrawWindow(m_rXTRJ, false);
}
void CWatchDogDlg::SetZPRJImage(BOOL bZPRJ)
{
	if (m_bZPRJ == bZPRJ)
		return;
	m_bZPRJ = bZPRJ;
	InvalidateRect(m_rZPRJ, false);
	Sleep(500);
	//RedrawWindow(m_rZPRJ, false);
}
void CWatchDogDlg::SetSBRJImage(BOOL bSBRJ)
{
	if (m_bSBRJ == bSBRJ)
		return;
	m_bSBRJ = bSBRJ;
	InvalidateRect(m_rSBRJ, false);
	Sleep(500);
	//RedrawWindow(m_rSBRJ, false);

}
void CWatchDogDlg::SetDiskPerUse(int iDPU)
{
	if (m_iDiskPerUse == iDPU)
		return;
	if (iDPU < 0) iDPU = 0;
	if (iDPU > 100) iDPU = 100;
	m_iDiskPerUse = iDPU;
	InvalidateRect(CRect(485, 53, 715, 121), false);
	Sleep(500);

	//RedrawWindow(CRect(485, 53, 715, 121), false);

}
void CWatchDogDlg::SetMemPerUse(int iMPU)
{
	if (m_iDiskPerUse == iMPU)
		return;
	if (iMPU < 0) iMPU = 0;
	if (iMPU > 100) iMPU = 100;
	m_iMemPerUse = iMPU;
	InvalidateRect(CRect(766, 53, 1011, 121), false);
	Sleep(500);
	//RedrawWindow(CRect(766, 53, 1011, 121), false);
}

void CWatchDogDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO:  在此处添加消息处理程序代码

	//界面
	delete pMemDC;
	delete pBitmap;
}
//开启udpserver接收心跳
UINT OpenUdpServer(LPVOID lpam)
{
	CWatchDogDlg *dlg = (CWatchDogDlg *)AfxGetApp()->GetMainWnd();

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
					CString csTemp(szBuffer);
					csTemp = csTemp.Mid(0, nLen);
					CString CSHeart = L"Heartbeat_";
					if (csTemp.Find(CSHeart) == 0)
					{
						int nFindLen = CSHeart.GetLength();
						CString csProgramIndex = csTemp.Mid(nFindLen, 1);
						USES_CONVERSION;
						char *pChStr = T2A(csProgramIndex);
						g_SProgramArr[atoi(pChStr)].cLastTime = COleDateTime::GetCurrentTime();
					}
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
//判断心跳 及界面输出
UINT GuardServerThand(LPVOID lpam)
{
	CWatchDogDlg *dlg = (CWatchDogDlg *)AfxGetApp()->GetMainWnd();
	COleDateTime cLastTime;
	AfxOleInit();

	while (true)
	{
		try
		{

			for (int nIndex = 0; nIndex < PROGRAM_MAX; nIndex++)
			{
				if (!g_SProgramArr[nIndex].bFlag)
				{
					continue;
				}
				BOOL bCompaare = dlg->CompareTime(g_SProgramArr[nIndex].cLastTime, nIndex);

				if (bCompaare == FALSE)
				{
					dlg->SetProgramIndex(nIndex, FALSE);
					dlg->SetListData(g_SProgramArr[nIndex].csName, L"心跳超时", g_SProgramArr[nIndex].cLastTime);

					if (!dlg->IsFindProgram(nIndex))
					{
						if (dlg->OpenProgram(nIndex))
						{
							dlg->SetProgramIndex(nIndex, TRUE);
							dlg->SetListData(g_SProgramArr[nIndex].csName, L"重启成功", g_SProgramArr[nIndex].cLastTime);
						}
						else
							dlg->SetListData(g_SProgramArr[nIndex].csName, L"重启失败", g_SProgramArr[nIndex].cLastTime);

					}
					else
					{
						if (dlg->CloseProgram(nIndex))
						{
							if (dlg->OpenProgram(nIndex))
							{
								dlg->SetProgramIndex(nIndex, TRUE);
								dlg->SetListData(g_SProgramArr[nIndex].csName, L"重启成功", g_SProgramArr[nIndex].cLastTime);
							}
							else
								dlg->SetListData(g_SProgramArr[nIndex].csName, L"重启失败", g_SProgramArr[nIndex].cLastTime);
						}
						else
							dlg->SetListData(g_SProgramArr[nIndex].csName, L"重启失败", g_SProgramArr[nIndex].cLastTime);
					}
				}
				else
				{
					dlg->SetProgramIndex(nIndex, TRUE);
				}
			}

			dlg->SetDiskPerUse(dlg->getDrive(L"C"));
			dlg->SetMemPerUse(dlg->getMemUsage());
			dlg->SetMYSQLImage(dlg->GetMySqlState());
			USES_CONVERSION;
			dlg->SetYPLXJImage(dlg->ICMPPing(T2A(dlg->m_csPingIp)));
			dlg->SetIISImage(dlg->IsSvcRun(L"WAS"));

			Sleep(CHECK_TIME);
		}
		catch (CException* e)
		{
		}

	}

	return 0;
}

//初始化3个server 
void CWatchDogDlg::InitProgram()
{

	CString stTemp;
	TCHAR pFileName[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, pFileName);
	CString csFullPath(pFileName);

	g_SProgramArr = new SProgramData[PROGRAM_MAX];

	for (int nProcessIndex = 0; nProcessIndex < PROGRAM_MAX; nProcessIndex++)
	{
		CString stTemp1(Progtam_Path[nProcessIndex]);
		stTemp = csFullPath + stTemp1;
		g_SProgramArr[nProcessIndex].bExist = FALSE;
		g_SProgramArr[nProcessIndex].nTimeIdex = Progtam_Time[nProcessIndex];
		g_SProgramArr[nProcessIndex].nTimeMax = TIME_MAX;
		g_SProgramArr[nProcessIndex].csPath = stTemp;
		g_SProgramArr[nProcessIndex].csName = Progtam_Name[nProcessIndex];
		if (GetProgramHandle(nProcessIndex))
			g_SProgramArr[nProcessIndex].bFlag = TRUE;
		else
			g_SProgramArr[nProcessIndex].bFlag = FALSE;
	}
}
//获取程序的handle
BOOL CWatchDogDlg::GetProgramHandle(int nIndex)
{
	CWnd*pWnd;
	pWnd = CWnd::FindWindowW(NULL, g_SProgramArr[nIndex].csName);
	if (pWnd != NULL)
	{
		HWND hWnd;
		hWnd = pWnd->GetSafeHwnd();
		DWORD ThreadID = -1;
		DWORD ProcessID = -1;
		ThreadID = GetWindowThreadProcessId(hWnd, &ProcessID);
		g_SProgramArr[nIndex].hProgramHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessID);
		g_SProgramArr[nIndex].cLastTime = COleDateTime::GetCurrentTime();
		SetListData(g_SProgramArr[nIndex].csName, L"获取成功", g_SProgramArr[nIndex].cLastTime);
		return TRUE;
	}
	else
	{
		if (OpenProgram(nIndex))
		{
			g_SProgramArr[nIndex].cLastTime = COleDateTime::GetCurrentTime();
			SetListData(g_SProgramArr[nIndex].csName, L"开启成功", g_SProgramArr[nIndex].cLastTime);
			return TRUE;
		}
	}

	return FALSE;
}
//打开程序
BOOL CWatchDogDlg::OpenProgram(int nIndex)
{

	LPTSTR szCmdline = _tcsdup((g_SProgramArr[nIndex].csPath));
	ZeroMemory(&(g_SProgramArr[nIndex].si), sizeof((g_SProgramArr[nIndex].si)));
	(g_SProgramArr[nIndex].si).cb = sizeof((g_SProgramArr[nIndex].si));
	ZeroMemory(&(g_SProgramArr[nIndex].Process), sizeof((g_SProgramArr[nIndex].Process)));

	if (!CreateProcess(szCmdline,
		NULL,
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&(g_SProgramArr[nIndex].si),
		&(g_SProgramArr[nIndex].Process))
		)
	{
		g_SProgramArr[nIndex].hProgramHandle = NULL;
		g_SProgramArr[nIndex].bExist = FALSE;
		return FALSE;
	}
	else
	{
		g_SProgramArr[nIndex].bExist = TRUE;
		g_SProgramArr[nIndex].hProgramHandle = g_SProgramArr[nIndex].Process.hProcess;
		return TRUE;
	}

	return FALSE;
}
//关闭程序
BOOL CWatchDogDlg::CloseProgram(int nIndex)
{
	return TerminateProcess(g_SProgramArr[nIndex].hProgramHandle, 0);
}
//判断程序是否存在
BOOL CWatchDogDlg::IsFindProgram(int nIndex)
{
	CString strTitle = g_SProgramArr[nIndex].csName;
	CWnd*pWnd;
	pWnd = CWnd::FindWindowW(NULL, strTitle);
	if (pWnd != NULL)
	{
		return TRUE;
	}
	return FALSE;
}
//时间对比
BOOL CWatchDogDlg::CompareTime(COleDateTime StartTime, int nInexe)
{
	COleDateTimeSpan delta;
	COleDateTime CurrentTime;
	CurrentTime = COleDateTime::GetCurrentTime();

	delta = CurrentTime - StartTime;
	CString date = StartTime.Format(L"%Y-%m-%d %H:%M:%S");
	CString date1 = CurrentTime.Format(L"%Y-%m-%d %H:%M:%S");
	CString cstemp;

	double dTimeInterval = delta.GetTotalSeconds();
	//	cstemp.Format(L"%s_%s_%s_%d", date, date1, g_SProgramArr[nInexe].csName, dTimeInterval);
	//	Log_Error(cstemp);;
	if (dTimeInterval <= TIME_MAX)
	{
		return TRUE;
	}
	else
		return FALSE;
}
//输出log
void CWatchDogDlg::Log_Error(CString csLog)
{
	//wchar_t 转为 char
	int iSize;
	char* pszMultiByte;
	iSize = WideCharToMultiByte(CP_ACP, 0, csLog, -1, NULL, 0, NULL, NULL);
	pszMultiByte = (char*)malloc(iSize*sizeof(char));
	WideCharToMultiByte(CP_ACP, 0, csLog, -1, pszMultiByte, iSize, NULL, NULL);

	DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, pszMultiByte);
	free(pszMultiByte);
}
//获取所有盘的名称 总大小，使用大小
int CWatchDogDlg::GetDiskCapacity(CStringArray &arNmae, CStringArray &arTotal, CStringArray &arCurrent)
{
	int nSum = 0;
	int DiskCount = 0;
	DWORD DiskInfo = GetLogicalDrives();
	while (DiskInfo)
	{
		if (DiskInfo & 1)
		{
			++DiskCount;
		}
		DiskInfo = DiskInfo >> 1;
	}
	//-----------------------------------------------------------------------------------------  
	int DSLength = GetLogicalDriveStrings(0, NULL);
	wchar_t* DStr = new wchar_t[DSLength];
	GetLogicalDriveStrings(DSLength, (LPTSTR)DStr);

	int DType;
	int si = 0;
	BOOL fResult;
	unsigned _int64 i64FreeBytesToCaller;
	unsigned _int64 i64TotalBytes;
	unsigned _int64 i64FreeBytes;

	for (int i = 0; i < DSLength / 4; ++i)
	{
		wchar_t dir[4] = { DStr[si], ':', '\\' };
		DType = GetDriveTypeW(DStr + i * 4);
		if (DType == DRIVE_FIXED)
		{
			//	cout << "Hard Disk";
		}
		else if (DType == DRIVE_CDROM)
		{
			//cout << "CD-ROM";
		}
		else if (DType == DRIVE_REMOVABLE)
		{
			//cout << "Removable Disk";
		}
		else if (DType == DRIVE_REMOTE)
		{
			//cout << "Network Disk";
		}
		else if (DType == DRIVE_RAMDISK)
		{
			//cout << "Virtual RAM Disk";
		}
		else if (DType == DRIVE_UNKNOWN)
		{
			//cout << "Unknown Device";
		}

		fResult = GetDiskFreeSpaceEx(
			dir,
			(PULARGE_INTEGER)&i64FreeBytesToCaller,
			(PULARGE_INTEGER)&i64TotalBytes,
			(PULARGE_INTEGER)&i64FreeBytes);
		if (fResult)
		{
			nSum++;
			double dTotal = (float)i64TotalBytes / 1024 / 1024 / 1024;//总
			double dCurrent = (float)i64FreeBytesToCaller / 1024 / 1024 / 1024;//当前
			CString csTemp(dir);
			arNmae.Add(csTemp);
			csTemp.Format(L"%f", dTotal);
			arTotal.Add(csTemp);
			csTemp.Format(L"%f", dCurrent);
			arCurrent.Add(csTemp);
		}
		else
		{
			//	cout << " 设备未准备好";
		}
		si += 4;
	}
	return nSum;
}
//获取sql的状态
BOOL CWatchDogDlg::GetMySqlState()
{

	CString csSqlConnect;
	_ConnectionPtr  pConnect;
	csSqlConnect = "Driver={MySQL ODBC 5.1 Driver};Server = 127.0.0.1; Database = facedetectsys; uid = root;pwd = cr;";
	//csSqlConnect = "Driver={MySQL ODBC 5.1 Driver};Server = 192.168.2.66; Database = facedetectsys; uid = root;pwd = cr;";
	try
	{
		pConnect.CreateInstance(__uuidof(Connection));
		_bstr_t strConnect = csSqlConnect;
		pConnect->Open(strConnect, "", "", adModeUnknown);
	}
	catch (_com_error e)
	{
		return FALSE;
	}
	pConnect->Close();
	return  TRUE;
}
//获取制定盘的使用率
double CWatchDogDlg::getDrive(CString csDrive)
{
	USES_CONVERSION;
	CStringArray arNmae, arTotal, arCurrent;
	int  nSum = GetDiskCapacity(arNmae, arTotal, arCurrent);
	for (int i = 0; i < nSum; i++)
	{
		if (arNmae.GetAt(i).Find(csDrive) == 0)
		{
			CString csTotal = arTotal.GetAt(i);
			CString csCurrent = arCurrent.GetAt(i);
			return  (1 - (atof(T2A(csCurrent)) / atof(T2A(csTotal)))) * 100;
		}
	}
	return -1;
}
//获取内存使用率
DWORD CWatchDogDlg::getMemUsage()
{
	MEMORYSTATUS ms;
	::GlobalMemoryStatus(&ms);
	return ms.dwMemoryLoad;
}
//初始化icmp
BOOL CWatchDogDlg::InitIcmp()
{
	HINSTANCE hIcmp = LoadLibrary(L"ICMP.DLL");
	if (hIcmp == NULL)

	{
		return false;

	}
	pIcmpCreateFile = (ICMPCREATEFILE)GetProcAddress(hIcmp, "IcmpCreateFile");
	pIcmpCloseHandle = (ICMPCLOSEHANDLE)GetProcAddress(hIcmp, "IcmpCloseHandle");
	pIcmpSendEcho = (ICMPSENDECHO)GetProcAddress(hIcmp, "IcmpSendEcho");
	if ((pIcmpCreateFile == NULL) || (pIcmpCloseHandle == NULL) || (pIcmpSendEcho == NULL))
		return false;
	return true;
}

// 函数功能：判断是否能ping通IP  IP地址或域名
BOOL CWatchDogDlg::ICMPPing(char* host)
{
	DWORD timeOut = 1000;                                              //设置超时   
	ULONG hAddr = inet_addr(host);                                     //如果是IP地址就直接转换   
	if (hAddr == INADDR_NONE)
	{
		hostent* hp = gethostbyname(host);                             //如果是域名就用DNS解析出IP地址   
		if (hp)
			memcpy(&hAddr, hp->h_addr_list, hp->h_length);             //IP地址   
		else
		{
			return false;
		}
	}
	HANDLE hIp = pIcmpCreateFile();
	IP_OPTION_INFORMATION ipoi;
	memset(&ipoi, 0, sizeof(IP_OPTION_INFORMATION));
	ipoi.Ttl = 128;                  //Time-To-Live   

	unsigned char pSend[36];                                                                   //发送包   
	memset(pSend, 'E', 32);

	int repSize = sizeof(ICMP_ECHO_REPLY)+32;
	unsigned char pReply[100];                                                                 //接收包   
	ICMP_ECHO_REPLY* pEchoReply = (ICMP_ECHO_REPLY*)pReply;

	DWORD nPackets = pIcmpSendEcho(hIp, hAddr, pSend, 32, &ipoi, pReply, repSize, timeOut);             //发送ICMP数据报文   

	if (pEchoReply->Status != 0)                                                                  //超时，可能是主机禁用了ICMP 或者目标主机不存在  
	{
		pIcmpCloseHandle(hIp);
		return false;
	}

	pIcmpCloseHandle(hIp);
	return true;
}

bool CWatchDogDlg::IsSvcRun(LPCTSTR lpszSvcName)
{
	SERVICE_STATUS svcStatus = { 0 };
	return QueryServiceStatus(OpenService(OpenSCManager(NULL, NULL, GENERIC_READ), lpszSvcName, GENERIC_READ), &svcStatus) ? (svcStatus.dwCurrentState == SERVICE_RUNNING) : false;
}

void CWatchDogDlg::SetProgramIndex(int nInexe, BOOL bFlag)
{
	if (nInexe == 0)
	{
		SetSBRJImage(bFlag);
	}
	else  if (nInexe == 1)
	{
		SetXTRJImage(bFlag);
	}
	else  if (nInexe == 2)
	{
		SetZPRJImage(bFlag);
	}
}

void CWatchDogDlg::SetListData(CString cname, CString cevent, COleDateTime ctime)
{
	int row = m_wndListLog.GetItemCount();
	m_wndListLog.InsertItem(row, L"");
	m_wndListLog.SetItemText(row, 0, cname);
	m_wndListLog.SetItemText(row, 1, cevent);
	CString date = ctime.Format(L"%Y-%m-%d %H:%M:%S");
	m_wndListLog.SetItemText(row, 2, date);
}

BOOL CWatchDogDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN&&pMsg->wParam == VK_RETURN)
		return TRUE;
	if (pMsg->message == WM_KEYDOWN&&pMsg->wParam == VK_ESCAPE)
		return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}

void CWatchDogDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	//界面
	if (m_rClose.PtInRect(point))
	{
		/*		SetIISImage(true);
		SetDiskPerUse(92);
		SetMemPerUse(81);*/
		PostMessage(WM_CLOSE);
	}
	else if (m_rMin.PtInRect(point))
	{
		/*		SetIISImage(false);
		SetDiskPerUse(81);
		SetMemPerUse(54);*/
		PostMessage(WM_SYSCOMMAND, SC_MINIMIZE);
	}
	else
	{
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}
