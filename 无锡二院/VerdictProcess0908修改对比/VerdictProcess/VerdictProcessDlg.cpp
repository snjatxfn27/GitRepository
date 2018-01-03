
// VerdictProcessDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VerdictProcess.h"
#include "VerdictProcessDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define UDP_SEND_PORT 30000  //udp�˿�
#define  PROGRAM_INDEX   3		//����ID
#define  SLEEP_TIME   30000	//����ID
UINT HeartbeatThread(LPVOID lpam); 

#pragma comment(lib , "FaceCamServerDll.lib")
#include"DLLMethodForCSharp.h"

static CStringArray g_arFeatureMatch;
_ConnectionPtr  g_pConnect;
_ConnectionPtr  g_pConnectMain;

static DEVICE_INFO * g_DeviceInfo;
static int g_DeviceSum = 0;//�豸����
static int g_nCurrent = 0;

static BOOL g_bMaintenancWarning = TRUE;
static BOOL g_bMaintenancSnap = TRUE;

UINT Thread_MaintenanceWarninfo(LPVOID lparam);
UINT Thread_MaintenanceSnapshot(LPVOID lparam);

CWinThread	 *g_MaintenanceWarninfo;
CWinThread	 *g_MaintenanceSnapsho;


#define  DBINI_PATH			 "\\Parameter.ini"
#define  BLACK_LIST			 "Blacklist"  //������
#define  DYNAMIC_LIST		 "dynamic"	  //��̬��
#define  TEMPORARY_LIST		 "temporary"  //��ʱ��

#define  COMPAR_REEOR		16
#define  FACE_SUM		4
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


// CVerdictProcessDlg �Ի���



CVerdictProcessDlg::CVerdictProcessDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CVerdictProcessDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVerdictProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CVerdictProcessDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CVerdictProcessDlg ��Ϣ�������

LONG CallBackCrashHandler(EXCEPTION_POINTERS *pException)
{

	return EXCEPTION_EXECUTE_HANDLER;
}

BOOL CVerdictProcessDlg::OnInitDialog()
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
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)CallBackCrashHandler);

	Start();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CVerdictProcessDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CVerdictProcessDlg::OnPaint()
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
HCURSOR CVerdictProcessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


CString CVerdictProcessDlg::GetiniPath()
{
	CString strIp;
	TCHAR pFileName[MAX_PATH];
	int nPos = GetCurrentDirectory(MAX_PATH, pFileName);

	CString csFullPath(pFileName);
	csFullPath += DBINI_PATH;

	return csFullPath;
}

int __BreakCString(CString csData, CStringArray &arOutCString, CString csFindFlag)
{
	int iPos = -1;
	CString csTemp;
	while ((iPos = csData.Find(csFindFlag)) != -1)
	{
		csTemp = csData.Left(iPos);
		csData.Delete(0, iPos + 1);
		arOutCString.Add(csTemp);
	}
	arOutCString.Add(csData);

	return arOutCString.GetCount();
}
//��ʼ
void CVerdictProcessDlg::Start()
{

	BOOL bWhile = TRUE;
	BOOL bInitDatbase = FALSE;
	BOOL bGetIni = FALSE;
	BOOL bGetDevice = FALSE;
	while (bWhile)
	{

		if (!bGetIni)
		{
			if (GetIni())
			{
				bGetIni = TRUE;
			}
			else
			{
				continue;
			}
		}

		if (!bInitDatbase)
		{
			if (InitDatabase())
			{
				bInitDatbase = TRUE;
			}
			else
			{
				continue;
			}
		}

		if (!bGetDevice)
		{
			if (GetDeviceFaceDb())
				bGetDevice = TRUE;
			else
			{
				continue;
			}
		}

		m_bOpenDataMove = FALSE;
		g_MaintenanceWarninfo = AfxBeginThread(&Thread_MaintenanceWarninfo, NULL);
		g_MaintenanceSnapsho = AfxBeginThread(&Thread_MaintenanceSnapshot, NULL);
		AfxBeginThread((AFX_THREADPROC)HeartbeatThread, (void*)this);

		bWhile = FALSE;
	}
}

BOOL CVerdictProcessDlg::GetIni()
{
	CString csFullPath = GetiniPath();

	int nLen = GetPrivateProfileString(("SqlConfiguration"), "Sql_ServerAdd", (""), m_csSqlIP.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);
	if (nLen <= 0)
		return FALSE;
	nLen = GetPrivateProfileString(("SqlConfiguration"), "Sql_Database", (""), m_csSqlDatabase.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);
	if (nLen <= 0)
		return FALSE;
	nLen = GetPrivateProfileString(("SqlConfiguration"), "Sql_DatabaseHistory", (""), m_csSqlData.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);
	if (nLen <= 0)
		return FALSE;
	nLen = GetPrivateProfileString(("Computations"), "Appear_Num", (""), m_csAppearNum.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);
	if (nLen <= 0)
		return FALSE;
	nLen = GetPrivateProfileString(("Computations"), "Current_Num", (""), m_csCurrentNum.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);
	if (nLen <= 0)
		return FALSE;
	nLen = GetPrivateProfileString(("Computations"), "Age_Num", (""), m_csAge.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);
	if (nLen <= 0)
		return FALSE;
	nLen = GetPrivateProfileString(("Computations"), "Age_Results", (""), m_csAgeResults.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);
	if (nLen <= 0)
		return FALSE;
	nLen = GetPrivateProfileString(("Computations"), "Time_TimeOutM", (""), m_csTimeOut.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);
	if (nLen <= 0)
		return FALSE;
	
	return TRUE; 

}
//�ж�sql�Ƿ����� ����Ͽ�����
void CVerdictProcessDlg::IsOpenSql(BOOL bFlag)
{
	if (bFlag)
	{
		if (g_pConnect->GetState() != adStateOpen)
		{
			while (!InitDatabase())
			{
				Sleep(2000);
			}
		}
	}
	else
	{
		if (g_pConnectMain->GetState() != adStateOpen)
		{
			while (!InitDatabaseMove())
			{
				Sleep(2000);
			}
		}
	}
}
//��ʼ�����ݿ�����
BOOL CVerdictProcessDlg::InitDatabase()
{

	CString csSqlConnect;
	csSqlConnect.Format("Driver={MySQL ODBC 5.1 Driver};Server=%s;Database=%s;uid=root;pwd=cr;", m_csSqlIP, m_csSqlDatabase);
	try
	{
		g_pConnect.CreateInstance(__uuidof(Connection));
		_bstr_t strConnect = csSqlConnect;
		g_pConnect->Open(strConnect, "", "", adModeUnknown);
	}
	catch (...)
	{
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "InitDatabase_Catch");
		return FALSE;
	}

	return  TRUE;
}
//��ʼ���������ݿ�����
BOOL CVerdictProcessDlg::InitDatabaseMove()
{
	CString csSqlConnect;
	csSqlConnect.Format("Driver={MySQL ODBC 5.1 Driver};Server=%s;Database=%s;uid=root;pwd=cr;", m_csSqlIP, m_csSqlData);
	try
	{
		g_pConnectMain.CreateInstance(__uuidof(Connection));
		_bstr_t strConnect = csSqlConnect;
		g_pConnectMain->Open(strConnect, "", "", adModeUnknown);
		m_bOpenDataMove = TRUE;
	}
	catch (...)
	{
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "InitDatabaseMove_Catch");
		return FALSE;

	}
	return  TRUE;
}
//�رձ������ݿ�����
void CVerdictProcessDlg::CloseDatabaseMove()
{
	try
	{
		g_pConnectMain->Close();
		m_bOpenDataMove = FALSE;
	}
	catch (...)
	{
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "CloseDatabaseMove_Catch");
	}
}
//��ȡ���ĳһ���ֶε����ֵ
BOOL CVerdictProcessDlg::GetDbTableMaxItem(CString csItem, CString csTable, CString &csMaxItem)
{
	try
	{
		CString csSql, csTemp, csCount;
		_RecordsetPtr pRecordset;
		csTemp.Format(("max(%s) as maxitem,count(*)"), csItem);
		csSql.Format(("select %s from %s"), csTemp, csTable);
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		if (!pRecordset->adoEOF)
		{
			csCount = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem(_variant_t("count(*)"))->Value;
			if (csCount != "0")
			{
				csMaxItem = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem(_variant_t("maxitem"))->Value;
				return TRUE;
			}
		}
	}
	catch (...)
	{
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "GetDbTableMaxItem_Catch");
	}

	return FALSE;
}
//�ж�ʱ��ʱ���ڱ�������
BOOL CVerdictProcessDlg::IsCycleInner(CString csCurrentDate, int  &nCurrent)
{
	try
	{
		CString csSql;
		_RecordsetPtr pRecordset;
		CString csCurrentNum, csCurrent, csMinTime, csStartTime, csEndTime;

		//csSql.Format(("select *,count(*) from cycleinfo where startdate <= '%s' and enddate <= '%s'"), csCurrentDate, csCurrentDate);
		csSql.Format(("select *,count(*) from cycleinfo where  enddate >= '%s'"), csCurrentDate, csCurrentDate);
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		if (!pRecordset->adoEOF)
		{
			csCurrentNum = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("count(*)")->Value;

			if (csCurrentNum == "0")
			{
				return FALSE;
			}

			csCurrent = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("currentcycle")->Value;
			nCurrent = atoi(csCurrent);
			return TRUE;
		}
	}
	catch (...)
	{
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "IsCycleInner_Catch");
	}
	return FALSE;
}
//���������һ���ֶ�����
BOOL CVerdictProcessDlg::SetCycleNext()
{
	try
	{
		_RecordsetPtr pRecordset;
		CString csSql, csMaxItem, csEndDate, csDateNum, csCurrentcycle, csMinTime;
		if (GetDbTableMaxItem(("id"), ("cycleinfo"), csMaxItem))
		{
			csSql.Format(("select *  from cycleinfo where  id = '%s'"), csMaxItem);
			pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
			if (!pRecordset->adoEOF)
			{
				csEndDate = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("enddate")->Value;
				csDateNum = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("datenum")->Value;
				csCurrentcycle = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("currentcycle")->Value;
				int  nDay = atoi(m_csCurrentNum);
				COleDateTimeSpan delta1(1, 0, 0, 0);
				COleDateTimeSpan delta2(atoi(m_csCurrentNum), 0, 0, 0);

				COleDateTime EndTime;
				int y, m, d, h, u, s;
				sscanf(csEndDate, ("%d-%d-%d "), &y, &m, &d);
				EndTime.SetDateTime(y, m, d, 0, 0, 0);

				COleDateTime CNextStartTime, CNextEndTime;
				CNextStartTime = EndTime + delta1;
				CNextEndTime = EndTime + delta2;
				CString csNextStartTime, csNextEndTime;
				csNextStartTime = CNextStartTime.Format("%Y-%m-%d");
				csNextEndTime = CNextEndTime.Format("%Y-%m-%d");

				csSql.Format(("insert into cycleinfo (datenum, startdate, enddate,currentcycle)values('%s','%s','%s','%d')"),
					m_csCurrentNum, csNextStartTime, csNextEndTime, atoi(csCurrentcycle) + 1);

				g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
				return TRUE;
			}
		}
		else
		{
			csSql = "SELECT MIN(alarmtime) FROM snapshot";
			pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
			if (!pRecordset->adoEOF)
				csMinTime = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("MIN(alarmtime)")->Value;
			else
				return FALSE;

			COleDateTime StartTime;
			int y, m, d, h, u, s;
			sscanf(csMinTime, ("%d/%d/%d %d:%d:%d"), &y, &m, &d, &h, &u, &s);
			StartTime.SetDateTime(y, m, d, h, u, s);

			CString csNextStartTime, csNextEndTime;
			csNextStartTime = StartTime.Format("%Y-%m-%d");
			COleDateTime  CNextEndTime;

			COleDateTimeSpan delta2(atoi(m_csCurrentNum) - 1 , 0, 0, 0);
			CNextEndTime = StartTime + delta2;
			csNextEndTime = CNextEndTime.Format("%Y-%m-%d");

			csSql.Format(("insert into cycleinfo (datenum, startdate, enddate,currentcycle)values('%s','%s','%s','%d')"),
				m_csCurrentNum, csNextStartTime, csNextEndTime, atoi(csCurrentcycle) + 1);

			g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
			return TRUE;
		}
	}
	catch (...)
	{
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "SetCycleNext_Catch");
	}
	return FALSE;
}
//��ȡdevice�������ݳ�ʼ������
BOOL CVerdictProcessDlg::GetDeviceFaceDb()
{
	try
	{
		CString csSql;
		_RecordsetPtr pRecordset;

		csSql = "select count(*)  as devicesum from device;";
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		if (!pRecordset->adoEOF)
		{
			CString csDeviceSum = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("devicesum")->Value;
			g_DeviceSum = atoi(csDeviceSum);
			if (g_DeviceSum == 0)
			{
				return FALSE;
			}
			g_DeviceInfo = new DEVICE_INFO[g_DeviceSum];
		}

		int nDeviceIndex = 0;
		csSql = "select * from device;";
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		while (!pRecordset->adoEOF)
		{
			g_DeviceInfo[nDeviceIndex].csDeviceIp = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("ip")->Value;
			g_DeviceInfo[nDeviceIndex].csDescription = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("description")->Value;
			g_DeviceInfo[nDeviceIndex].csFaceDbs = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("facedbs")->Value;
			g_DeviceInfo[nDeviceIndex].csType = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("type")->Value;
			CString csOnline = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("isonline")->Value;
			g_DeviceInfo[nDeviceIndex].bIsonline = csOnline == "1" ? TRUE : FALSE;

			CStringArray arFaceDb;
			int nNum = __BreakCString(g_DeviceInfo[nDeviceIndex].csFaceDbs, arFaceDb, "+");
			if (nNum != FACE_SUM)
			{
				return FALSE;
			}
			g_DeviceInfo[nDeviceIndex].csFacedbBlacekList = arFaceDb.GetAt(0);
			g_DeviceInfo[nDeviceIndex].csFacedbWhiteList = arFaceDb.GetAt(1);
			g_DeviceInfo[nDeviceIndex].csFacedbDynamicList = arFaceDb.GetAt(2);
			g_DeviceInfo[nDeviceIndex].csFacedbTemporaryList = arFaceDb.GetAt(3);

			nDeviceIndex++;
			pRecordset->MoveNext();
		}

		return TRUE;
	}
	catch (...)
	{
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "GetDeviceFaceDb_Catch");
	}
	return FALSE;
}
//
UINT Thread_MaintenanceWarninfo(LPVOID lparam)
{
	CVerdictProcessDlg *pWnd = (CVerdictProcessDlg *)AfxGetApp()->GetMainWnd();

	while (g_bMaintenancWarning)
	{
		try
		{
			pWnd->IsOpenSql(TRUE);
			pWnd->ManageWarninfo();
			Sleep(1000);
		}
		catch (...)
		{
			DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "Thread_MaintenanceWarninfo _Catch");

		}
	}
	return 0;
}
//
UINT Thread_MaintenanceSnapshot(LPVOID lparam)
{
	CVerdictProcessDlg *pWnd = (CVerdictProcessDlg *)AfxGetApp()->GetMainWnd();

	while (g_bMaintenancSnap)
	{
		try
		{
			pWnd->IsOpenSql(TRUE);
			pWnd->VerdictSnapshot();
			Sleep(500);
		}
		catch (...)
		{
			DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "Thread_MaintenanceWarninfo _Catch");
		}
	}
	return 0;
}
//����warning��
BOOL CVerdictProcessDlg::ManageWarninfo()
{
	try
	{
		CString csSql, csAlarmtime, csFaceDb, csSnapShotid, csIdCard, csSimilarity, csCameraid, csWarninfoId;
		_RecordsetPtr pRecordset;
		int nCurrent = 0;
		csSql = "SELECT a.*,b.alarmtime,b.cameraid from warninfo  a join snapshot b on a.snapshotid = b.Snapshotid;";
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		while (!pRecordset->adoEOF)
		{
			csWarninfoId = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("warninfoId")->Value;
			csSnapShotid = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("snapshotid")->Value;
			csIdCard = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("id_card")->Value;
			csSimilarity = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("similarity")->Value;
			csAlarmtime = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("alarmtime")->Value;
			csFaceDb = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("FaceDb")->Value;
			csCameraid = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("cameraid")->Value;

			COleDateTime StartTime;
			int y, m, d, h, u, s;
			sscanf(csAlarmtime, ("%d/%d/%d %d:%d:%d"), &y, &m, &d, &h, &u, &s);
			StartTime.SetDateTime(y, m, d, h, u, s);
			CString csMaxTimeTemp;
			csMaxTimeTemp = StartTime.Format("%Y-%m-%d");

			if (!IsCycleInner(csMaxTimeTemp, nCurrent))
			{
				continue;
			}

			if (nCurrent != 0)
			{
				for (int nIndex = 0; nIndex < g_DeviceSum; nIndex++)
				{
					if (csFaceDb == g_DeviceInfo[nIndex].csFacedbBlacekList)//��
					{
						WriteBlacekLis(csWarninfoId, csSnapShotid, csIdCard, csSimilarity, csCameraid, csAlarmtime, nCurrent);
					}
					else if (csFaceDb == g_DeviceInfo[nIndex].csFacedbDynamicList)//��̬
					{
						WriteDynamicList(csWarninfoId, csSnapShotid, csIdCard, csSimilarity, csCameraid, csAlarmtime, nCurrent);
					}
					// 					else if (csFaceDb == g_DeviceInfo[nIndex].csFacedbTemporaryList)//��ʱ
					// 					{
					// 						WriteTemporaryList(csWarninfoId, csSnapShotid, csIdCard, csSimilarity, csCameraid, csAlarmtime, nCurrent);
					// 					}
				}
				pRecordset->MoveNext();
			}
			else
			{
				return FALSE;
			}
		}
		return TRUE;
	}
	catch (...)
	{
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "ManageWarninfo_Catch");
	}
	return FALSE;
}
//
BOOL CVerdictProcessDlg::WriteBlacekLis(CString csWarninfoId, CString csSnapShotid, CString csIdCard,
	CString csSimilarity, CString csCameraid, CString csInserttime, int nCurrent)
{
	try
	{
		CString csSql;
		csSql.Format("insert into blacklist(snapshotid, id_card, similarity,alarmtime, cameraid, cyclnum)values('%s','%s','%s','%s','%s','%d')",
			csSnapShotid, csIdCard, csSimilarity, csInserttime, csCameraid, nCurrent);
		g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);

		csSql.Format("delete from warninfo where warninfoId = '%s' and snapshotid = '%s';", csWarninfoId, csSnapShotid);
		g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		return TRUE;
	}
	catch (...)
	{
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "WriteBlacekLiso _Catch");
	}
	return FALSE;
}
//
BOOL CVerdictProcessDlg::WriteDynamicList(CString csWarninfoId, CString csSnapShotid, CString csIdCard,
	CString csSimilarity, CString csCameraid, CString csInserttime, int nCurrent)
{
	try
	{
		CString csSql;
		csSql.Format("insert into dynamic(snapshotid, id_card, similarity,alarmtime, cameraid, cyclnum)values('%s','%s','%s','%s','%s','%d')",
			csSnapShotid, csIdCard, csSimilarity, csInserttime, csCameraid, nCurrent);
		g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);

		csSql.Format("delete from warninfo where warninfoId = '%s'and snapshotid = '%s';", csWarninfoId, csSnapShotid);
		g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		return TRUE;
	}
	catch (...)
	{
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "WriteDynamicList _Catch");
	}
	return FALSE;
}
//
BOOL CVerdictProcessDlg::WriteTemporaryList(CString csSnapShotid, CString csIdCard,
	CString csSimilarity, CString csCameraid, CString csInserttime, int nCurrent)
{
	try
	{
		CString csSql;
		csSql.Format("insert into temporary(snapshotid, id_card, similarity,alarmtime, cameraid, cyclnum)values('%s','%s','%s','%s','%s','%d')",
			csSnapShotid, csIdCard, csSimilarity, csInserttime, csCameraid, nCurrent);
		g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		return TRUE;
	}
	catch (...)
	{
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "WriteTemporaryList _Catch");
	}
	return FALSE;
}
//��ȡ���ƶ�
BOOL CVerdictProcessDlg::GetComputerSimilar(CString &csSim)
{
	try
	{
		CString csSql;
		_RecordsetPtr pRecordset;
		CString csIdcard;
		csSql = ("select * from parameterini where Parameter = 'computer';");
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		if (!pRecordset->adoEOF)
		{
			csSim = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("CameraSimilarity")->Value;
			return TRUE;
		}
	}
	catch (...)
	{
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "GetComputerSimilar _Catch");
	}
	return FALSE;
}
//��ȡte���idcard 
BOOL CVerdictProcessDlg::GetTeIdcard(CString csSnapid,CString &csIDcard)
{
	try
	{
		CString  csSql;
		_RecordsetPtr pRecordset;
		csSql.Format("SELECT * FROM `temporary` WHERE snapshotid = '%s'", csSnapid);
		//DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csSql);

		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		if (!pRecordset->adoEOF)
		{
			csIDcard = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("id_card")->Value;
			//DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csIDcard);
			return TRUE;
		}
	}
	catch (...)
	{
		return FALSE;
	}
	return FALSE;
}
//�������������
void CVerdictProcessDlg::MaintainVerdictTime(CString csTime)
{
	CString csTemp, csSnapidTemp, csTimeTemp;
	COleDateTime StartTime, StartTime1;
	int y, m, d, h, u, s;
	sscanf(csTime, ("%d-%d-%d %d:%d:%d"), &y, &m, &d, &h, &u, &s);
	StartTime.SetDateTime(y, m, d, h, u, s);

	int nlen = g_arFeatureMatch.GetSize();
	for (int i = 0; i<  g_arFeatureMatch.GetSize();i++)
	{
		csTemp = g_arFeatureMatch.GetAt(i);
		int nStart = 0;
		nStart = csTemp.Find("+");
		csSnapidTemp = csTemp.Mid(0, nStart);
		csTimeTemp = csTemp.Mid(nStart + 1, csTemp.GetLength());
		csTimeTemp.Replace("/", "-");

		int y1, m1, d1, h1, u1, s1;
		sscanf(csTimeTemp, ("%d-%d-%d %d:%d:%d"), &y1, &m1, &d1, &h1, &u1, &s1);
		StartTime1.SetDateTime(y1, m1, d1, h1, u1, s1);
		COleDateTimeSpan ts = StartTime - StartTime1;

		if (ts.GetSeconds() > 60)
		{
			g_arFeatureMatch.RemoveAt(i);
			i--;
		}
		nlen = g_arFeatureMatch.GetSize();
	}
}
//�ж϶Ա�����������1��1�Ƿ��жԱȳɹ���
BOOL CVerdictProcessDlg::VerdictTime(CString csSnapId, CString csTime, CString csPath, CString csIp, char pSession[37], CString &csSim, CString &csIDCard)
{
	try
	{
		CString csTemp, csSnapidTemp, csTimeTemp, csSql;
		_RecordsetPtr pRecordset;
		BOOL bIsMatch = FALSE;
		if (g_arFeatureMatch.GetSize() != 0)
		{
			//csTemp.Format("Time_%d", g_arFeatureMatch.GetSize());
		//	DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csTemp);

			csTime.Replace("/", "-");
			COleDateTime StartTime, StartTime1;
			int y, m, d, h, u, s;
			sscanf(csTime, ("%d-%d-%d %d:%d:%d"), &y, &m, &d, &h, &u, &s);
			StartTime.SetDateTime(y, m, d, h, u, s);

			CFile file;
			csPath.Replace("imgCurFaceJpg", "imgCurFaceFeature");
			csPath.Replace(".jpg", ".txt");
			if (!file.Open(csPath, CFile::modeRead))
			{
				return FALSE;
			}
			 
			int iFileLen1 = file.GetLength();
			char *featuresbuf2 = new char[iFileLen1 + 1];
			memset(featuresbuf2, 0, iFileLen1 + 1);
			file.Read(featuresbuf2, iFileLen1);
			file.Close();

			for (int i = 0; i < g_arFeatureMatch.GetSize();i++)
			{
				csTemp = g_arFeatureMatch.GetAt(i);
				int nStart = 0;
				nStart = csTemp.Find("+");
				csSnapidTemp = csTemp.Mid(0, nStart);
				csTimeTemp = csTemp.Mid(nStart + 1, csTemp.GetLength());
				csTimeTemp.Replace("/", "-");
				 
				int y1, m1, d1, h1, u1, s1;
				sscanf(csTimeTemp, ("%d-%d-%d %d:%d:%d"), &y1, &m1, &d1, &h1, &u1, &s1);
				StartTime1.SetDateTime(y1, m1, d1, h1, u1, s1);
				COleDateTimeSpan ts = StartTime - StartTime1;
				
				int  nTimeOut = ts.GetHours() *3600 + ts.GetMinutes() * 60 + ts.GetSeconds();

				if (nTimeOut >= atoi(m_csTimeOut))
				{
					CString csTemp;
					//csTemp.Format("deletear_%s_%d", g_arFeatureMatch.GetAt(i), nTimeOut);
					//DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csTemp);
					g_arFeatureMatch.RemoveAt(i);
					i--;
					continue;
				}

				csSql.Format("select * from `snapshot` where Snapshotid='%s'", csSnapidTemp);
				pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
				if (!pRecordset->adoEOF)
				{
					CString csPath1 = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("FacePhotoName")->Value;
					csPath1.Replace("imgCurFaceJpg", "imgCurFaceFeature");
					csPath1.Replace(".jpg", ".txt");
					if (!file.Open(csPath1, CFile::modeRead))
					{
						continue;
					}

					int iFileLen = file.GetLength();
					char *featuresbuf1 = new char[iFileLen + 1];
					memset(featuresbuf1, 0, iFileLen + 1);
					file.Read(featuresbuf1, iFileLen);
					file.Close();


					char  json[1024 * 1024];
					int nRe = FASFeatureMatch(csIp, pSession, featuresbuf1, featuresbuf2, json,1024*1024);
					if (nRe != 0 )
					{
						CString csTemp;
						csTemp.Format("FASFeatureMatch_False_%s_%s_%d", csPath1, csPath, nRe);
						DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csTemp);
						return FALSE;
					}
					delete[] featuresbuf1;

					int s(0), e(0);
					CString strRecv(json);
					s = strRecv.Find(_T("\"similarity\":"));
					s = s + 13;
					e = strRecv.Find(_T("}"), s);
					CString strFeatures = strRecv.Mid(s, e - s);

					if (atof(strFeatures) >  atof(m_csSimilarity))
					{
						if (GetTeIdcard(csSnapidTemp, csIDCard))
						{
							csSim = strFeatures;
							delete[] featuresbuf2;
							return TRUE;
						}
					}
				}
			}
			delete[] featuresbuf2;
		}
	}
	catch (CException* e)
	{
		return FALSE;
	}
	return FALSE;
}
//����snap��
BOOL CVerdictProcessDlg::ManageSnapshot(CString csStartTime, CString csEndTime, CString nCurrent, BOOL bIsdelete)
{
	try
	{
		CString csSql;
		char pSession[37];
		BOOL bMove = FALSE;
		_RecordsetPtr pRecordset;
		CString csIdcard;
		if (!GetComputerSimilar(m_csSimilarity))
		{
			DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "GetComputerSimilar_FALSE");
			return FALSE;
		}

		csEndTime += " 23:59:59";
		csSql.Format("select * from snapshot where (process_status = 0)and alarmtime >= '%s' and alarmtime <= '%s';", csStartTime, csEndTime);
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		if (!pRecordset->adoEOF)
		{
			if (!bIsdelete)
			{
				//DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, " Sleep_Dis");
				Sleep(5000);
				return FALSE; 
			}
		}

// 		csSql.Format("select * from snapshot where ( process_status = 16 )and alarmtime >= '%s' and alarmtime <= '%s';", csStartTime, csEndTime);
// 		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
// 		if (pRecordset->adoEOF)//û��ֵ
// 		{
// 			bMove = TRUE;
// 		}

		csSql.Format("select * from snapshot where process_status = %d and alarmtime >= '%s' and alarmtime <= '%s' ORDER BY Snapshotid LIMIT 0,10;", COMPAR_REEOR, csStartTime, csEndTime);
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		while (!pRecordset->adoEOF)
		{
			if (!g_bMaintenancSnap)
			{
				return TRUE;
			}

			CString csSnapshotId = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("Snapshotid")->Value;
			CString csCameraId = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("cameraid")->Value;
			CString csAlarmtime = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("alarmtime")->Value;
			CString csImgPath = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("FacePhotoName")->Value;

			//��ȡfeature ·��
			CString csFeaturesPath = csImgPath;
			csFeaturesPath.Replace("imgCurFaceJpg", "imgCurFaceFeature");
			csFeaturesPath.Replace(".jpg", ".txt");
// 			int ntemp = csFeaturesPath.Find("Data");
// 			csFeaturesPath = csFeaturesPath.Mid(ntemp, csFeaturesPath.GetLength() - ntemp);
			CFile file;
			if (!file.Open(csFeaturesPath, CFile::modeRead))
			{
				csSql.Format("UPDATE  snapshot set  process_status = '%d' where Snapshotid = %s;", 23, csSnapshotId);//��ȡ�ļ�ʧ��23
				g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
				return FALSE;
			}

			int iFileLen = file.GetLength();
			char *featuresbuf = new char[iFileLen + 1];
			memset(featuresbuf, 0, iFileLen + 1);
			file.Read(featuresbuf, iFileLen);
			file.Close();
			CString strFeatures(featuresbuf);

			int nMatchNum = 0;
			for (int nDeviceIndex = 0; nDeviceIndex < g_DeviceSum; nDeviceIndex++)
			{
				int nResult = DeviceLogin(g_DeviceInfo[nDeviceIndex].csDeviceIp, pSession);
				int intGender = 0;
				CString strBirthday = "1990-01-01";
				CString strValidTo = "2090-01-01";

				//�Ա�
				char chJsonMatch[1024 * 1024];
				memset(chJsonMatch, '\0', 1024 * 1024);

				srand((unsigned)time(NULL));
				int nRand = rand() % 10;
				double douSim = atof(m_csSimilarity);
				CString csSimilarity1;
				csSimilarity1.Format(_T("%0.1f%d"), douSim, nRand);

				nResult = FASMatchPerson(g_DeviceInfo[nDeviceIndex].csDeviceIp, pSession, g_DeviceInfo[nDeviceIndex].csFacedbTemporaryList, atof(csSimilarity1), featuresbuf, chJsonMatch, 1024 * 1024);

				if (nResult != 0)//һ���ʧ��
				{
					DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, " FASMatchPerson_FALSE");
					DeviceLogout(g_DeviceInfo[nDeviceIndex].csDeviceIp, pSession);
					return FALSE;
				}

				Json::Reader reader;
				Json::Value root;
				Json::Value content;

				BOOL readre = reader.parse(chJsonMatch, root);
				if (!readre)
					return FALSE;

				int iZero = 0;
				COleDateTime coTime;
				coTime = COleDateTime::GetCurrentTime();
				CString strCurTime2 = coTime.Format("%Y-%m-%d %H:%M:%S");

				content = root["data"]["content"][iZero];
				if (content.isNull())//�Ա�ʧ��
				{
					CString csSimMatch, csIDCardMatch;
					if (VerdictTime(csSnapshotId, strCurTime2, csImgPath, g_DeviceInfo[nDeviceIndex].csDeviceIp, pSession, csSimMatch, csIDCardMatch))//�Ա�������
					{
						WriteTemporaryList(csSnapshotId, csIDCardMatch, csSimMatch, csCameraId, csAlarmtime, atoi(nCurrent));
						//MaintainVerdictTime(strCurTime2);

						csSql.Format("UPDATE  snapshot set  process_status = '%d' where Snapshotid = %s;", 22, csSnapshotId);//�Ա���ʱ��ɹ�22
						g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
						continue;
					}

					char Personid[126];
					char chImage[2] = { '1', '\0' };
					if (!GetEmpIdcard(csIdcard))
					{
						DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "GetEmpIdcard_FALSE");
						return FALSE;
					}

					csSql.Format("UPDATE  parameterini set  MaxIdCard = '%s' where  Parameter = 'IdCard';", csIdcard);
					g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
				//	DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csSql);

					//ע��
					nResult = FASAddPersoninfo(g_DeviceInfo[nDeviceIndex].csDeviceIp, pSession, csIdcard, csIdcard, intGender, atoi(g_DeviceInfo[nDeviceIndex].csFacedbTemporaryList),
						featuresbuf, chImage, strBirthday, strValidTo, Personid, 126);
					//�ǳ�
					DeviceLogout(g_DeviceInfo[nDeviceIndex].csDeviceIp, pSession);
					if (nResult != 0)
					{
						DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "DeviceLogout_FALSE");
						delete[]featuresbuf;
						return FALSE;
					}
					CString csPersonid(Personid);
					//д����������  �Ա�ʧ��
					CString csSql;
					WriteTemporaryList(csSnapshotId, csIdcard, "0", csCameraId, csAlarmtime, atoi(nCurrent));

					csSql.Format("insert into employeeinfo(name, sex, birth, id_card,libraryid, nativeplace, phone,address ,isbatch, isdelete,type,personid)values('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",
						csIdcard, "female", " ", csIdcard, "4", " ", " ", " ", "0", "0", "��ʱ", csPersonid);
					g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);

					csSql.Format("insert into imginfo(id_card, imgpath, featurepath)values('%s','%s','%s')", csIdcard, csImgPath, csFeaturesPath);
					g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);

					csSql.Format("UPDATE  snapshot set  process_status = '%d' where Snapshotid = %s;", 21, csSnapshotId); //�Ա�ʧ������ӵ�������Ϊ  21
					g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);


					CString csadd = csSnapshotId + "+" + strCurTime2;
					g_arFeatureMatch.Add(csadd);
				}
				else
				{
					nMatchNum = 0;
					CString strIDCard;
					strIDCard = content["person"]["idCard"].asCString();
					double dbSim;
					dbSim = content["similarity"].asDouble();
					CString csSim;
					csSim.Format("%f", dbSim);

					WriteTemporaryList(csSnapshotId, strIDCard, csSim, csCameraId, csAlarmtime, atoi(nCurrent));

					csSql.Format("UPDATE  snapshot set  process_status = '%d' where Snapshotid = %s;", 22, csSnapshotId);//�Ա���ʱ��ɹ�22
					g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
				}
			}
			delete[]featuresbuf;
			pRecordset->MoveNext();
		}
		
		//if (!bIsdelete && bMove)
		if (!bIsdelete)
		{
			csSql.Format("select * from snapshot where (process_status = 16 )and alarmtime >= '%s' and alarmtime <= '%s';", csStartTime, csEndTime);
			pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
			if (!pRecordset->adoEOF)
			{
				return TRUE;
			}

			DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "bIsdelete_Start_" + nCurrent);

			if (!m_bOpenDataMove)
			{
				if (!InitDatabaseMove())
					return FALSE;
			}
			CString csTemp;
			//�޸�һ����ڵ������ƶ� ���ؼ�¼���ƶ� ���������� libid �޸�
			if (!MoveComputerTemLis(nCurrent, "id_card", 5))
			{
				DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "MoveComputerTemLis_FALSE");
				return FALSE;
			}

			//ɾ��һ�����TE������
			if (!DeleteComputerIdcard(csStartTime, csEndTime))
			{
				csTemp.Format("DeleteComputerIdcard_FALSE_%s", csStartTime);
				DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csTemp);
			}
			//ɾ����������Ա������
			if (!DeleteIdcardImgInfo(csStartTime, csEndTime))
			{
				csTemp.Format("DeleteIdcardImgInfo _FALSE_%s", csStartTime);
				DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csTemp);
			}
			//�������ݱ�
			if (!CreateTableALL(nCurrent))
			{
				csTemp.Format("CreateTableALL _FALSE_%s", nCurrent);
				DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csTemp);
			}
			//�ƶ�������
			if (MoveBlacekLis(nCurrent) && MoveDynamicList(nCurrent) && MoveTemporaryList(nCurrent))
			{
				Sleep(700000);
				SetNextCurrentValue(atoi(nCurrent) + 1);//�¸����ڵĶԱ�ʧ����0  ���±ȶ�

				csSql.Format("UPDATE  cycleinfo set  ismigration= '1' where currentcycle = %s", nCurrent);
				g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
			}
			else
			{
				csTemp.Format("MoveALL _FALSE_%s", nCurrent);
				DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csTemp);
				return FALSE;
			}

			DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "bIsdelete _End");

			if (m_bOpenDataMove)
				CloseDatabaseMove();
		}
	}
	catch (...)
	{
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "ManageSnapshot _Catch");
		return FALSE;
	}
	return TRUE;

}
//�����¸������ڵ�16 ��ֵΪ0
void CVerdictProcessDlg::SetNextCurrentValue(int nCurrent)
{
	try
	{
		CString csSql, csStartTime, csEndTime;
		_RecordsetPtr pRecordset;

		csSql.Format("select * from cycleinfo WHERE currentcycle = '%d'", nCurrent);
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csSql);

		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		if (!pRecordset->adoEOF)
		{
			csStartTime = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("startdate")->Value;
			csEndTime = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("enddate")->Value;


			csSql.Format("select * from snapshot where process_status = %d and alarmtime >= '%s' and alarmtime <= '%s 23:59:59';", COMPAR_REEOR, csStartTime, csEndTime);
			pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
			while (!pRecordset->adoEOF)
			{
				CString csSnapshotId = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("Snapshotid")->Value;
				csSql.Format("UPDATE  snapshot set  process_status= '0' where Snapshotid = %s", csSnapshotId);
			//	DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csSql);

				g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
				pRecordset->MoveNext();
			}
		}
	}
	catch (...)
	{
	}
}
//��ȡemp������idcard
BOOL CVerdictProcessDlg::GetEmpIdcard(CString &csIdcard)
{
	try
	{
		CString csSql;
		_RecordsetPtr pRecordset;
		CString csTemp;

		//csSql = ("SELECT MAX(id_card) as id FROM employeeinfo");
		csSql = ("SELECT MAX(MaxIdCard) as id FROM parameterini WHERE Parameter = 'IdCard'");
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		if (!pRecordset->adoEOF)
		{
			_variant_t vt_t = pRecordset->GetFields()->GetItem("id")->Value;
			if (vt_t.vt == VT_NULL)
			{
				csIdcard.Format("%010d", 1);
			}
			else
			{
				csTemp = (TCHAR *)(_bstr_t)vt_t;
				csIdcard.Format("%010d", atoi(csTemp) + 1);
			}
		}
	}
	catch (...)
	{
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "GetEmpIdcard _Catch");
		return FALSE;
	}
	return TRUE;

}
//��ȡ�������ݽ��д���
BOOL CVerdictProcessDlg::VerdictSnapshot()
{
	try
	{
		int nCurrent;
		BOOL	bIsdelete;
		CString csSql, csStartTime, csEndTime, csCurrent;
		_RecordsetPtr pRecordset;
		CString  csMaxTime, csMaxCurrer;


		if (GetDbTableMaxItem("alarmtime", "snapshot", csMaxTime))
		{
			COleDateTime StartTime;
			int y, m, d, h, u, s;
			sscanf(csMaxTime, ("%d/%d/%d %d:%d:%d"), &y, &m, &d, &h, &u, &s);
			StartTime.SetDateTime(y, m, d, h, u, s);
			CString csMaxTimeTemp;
			csMaxTimeTemp = StartTime.Format("%Y-%m-%d");


			while (!IsCycleInner(csMaxTimeTemp, nCurrent))
			{
				SetCycleNext();
			}

			if (GetDbTableMaxItem("cast(currentcycle as SIGNED INTEGER)", "cycleinfo", csMaxCurrer))
			{
				int nmaxWhile = 0;
				csSql = "select * from cycleinfo WHERE ismigration = 0 order by id limit 0,1";
				pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
				if (!pRecordset->adoEOF)
				{
					csStartTime = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("startdate")->Value;
					csEndTime = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("enddate")->Value;
					csCurrent = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("currentcycle")->Value;
					bIsdelete = csCurrent == csMaxCurrer ? TRUE : FALSE;
					if (m_csCurrent != csCurrent)
					{
						m_csCurrent = csCurrent;
						g_arFeatureMatch.RemoveAll();
					//	DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, m_csCurrent);
					}

					if (ManageSnapshot(csStartTime, csEndTime, csCurrent, bIsdelete))
					{
					}
				}
				return TRUE;
			}
		}
	}
	catch (...)
	{
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "VerdictSnapshot _Catch");
	}
	return FALSE;
}

//�������б��ݱ�
BOOL CVerdictProcessDlg::CreateTableALL(CString csCurrentCycle)
{
	try
	{
		CString csSql, csTable;
		_RecordsetPtr pRecordset;

		csSql.Format("select * from blacklist where cyclnum = '%s';", csCurrentCycle);
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		if (!pRecordset->adoEOF)
		{
			csTable.Format("blacklist_%s", csCurrentCycle);
			if (!CreateTable(csTable))
			{
				return FALSE;
			}
		}


		csSql.Format("select * from dynamic where cyclnum ='%s';", csCurrentCycle);
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		if (!pRecordset->adoEOF)
		{
			csTable.Format("dynamic_%s", csCurrentCycle);
			if (!CreateTable(csTable))
			{
				return FALSE;
			}
		}

		csSql.Format("select * from temporary where cyclnum = '%s';", csCurrentCycle);
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		if (!pRecordset->adoEOF)
		{
			csTable.Format("temporary_%s", csCurrentCycle);
			if (!CreateTable(csTable))
			{
				return FALSE;
			}
		}
		return TRUE;
	}
	catch (...)
	{
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "CreateTableALL _Catch");
	}
	return FALSE;
}

//������
BOOL CVerdictProcessDlg::CreateTable(CString csTable)
{
	try
	{
		CString csSql;
		csSql.Format("CREATE TABLE if not exists %s(id int(11) primary key auto_increment not null ,snapshotid varchar(255) not null ,id_card varchar(255) not null, similarity varchar(10) not null, alarmtime datetime not null, cyclnum varchar(50) not null, cameraid varchar(10) not null); ", csTable);
		g_pConnectMain->Execute(_bstr_t(csSql), NULL, adCmdText);

		return TRUE;
	}
	catch (...)
	{
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "CreateTable _Catch");
	}
	return FALSE;
}

//�ƶ��������ݿ� ������
BOOL CVerdictProcessDlg::MoveBlacekLis(CString csCurrentCycle)
{
	try
	{
		CString csSql, csTable;
		_RecordsetPtr pRecordset;

		csSql.Format("select * from blacklist where cyclnum = '%s';", csCurrentCycle);
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		csTable.Format("blacklist_%s", csCurrentCycle);
		while (!pRecordset->adoEOF)
		{
			csSql.Format("insert into %s(snapshotid, id_card, similarity,alarmtime, cameraid, cyclnum)values('%s','%s','%s','%s','%s','%s')",
				csTable,
				(TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("snapshotid")->Value, (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("id_card")->Value,
				(TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("similarity")->Value, (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("alarmtime")->Value,
				(TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("cameraid")->Value, (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("cyclnum")->Value);
			g_pConnectMain->Execute(_bstr_t(csSql), NULL, adCmdText);

			csSql.Format("delete from blacklist where id = %s and snapshotid = '%s' ;", (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("id")->Value, (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("snapshotid")->Value);
			g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);

			pRecordset->MoveNext();
		}
		return TRUE;
	}
	catch (...)
	{
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "MoveBlacekLis _Catch");
	}
	return FALSE;
}

//�ƶ��������ݿ� ��̬����
BOOL CVerdictProcessDlg::MoveDynamicList(CString csCurrentCycle)
{
	try
	{
		CString csSql, csTable;
		_RecordsetPtr pRecordset;

		csSql.Format("select * from dynamic where cyclnum = '%s';", csCurrentCycle);
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		csTable.Format("dynamic_%s", csCurrentCycle);
		while (!pRecordset->adoEOF)
		{
			csSql.Format("insert into %s(snapshotid, id_card, similarity,alarmtime, cameraid, cyclnum)values('%s','%s','%s','%s','%s','%s')",
				csTable,
				(TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("snapshotid")->Value, (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("id_card")->Value,
				(TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("similarity")->Value, (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("alarmtime")->Value,
				(TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("cameraid")->Value, (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("cyclnum")->Value);
			g_pConnectMain->Execute(_bstr_t(csSql), NULL, adCmdText);

			csSql.Format("delete from dynamic where id = %s and snapshotid = '%s';", (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("id")->Value, (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("snapshotid")->Value);
			g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);

			pRecordset->MoveNext();
		}
		return TRUE;
	}
	catch (...)
	{
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "MoveDynamicList _Catch");
	}
	return FALSE;

}

//�ƶ��������ݿ� ��ʱ��
BOOL CVerdictProcessDlg::MoveTemporaryList(CString csCurrentCycle)
{
	try
	{
		CString csSql, csTable;
		_RecordsetPtr pRecordset;
		csSql.Format("select * from temporary where cyclnum = '%s';", csCurrentCycle);
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		csTable.Format("temporary_%s", csCurrentCycle);
		while (!pRecordset->adoEOF)
		{
			csSql.Format("insert into %s(snapshotid, id_card, similarity,alarmtime, cameraid, cyclnum)values('%s','%s','%s','%s','%s','%s')",
				csTable,
				(TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("snapshotid")->Value, (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("id_card")->Value,
				(TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("similarity")->Value, (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("alarmtime")->Value,
				(TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("cameraid")->Value, (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("cyclnum")->Value);
			g_pConnectMain->Execute(_bstr_t(csSql), NULL, adCmdText);

			csSql.Format("delete from temporary where id = %s and snapshotid = '%s';", (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("id")->Value, (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("snapshotid")->Value);
			g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);

			pRecordset->MoveNext();
		}
		return TRUE;
	}
	catch (...)
	{
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "MoveTemporaryList _Catch");
	}
	return FALSE;
}

//ɾ����������Ա������
BOOL CVerdictProcessDlg::DeleteIdcardImgInfo(CString csStartTime, CString csEndTime)
{
	try
	{
		CString csSql, csIdcard;
		_RecordsetPtr pRecordset;

		COleDateTime StartTime;
		int y, m, d, s, f, mm;
		sscanf(csEndTime, ("%d-%d-%d %d:%d:%d"), &y, &m, &d, &s, &f, &mm);
		StartTime.SetDateTime(y, m, d + 1, 0, 0, 0);
		CString csMaxTimeTemp;
		csEndTime = StartTime.Format("%Y-%m-%d");
	/*	csSql.Format("SELECT * FROM employeeinfo WHERE employeeinfo.id_card in (SELECT temporary.id_card FROM `temporary` WHERE  temporary.alarmtime >='%s'  and temporary.alarmtime <'%s 00:00:00' and employeeinfo.libraryid = '4')", csStartTime, csEndTime);*/

		csSql.Format("select * from employeeinfo as a join `temporary`  as b on a.id_card = b.id_card  where a.libraryid = '4' and (b.alarmtime between '%s 00:00:00' and '%s 00:00:00')", csStartTime, csEndTime);
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		while (!pRecordset->adoEOF)
		{
			csIdcard = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("id_card")->Value;

			if (!DeleteEmploy(csIdcard))
			{
				DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "DeleteEmploy_FALSE");
				return FALSE;
			}

			if (!DeleteImg(csIdcard))
			{
				DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "DeleteImg_FALSE");
				return FALSE;
			}

			pRecordset->MoveNext();
		}

	}
	catch (...)
	{
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "DeleteIdcardImgInfo _Catch");
		return FALSE;
	}
	return TRUE;
}

//���emp���ڵ�����
BOOL CVerdictProcessDlg::DeleteEmploy(CString csIdcdrd)
{
	try
	{
		CString csSql;
		_RecordsetPtr pRecordset;

		csSql.Format("delete  FROM employeeinfo WHERE id_card = '%s';", csIdcdrd);
		g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);

		return TRUE;
	}
	catch (...)
	{
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "DeleteEmploy _Catch");
	}
	return FALSE;
}

//���img���ڵ�idcard����
BOOL CVerdictProcessDlg::DeleteImg(CString csIdcdrd)
{
	try
	{
		CString csSql;
		_RecordsetPtr pRecordset;

		csSql.Format("delete FROM imginfo WHERE id_card = '%s';", csIdcdrd);
		g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		return TRUE;
	}
	catch (...)
	{
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "DeleteEmploy _Catch");
	}
	return FALSE;
}

//���devic ����ʱ������
BOOL CVerdictProcessDlg::DeleteComputerIdcard(CString csStartTime, CString csEndTime)
{
	try
	{
		char pSession[37];
		char chJson[1024 * 1024 * 2];

		CString csSql, csPersonid;
		_RecordsetPtr pRecordset;
		COleDateTime StartTime;
		int y, m, d, s, f, mm;
		sscanf(csEndTime, ("%d-%d-%d %d:%d:%d"), &y, &m, &d, &s, &f, &mm);
		StartTime.SetDateTime(y, m, d + 1, 0, 0, 0);
		CString csMaxTimeTemp;
		csEndTime = StartTime.Format("%Y-%m-%d");
		/*csSql.Format("SELECT * FROM employeeinfo WHERE employeeinfo.id_card in (SELECT temporary.id_card FROM `temporary` WHERE  temporary.alarmtime >='%s'  and temporary.alarmtime <'%s 00:00:00')", csStartTime, csEndTime);*/
		csSql.Format("select personid from employeeinfo as a join `temporary`  as b on a.id_card = b.id_card  where b.alarmtime between '%s 00:00:00' and ' %s 00:00:00'", csStartTime, csEndTime);
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		if (!pRecordset->adoEOF)
		{
			for (int nDeviceIndex = 0; nDeviceIndex < g_DeviceSum; nDeviceIndex++)
			{
				memset(chJson, 0, 1024 * 1024 * 2);
				int nResult = DeviceLogin(g_DeviceInfo[nDeviceIndex].csDeviceIp, pSession);
				if (nResult != 0)
				{
					DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "DeviceLogin_FALSE");
					return FALSE;
				}

				while (!pRecordset->adoEOF)
				{
					csPersonid = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("personid")->Value;
					nResult = FASDELETEPersoninfo(g_DeviceInfo[nDeviceIndex].csDeviceIp, pSession, csPersonid);
					if (nResult != 0)
					{
						DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "FASDELETEPersoninfo_FALSE");
						DeviceLogout(g_DeviceInfo[nDeviceIndex].csDeviceIp, pSession);
						return FALSE;
					}

					pRecordset->MoveNext();
				}
				DeviceLogout(g_DeviceInfo[nDeviceIndex].csDeviceIp, pSession);
			}
		}

		return TRUE;
	}
	catch (...)
	{
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "DeleteComputerIdcard _Catch");
	}
	return FALSE;
}

//ͨ��idcard��ȡsnapid��path
BOOL CVerdictProcessDlg::OnIdCardGetSnpId(CString csIdcard, CString &csSnapid, CString &csPath)
{
	CString csSql;
	try
	{
		_RecordsetPtr pRecordset;
		csSql.Format("select * from `snapshot` where Snapshotid=(SELECT snapshotid FROM `temporary` WHERE id_card = '%s' and similarity = '0')", csIdcard);

		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		if (!pRecordset->adoEOF)
		{
			csSnapid = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("Snapshotid")->Value;
			csPath = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("FacePhotoName")->Value;
		//	DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csSql);
		//	DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csSnapid);
			return TRUE;
		}

	}
	catch (...)
	{
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "OnIdCardGetSnpId _Catch");
	}
	return FALSE;
}

//�޸�һ�������ʱ��ǰ�������ݵ���̬������ ͬʱ�ƶ��������ݿ������
BOOL CVerdictProcessDlg::MoveComputerTemLis(CString csCurrentCycle, CString csItem, int nTop)
{
	try
	{
		CString csSql,csTemp;
		_RecordsetPtr pRecordset;
	//	csSql.Format(_T("select *,count(*) as num  from temporary where  cyclnum = '%s'  group by %s order by num desc limit %d;"), csCurrentCycle, csItem, nTop);
		csSql.Format(_T("select *,count(*) as num  from temporary where  cyclnum = '%s'  group by %s order by num desc;"), csCurrentCycle, csItem);
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csSql);

		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		while (!pRecordset->adoEOF)
		{
			//�޸�һ������ݿ�
			CString csNum = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("num")->Value;
			CString csIdcerd = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("id_card")->Value;


			//���ִ����ж�
			if (atoi(csNum) < atoi(m_csAppearNum))
			{
				pRecordset->MoveNext();
				continue;
			}


			//�����ж�
			if (!IsAgeCorrect(atoi(csNum), csIdcerd, csCurrentCycle))
			{
				//DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "IsAgeCorrect" + csIdcerd + csCurrentCycle);
				pRecordset->MoveNext();
				continue;
			}

			DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "IsAgeCorrect" + csIdcerd);

			char pSession[37];
			char chJson[1024 * 1024 * 2];

			int intGender = 0;
			CString strBirthday = "1990-01-01";
			CString strValidTo = "2090-01-01";
			char Personid[126];
			char chImage[2] = { '1', '\0' };
			CString csImgPath, csSnapshotId;

			if (!OnIdCardGetSnpId(csIdcerd, csSnapshotId, csImgPath))
			{
				csTemp.Format("OnIdCardGetSnpId_Error_%s", csIdcerd);
				DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csTemp);
				pRecordset->MoveNext();
				//continue;
			}
			//��ȡfeature ·��
			CString csFeaturesPath = csImgPath;
			csFeaturesPath.Replace("imgCurFaceJpg", "imgCurFaceFeature");
			csFeaturesPath.Replace(".jpg", ".txt");

 			CFile file;
 			if (!file.Open(csFeaturesPath, CFile::modeRead))
 			{
 				csSql.Format("UPDATE  snapshot set  process_status = '%d' where Snapshotid = %s;", 23, csSnapshotId);//��ȡ�ļ�ʧ��23
 				g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
				csTemp.Format("CFile _Error_%s", csIdcerd);
				DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csTemp);
				pRecordset->MoveNext();
 				//continue;
 			}

 			int iFileLen = file.GetLength();
			char *featuresbuf = new char[iFileLen + 1];
 			memset(featuresbuf, 0, iFileLen + 1);
 			file.Read(featuresbuf, iFileLen);
 			file.Close();
 		//	CString strFeatures(featuresbuf);

 			for (int nDeviceIndex = 0; nDeviceIndex < g_DeviceSum; nDeviceIndex++)
 			{
 				memset(chJson, 0, 1024 * 1024 * 2);
 				CString csPersonid;
 				if (!GetPersonId(csIdcerd, csPersonid))
 				{
					csTemp.Format("GetPersonId _Error_%s", csIdcerd);
					DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csTemp);
					delete[] featuresbuf;
 					return FALSE;
 				}
				DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "IsAgeCorrect11" + csPersonid);

 				int nResult = DeviceLogin(g_DeviceInfo[nDeviceIndex].csDeviceIp, pSession);
 				if (nResult != 0)
 				{
 					DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "DeviceLogin_FALSE");
					delete[] featuresbuf;
 					return FALSE;
 				}
 
 				nResult = FASDELETEPersoninfo(g_DeviceInfo[nDeviceIndex].csDeviceIp, pSession, csPersonid);
 				if (nResult != 0)
 				{
					csTemp.Format("MoveComputerTemLis_FASDELETEPersoninfo _Error_%s", csIdcerd);
					DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csTemp);
 					delete[] featuresbuf;
 					DeviceLogout(g_DeviceInfo[nDeviceIndex].csDeviceIp, pSession);
 					return FALSE;
 				}

 				//ע��
 				nResult = FASAddPersoninfo(g_DeviceInfo[nDeviceIndex].csDeviceIp, pSession, csIdcerd, csIdcerd, intGender, atoi(g_DeviceInfo[nDeviceIndex].csFacedbDynamicList),
					featuresbuf, chImage, strBirthday, strValidTo, Personid, 126);
 				if (nResult != 0)
 				{
					csTemp.Format("MoveComputerTemLis_FASAddPersoninfo _Error_%s", csIdcerd);
					DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csTemp);
 					delete[] featuresbuf;
 					DeviceLogout(g_DeviceInfo[nDeviceIndex].csDeviceIp, pSession);

 					return FALSE;
 				}
				DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "IsAgeCorrect222");

			//	FASUpdatePersoninfo(g_DeviceInfo[nDeviceIndex].csDeviceIp, pSession, Personid, csIdcerd, atoi(g_DeviceInfo[nDeviceIndex].csFacedbDynamicList));
 				csSql.Format("UPDATE  employeeinfo set  personid = '%s' where id_card = %s;", Personid, csIdcerd);
 				g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
 
 				DeviceLogout(g_DeviceInfo[nDeviceIndex].csDeviceIp, pSession);
 			}
			delete[] featuresbuf;
			//�Ƽ�¼
			if (!MoveTempListToDynList(csCurrentCycle, csIdcerd))
			{
				csTemp.Format("MoveTempListToDynList _Error_%s", csIdcerd);
				DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csTemp);
				return FALSE;
			}
			//����Ա��
			if (!MoveTempEmpToDynEmp(csIdcerd))
			{
				csTemp.Format("MoveTempEmpToDynEmp _Error_%s", csIdcerd);
				DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csTemp);
				return FALSE;
			}
			csTemp.Format("MoveTempEmpToDynEmp");
			pRecordset->MoveNext();
		}//while end

	}
	catch (...)
	{
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "MoveComputerTemLis _Catch");
		return FALSE;
	}
	return TRUE;
}

BOOL CVerdictProcessDlg::IsAgeCorrect(int nMaxNum, CString csIdcard, CString csCurrect)
{
	try
	{
		CString csSql, csAge;
		_RecordsetPtr pRecordset;
		int nNum = 0;
	//	csSql.Format("select * from `snapshot` where Snapshotid in (SELECT snapshotid FROM `temporary` WHERE id_card = '%s' and cyclnum = '%s');", csIdcard, csCurrect);
		csSql.Format("select a.age from `snapshot` as a join `temporary` as b on a.Snapshotid = b.snapshotid where b.cyclnum = '%s' and b.id_card = '%s'; ", csCurrect, csIdcard);

		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		while (!pRecordset->adoEOF)
		{
			csAge = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("age")->Value;

			if (atoi(csAge) < atoi(m_csAge))
			{
				nNum++;
			}
			pRecordset->MoveNext();
		}
		double dResults = (double)nNum / (double)nMaxNum;
		if (dResults > atof(m_csAgeResults))
		{
			return TRUE;
		}
	}
	catch (...)
	{
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "IsAgeCorrect _Catch");
	}
	return FALSE;
}

//�ƶ�������ʱ�����ݵ���̬������
BOOL CVerdictProcessDlg::MoveTempListToDynList(CString csCurrentCycle, CString csIdcard)
{
	try
	{
		CString csSql, csTable;
		_RecordsetPtr pRecordset;

		csSql.Format("select * from temporary where cyclnum = '%s' and id_card = '%s';", csCurrentCycle, csIdcard);
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "MoveTempListToDynList_"+csSql);
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		while (!pRecordset->adoEOF)
		{
			csSql.Format("insert into dynamic(snapshotid, id_card, similarity,alarmtime, cameraid, cyclnum)values('%s','%s','%s','%s','%s','%s')",
				(TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("snapshotid")->Value, (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("id_card")->Value,
				(TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("similarity")->Value, (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("alarmtime")->Value,
				(TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("cameraid")->Value, (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("cyclnum")->Value);
			g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		
			///������ʷ��¼����
			csSql.Format("delete from temporary where id = %s and snapshotid = '%s';", (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("id")->Value, (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("snapshotid")->Value);
			g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);

			pRecordset->MoveNext();
		}
		return TRUE;
	}
	catch (...)
	{
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "MoveTempListToDynList_Catch");
	}
	return FALSE;
}
//
BOOL CVerdictProcessDlg::MoveTempEmpToDynEmp(CString csIdcard)
{
	try
	{
		CString csSql;
		csSql.Format("UPDATE  employeeinfo set libraryid = '3' WHERE id_card ='%s'", csIdcard);
		g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		return TRUE;
	}
	catch (...)
	{
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "MoveTempEmpToDynEmp_Catch");
	}
	return FALSE;
}
//
BOOL CVerdictProcessDlg::GetPersonId(CString  csIdcard, CString  &csPossonId)
{
	try
	{
		CString csSql;
		_RecordsetPtr pRecordset;
		csSql.Format("SELECT * FROM employeeinfo WHERE id_card = '%s'", csIdcard);
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		if (!pRecordset->adoEOF)
		{
			csPossonId = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("personid")->Value;
			return TRUE;
		}
	}
	catch (...)
	{
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, "GetPersonId_Catch");
	}
	return FALSE;
}
//
void CVerdictProcessDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	g_bMaintenancWarning = FALSE;
	WaitForSingleObject(g_MaintenanceWarninfo->m_hThread, INFINITE);

	g_bMaintenancSnap = FALSE;
	WaitForSingleObject(g_MaintenanceSnapsho->m_hThread, INFINITE);
	// TODO:  �ڴ˴������Ϣ����������
}


UINT HeartbeatThread(LPVOID lpam)
{
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