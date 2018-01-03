
// ReplaceFileDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ReplaceFile.h"
#include "ReplaceFileDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define UDP_SEND_PORT 30000  //udp�˿�
#define  PROGRAM_INDEX   2		//����ID
#define  SLEEP_TIME   30000		//����ID
UINT HeartbeatThread(LPVOID lpam);

UINT MainThread(LPVOID pParam);

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


// CReplaceFileDlg �Ի���



CReplaceFileDlg::CReplaceFileDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CReplaceFileDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CReplaceFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CReplaceFileDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()

LONG CallBackCrashHandler(EXCEPTION_POINTERS *pException)
{

	return EXCEPTION_EXECUTE_HANDLER;
}

// CReplaceFileDlg ��Ϣ�������

BOOL CReplaceFileDlg::OnInitDialog()
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	m_bQuit = false;
	AfxBeginThread((AFX_THREADPROC)MainThread, (void*)this);
	AfxBeginThread((AFX_THREADPROC)HeartbeatThread, (void*)this);


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CReplaceFileDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CReplaceFileDlg::OnPaint()
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
HCURSOR CReplaceFileDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CReplaceFileDlg::CreateAllDirectory(CString strImgpath)
{
	CString strCreatePath;
	strCreatePath.Format(_T("%s"), strImgpath);
	CreateDirectoryW(strCreatePath, NULL);
	strCreatePath.Format(_T("%s/imgCurFaceJpg"), strImgpath);
	CreateDirectoryW(strCreatePath, NULL);
	strCreatePath.Format(_T("%s/imgCurPhotoJpg"), strImgpath);
	CreateDirectoryW(strCreatePath, NULL);
	strCreatePath.Format(_T("%s/imgFaceDBJpg"), strImgpath);
	CreateDirectoryW(strCreatePath, NULL);
	strCreatePath.Format(_T("%s/PlaybackVideo"), strImgpath);
	CreateDirectoryW(strCreatePath, NULL);
	strCreatePath.Format(_T("%s/imgCurFaceFeature"), strImgpath);
	CreateDirectoryW(strCreatePath, NULL);
	return true;
}

UINT MainThread(LPVOID pParam)
{
	AfxOleInit();
	CReplaceFileDlg *pWnd = (CReplaceFileDlg*)pParam;
	_ConnectionPtr conn;
	conn.CreateInstance(__uuidof(Connection));
	_RecordsetPtr pSet;
	pSet.CreateInstance(__uuidof(Recordset));

	while (!pWnd->m_bQuit)
	{
		try
		{
			CString strConn;
			strConn.Format(_T("Driver={MySQL ODBC 5.1 Driver}; Server=%s;Database=%s; uid=%s; pwd=%s;"), theApp.m_strDBIP, theApp.m_strDBName, theApp.m_strDBUsername, theApp.m_strDBPassword);
			try
			{
				conn->Open(_bstr_t(strConn), "", "", adModeUnknown);
			}
			catch (_com_error e)
			{
				//			PutLonInfoToList(L"����ʧ��");
				Sleep(5000);
				continue; 
			}
			CString strSQL = _T("select CameraId,IP from Camera");
			pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
			while (!pSet->adoEOF)
			{
				CString strIP, strFilePath;
				int iCameraID;
				strIP = (char*)_bstr_t(pSet->GetCollect(_variant_t("IP")));
				iCameraID = pSet->GetCollect(_variant_t("CameraId")).dblVal;
				//	strFilePath = theApp.m_strSrcPath + L"/" + strIP + L"[1]";
				strFilePath.Format(_T("%s/%s[1]"), theApp.m_strSrcPath, strIP);
				pWnd->ProcessOneFile(strFilePath, iCameraID);
				pSet->MoveNext();
			}
			pSet->Close();
			conn->Close();
		}
		catch (...)
		{
		}
	}
	return 1;
}

void CReplaceFileDlg::ProcessOneFile(CString strPath, int iCameraID)
{
	USES_CONVERSION;
	int re = 0;
	CString strPicName, strFaceName;
	CFileFind ff;
	BOOL bFind = ff.FindFile(strPath + "/BackgroundPic*.jpg");
	if (bFind)
	{
		Sleep(1);
		bFind = ff.FindNextFile();
		strPicName = ff.GetFileName();
	}
	else
		return;
	bFind = ff.FindFile(strPath + "/SnapPic*.jpg");
	if (bFind)
	{
		Sleep(1);
		bFind = ff.FindNextFile();
		strFaceName = ff.GetFileName();
	}
	else
		return;
	int y, m, d, h, u, s, ms1, ms2;
	COleDateTime odt1, odt2;
	sscanf_s(T2A(strPicName), "BackgroundPic[%04d%02d%02d%02d%02d%02d%03d]", &y, &m, &d, &h, &u, &s, &ms1); 
	odt1.SetDateTime(y, m, d, h, u, s);
	sscanf_s(T2A(strFaceName), "SnapPic[%04d%02d%02d%02d%02d%02d%03d]", &y, &m, &d, &h, &u, &s, &ms2);
	odt2.SetDateTime(y, m, d, h, u, s);
	if (odt1 < odt2)
	{
		DeleteFile(strPath + L"/" + strPicName);
		return;
	}
	COleDateTimeSpan sp;
	sp = odt1 - odt2;
	int chasec, chams;
	chasec = sp.GetTotalSeconds();
	if (ms1 >= ms2)
		chams = ms1 - ms2;
	else
	{
		chams = ms1 + 1000 - ms2;
		chasec -= 1;
	}
	chams = chasec * 1000 + chams;
	if (chams < 0 || chams >= 100)
	{
		DeleteFile(strPath + L"/" + strFaceName);
		return;
	}

	CString strDstPath;
	strDstPath.Format(L"%s/%04d%02d%02d", theApp.m_strDstPath, y, m, d);
	//�ж��ļ����Ƿ����
	BOOL IsD = PathIsDirectory(strDstPath);
	if (!IsD)
		CreateAllDirectory(strDstPath);

	_ConnectionPtr conn;
	conn.CreateInstance(__uuidof(Connection));
	_RecordsetPtr pSet;
	pSet.CreateInstance(__uuidof(Recordset));
	CString strConn;
	strConn.Format(_T("Driver={MySQL ODBC 5.1 Driver}; Server=%s;Database=%s; uid=%s; pwd=%s;"), theApp.m_strDBIP, theApp.m_strDBName, theApp.m_strDBUsername, theApp.m_strDBPassword);
	try
	{
		conn->Open(_bstr_t(strConn), "", "", adModeUnknown);
	}
	catch (_com_error e)
	{
		return;
	}
	CString strSQL = _T("select * from Snapshot limit 0,1");
	CString strCameraID, strImgpath, strAlarmTime;
	strCameraID.Format(L"%d", iCameraID);
	strImgpath.Format(L"%04d%02d%02d", y, m, d);
	strAlarmTime.Format(L"%04d-%02d-%02d %02d:%02d:%02d", y, m, d, h, u, s);
	pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	pSet->AddNew();
	pSet->put_Collect(_variant_t("CameraId"), _variant_t(strCameraID));
	pSet->put_Collect(_variant_t("FacePhotoName"), _variant_t(""));
	pSet->put_Collect(_variant_t("FullPhotoName"), _variant_t(""));
	pSet->put_Collect(_variant_t("ImagePath"), _variant_t(strImgpath));
	pSet->put_Collect(_variant_t("process_status"), _variant_t("0"));
	pSet->put_Collect(_variant_t("imgisexist"), _variant_t("0"));
	pSet->put_Collect(_variant_t("AlarmTime"), _variant_t(strAlarmTime));
	pSet->put_Collect(_variant_t("age"), _variant_t("0"));
	pSet->Update();
	pSet->Close();
	strSQL = _T("select max(SnapshotId) as count from Snapshot");
	pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	int iCount = pSet->GetCollect("count").dblVal;
	pSet->Close();
	CString strDstFacePath, strDstPhotoPath;
	strDstFacePath.Format(L"%s/imgCurFaceJpg/%d.jpg", strDstPath, iCount);
	strDstPhotoPath.Format(L"%s/imgCurPhotoJpg/%d.jpg", strDstPath, iCount);
	CString strSrcFacePath, strSrcPhotoPath;
	strSrcFacePath.Format(L"%s/%s", strPath, strFaceName);
	strSrcPhotoPath.Format(L"%s/%s", strPath, strPicName);
	MoveFile(strSrcFacePath, strDstFacePath);
	MoveFile(strSrcPhotoPath, strDstPhotoPath);

	strSQL.Format(L"select * from Snapshot where SnapshotId=%d", iCount);
	pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	pSet->put_Collect(_variant_t("FacePhotoName"), _variant_t(strDstFacePath));
	pSet->put_Collect(_variant_t("FullPhotoName"), _variant_t(strDstPhotoPath));
	pSet->put_Collect(_variant_t("imgisexist"), _variant_t("1"));
	pSet->Update();
	pSet->Close();
	conn->Close();
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
