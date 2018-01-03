
// ToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Tool.h"
#include "ToolDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

_ConnectionPtr  g_pConnect;
CString g_csFaceId;
CString g_csFacePath;
CString g_csFaceRootName;
CString g_EndPath;
int  g_nMaxForNum;
UINT Thread_MaintenanceSnapshot(LPVOID lparam);
UINT Thread_ClearDb(LPVOID lparam);

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


// CToolDlg 对话框



CToolDlg::CToolDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CToolDlg::IDD, pParent)
, m_csDeviceIp(_T(""))
, m_csLibraryId(_T(""))
, m_csSqlIp(_T(""))
, m_csSqlName(_T(""))
, m_csMaxNum(_T(""))
, m_Bruku(FALSE)
, m_bBlack(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_IP, m_csDeviceIp);
	DDX_Text(pDX, IDC_EDIT_ID, m_csLibraryId);
	DDX_Text(pDX, IDC_EDIT_IP2, m_csSqlIp);
	DDX_Text(pDX, IDC_EDIT_IP3, m_csSqlName);
	DDX_Text(pDX, IDC_EDIT_ID2, m_csMaxNum);
	DDX_Check(pDX, IDC_CHK_ADD, m_Bruku);
	DDX_Control(pDX, IDC_LIST1, m_wndList);
	DDX_Check(pDX, IDC_CHK_ADD2, m_bBlack);
}

BEGIN_MESSAGE_MAP(CToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUT_LOGIN, &CToolDlg::OnBnClickedButLogin)
	ON_BN_CLICKED(IDC_BUT_ADD, &CToolDlg::OnBnClickedButAdd)
	ON_BN_CLICKED(IDC_BUT_CONNECT, &CToolDlg::OnBnClickedButConnect)
	ON_BN_CLICKED(IDC_BUT_DELETE, &CToolDlg::OnBnClickedButDelete)
END_MESSAGE_MAP()


// CToolDlg 消息处理程序

BOOL CToolDlg::OnInitDialog()
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

	//	ShowWindow(SW_MAXIMIZE);//最大化

	// TODO:  在此添加额外的初始化代码
	m_wndList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_wndList.InsertColumn(0, _T("文件"), LVCFMT_LEFT, 300);
	m_wndList.InsertColumn(1, _T("时间"), LVCFMT_LEFT, 140);
	m_wndList.InsertColumn(2, _T("结果"), LVCFMT_LEFT, 90);
	m_wndList.GetHeaderCtrl()->EnableWindow(FALSE);

	m_bBlack = TRUE;

	m_bLogin = FALSE;
	m_bConnect = FALSE;
	m_bWork = FALSE;

	GetConfigIni();

	m_csDeviceIp = "192.168.2.30";
	m_csLibraryId = "10304";
	m_csMaxNum = "1";
	UpdateData(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CToolDlg::OnPaint()
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
HCURSOR CToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CToolDlg::OnBnClickedButLogin()
{
	UpdateData();
	if (!m_bLogin)
	{
		int nResult = DeviceLogin(m_csDeviceIp, pSession);
		if (nResult != 0)
		{
			MessageBox("登录失败");
			return;
		}
		m_bLogin = TRUE;
		GetDlgItem(IDC_BUT_LOGIN)->SetWindowText("登出");
	}
	else
	{
		if (m_bWork)
		{
			MessageBox("正在使用");
			return;
		}
		GetDlgItem(IDC_BUT_LOGIN)->SetWindowText("登录");
		DeviceLogout(m_csDeviceIp, pSession);
		m_bLogin = FALSE;
		memset(pSession, 0, 37);
	}

}

void CToolDlg::OnBnClickedButConnect()
{
	CString csSqlConnect;
	csSqlConnect.Format("Driver={MySQL ODBC 5.1 Driver};Server=%s;Database=%s;uid=root;pwd=cr;", m_csSqlIp, m_csSqlName);
	try
	{
		g_pConnect.CreateInstance(__uuidof(Connection));
		_bstr_t strConnect = csSqlConnect;
		g_pConnect->Open(strConnect, "", "", adModeUnknown);
		GetDlgItem(IDC_BUT_CONNECT)->EnableWindow(0);
		m_bConnect = TRUE;

	}
	catch (...)
	{
		MessageBox("数据路连接错误");
		return;
	}
}

void CToolDlg::OnBnClickedButAdd()
{
	UpdateData();
	if (!m_bLogin || !m_bConnect)
	{
		MessageBox("未登录");
		return;
	}
	BROWSEINFO  bi;
	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = NULL;
	bi.lpszTitle = NULL;
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.iImage = 0;
	LPCITEMIDLIST pidl = SHBrowseForFolder(&bi);
	if (!pidl)
		return;
	TCHAR  szDisplayName[255];
	SHGetPathFromIDList(pidl, szDisplayName);
	CString strPath(szDisplayName);
	int dotPos = strPath.ReverseFind('\\');

	g_csFaceRootName = strPath.Right(strPath.GetLength() - dotPos);
	g_csFaceRootName.Replace("\\", "");
	g_csFaceId = m_csLibraryId;
	g_csFacePath = strPath;
	g_nMaxForNum = atoi(m_csMaxNum);
	AfxBeginThread(&Thread_MaintenanceSnapshot, NULL);
}

BOOL CToolDlg::GetEmpIdcard(CString &csIdcard)
{
	try
	{
		CString csSql;
		_RecordsetPtr pRecordset;
		CString csTemp;

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
		return FALSE;
	}
	return TRUE;

}
void CToolDlg::BayesCategoryTest(CString tp)
{

	CFileFind fileFinder;
	CString filePath = tp + _T("\\*.*");

	BOOL bFinished = fileFinder.FindFile(filePath);
	while (bFinished)  //每次循环对应一个类别目录
	{
		try
		{
			bFinished = fileFinder.FindNextFile();
			CString fileName = fileFinder.GetFileName();

			if (fileFinder.IsDirectory() && !fileFinder.IsDots())  //若是目录则递归调用此方法
			{
				BayesCategoryTest(fileFinder.GetFilePath());
			}
			else  //再判断是否为jpg文件
			{
				if (fileName.GetLength() < 4)
				{
					continue;
				}
				int dotPos = fileName.ReverseFind('.');
				CString csFeature = tp + "\\" + fileName;
				CString csName = fileName.Mid(0, dotPos);
				CString fileExt = fileName.Right(fileName.GetLength() - dotPos);

				if (fileExt == _T(".jpg"))  //若是txt文件则开始分类测试
				{

					//对照片提取Face
					int nLen = 1024 * 1024 * 3;
					char *pJson = new char[nLen * 3];
					memset(pJson, '\0', nLen * 3);
					int nResult = FASFeatureDetectforImageFile(m_csDeviceIp, pSession, csFeature, pJson, nLen);
					if (nResult != 0)
					{
						CString csTemp;
						csTemp.Format("提取Feature失败_%s", csFeature);
						SetListData(csFeature, csTemp);
						continue;
					}

					//提取Img 和face 的b64
					Json::Reader reader;
					Json::Value root;
					Json::Value result;
					Json::Value content;
					BOOL readre = reader.parse(pJson, root);
					if (!readre)//json失败
					{
						CString csTemp;
						csTemp.Format("json失败_%s", csFeature);
						SetListData(csFeature, csTemp);
						continue;
					}

					int iZero = 0;
					result = root["data"]["result"][iZero];
					if (result.isNull())//没有提取成功
					{
						CString csTemp;
						csTemp.Format("json为NULL_%s", csFeature);
						SetListData(csFeature, csTemp);
						continue;
					}
					else
					{
						CString csAge;
						CString strFeatures = result["feature"].asCString();
						CString csImageData = result["face_image"].asCString();
						int nAge = result["age"].asInt();
						csAge.Format("%d", nAge);

						CString csFeaturePath;
						csFeaturePath.Format("%s//%s.txt", g_EndPath, csName);
						CString csImgPath;
						csImgPath.Format("%s//%s.jpg", g_EndPath, csName);
						csFeaturePath.Replace("\\", "//");
						csImgPath.Replace("\\", "//");

// 						CString strSQL;
// 						strSQL.Format(("INSERT INTO imginfo (id_card,imgpath,featurepath)values('%s','%s','%s')"), csName, csImgPath, csFeaturePath);
// 						g_pConnect->Execute(_bstr_t(strSQL), NULL, adCmdText);

						CFile file;
						if (file.Open(csFeaturePath, CFile::modeCreate | CFile::modeWrite))
						{
							file.Write(strFeatures, strFeatures.GetLength());
							file.Close();
						}
						else
						{
							CString csTemp;
							csTemp.Format("写Feature失败_%s", csFeature);
							SetListData(csFeature, csTemp);
							continue;
						}


						//注册
						char pPossoid[128];
						CString csBirthday = "1990-01-01";
						CString csValidTo = "2090-01-01";
						CString csGender = "1";
					//	CString csIdcard = "1";
						CString csIdcard, csSql;
						if (!GetEmpIdcard(csIdcard))
						{
							CString csTemp;
							csTemp.Format("获取Idcard失败_%s", csFeature);
							SetListData(csFeature, csTemp);
							continue;
						}
						csSql.Format("UPDATE  parameterini set  MaxIdCard = '%s' where  Parameter = 'IdCard';", csIdcard);
						g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);

						nResult = FASAddPersoninfo(m_csDeviceIp, pSession, csIdcard, csIdcard, atoi(csGender), atof(m_csLibraryId), strFeatures, csImageData, csBirthday, csValidTo, pPossoid, 128);

						if (nResult != 0)
						{
							CString csTemp;
							csTemp.Format("注册失败_%s", csFeature);
							SetListData(csFeature, csTemp);
							continue;
						}
						CString csPresentId(pPossoid);

						if (m_Bruku)
						{
							CString strSQL;
							CString csLibid = "2";
							if (m_bBlack)
							{
								csLibid = "1";
							}
							strSQL.Format(("INSERT INTO employeeinfo (name,sex,birth,id_card,libraryid,nativeplace,phone,address,isbatch,isdelete,type,personid)values('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')"), csName, "female", csBirthday, csIdcard, csLibid, "", "", "", "0", "0", "批量", csPresentId);
							g_pConnect->Execute(_bstr_t(strSQL), NULL, adCmdText);

							strSQL.Format(("INSERT INTO imginfo (id_card,imgpath,featurepath)values('%s','%s','%s')"), csIdcard, csImgPath, csFeaturePath);
							g_pConnect->Execute(_bstr_t(strSQL), NULL, adCmdText);
							MoveImg(csFeature, csName);
						}
					}
					delete[] pJson;
					SetListData(csFeature, "成功");
					Sleep(100);
				}
			}
		}
		catch (CException* e)
		{
			int ttt = 0;
		}
	}

	fileFinder.Close();

}

UINT Thread_MaintenanceSnapshot(LPVOID lparam)
{
	CToolDlg *pWnd = (CToolDlg *)AfxGetApp()->GetMainWnd();

	try
	{
		for (int i = 0; i < g_nMaxForNum; i++)
		{
			pWnd->m_bWork = TRUE;
			pWnd->BayesCategoryTest(g_csFacePath);
			pWnd->MessageBox("成功");
			Sleep(1000);
		}
		pWnd->m_bWork = FALSE;

	}
	catch (...)
	{
	}
	return 0;
}

UINT Thread_ClearDb(LPVOID lparam)
{
	CToolDlg *pWnd = (CToolDlg *)AfxGetApp()->GetMainWnd();

	try
	{
		pWnd->m_bWork = TRUE;
		pWnd->ClearDb();
		pWnd->m_bWork = FALSE;

	}
	catch (...)
	{
	}
	return 0;
}

void CToolDlg::OnBnClickedButDelete()
{
	UpdateData();
	if (!m_bLogin)
	{
		MessageBox("请登录");
		return;
	}
	AfxBeginThread(&Thread_ClearDb, NULL);

}
//清库
void CToolDlg::ClearDb()
{
	char chJson[1024 * 128];

	while (true)
	{
		int result = FASGetFaceListPageInDb(m_csDeviceIp, pSession, m_csLibraryId, "", chJson, 1024 * 128);
		CString csJson(chJson);
		int ss = 0;
		ss = csJson.Find(_T("\"personStatus\":"));
		if (ss == -1)
		{
			break;
		}
		int s(0), e(0);
		while (TRUE)
		{
			s = csJson.Find(_T("\"personStatus\":"), e);
			if (s == -1)
			{
				break;
			}
			s = s + 1;
			s = csJson.Find(_T("\"id\":"), s);
			s = s + 5;
			e = csJson.Find(_T(","), s);
			CString strFeatures = csJson.Mid(s, e - s);
			int result = FASDELETEPersoninfo(m_csDeviceIp, pSession, strFeatures);
		}
	}
}

BOOL CToolDlg::GetConfigIni()
{
	CString strIp;
	TCHAR pFileName[MAX_PATH];
	int nPos = GetCurrentDirectory(MAX_PATH, pFileName);
	if (nPos == 0)
	{
		return FALSE;
	}
	CString csFullPath(pFileName);
	m_csPath = csFullPath + "\\Parameter.ini";

	if (GetPrivateProfileString(("SqlConfiguration"), "Sql_ServerAdd", (""), m_csSqlIp.GetBuffer(MAX_PATH), MAX_PATH, m_csPath) <= 0)
		return FALSE;
	if (GetPrivateProfileString(("SqlConfiguration"), "Sql_Database", (""), m_csSqlName.GetBuffer(MAX_PATH), MAX_PATH, m_csPath) <= 0)
		return FALSE;
	//if (GetPrivateProfileString(("MaintenanceConfiguration"), "Maintenance_MaxPeople", (""), m_csMaxNPeople.GetBuffer(MAX_PATH), MAX_PATH, m_csPath) <= 0)
	//	return FALSE;

	if (GetPrivateProfileString(("Path"), "End_Path", (""), g_EndPath.GetBuffer(MAX_PATH), MAX_PATH, m_csPath) <= 0)
		return FALSE;


	return TRUE;
}

BOOL CToolDlg::TableIsFull(_ConnectionPtr pConnect, CString csTable, int nMax)
{
	try
	{
		CString csSql;
		//csSql.Format("set")

	}
	catch (CException* e)
	{
		DWORD dError = GetLastError();
		return FALSE;
	}
	return TRUE;
}

BOOL CToolDlg::MoveImg(CString csImgPath, CString csImgName)
{
	try
	{
		CString csTargetPath;
		csTargetPath.Format("%s//%s.jpg", g_EndPath, csImgName);
		if (MoveFile(csImgPath, csTargetPath) == 0)
			return FALSE;
// 		csImgPath.Replace(".jpg", ".txt");
// 		csTargetPath.Replace(".jpg", ".txt");
// 		if (MoveFile(csImgPath, csTargetPath) == 0)
// 			return FALSE;
	}
	catch (...)
	{
		return FALSE;
	}
	return TRUE;
}


void CToolDlg::SetListData(CString csPath, CString csResults)
{
	COleDateTime coTime;
	coTime = COleDateTime::GetCurrentTime();
	CString strCurTime = coTime.Format("%Y-%m-%d %H:%M:%S");
	m_wndList.InsertItem(0, "");
	m_wndList.SetItemText(0, 0, csPath);
	m_wndList.SetItemText(0, 1, strCurTime);
	m_wndList.SetItemText(0, 2, csResults);
}