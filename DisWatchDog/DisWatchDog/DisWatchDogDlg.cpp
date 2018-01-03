
// DisWatchDogDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DisWatchDog.h"
#include "DisWatchDogDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
static SProgramData * g_SProgramArr;

UINT OpenUdpServer(LPVOID lpam);
UINT GuardServerThand(LPVOID lpam);

#define	PROGRAM_MAX   1		//�������
#define  PROGRAM_DIS_NAME  "Dispatcher"
#define  PROGRAM_DIS_PATH "\\Dispatcher.exe"

#define UDP_RECV_PORT 30000  //udp�˿�

#define CHECK_TIME 70000    //���ʱ��


#define TIME_MAX   50		//���ʱ��ֵ
char Progtam_Name[PROGRAM_MAX][126] =
{
	PROGRAM_DIS_NAME
};

char Progtam_Path[PROGRAM_MAX][126] =
{
	PROGRAM_DIS_PATH
};
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


// CDisWatchDogDlg �Ի���

CDisWatchDogDlg dlg;
LONG CallBackCrashHandler(EXCEPTION_POINTERS *pException)
{
	dlg.Log_Error(L"Dog����");
	::PostMessage(AfxGetMainWnd()->m_hWnd, WM_SYSCOMMAND, SC_CLOSE, NULL);
	//��ȡexe����ǰ·�� 
	extern CDisWatchDogApp theApp;
	TCHAR szAppName[MAX_PATH];
	::GetModuleFileName(theApp.m_hInstance, szAppName, MAX_PATH);
	CString strAppFullName;
	strAppFullName.Format(_T("%s"), szAppName);
	//�������� 
	STARTUPINFO StartInfo;
	PROCESS_INFORMATION procStruct;
	memset(&StartInfo, 0, sizeof(STARTUPINFO));
	StartInfo.cb = sizeof(STARTUPINFO);
	::CreateProcess(
		(LPCTSTR)strAppFullName,
		NULL,
		NULL,
		NULL,
		FALSE,
		NORMAL_PRIORITY_CLASS,
		NULL,
		NULL,
		&StartInfo,
		&procStruct);

	return EXCEPTION_EXECUTE_HANDLER;
}
CDisWatchDogDlg::CDisWatchDogDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDisWatchDogDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDisWatchDogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDisWatchDogDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CDisWatchDogDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CDisWatchDogDlg ��Ϣ�������

BOOL CDisWatchDogDlg::OnInitDialog()
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	AfxOleInit();
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
	{
		AfxMessageBox(L"Sock init fail!");
		return false;
	}

	InitProgram();

	AfxBeginThread(&OpenUdpServer, 0);
	AfxBeginThread(&GuardServerThand, 0);
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)CallBackCrashHandler);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CDisWatchDogDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDisWatchDogDlg::OnPaint()
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
HCURSOR CDisWatchDogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDisWatchDogDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}
void CDisWatchDogDlg::InitProgram()
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
		g_SProgramArr[nProcessIndex].nTimeMax = TIME_MAX;
		g_SProgramArr[nProcessIndex].csPath = stTemp;
		g_SProgramArr[nProcessIndex].csName = Progtam_Name[nProcessIndex];
		if (GetProgramHandle(nProcessIndex))
			g_SProgramArr[nProcessIndex].bFlag = TRUE;
		else
			g_SProgramArr[nProcessIndex].bFlag = FALSE;
	}
}

//��ȡ�����handle
BOOL CDisWatchDogDlg::GetProgramHandle(int nIndex)
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
		//SetListData(g_SProgramArr[nIndex].csName, L"��ȡ�ɹ�", g_SProgramArr[nIndex].cLastTime);
		return TRUE;
	}
	else
	{
		if (OpenProgram(nIndex))
		{
			g_SProgramArr[nIndex].cLastTime = COleDateTime::GetCurrentTime();
		//	SetListData(g_SProgramArr[nIndex].csName, L"�����ɹ�", g_SProgramArr[nIndex].cLastTime);
			return TRUE;
		}
	}

	return FALSE;
}
//�򿪳���
BOOL CDisWatchDogDlg::OpenProgram(int nIndex)
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
//�رճ���
BOOL CDisWatchDogDlg::CloseProgram(int nIndex)
{
	return TerminateProcess(g_SProgramArr[nIndex].hProgramHandle, 0);
}
//�жϳ����Ƿ����
BOOL CDisWatchDogDlg::IsFindProgram(int nIndex)
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
//ʱ��Ա�
BOOL CDisWatchDogDlg::CompareTime(COleDateTime StartTime, int nInexe)
{
	COleDateTimeSpan delta;
	COleDateTime CurrentTime;
	CurrentTime = COleDateTime::GetCurrentTime();

	delta = CurrentTime - StartTime;
	CString date = StartTime.Format(L"%Y-%m-%d %H:%M:%S");
	CString date1 = CurrentTime.Format(L"%Y-%m-%d %H:%M:%S");
	CString cstemp;

	double dTimeInterval = delta.GetTotalSeconds();
	if (dTimeInterval <= TIME_MAX)
	{
		return TRUE;
	}
	else
		return FALSE;
}

void CDisWatchDogDlg::Log_Error(CString csLog)
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

UINT OpenUdpServer(LPVOID lpam)
{
	CDisWatchDogDlg *dlg = (CDisWatchDogDlg *)AfxGetApp()->GetMainWnd();

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
						//USES_CONVERSION;
						//char *pChStr = T2A(csProgramIndex);
						g_SProgramArr[_wtoi(csProgramIndex)].cLastTime = COleDateTime::GetCurrentTime();
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

//�ж����� ���������
UINT GuardServerThand(LPVOID lpam)
{
	CDisWatchDogDlg *dlg = (CDisWatchDogDlg *)AfxGetApp()->GetMainWnd();
	COleDateTime cLastTime;
	AfxOleInit();

	while (true)
	{
		try
		{
			CString csTemp;

			for (int nIndex = 0; nIndex < PROGRAM_MAX; nIndex++)
			{
				if (!g_SProgramArr[nIndex].bFlag)
				{
					continue;
				}
				BOOL bCompaare = dlg->CompareTime(g_SProgramArr[nIndex].cLastTime, nIndex);

				if (bCompaare == FALSE)
				{
					//dlg->SetListData(g_SProgramArr[nIndex].csName, L"������ʱ", g_SProgramArr[nIndex].cLastTime);
					csTemp.Format(_T("%s_������ʱ"), g_SProgramArr[nIndex].csName);
					dlg->Log_Error(csTemp);

					if (!dlg->IsFindProgram(nIndex))
					{
						if (dlg->OpenProgram(nIndex))
						{
						//	dlg->SetListData(g_SProgramArr[nIndex].csName, L"�����ɹ�", g_SProgramArr[nIndex].cLastTime);
							csTemp.Format(_T("%s_�����ɹ�"), g_SProgramArr[nIndex].csName);
							dlg->Log_Error(csTemp);
						}
						else
						{
							//	dlg->SetListData(g_SProgram,Arr[nIndex].csName, L"����ʧ��", g_SProgramArr[nIndex].cLastTime);
							csTemp.Format(_T("%s_����ʧ��"), g_SProgramArr[nIndex].csName);
							dlg->Log_Error(csTemp);
						}

					}
					else
					{
						if (dlg->CloseProgram(nIndex))
						{
							if (dlg->OpenProgram(nIndex))
							{
							//	dlg->SetListData(g_SProgramArr[nIndex].csName, L"�����ɹ�", g_SProgramArr[nIndex].cLastTime);
								csTemp.Format(_T("%s_�����ɹ�"), g_SProgramArr[nIndex].csName);
								dlg->Log_Error(csTemp);
							}
							else
							{
								//	dlg->SetListData(g_SProgramArr[nIndex].csName, L"����ʧ��", g_SProgramArr[nIndex].cLastTime);
								csTemp.Format(_T("%s_����ʧ��"), g_SProgramArr[nIndex].csName);
								dlg->Log_Error(csTemp);
							}
						}
						else
						{
							//	dlg->SetListData(g_SProgramArr[nIndex].csName, L"����ʧ��", g_SProgramArr[nIndex].cLastTime);
							csTemp.Format(_T("%s_����ʧ��"), g_SProgramArr[nIndex].csName);
							dlg->Log_Error(csTemp);
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