
// FaceComparDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FaceCompar.h"
#include "FaceComparDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CFaceComparDlg 对话框



CFaceComparDlg::CFaceComparDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFaceComparDlg::IDD, pParent)
	, m_csName(_T(""))
	, m_csAge(_T(""))
	, m_csGender(_T(""))
	, m_csOrigin(_T(""))
	, m_csAddress(_T(""))
	, m_csTelephone(_T(""))
	, m_csCertificates(_T(""))
	, m_csSimilar(_T(""))
	, m_csMaxSum(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFaceComparDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_ComputerTree);
	DDX_Control(pDX, IDC_LIST_COMPAR, m_wndComparList);
	DDX_Text(pDX, IDC_EDIT1, m_csName);
	DDX_Text(pDX, IDC_EDIT2, m_csAge);
	DDX_CBString(pDX, IDC_COMBO1, m_csGender);
	DDX_Text(pDX, IDC_EDIT3, m_csOrigin);
	DDX_Text(pDX, IDC_EDIT4, m_csAddress);
	DDX_Text(pDX, IDC_EDIT5, m_csTelephone);
	DDX_Text(pDX, IDC_EDIT6, m_csCertificates);
	DDX_Text(pDX, IDC_EDIT7, m_csSimilar);
	DDX_Text(pDX, IDC_EDIT8, m_csMaxSum);
}

BEGIN_MESSAGE_MAP(CFaceComparDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_CLICK, IDC_TREE1, &CFaceComparDlg::OnNMClickTree1)
	ON_BN_CLICKED(IDC_BUT_OPEN, &CFaceComparDlg::OnBnClickedButOpen)
	ON_BN_CLICKED(IDC_BUTCOMPAR, &CFaceComparDlg::OnBnClickedButcompar)
END_MESSAGE_MAP()


// CFaceComparDlg 消息处理程序

BOOL CFaceComparDlg::OnInitDialog()
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

	m_wndComparList.SetExtendedStyle(LVS_EX_INFOTIP | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_wndComparList.InsertColumn(0, _T("姓名"), LVCFMT_LEFT, 90);
	m_wndComparList.InsertColumn(1, _T("性别"), LVCFMT_LEFT, 50);
	m_wndComparList.InsertColumn(2, _T("证件号"), LVCFMT_LEFT, 120);
	m_wndComparList.InsertColumn(3, _T("相似度"), LVCFMT_LEFT, 60);
	m_wndComparList.InsertColumn(4, _T("地址"), LVCFMT_LEFT, 130);
	m_wndComparList.GetHeaderCtrl()->EnableWindow(FALSE);

	InitDbParameter();
	UpdatComputerTree();
	m_csSimilar = "50";
	m_csMaxSum = "50";
	UpdateData(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFaceComparDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFaceComparDlg::OnPaint()
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

		HDC hDC = GetDC()->m_hDC;
		Graphics g(hDC);
		//画总背景
		if (!m_csImgPath.IsEmpty())
		{
			USES_CONVERSION;
			WCHAR* pBuf = T2W((LPCTSTR)m_csImgPath);
			Image imgpeople(pBuf);
			g.DrawImage(&imgpeople, 8, 13, 165, 185);
		}
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFaceComparDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CFaceComparDlg::InitDbParameter()
{
	CString m_csSqlIP, m_csSqlDatabase, m_csSqlName, m_csSqlPwd;

	int nPos;
	CString    sPath, csFullPath;
	GetModuleFileName(NULL, sPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	sPath.ReleaseBuffer();
	nPos = sPath.ReverseFind('\\');
	sPath = sPath.Left(nPos);
	csFullPath = sPath;
	csFullPath += _T("\\Parameter.ini");
	GetPrivateProfileString(_T("SqlConfiguration"), _T("Sql_ServerAdd"), _T(""), m_csSqlIP.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);
	GetPrivateProfileString(_T("SqlConfiguration"), _T("Sql_Name"), _T(""), m_csSqlName.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);
	GetPrivateProfileString(_T("SqlConfiguration"), _T("Sql_Pwd"), _T(""), m_csSqlPwd.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);
	GetPrivateProfileString(_T("SqlConfiguration"), _T("Sql_Database"), _T(""), m_csSqlDatabase.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);
	GetPrivateProfileString(_T("FeatureFace"), _T("FaceIp "), _T(""), m_csFaceIp.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);

	CString csSqlConnect; 
	csSqlConnect.Format(_T("Driver={MySQL ODBC 5.1 Driver};Server=%s;Database=%s;uid=%s;pwd=%s;"), m_csSqlIP, m_csSqlDatabase, m_csSqlName, m_csSqlPwd);

	m_pConnect.CreateInstance(__uuidof(Connection));
	try
	{
		_bstr_t strConnect = csSqlConnect;
		m_pConnect->Open(strConnect, _T(""), _T(""), adModeUnknown);
	}
	catch (_com_error e)
	{
		return FALSE;
	}

	return  TRUE;
}

int CFaceComparDlg::OpenFileGetPath(CString &csPath)
{
	int nPos;
	UpdateData();
	UpdateData(FALSE);

	CString    sPath;
	GetModuleFileName(NULL, sPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	sPath.ReleaseBuffer();
	nPos = sPath.ReverseFind('\\');
	sPath = sPath.Left(nPos);

	TCHAR szBuffer[1024] = { 0 };
	OPENFILENAME ofn = { 0 };
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFilter = _T("*.jpg)\0*.jpg\0");//要选择的文件后缀   
//	ofn.lpstrInitialDir = "D:\\";//默认的文件路径   
	ofn.lpstrInitialDir = sPath;//默认的文件路径   
	ofn.lpstrFile = szBuffer;//存放文件的缓冲区   
	ofn.nMaxFile = sizeof(szBuffer) / sizeof(*szBuffer);
	ofn.nFilterIndex = 0;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;//标志如果是多选要加上OFN_ALLOWMULTISELECT  
	if (GetOpenFileName(&ofn))
	{
		csPath = ofn.lpstrFile;
	}

	return 0;
}

BOOL CFaceComparDlg::UpdatComputerTree()
{
	BOOL bReturn = TRUE;

	try
	{
		CString csSql;
		_RecordsetPtr pRecordset;
		m_ComputerTree.DeleteAllItems();
		m_hRoot = m_ComputerTree.InsertItem(_T("人脸库"));
		//设置库名称
		StructSetTreeData *pRootInfo = new StructSetTreeData();
		pRootInfo->csName = _T("人脸库");
		pRootInfo->nFlag = 1;
		m_ComputerTree.SetItemData(m_hRoot, (DWORD)pRootInfo);

		csSql.Format(_T("select * from device where type = 1"));
		VARIANT vTmp;
		vTmp.vt = VT_BSTR;
		vTmp.bstrVal = NULL;
		pRecordset = m_pConnect->Execute(_bstr_t(csSql), &vTmp, adCmdText);
		char pSession[19];
		StructDblist sDblist[50];
		ZeroMemory(&sDblist, sizeof(sDblist));
		while (!pRecordset->adoEOF)
		{
			CString csOtherIp = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("ip")->Value;
			HTREEITEM hChildIP = m_ComputerTree.InsertItem(csOtherIp, m_hRoot);
			//显示一体机IP
			StructSetTreeData *pIpInfo = new StructSetTreeData();
			pIpInfo->csName = csOtherIp;
			pIpInfo->csIP = csOtherIp;
			pIpInfo->nFlag = 2;
			m_ComputerTree.SetItemData(hChildIP, (DWORD)pIpInfo);


			int nTotalcount, nListsize;
			int nResult = -1;
			nResult = m_CFASLoginOrLogout.DeviceLogin(csOtherIp.GetBuffer(), pSession);
			if (nResult != 0)
			{
				bReturn = FALSE;
				pRecordset->MoveNext();
				__ExpandAllNode(m_hRoot, m_ComputerTree);
				continue;
			}
			nResult = m_CFASStatusQuery.FASFaceDBQuery(csOtherIp.GetBuffer(), pSession, sDblist, &nTotalcount, &nListsize);
			if (nResult != 0)
			{
				bReturn = FALSE;
				pRecordset->MoveNext();
				__ExpandAllNode(m_hRoot, m_ComputerTree);
				m_CFASLoginOrLogout.DeviceLogout(csOtherIp.GetBuffer(), pSession);
				continue;
			}

			nResult = m_CFASLoginOrLogout.DeviceLogout(csOtherIp.GetBuffer(), pSession);
			if (nResult != 0)
			{
				bReturn = FALSE;
			}

			for (int i = 0; i < nListsize; i++)
			{
				//显示苦命
				StructSetTreeData *FaceTree = new StructSetTreeData();
			//	CString csName = GetFaceShowName(sDblist[i].name, sDblist[i].db_id);
				HTREEITEM hFace = m_ComputerTree.InsertItem(sDblist[i].name, hChildIP);
				FaceTree->csID = sDblist[i].db_id;
				FaceTree->csName = sDblist[i].name;
				FaceTree->csIP = csOtherIp;
				FaceTree->nFlag = 3;
				m_ComputerTree.SetItemData(hFace, (DWORD)FaceTree);
			}
			ZeroMemory(&sDblist, sizeof(sDblist));
			pRecordset->MoveNext();
		}

	}
	catch (_com_error e)
	{
		return FALSE;
	}

	__ExpandAllNode(m_hRoot, m_ComputerTree);

	return bReturn;
}

void CFaceComparDlg::OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPoint pt;
	GetCursorPos(&pt);
	m_ComputerTree.ScreenToClient(&pt);
	UINT uFlags = 0;
	HTREEITEM hOtherlChild = NULL;
	m_hComputer = m_ComputerTree.HitTest(pt, &uFlags);
	if (NULL != m_hComputer)
	{
		if (uFlags & TVHT_ONITEMSTATEICON)//点击复选框
		{
			m_ComputerTree.SelectItem(m_hComputer);
			if (!m_ComputerTree.GetCheck(m_hComputer))
			{
				hOtherlChild = m_ComputerTree.GetParentItem(m_hComputer);
			}
		}
		else if (uFlags & TVHT_ONITEMLABEL)//点击文字
		{
			if (!m_ComputerTree.GetCheck(m_hComputer))
			{
				m_ComputerTree.SetCheck(m_hComputer, TRUE);
				hOtherlChild = m_ComputerTree.GetParentItem(m_hComputer);
			}
			else
			{
				m_ComputerTree.SetCheck(m_hComputer, FALSE);
			}
		}

		if (NULL != hOtherlChild && hOtherlChild != m_hRoot)
		{
			if (!m_ComputerTree.GetCheck(hOtherlChild))
				m_ComputerTree.SetCheck(hOtherlChild, TRUE);
		}
	}
	*pResult = 0;
}
//遍历一体机勾选
BOOL CFaceComparDlg::GetComputerTreeCheck(HTREEITEM  hFather)
{
	try
	{
		int nResult;
		HTREEITEM hChild = m_ComputerTree.GetChildItem(hFather);
		if (NULL == hChild) return FALSE;
		do
		{
			if (m_ComputerTree.GetCheck(hChild))
			{
				StructSetTreeData * pNode = (StructSetTreeData *)m_ComputerTree.GetItemData(hChild);
				if (pNode == NULL)
				{
					continue;
				}				
				memset(m_pSession, 0, sizeof(m_pSession));
				nResult = m_CFASLoginOrLogout.DeviceLogin(pNode->csIP.GetBuffer(), m_pSession);
				GetFaceTreeCheck(hChild);
				nResult = m_CFASLoginOrLogout.DeviceLogout(pNode->csIP.GetBuffer(), m_pSession);

			}
		} while ((hChild = m_ComputerTree.GetNextSiblingItem(hChild)) != NULL);
	}
	catch (...)
	{
		return FALSE;
	}
	return TRUE;
}

void CFaceComparDlg::ComparFace(CString csIP, CString csID)
{
	StructRecognitionList pStructRecognitionlist[50];
	ZeroMemory(&pStructRecognitionlist, sizeof(pStructRecognitionlist));
	CString csSql;
	_RecordsetPtr pRecordset;

	int nConut = 0, nContrast;
// 	nContrast = m_CFASRecognition.FASImgRecongnition(
// 		(LPSTR)(LPCTSTR)csIP,	//一体机IP
// 		m_pSession,
// 		(LPSTR)(LPCTSTR)csID,  //对比人脸库ID
// 		m_pImage,																			 //照片信息
// 		(LPSTR)(LPCTSTR)m_csMaxSum,		//最大返回个数
// 		(LPSTR)(LPCTSTR)m_csSimilar,		//对比相似度
// 		&nConut,																					//返回个数
// 		pStructRecognitionlist);

	nContrast = m_CFASRecognition.FASImgRecongnitionForSelect((LPSTR)(LPCTSTR)csIP, m_pSession, (LPSTR)(LPCTSTR)csID, m_pImage, 
		(LPSTR)(LPCTSTR)m_csMaxSum, (LPSTR)(LPCTSTR)m_csSimilar, (LPSTR)(LPCTSTR)m_csAge, (LPSTR)(LPCTSTR)m_csGender, 
		(LPSTR)(LPCTSTR)m_csOrigin, (LPSTR)(LPCTSTR)m_csAddress, (LPSTR)(LPCTSTR)m_csName,
		(LPSTR)(LPCTSTR)m_csTelephone, (LPSTR)(LPCTSTR)m_csCertificates, &nConut, pStructRecognitionlist);
	if (nContrast == 0)
	{
		for (int i = 0; i < nConut; i++)
		{
			int row = m_wndComparList.GetItemCount();
			m_wndComparList.InsertItem(row, "");
			m_wndComparList.SetItemText(row, 2, pStructRecognitionlist[i].aId_card);
			m_wndComparList.SetItemText(row, 3, pStructRecognitionlist[i].aSimilarity);
			csSql.Format("SELECT * FROM employeeinfo WHERE idcard = '%s'", pStructRecognitionlist[i].aId_card);
			pRecordset = m_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
			if (!pRecordset->adoEOF)
			{
				CString csName = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("name")->Value;
				CString csSex = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("sex")->Value;
				CString csAddress = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("address")->Value;
				m_wndComparList.SetItemText(row, 0, csName);
				m_wndComparList.SetItemText(row, 1, csSex);
				m_wndComparList.SetItemText(row, 4, csAddress);

			}
		}
	}
}
//遍历单个一体机下的库
BOOL CFaceComparDlg::GetFaceTreeCheck(HTREEITEM  hFather)
{
	try
	{
		HTREEITEM hChild = m_ComputerTree.GetChildItem(hFather);
		int nContrast;
		if (NULL == hChild) return FALSE;
		do
		{
			if (m_ComputerTree.GetCheck(hChild))
			{
				StructSetTreeData * pNode = (StructSetTreeData *)m_ComputerTree.GetItemData(hChild);
				if (pNode == NULL)
				{
					continue;
				}
				//对比图片
				ComparFace(pNode->csIP, pNode->csID);
			}
		} while ((hChild = m_ComputerTree.GetNextSiblingItem(hChild)) != NULL);
	}
	catch (...)
	{
		return FALSE;
	}
	return TRUE;
}

void CFaceComparDlg::OnBnClickedButOpen()
{


	TCHAR szBuffer[MAX_PATH] = { 0 };
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = NULL;
	bi.pszDisplayName = szBuffer;
	bi.lpszTitle = _T("选择照片:"); 
	bi.ulFlags = BIF_BROWSEINCLUDEFILES;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (NULL == idl)
	{
		return;
	}
	SHGetPathFromIDList(idl, szBuffer);
	CString strPath(szBuffer);
	if (strPath.Find(".jpg") == -1 && strPath.Find(".JPG") == -1)
	{
		MessageBox("请使用正确的jpg图片");
		return;
	}

 	m_csImgPath.Empty();
 //	OpenFileGetPath(m_csImgPath);
	m_csImgPath = strPath;
 	if (!m_csImgPath.IsEmpty())
 	{
 		//读取照片b64
 		CFile file;
 		char pSession[19];
 		int nContrast;
 		if (file.Open(m_csImgPath, CFile::modeRead))//11
 		{
 			UINT  iFileLen = file.GetLength();
 			BYTE *imagebuf = new BYTE[iFileLen];
 			int iEflen = iFileLen * 2;
 			file.Read(imagebuf, iFileLen);
 			file.Close();
 
 			char *pcEimagebuf = new char[iEflen];
 			memset(m_pImage, '\0', MAX_LEN);
 			memset(m_pFeature, '\0', MAX_LEN);
 
 			int len = __EncodeBase64(imagebuf, pcEimagebuf, iFileLen, MAX_LEN);
 			nContrast = m_CFASLoginOrLogout.DeviceLogin(m_csFaceIp.GetBuffer(), pSession);
 			if (nContrast == 0)
 			{
 				nContrast = m_CFASRecognition.FASFeatureDetect((LPSTR)(LPCTSTR)m_csFaceIp, pSession, pcEimagebuf, m_pImage, m_pFeature);
 				if (nContrast == 0)
 				{
 					Invalidate(false);
 				}
 				m_CFASLoginOrLogout.DeviceLogout(m_csFaceIp.GetBuffer(), pSession);
 			}
 			delete pcEimagebuf;
 			delete imagebuf;
 		}
 	}
}


void CFaceComparDlg::OnBnClickedButcompar()
{
	
	UpdateData();
	if (m_csImgPath.IsEmpty())
	{
		MessageBox("请选择照片");
		return;
	}

	if (m_csSimilar.IsEmpty() || m_csMaxSum.IsEmpty())
	{
		MessageBox("相似度与最大返回个数不能为空");
		return;
	}
	CString csStartTime, csEndTime;

	CTime tm;
	tm = CTime::GetCurrentTime();
	csStartTime = tm.Format("%Y-%m-%d %H:%M:%S");
	m_wndComparList.DeleteAllItems();

	GetComputerTreeCheck(m_hRoot);

	tm = CTime::GetCurrentTime();
	csEndTime = tm.Format("%Y-%m-%d %H:%M:%S");
	MessageBox(csStartTime + "  " + csEndTime);
}
