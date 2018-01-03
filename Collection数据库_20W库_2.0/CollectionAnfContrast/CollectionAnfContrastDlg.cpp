
// CollectionAnfContrastDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CollectionAnfContrast.h"
#include "CollectionAnfContrastDlg.h"
#include "afxdialogex.h"
#include "SetValuToDb.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define  DBINI_PATH						  "\\Parameter.ini"
#define  IMAGE_ROOTPATH			  "\\..\\..\\Data\\"
#define  IMAGE_PATH					  "\\imgCurFaceBase64\\"
#define  WARNING_PATH				  "\\ImageWarningb64\\"

#define  RETURN_MAXSUM			  "50";

#define UDP_SEND_PORT 30000  //udp端口
#define  PROGRAM_INDEX   0		//程序ID

#define  SLEEP_TIME   30000		//程序ID
/////////////全局变量/////////////////////////////////////////////////////
static ThandScheduling * g_ThandSchedulingArr;
CWinThread * g_UpdataOnlineThread;
CWinThread * g_SchedulingThread;
static int g_ThandWorkState = 0; //工作中的一体机
static int g_ThandWorkOnline = 0;//在线的一体机
static int g_nContrastComputer = 0;//所有一体机个数
static BOOL g_bIsMOdify = FALSE;

_ConnectionPtr  g_pConnect;
_RecordsetPtr g_pRecordset;
UINT ThreadContrast(LPVOID lparam);
UINT ThreadScheduling(LPVOID lparam);
UINT ThreadUpdataComputerOnline(LPVOID lparam);
UINT ThreadRestart(LPVOID lparam);
UINT HeartbeatThread(LPVOID lpam);
/////////////////////////////////////////////////////////////////////////////

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


// CCollectionAnfContrastDlg 对话框



CCollectionAnfContrastDlg::CCollectionAnfContrastDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CCollectionAnfContrastDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nSleepTime = 1;
	m_nBideTime = 1;
	m_nIsOnlineTime = 1;
}

void CCollectionAnfContrastDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCollectionAnfContrastDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUT_DETECTIONDB, &CCollectionAnfContrastDlg::OnBnClickedButDetectiondb)
	ON_BN_CLICKED(IDC_BUT_STARTTHAND, &CCollectionAnfContrastDlg::OnBnClickedButStartthand)
	ON_BN_CLICKED(IDC_BUT_SETEVENT, &CCollectionAnfContrastDlg::OnBnClickedButSetevent)
	ON_MESSAGE(WM_SHOW_TASK, OnShowNotify)

	ON_COMMAND(ID_DELETETARY, &CCollectionAnfContrastDlg::OnDeletetary)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUT_SETVALUETODB, &CCollectionAnfContrastDlg::OnBnClickedButSetvaluetodb)
	ON_BN_CLICKED(IDCANCEL2, &CCollectionAnfContrastDlg::OnBnClickedCancel2)
END_MESSAGE_MAP()


// CCollectionAnfContrastDlg 消息处理程序
LRESULT CCollectionAnfContrastDlg::OnShowNotify(WPARAM wParam, LPARAM lParam)
{
	return m_tray.OnShowTask(wParam, lParam);
}
BOOL CCollectionAnfContrastDlg::OnInitDialog()
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

	//GetInITime();

	//CoInitialize(NULL);
	InitializeCriticalSection(&cSECTION);

	if (InitDbParameter())
	{

		g_ThandWorkOnline = UpdataComputerOline();
		GetDlgItem(IDC_BUT_DETECTIONDB)->EnableWindow(FALSE);

		if (GetParameterToDb())
		{
			g_SchedulingThread = AfxBeginThread(&ThreadScheduling, NULL);
			GetDlgItem(IDC_BUT_STARTTHAND)->EnableWindow(FALSE);
		}
		else
		{
			MessageBox("数据库配置文件读取出错", "提示");
		}

	}
	else
	{

		MessageBox("数据库连接失败", "提示");
	}

	SetTimer(1, 1, NULL);
	AfxBeginThread(&HeartbeatThread, 0);


	//OnBnClickedButSetevent();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCollectionAnfContrastDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCollectionAnfContrastDlg::OnPaint()
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
HCURSOR CCollectionAnfContrastDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
bool CCollectionAnfContrastDlg::GetParameterToDb()
{
	CString csSql;
	int nThreadIdex = 0;
	_RecordsetPtr pRecordset;

	//获取识别一体机个数
	csSql = "select * from ParameterIni where Parameter = 'computer' ";
	pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
	if (!pRecordset->adoEOF)
	{
		m_csSleepTime = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("SleepTime")->Value;
		m_csBideTime = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("BideTime")->Value;
		m_csIsOnlineTime = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("IsOnlineTime")->Value;
		m_cswarningsum = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("warningsum")->Value;
		m_csCameraSimilarity = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("CameraSimilarity")->Value;

		m_nSleepTime = atoi(m_csSleepTime);
		m_nBideTime = atoi(m_csBideTime);
		m_nIsOnlineTime = atoi(m_csIsOnlineTime);
		m_nwarningsum = atoi(m_cswarningsum);

		return true;
	}
	else
		return false;

}
//获取INI 文件路径
CString CCollectionAnfContrastDlg::GetiniPath()
{
	CString strIp;
	TCHAR pFileName[MAX_PATH];
	int nPos = GetCurrentDirectory(MAX_PATH, pFileName);

	CString csFullPath(pFileName);
	csFullPath += DBINI_PATH;

	return csFullPath;
}
//获取image 文件路径
CString CCollectionAnfContrastDlg::GetImagePath(CString csImageName, CString csFile)
{
	CString strIp;
	TCHAR pFileName[MAX_PATH];
	int nPos = GetCurrentDirectory(MAX_PATH, pFileName);

	CString csFullPath(pFileName);
	csFullPath.Format("%s%s%s%scurface_%s.b64", csFullPath, IMAGE_ROOTPATH, csFile, IMAGE_PATH, csImageName);
	return csFullPath;
}
//获取warning 路径
CString CCollectionAnfContrastDlg::GetWarningPath(CString csImageName, CString csFile)
{
	CString strIp;
	TCHAR pFileName[MAX_PATH];
	int nPos = GetCurrentDirectory(MAX_PATH, pFileName);

	CString csFullPath(pFileName);
	csFullPath += IMAGE_ROOTPATH;
	csFullPath += csFile;
	csFullPath += WARNING_PATH;
	csFullPath += csImageName;
	csFullPath += ".txt";
	return csFullPath;
}
BOOL CCollectionAnfContrastDlg::WriteWarningb64(CString csWarning, CString csWarningPath)
{
	BOOL bReturn = FALSE;
	CStdioFile File;
	bReturn = File.Open(csWarningPath, CFile::modeCreate | CFile::typeBinary | CFile::modeNoTruncate | CFile::modeReadWrite | CFile::shareDenyNone);
	if (!bReturn)
		return  bReturn;

	File.SeekToEnd();
	File.WriteString(csWarning);
	File.Flush();
	File.Close();
	return  bReturn;
}

void CCollectionAnfContrastDlg::GetInITime()
{
	CString csFullPath = GetiniPath();
	CString csSleep, csBide, csOnline;
	GetPrivateProfileString(("Computer"), "Sleep_Time", (""), csSleep.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);
	GetPrivateProfileString(("Computer"), "Bide_Time", (""), csBide.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);
	GetPrivateProfileString(("Computer"), "IsOnline_Time", (""), csOnline.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);
	m_nSleepTime = atoi(csSleep);
	m_nBideTime = atoi(csBide);
	m_nIsOnlineTime = atoi(csOnline);
}
//初始化 数据库
BOOL CCollectionAnfContrastDlg::InitDbParameter()
{
	//AfxOleInit();
	CString csFullPath = GetiniPath();
	CString csSqlIP, csSqlDatabase;
	GetPrivateProfileString(("SqlConfiguration"), "Sql_ServerAdd", (""), csSqlIP.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);
	GetPrivateProfileString(("SqlConfiguration"), "Sql_Database", (""), csSqlDatabase.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);
	CString csSqlConnect;
	//csSqlConnect = "Driver={MySQL ODBC 5.1 Driver};Server = 127.0.0.1; Database = facedetectsys; uid = root;pwd = cr;";
	//csSqlConnect.Format( "Driver={MySQL ODBC 5.1 Driver};Server = %s; Database = %s; uid = root;pwd = cr;", csSqlIP, csSqlDatabase);
	csSqlConnect.Format("Driver={MySQL ODBC 5.1 Driver};Server=%s;Database=%s;uid=root;pwd=cr;", csSqlIP, csSqlDatabase);
	try
	{
		g_pConnect.CreateInstance(__uuidof(Connection));
		g_pRecordset.CreateInstance(__uuidof(Recordset));
		_bstr_t strConnect = csSqlConnect;
		g_pConnect->Open(strConnect, "", "", adModeUnknown);
	}
	catch (_com_error e)
	{
		return FALSE;
	}
	return  TRUE;
}
//初始化识别线程返回个数
int CCollectionAnfContrastDlg::InitComputerThread()
{
	try
	{
		CString csSql;
		int nThreadIdex = 0;
		_RecordsetPtr pRecordset;

		//获取识别一体机个数
		csSql = "select count(1) from device where type = 1 ";
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		if (!pRecordset->adoEOF)
		{
			CString csEmpInfoName = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("count(1)")->Value;
			g_nContrastComputer = atoi(csEmpInfoName);
		}

		//有识别一体机时创建相应对比线程
		if (g_nContrastComputer > 0)
		{
			g_ThandSchedulingArr = new ThandScheduling[g_nContrastComputer];
			csSql = "select * from device where type = 1 ";
			pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
			while (!pRecordset->adoEOF)
			{
				CString csComputerIp = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("ip")->Value;
				CString csFaceDbIdex = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("facedb")->Value;
				CString csIsonline = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("isonline")->Value;
				if (csIsonline == "1")
					g_ThandSchedulingArr[nThreadIdex].bIsOnline = TRUE;
				else
					g_ThandSchedulingArr[nThreadIdex].bIsOnline = FALSE;

				g_ThandSchedulingArr[nThreadIdex].bWorkState = FALSE;
				g_ThandSchedulingArr[nThreadIdex].HEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
				g_ThandSchedulingArr[nThreadIdex].csFaceDbIdex = csFaceDbIdex;
				g_ThandSchedulingArr[nThreadIdex].csComputerIp = csComputerIp;
				g_ThandSchedulingArr[nThreadIdex].csSimilarity = m_csCameraSimilarity;
				g_ThandSchedulingArr[nThreadIdex].nWarningSum = m_nwarningsum;
				g_ThandSchedulingArr[nThreadIdex].csMaxSum = RETURN_MAXSUM;
				g_ThandSchedulingArr[nThreadIdex].HThand = AfxBeginThread(&ThreadContrast, (LPVOID)nThreadIdex);
				CString csTemp;
				csTemp.Format("InitComputerThread_%s", csComputerIp);
				DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csTemp);
				nThreadIdex++;

				pRecordset->MoveNext();
			}
		}
		//如果一体机线程与数量不对则删除
		if (nThreadIdex != g_nContrastComputer)
		{
			delete[] g_ThandSchedulingArr;
			return 0;
		}

		return g_nContrastComputer;
	}
	catch (CException* e)
	{
		return 0;
	}
}
//返回在线识别一体机个数
int CCollectionAnfContrastDlg::UpdataComputerOline()
{
	try
	{

		CString csSql;
		int nThreadIdexIsOnline = 0;
		_RecordsetPtr  pRecordset;

		//判断是否有修改数据库内相似度与错误次数值 2017年5月5日
		csSql = "select * from ParameterIni where IsModify = 1 and Parameter = 'computer'";
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);

		if (!pRecordset->adoEOF)
		{
			m_csSleepTime = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("SleepTime")->Value;
			m_csBideTime = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("BideTime")->Value;
			m_csIsOnlineTime = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("IsOnlineTime")->Value;
			m_cswarningsum = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("warningsum")->Value;
			m_csCameraSimilarity = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("CameraSimilarity")->Value;

			m_nSleepTime = atoi(m_csSleepTime);
			m_nBideTime = atoi(m_csBideTime);
			m_nIsOnlineTime = atoi(m_csIsOnlineTime);
			m_nwarningsum = atoi(m_cswarningsum);

			csSql = "UPDATE ParameterIni SET IsModify = '0' where Parameter = 'computer' ";
			g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		}
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "c3");

		//end
		EnterCriticalSection(&cSECTION);
		//有识别一体机时创建相应对比线程
		csSql = "select * from device where type = 1 ";
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);

		BOOL bOnline = FALSE;
		while (!pRecordset->adoEOF)
		{
			CString csIsonline = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("isonline")->Value;
			CString csComputerIp = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("ip")->Value;
			if (csIsonline == "1")
			{
				nThreadIdexIsOnline++;
				bOnline = TRUE;
			}
			else
			{
				bOnline = FALSE;
			}

			for (int i = 0; i < g_nContrastComputer; i++)
			{
				if (g_ThandSchedulingArr[i].csComputerIp == csComputerIp)
				{
					g_ThandSchedulingArr[i].bIsOnline = bOnline;
					//判断是否有修改数据库内相似度与错误次数值 2017年5月5日
					g_ThandSchedulingArr[i].csSimilarity = m_csCameraSimilarity;
					g_ThandSchedulingArr[i].nWarningSum = m_nwarningsum;
					//end
				}
			}
			pRecordset->MoveNext();
		}
		LeaveCriticalSection(&cSECTION);
		return nThreadIdexIsOnline;
	}
	catch (CException* e)
	{
		return -1;
	}
}
//总调度线程
UINT ThreadScheduling(LPVOID lparam)
{
	CCollectionAnfContrastDlg *pWnd = (CCollectionAnfContrastDlg *)AfxGetApp()->GetMainWnd();

	CString csSql, csName, csFile;
	int nIdex = 0;
	int nWorkTheradSum;//总识别一体机个数
	BOOL bWhile = TRUE;
	BOOL bInitInfo = TRUE;
	BOOL bSetEvent = FALSE;
	_RecordsetPtr pRecordset;


	while (bWhile)
	{
		try
		{
			//第一次初始化数组 启动 线程获取在线一体机个数
			if (bInitInfo)
			{

				//初始化识别一体机线程当返回 0 时失败
				nWorkTheradSum = pWnd->InitComputerThread();
				CString  csTemp;
				csTemp.Format("WorkTheradSum_%d", nWorkTheradSum);
				DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csTemp);
				if (nWorkTheradSum == 0)
				{
					continue;
				}
				//启动线程查询在线一体机个数
				g_UpdataOnlineThread = AfxBeginThread(&ThreadUpdataComputerOnline, NULL);
				bInitInfo = FALSE;
			}

			//读数据库内需要对比的图像ID 
			csSql = "select * from snapshotinfo where process_status = 1";
			pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
			while (!pRecordset->adoEOF)
			{

				//将查找标志位值位假
				bSetEvent = FALSE;

				//在线一体机为0 休眠
				if (g_ThandWorkOnline == 0)
				{
					DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "g_ThandWorkOnline_0");
					Sleep(pWnd->m_nBideTime * 1000);
					continue;
				}

				//如果在线一体机数量大于总一体机数量
				if (g_ThandWorkOnline > nWorkTheradSum)
					g_ThandWorkOnline = nWorkTheradSum;
				EnterCriticalSection(&cSECTION);
				//循环判断在线一体机是否在工作
				for (nIdex = 0; nIdex < nWorkTheradSum; nIdex++)
				{
					//需要等表修改完做修改
					csName = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("id")->Value;
					csFile = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("imgpath")->Value;

					g_ThandSchedulingArr[nIdex].csImgName.Add(csName);
					g_ThandSchedulingArr[nIdex].csImgFile.Add(csFile);
					//修改snapshotinfo 标记已读取
					csSql.Format("UPDATE snapshotinfo SET process_status = '4'  where id = '%s' ", csName);
					g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);

					//插入对比流水表数据
					csSql.Format("insert into Comparerecord (process_status, snapshotid, RecongnitionIp)values('1','%s','%s')",
						csName, g_ThandSchedulingArr[nIdex].csComputerIp);
					g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
				}
				LeaveCriticalSection(&cSECTION);
				pRecordset->MoveNext();
			}//end pRecordset->adoEOF

			Sleep(pWnd->m_nSleepTime * 1000);
		}
		catch (...)
		{
			DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "ThreadScheduling_catchEnd");
			return -1;
		}
	}//end while

	delete[] g_ThandSchedulingArr;

	DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "ThreadScheduling_End");
	return 0;
}
//线程处理函数
UINT ThreadContrast(LPVOID lparam)
{
	int nIdex = (int)lparam;
	CCollectionAnfContrastDlg *pWnd = (CCollectionAnfContrastDlg *)AfxGetApp()->GetMainWnd();
	DWORD dReturn;;
	BOOL bWhile = TRUE;
	CString  csSql;
	int nContrast = 0;
	int nConut = 0;
	char pSession[20];
	StructRecognitionList pStructRecognitionlist[50];
	int nWarning = 0;

	CString csTemp, csNameID, csImgFile, csImgPath;

	while (bWhile)
	{
		try
		{
			Sleep(pWnd->m_nSleepTime * 1000);
			EnterCriticalSection(&cSECTION);
			if (g_ThandSchedulingArr[nIdex].csImgName.GetSize() <= 0)
			{
				Sleep(500);
				csTemp.Format("ThreadContrast_%s_Sleep", g_ThandSchedulingArr[nIdex].csComputerIp);
				//DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csTemp);
				LeaveCriticalSection(&cSECTION);
				continue;
			}
			LeaveCriticalSection(&cSECTION);

			memset(pSession, '\0', 20);

			//需要加锁		//将结构体内的值赋值到局部变量防止外部修改
			EnterCriticalSection(&cSECTION);
			csNameID = g_ThandSchedulingArr[nIdex].csImgName.GetAt(0);
			csImgFile = g_ThandSchedulingArr[nIdex].csImgFile.GetAt(0);
			g_ThandSchedulingArr[nIdex].csImgName.RemoveAt(0);
			g_ThandSchedulingArr[nIdex].csImgFile.RemoveAt(0);

			csImgPath.Empty();
			csImgPath = pWnd->GetImagePath(csNameID, csImgFile);	//获取照片路径
			LeaveCriticalSection(&cSECTION);

			csTemp.Format("ThreadContrast_%s_%s_%d_Start", g_ThandSchedulingArr[nIdex].csComputerIp, csNameID, nIdex);
			DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csTemp);


			//读取照片b64
			CFile file;
			if (!file.Open(csImgPath, CFile::modeRead))//11
			{
				csSql.Format("UPDATE comparerecord SET process_status = '11' where snapshotid = '%s'  and RecongnitionIp = '%s'  "
					, csNameID, g_ThandSchedulingArr[nIdex].csComputerIp);
				g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
				csTemp.Format("ThreadContrast_%s_%s_%d_OpenError", g_ThandSchedulingArr[nIdex].csComputerIp, csNameID, nIdex);
				DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csTemp);
				continue;
			}

			UINT  iFileLen = file.GetLength();
			char *imagebuf = new char[iFileLen];
			int iEflen = iFileLen * 2;
			file.Read(imagebuf, iFileLen);
			file.Close();
			//调用dll对比函数
			nContrast = pWnd->m_CFASLoginOrLogout.DeviceLogin((LPSTR)(LPCTSTR)g_ThandSchedulingArr[nIdex].csComputerIp, pSession);
			if (nContrast == 10 || nContrast == 11)
			{//断线或者不在的情况下
				EnterCriticalSection(&cSECTION);
				g_ThandSchedulingArr[nIdex].bIsOnline = FALSE;
				g_ThandWorkOnline--;
				LeaveCriticalSection(&cSECTION);
				continue;
			}

			//对比图片
			nContrast = pWnd->m_CFASRecognition.FASImgRecongnition(
				(LPSTR)(LPCTSTR)g_ThandSchedulingArr[nIdex].csComputerIp,	//一体机IP
				pSession,
				(LPSTR)(LPCTSTR)g_ThandSchedulingArr[nIdex].csFaceDbIdex,  //对比人脸库ID
				imagebuf,																			 //照片信息
				(LPSTR)(LPCTSTR)g_ThandSchedulingArr[nIdex].csMaxSum,		//最大返回个数
				(LPSTR)(LPCTSTR)g_ThandSchedulingArr[nIdex].csSimilarity,		//对比相似度
				&nConut,																					//返回个数
				pStructRecognitionlist);																//返回结构体

			if (nContrast == 0)//对比成功3
			{
				nWarning = 0;

				//修改抓拍表 标志位
				csSql.Format("UPDATE snapshotinfo SET process_status = '3'  ,warningcount = %d ,RecognitionIp = '%s' where id = '%s' ",
					nConut, g_ThandSchedulingArr[nIdex].csComputerIp, csNameID);
				g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);


				csSql.Format("UPDATE Comparerecord SET process_status = '3' where snapshotid = '%s'  and RecongnitionIp = '%s'  "
					, csNameID, g_ThandSchedulingArr[nIdex].csComputerIp);
				g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);

				//向warning 表写对比到的八项数据
				for (int i = 0; i < nConut; i++)
				{
					csSql.Format("insert into warninginfo (id_card, similarity, snapshotid,RecognitionIp)values('%s','%s','%s','%s')",
						pStructRecognitionlist[i].aId_card,
						pStructRecognitionlist[i].aSimilarity,
						csNameID,
						g_ThandSchedulingArr[nIdex].csComputerIp);
					g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
				}
			}
			else //对比失败10
			{
				nWarning++;
				if (nWarning >= g_ThandSchedulingArr[nIdex].nWarningSum)
				{
					EnterCriticalSection(&cSECTION);
					g_ThandSchedulingArr[nIdex].bIsOnline = FALSE;
					LeaveCriticalSection(&cSECTION);
				}
				csSql.Format("UPDATE comparerecord SET process_status = '10' where snapshotid = '%s'  and RecongnitionIp = '%s'  "
					, csNameID, g_ThandSchedulingArr[nIdex].csComputerIp);

				g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
			}
			nContrast = pWnd->m_CFASLoginOrLogout.DeviceLogout((LPSTR)(LPCTSTR)g_ThandSchedulingArr[nIdex].csComputerIp, pSession);


			csTemp.Format("ThreadContrast_%s_End", g_ThandSchedulingArr[nIdex].csComputerIp);
			DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csTemp);

			//需要加锁
			EnterCriticalSection(&cSECTION);
			g_ThandSchedulingArr[nIdex].bWorkState = FALSE;
			LeaveCriticalSection(&cSECTION);
			delete[] imagebuf;
		}
		catch (...)
		{
			CString csTemp;
			csTemp.Format("ThreadContrast_%s_%d_catch", g_ThandSchedulingArr[nIdex].csComputerIp, csNameID);
			DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csTemp);
		}

	}

	DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "ThreadContrast_End");
	return 0;
}

//在数据库中获取在线的一体机
UINT ThreadUpdataComputerOnline(LPVOID lparam)
{
	int nIdex = (int)lparam;
	CCollectionAnfContrastDlg *pWnd = (CCollectionAnfContrastDlg *)AfxGetApp()->GetMainWnd();
	BOOL bWhile = TRUE;
	while (bWhile)
	{
		//重启程序 2017年5月5日10:43:19
		// 		CString csSql;
		// 		_RecordsetPtr  pRecordset;
		// 		csSql = "select * from ParameterIni where IsModify = 1 ";
		// 		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		// 		if (!pRecordset->adoEOF)
		// 		{
		// 			csSql = "UPDATE ParameterIni SET IsModify = '0' where Parameter = 'computer' ";
		// 			g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		// 			g_bIsMOdify = TRUE;
		// 			bWhile = FALSE;
		// 			continue;
		// 		}

		g_ThandWorkOnline = pWnd->UpdataComputerOline();
		CString csTemp;
		csTemp.Format("WorkOnline_%d", g_ThandWorkOnline);
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csTemp);
		Sleep(pWnd->m_nIsOnlineTime * 1000);
	}
	return 0;
}
//重启线程 2017年5月5日10:43:28 暂时不用
UINT ThreadRestart(LPVOID lparam)
{
	if (g_ThandWorkState == 0)
	{

		::PostMessage(AfxGetMainWnd()->m_hWnd, WM_SYSCOMMAND, SC_CLOSE, NULL);
		//获取exe程序当前路径 
		extern CCollectionAnfContrastApp theApp;
		TCHAR szAppName[MAX_PATH];
		::GetModuleFileName(theApp.m_hInstance, szAppName, MAX_PATH);
		CString strAppFullName;
		strAppFullName.Format(_T("%s"), szAppName);
		//重启程序 
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
	}
	return 0;
}
//检测数据库连接
void CCollectionAnfContrastDlg::OnBnClickedButDetectiondb()
{
	if (InitDbParameter())
	{
		GetDlgItem(IDC_BUT_DETECTIONDB)->EnableWindow(FALSE);
	}
	else
	{
		MessageBox("数据库连接失败", "提示");
	}
}
//启动主线程级开启处理线程
void CCollectionAnfContrastDlg::OnBnClickedButStartthand()
{
	InitializeCriticalSection(&cSECTION);
	g_SchedulingThread = AfxBeginThread(&ThreadScheduling, NULL);
	GetDlgItem(IDC_BUT_STARTTHAND)->EnableWindow(FALSE);
}
//托盘
void CCollectionAnfContrastDlg::OnBnClickedButSetevent()
{
	m_tray.CreateTray(IDR_MENU1, this, WM_SHOW_TASK, "人脸识别服务端", this->m_hIcon, true);
	ShowWindow(SW_HIDE);
}
//退出托盘
void CCollectionAnfContrastDlg::OnDeletetary()
{
	m_tray.DelTray();
	this->ShowWindow(SW_NORMAL);
	OnOK();
}

//设置
void CCollectionAnfContrastDlg::OnBnClickedButSetvaluetodb()
{
	CSetValuToDb dlg(NULL, atoi(m_csCameraSimilarity), m_nSleepTime, m_nBideTime, m_nIsOnlineTime);
	dlg.DoModal();
}

void CCollectionAnfContrastDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1)
	{
		KillTimer(1);
		OnBnClickedButSetevent();
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CCollectionAnfContrastDlg::OnBnClickedCancel2()
{

	AfxBeginThread(&HeartbeatThread, 0);

}

UINT HeartbeatThread(LPVOID lpam)
{
	BOOL bInitSocket = FALSE;
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
			strSend.Format("Heartbeat_%d", PROGRAM_INDEX);
			int length;
			length = sendto(sock, strSend, strSend.GetLength(), 0, (SOCKADDR*)&sin, sizeof(sin));

			Sleep(SLEEP_TIME);
		}
		catch (CException* e)
		{
		}
	}
	return 0;
}