
// CollectionAnfContrastDlg.cpp : ʵ���ļ�
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

#define UDP_SEND_PORT 30000  //udp�˿�
#define  PROGRAM_INDEX   0		//����ID

#define  SLEEP_TIME   30000		//����ID
/////////////ȫ�ֱ���/////////////////////////////////////////////////////
static ThandScheduling * g_ThandSchedulingArr;
CWinThread * g_UpdataOnlineThread;
CWinThread * g_SchedulingThread;
static int g_ThandWorkState = 0; //�����е�һ���
static int g_ThandWorkOnline = 0;//���ߵ�һ���
static int g_nContrastComputer = 0;//����һ�������
static BOOL g_bIsMOdify = FALSE;

_ConnectionPtr  g_pConnect;
_RecordsetPtr g_pRecordset;
UINT ThreadContrast(LPVOID lparam);
UINT ThreadScheduling(LPVOID lparam);
UINT ThreadUpdataComputerOnline(LPVOID lparam);
UINT ThreadRestart(LPVOID lparam);
UINT HeartbeatThread(LPVOID lpam);
/////////////////////////////////////////////////////////////////////////////

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


// CCollectionAnfContrastDlg �Ի���



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


// CCollectionAnfContrastDlg ��Ϣ�������
LRESULT CCollectionAnfContrastDlg::OnShowNotify(WPARAM wParam, LPARAM lParam)
{
	return m_tray.OnShowTask(wParam, lParam);
}
BOOL CCollectionAnfContrastDlg::OnInitDialog()
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
			MessageBox("���ݿ������ļ���ȡ����", "��ʾ");
		}

	}
	else
	{

		MessageBox("���ݿ�����ʧ��", "��ʾ");
	}

	SetTimer(1, 1, NULL);
	AfxBeginThread(&HeartbeatThread, 0);


	//OnBnClickedButSetevent();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCollectionAnfContrastDlg::OnPaint()
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
HCURSOR CCollectionAnfContrastDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
bool CCollectionAnfContrastDlg::GetParameterToDb()
{
	CString csSql;
	int nThreadIdex = 0;
	_RecordsetPtr pRecordset;

	//��ȡʶ��һ�������
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
//��ȡINI �ļ�·��
CString CCollectionAnfContrastDlg::GetiniPath()
{
	CString strIp;
	TCHAR pFileName[MAX_PATH];
	int nPos = GetCurrentDirectory(MAX_PATH, pFileName);

	CString csFullPath(pFileName);
	csFullPath += DBINI_PATH;

	return csFullPath;
}
//��ȡimage �ļ�·��
CString CCollectionAnfContrastDlg::GetImagePath(CString csImageName, CString csFile)
{
	CString strIp;
	TCHAR pFileName[MAX_PATH];
	int nPos = GetCurrentDirectory(MAX_PATH, pFileName);

	CString csFullPath(pFileName);
	csFullPath.Format("%s%s%s%scurface_%s.b64", csFullPath, IMAGE_ROOTPATH, csFile, IMAGE_PATH, csImageName);
	return csFullPath;
}
//��ȡwarning ·��
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
//��ʼ�� ���ݿ�
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
//��ʼ��ʶ���̷߳��ظ���
int CCollectionAnfContrastDlg::InitComputerThread()
{
	try
	{
		CString csSql;
		int nThreadIdex = 0;
		_RecordsetPtr pRecordset;

		//��ȡʶ��һ�������
		csSql = "select count(1) from device where type = 1 ";
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		if (!pRecordset->adoEOF)
		{
			CString csEmpInfoName = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("count(1)")->Value;
			g_nContrastComputer = atoi(csEmpInfoName);
		}

		//��ʶ��һ���ʱ������Ӧ�Ա��߳�
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
		//���һ����߳�������������ɾ��
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
//��������ʶ��һ�������
int CCollectionAnfContrastDlg::UpdataComputerOline()
{
	try
	{

		CString csSql;
		int nThreadIdexIsOnline = 0;
		_RecordsetPtr  pRecordset;

		//�ж��Ƿ����޸����ݿ������ƶ���������ֵ 2017��5��5��
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
		//��ʶ��һ���ʱ������Ӧ�Ա��߳�
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
					//�ж��Ƿ����޸����ݿ������ƶ���������ֵ 2017��5��5��
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
//�ܵ����߳�
UINT ThreadScheduling(LPVOID lparam)
{
	CCollectionAnfContrastDlg *pWnd = (CCollectionAnfContrastDlg *)AfxGetApp()->GetMainWnd();

	CString csSql, csName, csFile;
	int nIdex = 0;
	int nWorkTheradSum;//��ʶ��һ�������
	BOOL bWhile = TRUE;
	BOOL bInitInfo = TRUE;
	BOOL bSetEvent = FALSE;
	_RecordsetPtr pRecordset;


	while (bWhile)
	{
		try
		{
			//��һ�γ�ʼ������ ���� �̻߳�ȡ����һ�������
			if (bInitInfo)
			{

				//��ʼ��ʶ��һ����̵߳����� 0 ʱʧ��
				nWorkTheradSum = pWnd->InitComputerThread();
				CString  csTemp;
				csTemp.Format("WorkTheradSum_%d", nWorkTheradSum);
				DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csTemp);
				if (nWorkTheradSum == 0)
				{
					continue;
				}
				//�����̲߳�ѯ����һ�������
				g_UpdataOnlineThread = AfxBeginThread(&ThreadUpdataComputerOnline, NULL);
				bInitInfo = FALSE;
			}

			//�����ݿ�����Ҫ�Աȵ�ͼ��ID 
			csSql = "select * from snapshotinfo where process_status = 1";
			pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
			while (!pRecordset->adoEOF)
			{

				//�����ұ�־λֵλ��
				bSetEvent = FALSE;

				//����һ���Ϊ0 ����
				if (g_ThandWorkOnline == 0)
				{
					DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "g_ThandWorkOnline_0");
					Sleep(pWnd->m_nBideTime * 1000);
					continue;
				}

				//�������һ�������������һ�������
				if (g_ThandWorkOnline > nWorkTheradSum)
					g_ThandWorkOnline = nWorkTheradSum;
				EnterCriticalSection(&cSECTION);
				//ѭ���ж�����һ����Ƿ��ڹ���
				for (nIdex = 0; nIdex < nWorkTheradSum; nIdex++)
				{
					//��Ҫ�ȱ��޸������޸�
					csName = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("id")->Value;
					csFile = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("imgpath")->Value;

					g_ThandSchedulingArr[nIdex].csImgName.Add(csName);
					g_ThandSchedulingArr[nIdex].csImgFile.Add(csFile);
					//�޸�snapshotinfo ����Ѷ�ȡ
					csSql.Format("UPDATE snapshotinfo SET process_status = '4'  where id = '%s' ", csName);
					g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);

					//����Ա���ˮ������
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
//�̴߳�����
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

			//��Ҫ����		//���ṹ���ڵ�ֵ��ֵ���ֲ�������ֹ�ⲿ�޸�
			EnterCriticalSection(&cSECTION);
			csNameID = g_ThandSchedulingArr[nIdex].csImgName.GetAt(0);
			csImgFile = g_ThandSchedulingArr[nIdex].csImgFile.GetAt(0);
			g_ThandSchedulingArr[nIdex].csImgName.RemoveAt(0);
			g_ThandSchedulingArr[nIdex].csImgFile.RemoveAt(0);

			csImgPath.Empty();
			csImgPath = pWnd->GetImagePath(csNameID, csImgFile);	//��ȡ��Ƭ·��
			LeaveCriticalSection(&cSECTION);

			csTemp.Format("ThreadContrast_%s_%s_%d_Start", g_ThandSchedulingArr[nIdex].csComputerIp, csNameID, nIdex);
			DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csTemp);


			//��ȡ��Ƭb64
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
			//����dll�ԱȺ���
			nContrast = pWnd->m_CFASLoginOrLogout.DeviceLogin((LPSTR)(LPCTSTR)g_ThandSchedulingArr[nIdex].csComputerIp, pSession);
			if (nContrast == 10 || nContrast == 11)
			{//���߻��߲��ڵ������
				EnterCriticalSection(&cSECTION);
				g_ThandSchedulingArr[nIdex].bIsOnline = FALSE;
				g_ThandWorkOnline--;
				LeaveCriticalSection(&cSECTION);
				continue;
			}

			//�Ա�ͼƬ
			nContrast = pWnd->m_CFASRecognition.FASImgRecongnition(
				(LPSTR)(LPCTSTR)g_ThandSchedulingArr[nIdex].csComputerIp,	//һ���IP
				pSession,
				(LPSTR)(LPCTSTR)g_ThandSchedulingArr[nIdex].csFaceDbIdex,  //�Ա�������ID
				imagebuf,																			 //��Ƭ��Ϣ
				(LPSTR)(LPCTSTR)g_ThandSchedulingArr[nIdex].csMaxSum,		//��󷵻ظ���
				(LPSTR)(LPCTSTR)g_ThandSchedulingArr[nIdex].csSimilarity,		//�Ա����ƶ�
				&nConut,																					//���ظ���
				pStructRecognitionlist);																//���ؽṹ��

			if (nContrast == 0)//�Աȳɹ�3
			{
				nWarning = 0;

				//�޸�ץ�ı� ��־λ
				csSql.Format("UPDATE snapshotinfo SET process_status = '3'  ,warningcount = %d ,RecognitionIp = '%s' where id = '%s' ",
					nConut, g_ThandSchedulingArr[nIdex].csComputerIp, csNameID);
				g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);


				csSql.Format("UPDATE Comparerecord SET process_status = '3' where snapshotid = '%s'  and RecongnitionIp = '%s'  "
					, csNameID, g_ThandSchedulingArr[nIdex].csComputerIp);
				g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);

				//��warning ��д�Աȵ��İ�������
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
			else //�Ա�ʧ��10
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

			//��Ҫ����
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

//�����ݿ��л�ȡ���ߵ�һ���
UINT ThreadUpdataComputerOnline(LPVOID lparam)
{
	int nIdex = (int)lparam;
	CCollectionAnfContrastDlg *pWnd = (CCollectionAnfContrastDlg *)AfxGetApp()->GetMainWnd();
	BOOL bWhile = TRUE;
	while (bWhile)
	{
		//�������� 2017��5��5��10:43:19
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
//�����߳� 2017��5��5��10:43:28 ��ʱ����
UINT ThreadRestart(LPVOID lparam)
{
	if (g_ThandWorkState == 0)
	{

		::PostMessage(AfxGetMainWnd()->m_hWnd, WM_SYSCOMMAND, SC_CLOSE, NULL);
		//��ȡexe����ǰ·�� 
		extern CCollectionAnfContrastApp theApp;
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
	}
	return 0;
}
//������ݿ�����
void CCollectionAnfContrastDlg::OnBnClickedButDetectiondb()
{
	if (InitDbParameter())
	{
		GetDlgItem(IDC_BUT_DETECTIONDB)->EnableWindow(FALSE);
	}
	else
	{
		MessageBox("���ݿ�����ʧ��", "��ʾ");
	}
}
//�������̼߳����������߳�
void CCollectionAnfContrastDlg::OnBnClickedButStartthand()
{
	InitializeCriticalSection(&cSECTION);
	g_SchedulingThread = AfxBeginThread(&ThreadScheduling, NULL);
	GetDlgItem(IDC_BUT_STARTTHAND)->EnableWindow(FALSE);
}
//����
void CCollectionAnfContrastDlg::OnBnClickedButSetevent()
{
	m_tray.CreateTray(IDR_MENU1, this, WM_SHOW_TASK, "����ʶ������", this->m_hIcon, true);
	ShowWindow(SW_HIDE);
}
//�˳�����
void CCollectionAnfContrastDlg::OnDeletetary()
{
	m_tray.DelTray();
	this->ShowWindow(SW_NORMAL);
	OnOK();
}

//����
void CCollectionAnfContrastDlg::OnBnClickedButSetvaluetodb()
{
	CSetValuToDb dlg(NULL, atoi(m_csCameraSimilarity), m_nSleepTime, m_nBideTime, m_nIsOnlineTime);
	dlg.DoModal();
}

void CCollectionAnfContrastDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
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