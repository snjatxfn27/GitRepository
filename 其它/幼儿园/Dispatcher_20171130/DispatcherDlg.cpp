
// DispatcherDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Dispatcher.h"
#include "DispatcherDlg.h"
#include "afxdialogex.h"
#include <Mmsystem.h>
static CStringArray g_arCameraId;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define UDP_SEND_PORT 30000  //udp�˿�
#define  PROGRAM_INDEX   0		//����ID
#define  SLEEP_TIME   30000		//�������

UINT MainThread(LPVOID pParam);
UINT HeartbeatThread(LPVOID lpam);

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


// CDispatcherDlg �Ի���



CDispatcherDlg::CDispatcherDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDispatcherDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDispatcherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TOP, m_staticTop);
	DDX_Control(pDX, IDC_STATIC_SIM, m_staticSim);
	DDX_Control(pDX, IDC_LIST1, m_listOutput);
}

BEGIN_MESSAGE_MAP(CDispatcherDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CDispatcherDlg ��Ϣ�������

BOOL CDispatcherDlg::OnInitDialog()
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
	CString strTop,strSim;
	strTop.Format(L"���ݿ�IP��%s\t���ݿ�����%s", theApp.m_strDBIP, theApp.m_strDBName);
	strSim.Format(L"��ǰʹ�õĶԱ����ƶ���ֵΪ��%.2f", theApp.m_dbTotalSim);
	m_staticTop.SetWindowTextW(strTop);
	m_staticSim.SetWindowTextW(strSim);
	m_listOutput.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);// | LVS_EX_TRACKSELECT);
	m_listOutput.InsertColumn(0, L"info", LVCFMT_CENTER, 650);
	m_listOutput.InsertColumn(1, L"time", LVCFMT_CENTER, 160);
	int i = 0;
	for (i = 0; i < 13; i++)
	{
		m_strOutput[i].Empty();
		m_strTime[i].Empty();
		m_listOutput.InsertItem(i, m_strOutput[i]);
		m_listOutput.SetItemText(i, 1, m_strTime[i]);
	}

	m_bQuit = false;
	m_bConnDisconnect = true;
	AfxBeginThread((AFX_THREADPROC)MainThread, (void*)this);
	AfxBeginThread((AFX_THREADPROC)HeartbeatThread, (void*)this);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CDispatcherDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDispatcherDlg::OnPaint()
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
HCURSOR CDispatcherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

UINT MainThread(LPVOID pParam)
{
	AfxOleInit();
	USES_CONVERSION;
	CDispatcherDlg *pWnd = (CDispatcherDlg*)pParam;
	pWnd->PutInfoToList(L"���봦�����߳�");
	pWnd->conn.CreateInstance(__uuidof(Connection));
	CString strBox;
	pWnd->m_bLogIn = TRUE;
	CString strSim;
	while (!pWnd->m_bQuit)
	{
		if (!pWnd->ConnectDB())
		{
			Sleep(5000);
			continue;
		}
		while (!pWnd->m_bConnDisconnect)
		{
			if (pWnd->m_bLogIn)
			{
				memset(pWnd->chSession, 0, 2048);
				int nReturn = DeviceLogin(T2A(theApp.m_strDetectServerIP), pWnd->chSession);

				if (nReturn != 0)
				{
					strBox.Format(L"��ȡSessionʧ�ܣ�����ֵΪ��%d", nReturn);
					pWnd->PutInfoToList(strBox);
					continue;
				}
				CString strS;
				strS = pWnd->chSession;
				strBox.Format(L"��ȡSession�ɹ���SessionΪ��%s", strS);
				pWnd->PutInfoToList(strBox);
				pWnd->m_bLogIn = FALSE;
			}
			//�����ݿ��л�ȡ���ƶȣ����±���ȫ�ֱ���
			strSim = pWnd->GetSimFromDB();
			double dbCurSim;
			dbCurSim = _wtof(strSim);
			if (theApp.m_dbTotalSim != dbCurSim)
			{
				theApp.m_dbTotalSim = dbCurSim;
				CString strSimDis;
				strSimDis.Format(L"��ǰʹ�õĶԱ����ƶ���ֵΪ��%.2f", theApp.m_dbTotalSim);
				pWnd->m_staticSim.SetWindowTextW(strSimDis);
			}
			//�����ݿ��л�ȡ�ڰ������⣬���±���ȫ�ֱ���
			pWnd->UpdateFaceDBsFromDB();
			pWnd->UpdateCameraIDFromDB();
			//��snapshot���ж�ȡһ��δ���������
			int iSnapshotid;
			CString strFacePhotoName, strFullPhotoName;
			if (!pWnd->GetOneSnapshotFromDB(iSnapshotid, strFacePhotoName, strFullPhotoName))
			{
				Sleep(2000);
				continue;
			}
			int re = 0;
			int iAge = 0;
			int iGender = 0;
			re = pWnd->ProcessOneSnapData(iSnapshotid, strFacePhotoName, strFullPhotoName, iAge, iGender);
			strBox.Format(L"������ɣ�����ֵΪ��%d", re);
			pWnd->PutInfoToList(strBox);
			if (re == 0)
			{
				pWnd->UpdateProcessStatusToDB(iSnapshotid, 3,iAge,iGender);
				if (pWnd->IsWarnig(iSnapshotid))
				{
					pWnd->SetVoiceQueueWarnig(L"1",iSnapshotid);
				}
			}
			else
			{
				pWnd->UpdateProcessStatusToDB(iSnapshotid, re + 10, iAge, iGender);
				if (pWnd->IsWarnig(iSnapshotid))
				{
					pWnd->SetVoiceQueueWarnig(L"0", iSnapshotid);
				}
			}
		}
	}
	return 1;
}

//0;�Աȱ��У�1:��¼����ȡdllʧ�ܣ�2:json���ش���3:û����ȡ��������4:ƥ��dllʧ�ܣ�5:ƥ��json����6:û�б��У�7:�����ݿ�ʧ��
int CDispatcherDlg::ProcessOneSnapData(int iSnapshotid, CString strFacePhotoName, CString strFullPhotoName,int &iAge,int &iGender)
{
	USES_CONVERSION;
	CString strBox;
	int iRe = 0;
	char chJson[1024 * 1024];
	memset(chJson, 0, 1024 * 1024);
	iRe = FASFeatureDetectforImageFile(T2A(theApp.m_strDetectServerIP), chSession, T2A(strFacePhotoName), chJson, 1024 * 1024);
	if(iRe != 0)
	{
		strBox.Format(L"FASFeatureDetectforImageFile����%d", iRe);
		PutInfoToList(strBox);
		m_bLogIn = TRUE;
		return 1;
	}

	Json::Reader reader;
	Json::Value root;
	Json::Value result;
	Json::Value content;
	BOOL readre = reader.parse(chJson, root);
	if (!readre)
	{
		m_bLogIn = TRUE;
//		DeviceLogout(T2A(theApp.m_strDetectServerIP), chSession);
		return 2;
	}
	int iZero = 0;
	result = root["data"]["result"][iZero];
	if (result.isNull())
	{
		return 3;
	}
	else
	{
		CString strFeature;
		strFeature = result["feature"].asCString();
		iAge = result["age"].asInt();
		iGender = result["gender"].asInt();
		CString strB64FileName = strFacePhotoName;
		strB64FileName.Replace(L"imgCurFaceJpg", L"imgCurFaceFeature");
		strB64FileName.Delete(strB64FileName.GetLength() - 3, 3);
		strB64FileName += L"txt";
		try{
			CFile file;
			file.Open(strB64FileName, CFile::modeCreate | CFile::modeWrite);
			file.Write(T2A(strFeature), strFeature.GetLength());
			file.Close();
		}
		catch (_com_error e)
		{
			AfxMessageBox(L"������д�����ļ�ʧ��");
		}
		//�ȶ԰�����
		CString strWhiteDBs;
		strWhiteDBs = theApp.m_strTotalWhiteDBIDs;
		strWhiteDBs.Replace(L"+", L",");
		memset(chJson, 0, 1024 * 1024);
		iRe = FASMatchPerson(T2A(theApp.m_strDetectServerIP), chSession, T2A(strWhiteDBs), theApp.m_dbTotalSim, T2A(strFeature), chJson, 1024 * 1024);
		if(iRe != 0)
		{
			strBox.Format(L"FASMatchPerson����%d", iRe);
			PutInfoToList(strBox);
			m_bLogIn = TRUE;
			return 4;
		}
		readre = reader.parse(chJson, root);
		if (!readre)
		{
			m_bLogIn = TRUE;
			return 5;
		}
		content = root["data"]["content"][iZero];
		if (content.isNull())
		{
			//�ȶԺ�����
			CString strBlackDBs;
			strBlackDBs = theApp.m_strTotalBlackDBIDs;
			strBlackDBs.Replace(L"+", L",");
			memset(chJson, 0, 1024 * 1024);
			iRe = FASMatchPerson(T2A(theApp.m_strDetectServerIP), chSession, T2A(strBlackDBs), theApp.m_dbTotalSim, T2A(strFeature), chJson, 1024 * 1024);
			if (iRe != 0)
			{
				strBox.Format(L"FASMatchPerson����%d", iRe);
				PutInfoToList(strBox);
				m_bLogIn = TRUE;
				return 4;
			}
			readre = reader.parse(chJson, root);
			if (!readre)
			{
				m_bLogIn = TRUE;
				return 5;
			}
			content = root["data"]["content"][iZero];
			if (content.isNull())
			{
				return 6;
			}
			CString strIDCard, strFaceDB;
			strIDCard = content["person"]["idCard"].asCString();
			double dbSim;
			dbSim = content["similarity"].asDouble();
			CString strSnapshotId, strSim;
			strSnapshotId.Format(L"%d", iSnapshotid);
			strSim.Format(L"%.4f", dbSim);
			int iFaceDB;
			iFaceDB = content["person"]["facedb"]["id"].asInt();
			strFaceDB.Format(L"%d", iFaceDB);
			if (!InsertAlarmToDB(strSnapshotId, strIDCard, strSim, strFaceDB))
			{
				return 7;
			}
			return 0;
		}
		CString strIDCard,strFaceDB;
		strIDCard = content["person"]["idCard"].asCString();
		double dbSim;
		dbSim = content["similarity"].asDouble();
		CString strSnapshotId, strSim;
		strSnapshotId.Format(L"%d", iSnapshotid);
		strSim.Format(L"%.4f", dbSim);
		int iFaceDB;
		iFaceDB = content["person"]["facedb"]["id"].asInt();
		strFaceDB.Format(L"%d", iFaceDB);
		if (!InsertAlarmToDB(strSnapshotId, strIDCard, strSim,strFaceDB))
		{
			return 7;
		}
		return 0;
	}
}
void CDispatcherDlg::PutInfoToList(CString strInfo)
{
	SYSTEMTIME time;
	GetLocalTime(&time);
	CString strTime;
	strTime.Format(L"%02d-%02d-%02d %02d:%02d:%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	int i;
	for (i = 0; i < 12; i++)
	{
		m_strOutput[i] = m_strOutput[i + 1];
		m_strTime[i] = m_strTime[i + 1];
	}
	m_strOutput[12] = strInfo;
	m_strTime[12] = strTime;
	for (i = 0; i<13; i++)
	{
		m_listOutput.SetItemText(i, 0, m_strOutput[i]);
		m_listOutput.SetItemText(i, 1, m_strTime[i]);
	}
}

CString CDispatcherDlg::GetSimFromDB()
{
	CString strSim = L"";
	if (!ConnectDB())
		return strSim;
	_RecordsetPtr pSet;
	pSet.CreateInstance(__uuidof(Recordset));
	try{
		CString strSQL = _T("select CameraSimilarity from parameterini where Parameter='computer'");
		pSet = conn->Execute(_bstr_t(strSQL), NULL, adCmdText);
		strSim = (char*)_bstr_t(pSet->GetCollect(_variant_t("CameraSimilarity")));
		pSet->Close();
		pSet.Release();
	}
	catch (_com_error e)
	{
		AfxMessageBox(L"ȡ�����ƶ�ʧ��");
	}
	return strSim;
}

BOOL CDispatcherDlg::UpdateCameraIDFromDB()
{
	if (!ConnectDB())
		return false;
	_RecordsetPtr pSet;
	pSet.CreateInstance(__uuidof(Recordset));

	try{
		g_arCameraId.RemoveAll();

		CString strSQL = _T("select * from Camera where Camera_Type = '2';");
		pSet = conn->Execute(_bstr_t(strSQL), NULL, adCmdText);
		//AfxMessageBox(L"1");
		while (!pSet->adoEOF)
		{
			g_arCameraId.Add((TCHAR *)(_bstr_t)pSet->GetFields()->GetItem("CameraId")->Value);
		//	AfxMessageBox(L"2");

			pSet->MoveNext();
		}
		//AfxMessageBox(L"3");

		pSet->Close();
		pSet.Release();
	}
	catch (_com_error e)
	{
		AfxMessageBox(L"��ȡ�����ʧ��");
		return false;
	}
	return true;
}

BOOL CDispatcherDlg::IsWarnig(int iSnapshotid)
{
	try
	{
		if (!ConnectDB())
			return FALSE;
		//AfxMessageBox(L"4");
		_RecordsetPtr pSet;
		pSet.CreateInstance(__uuidof(Recordset));
		CString strSQL, csCameraId;
		strSQL.Format(_T("select * from Snapshot where SnapshotId = %d"), iSnapshotid);

		pSet = conn->Execute(_bstr_t(strSQL), NULL, adCmdText);
	//	AfxMessageBox(L"5");
		if (!pSet->adoEOF)
		{
			csCameraId = (TCHAR *)(_bstr_t)pSet->GetFields()->GetItem("CameraId")->Value;
			if (g_arCameraId.GetSize() == 0)
			{
				return FALSE;
			}

			for (int i = 0; i < g_arCameraId.GetSize(); i++)
			{
				if (g_arCameraId.GetAt(i) == csCameraId)
				{
					return TRUE;
				}
			}
		}
		//AfxMessageBox(L"6");
	}
	catch (CException* e)
	{
		return FALSE;
	}
	return FALSE;
}
BOOL CDispatcherDlg::SetVoiceQueueWarnig(CString csType,int nSnapId)
{
	try
	{
		if (!ConnectDB())
			return FALSE;

		_RecordsetPtr pSet;
		pSet.CreateInstance(__uuidof(Recordset));
		//AfxMessageBox(L"7");
		CString strSQL; 
		strSQL.Format(_T("select * from VoiceQueue where Type ='%s' and SnapShotId = '%d'"), csType, nSnapId);
		pSet = conn->Execute(_bstr_t(strSQL), NULL, adCmdText);
		//AfxMessageBox(L"8");
		if (pSet->adoEOF)
		{
			strSQL.Format(_T("INSERT INTO VoiceQueue ([Type],SnapShotId)values('%s','%d')"), csType, nSnapId);
			conn->Execute(_bstr_t(strSQL), NULL, adCmdText);
		}
	//	AfxMessageBox(L"9");

	}
	catch (CException* e)
	{
		return FALSE;
	}
	return FALSE;
}

BOOL CDispatcherDlg::UpdateFaceDBsFromDB()
{
	if (!ConnectDB())
		return false;
	_RecordsetPtr pSet;
	pSet.CreateInstance(__uuidof(Recordset));
	try{
		CString strSQL = _T("select BlackFaceDbs,WhiteFaceDbs from device where delflag=0 and IsFaceServer=1");
		pSet = conn->Execute(_bstr_t(strSQL), NULL, adCmdText);
		theApp.m_strTotalBlackDBIDs = (char*)_bstr_t(pSet->GetCollect(_variant_t("BlackFaceDbs")));
		theApp.m_strTotalWhiteDBIDs = (char*)_bstr_t(pSet->GetCollect(_variant_t("WhiteFaceDbs")));
		pSet->Close();
		pSet.Release();
	}
	catch (_com_error e)
	{
		AfxMessageBox(L"ȡ���ڰ�������ʧ��");
		return false;
	}
	return true;
}

BOOL CDispatcherDlg::GetOneSnapshotFromDB(int &iSnapshotid, CString &strFacePhotoName, CString &strFullPhotoName)
{
	CString strBox;
	if (!ConnectDB())
		return false;
	_RecordsetPtr pSet;
	pSet.CreateInstance(__uuidof(Recordset));
	try{
		CString strSQL = _T("select top 1 * from snapshot where imgisexist=1 and process_status=0");
		pSet = conn->Execute(_bstr_t(strSQL), NULL, adCmdText);
		if (pSet->adoEOF)
		{
			PutInfoToList(L"û����Ҫ���������");
			pSet->Close();
			pSet.Release();
			return false;
		}
		strFacePhotoName = (char*)_bstr_t(pSet->GetCollect(_variant_t("FacePhotoName")));
		strFullPhotoName = (char*)_bstr_t(pSet->GetCollect(_variant_t("FullPhotoName")));
		iSnapshotid = pSet->GetCollect(_variant_t("Snapshotid")).lVal;
		pSet->Close();
		pSet.Release();
		strBox.Format(L"ȡ��һ��snapshotid=%d������", iSnapshotid);
		PutInfoToList(strBox);
	}
	catch (_com_error e)
	{
		AfxMessageBox(L"ȡ��snapshot����ʧ��");
		pSet.Release();
		return false;
	}
	return true;
}

BOOL CDispatcherDlg::InsertAlarmToDB(CString strSnapshotId, CString strIDCard, CString strSim,CString strFaceDB)
{
	if (!ConnectDB())
		return false;
	try{
		CString strSQL;
		strSQL.Format(L"INSERT INTO warninfo (snapshotid,id_card,similarity,facedb) VALUES ('%s','%s','%s','%s')", strSnapshotId, strIDCard, strSim, strFaceDB);
		conn->Execute(_bstr_t(strSQL), NULL, adCmdText);
	}
	catch (_com_error e)
	{
		AfxMessageBox(L"����warninfo��ʧ��");
		return false;
	}
	return true;
}

BOOL CDispatcherDlg::ConnectDB()
{
	if (conn->State == adStateOpen)
		return true;
	CString strConn;
	strConn.Format(_T("Provider=SQLOLEDB; Server=%s;Database=%s; uid=%s; pwd=%s;"), theApp.m_strDBIP, theApp.m_strDBName, theApp.m_strDBUsername, theApp.m_strDBPassword);
	try
	{
		conn->Open(_bstr_t(strConn), "", "", adModeUnknown);
	}
	catch (_com_error e)
	{
		//			AfxMessageBox(L"�������ݿ�ʧ��");
		PutInfoToList(L"�������ݿ�ʧ��");
		m_bConnDisconnect = true;
		return false;
	}
	m_bConnDisconnect = false;
	return true;

}

BOOL CDispatcherDlg::UpdateProcessStatusToDB(int iSnapshotid, int iStatus, int iAge, int iGender)
{
	if (!ConnectDB())
		return false;
	try{
		CString strSQL;
		strSQL.Format(L"UPDATE snapshot SET process_status=%d,age=%d,gender=%d WHERE snapshotid=%d", iStatus, iAge, iGender, iSnapshotid);
		conn->Execute(_bstr_t(strSQL), NULL, adCmdText);
	}
	catch (_com_error e)
	{
		AfxMessageBox(L"�޸�snapshot��process_status�ֶ�ʧ��");
		return false;
	}
	return true;
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