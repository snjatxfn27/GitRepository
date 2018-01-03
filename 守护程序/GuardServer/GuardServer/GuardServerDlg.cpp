
// GuardServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GuardServer.h"
#include "GuardServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define  PROGRAM_CONTRAST_NAME  "��·ʶ��"
#define  PROGRAM_CHATTER_NAME  "WinChatterServer"
#define  PROGRAM_SNAPSHOT_NAME  "WinSnapshotServer"

#define  PROGRAM_CONTRAST_PATH "\\..\\CollectionAndContrast\\CollectionAnfContrast.exe"
#define  PROGRAM_CHATTER_PATH  "\\..\\WinChatterServer\\WinChatterServer.exe"
#define  PROGRAM_SNAPSHOT_PATH  "\\..\\WinSnapshotServer\\WinSnapshotServer.exe"

#define STATE_START L"����"
#define STATE_OK L"����"
#define STATE_ON L"�ر�"

#define COLLECTION_TIME		1
#define CHATTER_TIME		2
#define SNAPSHOT_TIME		3

#define PROGRAM_MAX   3		//�������

#define UDP_RECV_PORT 30000  //udp�˿�

#define CHECK_TIME 4000    //���ʱ��

#define TIME_MAX   5		//���ʱ��ֵ

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
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
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


// CGuardServerDlg �Ի���



CGuardServerDlg::CGuardServerDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CGuardServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGuardServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOG, m_wndListLog);
}

BEGIN_MESSAGE_MAP(CGuardServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CGuardServerDlg::OnBnClickedOk)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CGuardServerDlg ��Ϣ�������

BOOL CGuardServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	InitializeCriticalSection(&cSECTION);

	m_wndListLog.SetExtendedStyle(LVS_EX_INFOTIP | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_wndListLog.InsertColumn(0, _T("���� "), LVCFMT_LEFT, 120);
	m_wndListLog.InsertColumn(1, _T("״̬"), LVCFMT_LEFT, 80);
	m_wndListLog.InsertColumn(2, _T("�¼�"), LVCFMT_LEFT, 160);
	m_wndListLog.InsertColumn(3, _T("ʱ��"), LVCFMT_LEFT, 170);
	m_wndListLog.GetHeaderCtrl()->EnableWindow(FALSE);

	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
	{
		AfxMessageBox(L"Sock init fail!");
		return false;
	}

	InitProgram();
	AfxBeginThread(&OpenUdpServer, 0);
	AfxBeginThread(&GuardServerThand, 0);



	COleDateTime sss;
	//	SetListData(L"1", L"1222", L"2", sss);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CGuardServerDlg::OnPaint()
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
HCURSOR CGuardServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CGuardServerDlg::SetListData(CString cname, CString cstate, CString cevent, COleDateTime ctime)
{
	try
	{
		int row = m_wndListLog.GetItemCount();
		m_wndListLog.InsertItem(row, L"");
		m_wndListLog.SetItemText(row, 0, cname);
		m_wndListLog.SetItemText(row, 1, cstate);
		m_wndListLog.SetItemText(row, 2, cevent);
		CString date = ctime.Format(L"%Y-%m-%d %H:%M:%S");
		m_wndListLog.SetItemText(row, 3, date);
	}
	catch (CException* e)
	{
	}
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


				if (nLen <= 0)
				{
					dlg->Log_Error(L"Data receive fail!");
					closesocket(sock);
					continue;
				}
				else
				{
					//�յ���Ϣ
					//CString strIPAddress(inet_ntoa(((SOCKADDR_IN *)&sa)->sin_addr));  //ȡIP��ַ
					//WORD wPort = ((SOCKADDR_IN *)&sa)->sin_port;                       //ȡ�˿ں�
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

UINT GuardServerThand(LPVOID lpam)
{
	CGuardServerDlg *dlg = (CGuardServerDlg *)AfxGetApp()->GetMainWnd();
	COleDateTime cLastTime;

	while (true)
	{
		try
		{
			for (int nIndex = 0; nIndex < PROGRAM_MAX; nIndex++)
			{
				BOOL bCompaare = dlg->CompareTime(g_SProgramArr[nIndex].cLastTime, nIndex);
			
				if (bCompaare == FALSE)
				{
					dlg->SetListData(g_SProgramArr[nIndex].csName, STATE_ON, L"������ʱ", g_SProgramArr[nIndex].cLastTime);

					if (!dlg->IsFindProgram(nIndex))
					{
						if (dlg->OpenProgram(nIndex))
						{
							dlg->SetListData(g_SProgramArr[nIndex].csName, STATE_ON, L"��������", g_SProgramArr[nIndex].cLastTime);

						}
					}
					else
					{
						if (dlg->CloseProgram(nIndex))
						{
							if (dlg->OpenProgram(nIndex))
							{
								dlg->SetListData(g_SProgramArr[nIndex].csName, STATE_ON, L"��������", g_SProgramArr[nIndex].cLastTime);
							}
						}
					}
				}
			}

			Sleep(CHECK_TIME);
		}
		catch (CException* e)
		{
		}

	}
	
	return 0;
}

void CGuardServerDlg::InitProgram()
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
		//stTemp.Format(L"%s%s",csFullPath , Progtam_Path[nProcessIndex]);
		//MessageBox(stTemp);
		g_SProgramArr[nProcessIndex].bExist = FALSE;
		g_SProgramArr[nProcessIndex].nTimeIdex = Progtam_Time[nProcessIndex] + nProcessIndex * 1000;
		g_SProgramArr[nProcessIndex].nTimeMax = TIME_MAX ;
		g_SProgramArr[nProcessIndex].csPath = stTemp;
		g_SProgramArr[nProcessIndex].csName = Progtam_Name[nProcessIndex];
		if (GetProgramHandle(nProcessIndex))
		{
			//SetTimer(g_SProgramArr[nProcessIndex].nTimeIdex, CHECK_TIME, NULL);
		}
	}
}

BOOL CGuardServerDlg::GetProgramHandle(int nIndex)
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
		SetListData(g_SProgramArr[nIndex].csName, STATE_OK, L"��ȡ���̿�ʼ�ػ�", g_SProgramArr[nIndex].cLastTime);
		return TRUE;
	}
	else
	{
		if (OpenProgram(nIndex))
		{
			g_SProgramArr[nIndex].cLastTime = COleDateTime::GetCurrentTime();
			SetListData(g_SProgramArr[nIndex].csName, STATE_START, L"�������̿�ʼ�ػ�", g_SProgramArr[nIndex].cLastTime);
			return TRUE;
		}
	}
	SetListData(g_SProgramArr[nIndex].csName, STATE_ON, L"��������ʧ��", g_SProgramArr[nIndex].cLastTime);

	return FALSE;
}

BOOL CGuardServerDlg::OpenProgram(int nIndex)
{
	LPTSTR szCmdline = _tcsdup((g_SProgramArr[nIndex].csPath));
	ZeroMemory(&(g_SProgramArr[nIndex].si), sizeof((g_SProgramArr[nIndex].si)));
	(g_SProgramArr[nIndex].si).cb = sizeof((g_SProgramArr[nIndex].si));
	ZeroMemory(&(g_SProgramArr[nIndex].Process), sizeof((g_SProgramArr[nIndex].Process)));

	if (!CreateProcess(NULL,
		szCmdline,
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
BOOL CGuardServerDlg::CloseProgram(int nIndex)
{
	//SetListData(g_SProgramArr[nIndex].csName, STATE_ON, L"�رս���", g_SProgramArr[nIndex].cLastTime);
	return TerminateProcess(g_SProgramArr[nIndex].hProgramHandle, 0);
}

BOOL CGuardServerDlg::IsFindProgram(int nIndex)
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

BOOL CGuardServerDlg::CompareTime(COleDateTime StartTime, int nInexe)
{

	
		COleDateTimeSpan delta;
		COleDateTime CurrentTime;
		CurrentTime = COleDateTime::GetCurrentTime();

		delta = CurrentTime - StartTime;
		CString date = StartTime.Format(L"%Y-%m-%d %H:%M:%S");
		CString date1 = CurrentTime.Format(L"%Y-%m-%d %H:%M:%S");
		CString cstemp;
	
		double dTimeInterval = delta.GetTotalSeconds();
		cstemp.Format(L"%s_%s_%s_%d", date, date1, g_SProgramArr[nInexe].csName, dTimeInterval);
		Log_Error(cstemp);;
		if (dTimeInterval <= TIME_MAX)
		{
			return TRUE;
		}
		else
			return FALSE;

	
}

void CGuardServerDlg::Log_Error(CString csLog)
{
	//wchar_t תΪ char
	int iSize;
	char* pszMultiByte;
	iSize = WideCharToMultiByte(CP_ACP, 0, csLog, -1, NULL, 0, NULL, NULL);
	pszMultiByte = (char*)malloc(iSize*sizeof(char));
	WideCharToMultiByte(CP_ACP, 0, csLog, -1, pszMultiByte, iSize, NULL, NULL);

	DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, pszMultiByte);
	free(pszMultiByte);
}

void CGuardServerDlg::OnBnClickedOk()
{
	CString strIp;
	TCHAR pFileName[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, pFileName);
	CString csFullPath(pFileName);
	//ShellExecute(NULL, L"open", csFullPath, NULL, NULL, SW_SHOWNORMAL);
	//SetTimer(g_SProgramArr[0].nTimeIdex, CHECK_TIME, NULL);
	COleDateTime ssss;

	ssss = COleDateTime::GetCurrentTime();
	Sleep(5000);
	//CompareTime(ssss);
	int t = 0;
	//CDialogEx::OnOK();
}


int CGuardServerDlg::GetDiskCapacity(CStringArray &arNmae, CStringArray &arTotal, CStringArray &arCurrent)
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
			double dTotal = (float)i64TotalBytes / 1024 / 1024 / 1024;//��
			double dCurrent = (float)i64FreeBytesToCaller / 1024 / 1024 / 1024;//��ǰ
			CString csTemp(dir);
			arNmae.Add(csTemp);
			csTemp.Format(L"%f", dTotal);
			arTotal.Add(csTemp);
			csTemp.Format(L"%f", dCurrent);
			arCurrent.Add(csTemp);
		}
		else
		{
			//	cout << " �豸δ׼����";
		}
		si += 4;
	}
	return nSum;
}


BOOL CGuardServerDlg::GetMySqlState()
{

	CString csSqlConnect;
	csSqlConnect = "Driver={MySQL ODBC 5.1 Driver};Server = 127.0.0.1; Database = facedetectsys; uid = root;pwd = cr;";
	try
	{
		_ConnectionPtr  pConnect;
		pConnect.CreateInstance(__uuidof(Connection));
		_bstr_t strConnect = csSqlConnect;
		pConnect->Open(strConnect, "", "", adModeUnknown);
	}
	catch (_com_error e)
	{
		return FALSE;
	}
	return  TRUE;

}

//��ʱ���ø��߳��ж�
void CGuardServerDlg::OnTimer(UINT_PTR nIDEvent)
{
	CString  csTemp;
	if (nIDEvent < PROGRAM_MAX)
	{
		if (!CompareTime(g_SProgramArr[nIDEvent].cLastTime, nIDEvent))
		{
			csTemp.Format(_T("%d"), nIDEvent);

			Log_Error(csTemp);
			SetListData(g_SProgramArr[nIDEvent].csName, STATE_ON, L"������ʱ", g_SProgramArr[nIDEvent].cLastTime);

			KillTimer(g_SProgramArr[nIDEvent].nTimeIdex);
			if (!IsFindProgram(nIDEvent))
			{
				if (OpenProgram(nIDEvent))
				{
					SetTimer(g_SProgramArr[nIDEvent].nTimeIdex, CHECK_TIME, NULL);
					SetListData(g_SProgramArr[nIDEvent].csName, STATE_ON, L"��������", g_SProgramArr[nIDEvent].cLastTime);

				}
			}
			else
			{
				if (CloseProgram(nIDEvent))
				{
					if (OpenProgram(nIDEvent))
					{
						SetTimer(g_SProgramArr[nIDEvent].nTimeIdex, CHECK_TIME, NULL);
						SetListData(g_SProgramArr[nIDEvent].csName, STATE_ON, L"��������", g_SProgramArr[nIDEvent].cLastTime);
					}
				}
			}
		}
	}
	

	CDialogEx::OnTimer(nIDEvent);
}
