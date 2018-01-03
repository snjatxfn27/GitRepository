
// FaceComparDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FaceCompar.h"
#include "FaceComparDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CFaceComparDlg �Ի���



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


// CFaceComparDlg ��Ϣ�������

BOOL CFaceComparDlg::OnInitDialog()
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

	m_wndComparList.SetExtendedStyle(LVS_EX_INFOTIP | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_wndComparList.InsertColumn(0, _T("����"), LVCFMT_LEFT, 90);
	m_wndComparList.InsertColumn(1, _T("�Ա�"), LVCFMT_LEFT, 50);
	m_wndComparList.InsertColumn(2, _T("֤����"), LVCFMT_LEFT, 120);
	m_wndComparList.InsertColumn(3, _T("���ƶ�"), LVCFMT_LEFT, 60);
	m_wndComparList.InsertColumn(4, _T("��ַ"), LVCFMT_LEFT, 130);
	m_wndComparList.GetHeaderCtrl()->EnableWindow(FALSE);

	InitDbParameter();
	UpdatComputerTree();
	m_csSimilar = "50";
	m_csMaxSum = "50";
	UpdateData(FALSE);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFaceComparDlg::OnPaint()
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

		HDC hDC = GetDC()->m_hDC;
		Graphics g(hDC);
		//���ܱ���
		if (!m_csImgPath.IsEmpty())
		{
			USES_CONVERSION;
			WCHAR* pBuf = T2W((LPCTSTR)m_csImgPath);
			Image imgpeople(pBuf);
			g.DrawImage(&imgpeople, 8, 13, 165, 185);
		}
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	ofn.lpstrFilter = _T("*.jpg)\0*.jpg\0");//Ҫѡ����ļ���׺   
//	ofn.lpstrInitialDir = "D:\\";//Ĭ�ϵ��ļ�·��   
	ofn.lpstrInitialDir = sPath;//Ĭ�ϵ��ļ�·��   
	ofn.lpstrFile = szBuffer;//����ļ��Ļ�����   
	ofn.nMaxFile = sizeof(szBuffer) / sizeof(*szBuffer);
	ofn.nFilterIndex = 0;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;//��־����Ƕ�ѡҪ����OFN_ALLOWMULTISELECT  
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
		m_hRoot = m_ComputerTree.InsertItem(_T("������"));
		//���ÿ�����
		StructSetTreeData *pRootInfo = new StructSetTreeData();
		pRootInfo->csName = _T("������");
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
			//��ʾһ���IP
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
				//��ʾ����
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
		if (uFlags & TVHT_ONITEMSTATEICON)//�����ѡ��
		{
			m_ComputerTree.SelectItem(m_hComputer);
			if (!m_ComputerTree.GetCheck(m_hComputer))
			{
				hOtherlChild = m_ComputerTree.GetParentItem(m_hComputer);
			}
		}
		else if (uFlags & TVHT_ONITEMLABEL)//�������
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
//����һ�����ѡ
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
// 		(LPSTR)(LPCTSTR)csIP,	//һ���IP
// 		m_pSession,
// 		(LPSTR)(LPCTSTR)csID,  //�Ա�������ID
// 		m_pImage,																			 //��Ƭ��Ϣ
// 		(LPSTR)(LPCTSTR)m_csMaxSum,		//��󷵻ظ���
// 		(LPSTR)(LPCTSTR)m_csSimilar,		//�Ա����ƶ�
// 		&nConut,																					//���ظ���
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
//��������һ����µĿ�
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
				//�Ա�ͼƬ
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
	bi.lpszTitle = _T("ѡ����Ƭ:"); 
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
		MessageBox("��ʹ����ȷ��jpgͼƬ");
		return;
	}

 	m_csImgPath.Empty();
 //	OpenFileGetPath(m_csImgPath);
	m_csImgPath = strPath;
 	if (!m_csImgPath.IsEmpty())
 	{
 		//��ȡ��Ƭb64
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
		MessageBox("��ѡ����Ƭ");
		return;
	}

	if (m_csSimilar.IsEmpty() || m_csMaxSum.IsEmpty())
	{
		MessageBox("���ƶ�����󷵻ظ�������Ϊ��");
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
