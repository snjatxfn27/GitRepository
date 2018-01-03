
// BatchAddDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BatchAdd.h"
#include "BatchAddDlg.h"
#include "afxdialogex.h"
#include "AddServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define  PROGRAM_NAME			 "BatchAddError.log" 
#define  BLACK_TYPE		"1"
#define  WHITE_TYPE    "0"
#define  ISFACESERVER_ACTIVITY   1	//���÷��������ͻ��
#define  ISFACESERVER_IMMOBILI   0	//���÷��������͹̶���
#define  NATIONAL_SERVER  "[National]"
#define  NATIONAL_MYSQL   "National"
static _ConnectionPtr  g_pConnect;
static BOOL g_bFlag = TRUE;
static DEVICE_INFO * g_DeviceInfo;
static int g_DeviceSum = 0;//�豸����
static int g_RegistrationSum = 0;//ע�����
static int g_FlagSum = 0;//���ƹ���
static CString g_csSavePath;//�����ַ
static CWinThread * hGetImgPath;//�߳̾��
static CStringArray g_arRegistrationIndex;
UINT ThreadAddFace(LPVOID lparam);
UINT ThreadGetImgPath(LPVOID lparam);

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


// CBatchAddDlg �Ի���



CBatchAddDlg::CBatchAddDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CBatchAddDlg::IDD, pParent)
, m_csComName(_T(""))
, m_csComIdcard(_T(""))
, m_csComGender(_T(""))
, m_csComBitrhday(_T(""))
, m_csComnational(_T(""))
, m_csComAddress(_T(""))
, m_csComPhone(_T(""))
, m_csComSeparator(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBatchAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_BLACK, m_wndBlackTree);
	DDX_Control(pDX, IDC_TREE_WHITE, m_wndWhiteTree);
	DDX_Control(pDX, IDC_LIST1, m_wndList);
	DDX_Control(pDX, IDC_COMBO_NAME, m_wndComName);
	DDX_CBString(pDX, IDC_COMBO_NAME, m_csComName);
	DDX_Control(pDX, IDC_COMBO_IDCARD, m_wndComIdcard);
	DDX_CBString(pDX, IDC_COMBO_IDCARD, m_csComIdcard);
	DDX_Control(pDX, IDC_COMBO_GENDER, m_wndComGender);
	DDX_CBString(pDX, IDC_COMBO_GENDER, m_csComGender);
	DDX_Control(pDX, IDC_COMBO_BIRTHDAY, m_wndComBitrhday);
	DDX_CBString(pDX, IDC_COMBO_BIRTHDAY, m_csComBitrhday);
	DDX_Control(pDX, IDC_COMBO_NATIONAL, m_wndComNational);
	DDX_CBString(pDX, IDC_COMBO_NATIONAL, m_csComnational);
	DDX_Control(pDX, IDC_COMBO_ADDRESS, m_wndComAddress);
	DDX_CBString(pDX, IDC_COMBO_ADDRESS, m_csComAddress);
	DDX_Control(pDX, IDC_COMBO_PHONE, m_wndComPhone);
	DDX_CBString(pDX, IDC_COMBO_PHONE, m_csComPhone);
	DDX_Control(pDX, IDC_COMBO_SEPARATOR, m_wndComSeparator);
	DDX_CBString(pDX, IDC_COMBO_SEPARATOR, m_csComSeparator);
}

BEGIN_MESSAGE_MAP(CBatchAddDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONNECTDB, &CBatchAddDlg::OnBnClickedButtonConnectdb)
	ON_NOTIFY(NM_CLICK, IDC_TREE_BLACK, &CBatchAddDlg::OnNMClickTreeBlack)
	ON_NOTIFY(NM_CLICK, IDC_TREE_WHITE, &CBatchAddDlg::OnNMClickTreeWhite)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_BLACK, &CBatchAddDlg::OnNMRClickTreeBlack)
	ON_COMMAND(ID_ADDFACE, &CBatchAddDlg::OnAddface)
	ON_COMMAND(ID_DELETEFACE, &CBatchAddDlg::OnDeleteface)
	ON_COMMAND(ID_EDITFACE, &CBatchAddDlg::OnEditface)
	ON_COMMAND(ID_UPDATATREE, &CBatchAddDlg::OnUpdatatree)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_WHITE, &CBatchAddDlg::OnNMRClickTreeWhite)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, &CBatchAddDlg::OnBnClickedButtonSelect)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CBatchAddDlg::OnBnClickedButtonAdd)
	ON_WM_TIMER()
	ON_NOTIFY(LVN_GETINFOTIP, IDC_LIST1, &CBatchAddDlg::OnLvnGetInfoTipList1)
	ON_COMMAND(ID_ADDSERVER, &CBatchAddDlg::OnAddserver)
	ON_COMMAND(ID_EDITSERVER, &CBatchAddDlg::OnEditserver)
	ON_COMMAND(ID_DELETESERVER, &CBatchAddDlg::OnDeleteserver)
END_MESSAGE_MAP()


// CBatchAddDlg ��Ϣ�������

BOOL CBatchAddDlg::OnInitDialog()
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

	InitializeCriticalSection(&cSECTION);
	((CButton *)GetDlgItem(IDC_RADIO_SQLSERVER))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_RADIO_JING))->SetCheck(TRUE);
	GetDlgItem(IDC_STATIC_PATH)->SetWindowText("");
	GetDlgItem(IDC_STATIC_ZHUANGTAI)->SetWindowText("");

	m_wndComName.SetCurSel(1);
	m_wndComIdcard.SetCurSel(0);
	m_wndComGender.SetCurSel(0);
	m_wndComBitrhday.SetCurSel(0);
	m_wndComNational.SetCurSel(0);
	m_wndComAddress.SetCurSel(0);
	m_wndComPhone.SetCurSel(0);
	m_wndComSeparator.SetCurSel(0);

	m_bInitDb = FALSE;
	m_bFaceBorW = FALSE;
	m_bSelectPath = FALSE;
	m_bAddFalg = FALSE;

	m_wndList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_wndList.InsertColumn(0, _T("�ļ�"), LVCFMT_LEFT, 400);
	m_wndList.InsertColumn(1, _T("ʱ��"), LVCFMT_LEFT, 140);
	m_wndList.InsertColumn(2, _T("������"), LVCFMT_LEFT, 100);
	m_wndList.InsertColumn(3, _T("���"), LVCFMT_LEFT, 90);
	m_wndList.GetHeaderCtrl()->EnableWindow(FALSE);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CBatchAddDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBatchAddDlg::OnPaint()
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
HCURSOR CBatchAddDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//��ȡ�ļ����ڵ�ͼƬ·����ӵ��̵߳��ļ���������
UINT ThreadGetImgPath(LPVOID lparam)
{
	try
	{
		CBatchAddDlg *pWnd = (CBatchAddDlg *)AfxGetApp()->GetMainWnd();
		pWnd->BayesCategoryTest(pWnd->m_csImgPath);
	}
	catch (...)
	{

	}
	return 0;
}
//�����Ƭ��������
UINT ThreadAddFace(LPVOID lparam)
{
	int nIdex = (int)lparam;

	CBatchAddDlg *pWnd = (CBatchAddDlg *)AfxGetApp()->GetMainWnd();

	char pSession[37];
	char pPossoid[128];
	CString csName, csIdcard, csGender, csNational, csPhone, csAddr, csAge;
	int nAge;
	CString csBirthday = "1990-01-01";
	CString csValidTo = "2090-01-01";
	csGender = "1";
	csName = "1";
	csIdcard = "1";
	CString csPhotoPath, csDeviceIp;

	while (g_bFlag)
	{
		try
		{
			EnterCriticalSection(&cSECTION);
			if (g_DeviceInfo[nIdex].arFacePath.GetSize() <= 0)
			{
				LeaveCriticalSection(&cSECTION);
				Sleep(1000);
				continue;
			}
			LeaveCriticalSection(&cSECTION);
			memset(pPossoid, '\0', 128);
			int nSize = g_DeviceInfo[nIdex].arFacePath.GetSize();

			//��ȡ�����ļ��ĵ�ַ
			EnterCriticalSection(&cSECTION);
			csPhotoPath = g_DeviceInfo[nIdex].arFacePath.GetAt(0);
			g_DeviceInfo[nIdex].arFacePath.RemoveAt(0);
			csDeviceIp = g_DeviceInfo[nIdex].csDeviceIp;
			LeaveCriticalSection(&cSECTION);

			csPhotoPath.Replace("\\", "//");
			//��¼
			int nResult = DeviceLogin(csDeviceIp, pSession);
			if (nResult != 0)
				continue;

			//����Ƭ��ȡFace
			int nLen = 1024 * 1024 * 3;
			char *pJson = new char[nLen * 3];
			memset(pJson, '\0', nLen * 3);
			nResult = FASFeatureDetectforImageFile(csDeviceIp, pSession, csPhotoPath, pJson, nLen);
			if (nResult != 0)
			{
				//move file
				CString csTemp;
				csTemp.Format("��ȡFeatureʧ��_%s", csPhotoPath);
				DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csTemp, PROGRAM_NAME);
				pWnd->SetListData(csPhotoPath, csTemp, csDeviceIp);
				pWnd->MoveImg(csPhotoPath, 2);
				continue;
			}

			//��ȡImg ��face ��b64
			Json::Reader reader;
			Json::Value root;
			Json::Value result;
			Json::Value content;
			BOOL readre = reader.parse(pJson, root);
			delete[] pJson;
			if (!readre)//jsonʧ��
			{
				//move file
				CString csTemp;
				csTemp.Format("jsonʧ��_%s", csPhotoPath);
				DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csTemp, PROGRAM_NAME);
				pWnd->SetListData(csPhotoPath, csTemp, csDeviceIp);
				pWnd->MoveImg(csPhotoPath, 2);
				continue;
			}

			int iZero = 0;
			result = root["data"]["result"][iZero];
			if (result.isNull())//û����ȡ�ɹ�
			{
				//move file
				CString csTemp;
				csTemp.Format("jsonΪNULL_%s", csPhotoPath);
				DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csTemp, PROGRAM_NAME);
				pWnd->SetListData(csPhotoPath, csTemp, csDeviceIp);
				pWnd->MoveImg(csPhotoPath, 2);
				continue;
			}
			else
			{
				CString strFeatures = result["feature"].asCString();
				CString csImageData = result["face_image"].asCString();
				nAge = result["age"].asInt();
				csAge.Format("%d", nAge);
				//ȡ�ļ���
				int nFind = csPhotoPath.ReverseFind('//');
				CString csFaceName = csPhotoPath.Mid(nFind + 1, csPhotoPath.GetLength() - nFind - 5);
				CString csStartPath = csPhotoPath.Mid(0, nFind - 1);
				CString csFeaturePath = csPhotoPath;
				csFeaturePath.Replace(".jpg", ".txt");

				CFile file;
				if (file.Open(csFeaturePath, CFile::modeCreate | CFile::modeWrite))
				{
					file.Write(strFeatures, strFeatures.GetLength());
					file.Close();
				}
				else
				{
					CString csTemp;
					csTemp.Format("дFeatureʧ��_%s", csPhotoPath);
					DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csTemp, PROGRAM_NAME);
					pWnd->SetListData(csPhotoPath, csTemp, csDeviceIp);
					pWnd->MoveImg(csPhotoPath, 2);
					continue;
				}

				CStringArray arFaceName;
				if (g_FlagSum >= 2)
				{
					int nBreak = __BreakCString(csFaceName, arFaceName, pWnd->m_csComSeparator);
					if (nBreak != g_FlagSum)
					{
						//move file
						CString csTemp;
						csTemp.Format("������ʧ��_%s", csPhotoPath);
						DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csTemp, PROGRAM_NAME);
						pWnd->SetListData(csPhotoPath, csTemp, csDeviceIp);
						pWnd->MoveImg(csFeaturePath, 2);
						pWnd->MoveImg(csPhotoPath, 2);
						continue;
					}

					if (atoi(pWnd->m_csComName) != 0 && atoi(pWnd->m_csComIdcard) != 0)
					{
						csName = arFaceName.GetAt(atoi(pWnd->m_csComName));
						csIdcard = arFaceName.GetAt(atoi(pWnd->m_csComIdcard));
					}
					else if (atoi(pWnd->m_csComName) == 0)
					{
						csIdcard = arFaceName.GetAt(atoi(pWnd->m_csComIdcard));
						csName = csIdcard;
					}
					else if (atoi(pWnd->m_csComIdcard) == 0)
					{
						csName = arFaceName.GetAt(atoi(pWnd->m_csComName));
						csIdcard = csName;
					}

					if (atoi(pWnd->m_csComGender) != 0)
					{
						csGender = arFaceName.GetAt(atoi(pWnd->m_csComGender));
					}

					if (atoi(pWnd->m_csComBitrhday) != 0)
					{
						csBirthday = arFaceName.GetAt(atoi(pWnd->m_csComBitrhday));
					}

					if (atoi(pWnd->m_csComnational) != 0)
					{
						csNational = arFaceName.GetAt(atoi(pWnd->m_csComnational));
					}

					if (atoi(pWnd->m_csComAddress) != 0)
					{
						csAddr = arFaceName.GetAt(atoi(pWnd->m_csComAddress));
					}

					if (atoi(pWnd->m_csComPhone) != 0)
					{
						csPhone = arFaceName.GetAt(atoi(pWnd->m_csComPhone));
					}
				}
				else if (g_FlagSum == 1)
				{
					csName = csFaceName;
					csIdcard = csName;
				}
				//ע��
				nResult = FASAddPersoninfo(csDeviceIp, pSession, "1", "1", atoi(csGender), atof(g_DeviceInfo[nIdex].csFacedb), strFeatures, csImageData, csBirthday, csValidTo, pPossoid, 128);
				if (nResult != 0)
				{
					CString csTemp;
					csTemp.Format("ע��ʧ��_%s", csPhotoPath);
					DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csTemp, PROGRAM_NAME);
					pWnd->SetListData(csPhotoPath, csTemp, csDeviceIp);
					pWnd->MoveImg(csFeaturePath, 2);
					pWnd->MoveImg(csPhotoPath, 2);
					continue;
				}
				CString csPresentId(pPossoid);

				CString csImgPath;
				csImgPath.Format("%s//%s.jpg", csStartPath, csPresentId);//��ȡ����ͼ
				int nImgLen = csImageData.GetLength();
				BYTE *impDst = new BYTE[nImgLen];
				//���벢��ʾ
				int nLen = __DecodeBase64(csImageData, impDst, nImgLen);
				if (nLen <= 0)
				{
					CString csTemp;
					csTemp.Format("������Ƭ����ʧ��_%s", csPhotoPath);
					DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csTemp, PROGRAM_NAME);
					nResult = FASDELETEPersoninfo(csDeviceIp, pSession, pPossoid);
					if (nResult != 0)
					{

					}
					pWnd->SetListData(csPhotoPath, csTemp, csDeviceIp);
					pWnd->MoveImg(csFeaturePath, 2);
					pWnd->MoveImg(csPhotoPath, 2);
					continue;
				}
				CFile fileJPG;
				if (fileJPG.Open(csImgPath, CFile::modeCreate | CFile::modeWrite))
				{
					fileJPG.Write(impDst, nLen);
					fileJPG.Close();
				}
				else
				{
					CString csTemp;
					csTemp.Format("������Ƭд��ʧ��_%s", csPhotoPath);
					DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csTemp, PROGRAM_NAME);
					nResult = FASDELETEPersoninfo(csDeviceIp, pSession, pPossoid);
					if (nResult != 0)
					{

					}
					pWnd->SetListData(csPhotoPath, csTemp, csDeviceIp);
					pWnd->MoveImg(csFeaturePath, 2);
					pWnd->MoveImg(csPhotoPath, 2);
					delete[] impDst;
					continue;
				}
				delete[] impDst;

				BOOL bResult = pWnd->SaveToSqlSrver(g_DeviceInfo[nIdex].csFacedb, csPresentId, csName, csGender, csBirthday, csAge, csNational, csPhone, csAddr, g_DeviceInfo[nIdex].csDeviceId, csIdcard, csPresentId);
				if (!bResult)
				{
					//move file
					nResult = FASDELETEPersoninfo(csDeviceIp, pSession, pPossoid);
					if (nResult != 0)
					{

					}
					CString csTemp;
					csTemp.Format("���ݿ�洢ʧ��_%s", csPhotoPath);
					DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 1, csTemp, PROGRAM_NAME);
					pWnd->SetListData(csPhotoPath, csTemp, csDeviceIp);
					pWnd->MoveImg(csFeaturePath, 2);
					pWnd->MoveImg(csPhotoPath, 2);
					pWnd->MoveImg(csImgPath, 2);

					continue;
				}
				pWnd->MoveImg(csImgPath, 0);
				pWnd->MoveImg(csPhotoPath, 3);
				pWnd->MoveImg(csFeaturePath, 1);
				pWnd->SetListData(csPhotoPath, "ע��ɹ�", csDeviceIp);

				DeviceLogout(csDeviceIp, pSession);
			}
		}
		catch (...)
		{
		}
	}
	return 0;
}
//����mysql��prelist_��ID_��� �� img ��
BOOL CBatchAddDlg::CreateTableToMySql(int nIndex, CString csDeviceId)
{
	try
	{
		CString csSql, csTableName;
		csTableName.Format("PersonsList_%s_%d", csDeviceId, nIndex);
		csSql.Format("CREATE TABLE if not exists %s(\
		PersonId int(11) primary key auto_increment not null,\
		FaceDBId int(11)   not null,\
		FaceDBPersonId varchar(255) not null,\
		Name varchar(255) not null,\
		Gender int(11)   not null,\
		Birthday datetime not null,\
		Age int(11)   not null,\
		IdCard varchar(255) not null,\
		National varchar(255) not null,\
		Phone varchar(255) not null,\
		PresentAddress varchar(255) not null,\
		CreateTime datetime not null,\
		DelFlag int(11)   not null,\
		Remark varchar(255) not null,\
		DeviceId int(11)   not null)", \
		csTableName);
		g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);

		csTableName.Format("PhotoInfo_%s_%d", csDeviceId, nIndex);
		csSql.Format("CREATE TABLE if not exists %s\
		(PhotoId int(11)  primary key auto_increment not null,\
		PersonId int(11) not null,\
		Type int(11) not null,\
		IdCard varchar(255) not null,\
		PhotoPath varchar(255)   not null,\
		FacePhotoPath  varchar(255) not null,\
		Feature varchar(255)  not null,\
		DelFlag int(11) not null,\
		CreateTime datetime not null,\
		DeviceId int(11) not null)", \
		csTableName);
		g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);

	}
	catch (...)
	{
		return FALSE;
	}

	return TRUE;
}
//����sqlserver ��prelist_��ID_��� �� img ��
BOOL CBatchAddDlg::CreateListAndImgToSqlServer(int nIndex, CString csDeviceId)
{
	try
	{
		CString csSql, csTableName;
		csTableName.Format("PersonsList_%s_%d", csDeviceId, nIndex);
		csSql.Format("IF NOT EXISTS(SELECT * FROM sys.Tables WHERE name='%s') CREATE TABLE %s\
		(PersonId INT IDENTITY(1,1) PRIMARY KEY  NOT NULL,\
		FaceDBId INT  not null,\
		FaceDBPersonId varchar(255) not null,\
		Name varchar(255) not null,\
		Gender INT not null,\
		Birthday datetime not null,\
		Age INT not null,\
		IdCard varchar(255) not null,\
		[National] varchar(255) not null,\
		Phone varchar(255) not null,\
		PresentAddress varchar(255) not null,\
		CreateTime datetime not null,\
		DelFlag INT not null,\
		Remark varchar(255) not null,\
		DeviceId INT not null)", \
		csTableName, csTableName);
		g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);


		csTableName.Format("PhotoInfo_%s_%d", csDeviceId, nIndex);
		csSql.Format("IF NOT EXISTS(SELECT * FROM sys.Tables WHERE name='%s') CREATE TABLE %s\
		(PhotoId INT  IDENTITY(1,1)  PRIMARY KEY  NOT NULL,\
		PersonId INT not null,\
		Type INT not null,\
		IdCard varchar(255) not null,\
		PhotoPath varchar(255)   not null,\
		FacePhotoPath  varchar(255) not null,\
		Feature varchar(255)  not null,\
		DelFlag INT not null,\
		CreateTime datetime not null,\
		DeviceId INT not null)", \
		csTableName, csTableName);
		g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);

	}
	catch (...)
	{
		return FALSE;
	}

	return TRUE;
}
//��ȡ���ڵ�����
int CBatchAddDlg::GetTableSum(CString csTableName)
{
	try
	{
		CString csSql;
		_RecordsetPtr pRecordset;
		csSql.Format("SELECT count(*) as num  FROM %s ;", csTableName);
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		if (!pRecordset->adoEOF)
		{
			CString csNum = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("num")->Value;
			return atoi(csNum);
		}
		else
		{
			return -1;
		}
	}
	catch (...)
	{
		return -1;
	}
	return -1;
}
//��PersonsListCount���������������豸id
BOOL CBatchAddDlg::InsertPersonsListCount(CString csTableName, int nIndex, CString csDeviceId)
{
	try
	{
		CString csSql;
		csSql.Format("insert into PersonsListCount (DataBaseName, Range_Start, Range_End ,PersonsIndex,UsedCount,DeviceId)values('%s','%s','%s','%d','%s','%s')", csTableName, "0", "0", nIndex, "0", csDeviceId);
		g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		return TRUE;
	}
	catch (...)
	{
		return FALSE;
	}
	return FALSE;
}
//��ȡPersonsListCount����device��ǰʹ�õ����
BOOL CBatchAddDlg::GetPersonsListName(CString csDeviceId, int & PersonsIndex)
{
	try
	{
		CString csSql, csTableName;
		_RecordsetPtr pRecordset;
		csSql.Format("SELECT count(*) as num  FROM PersonsListCount where DeviceId = '%s'", csDeviceId);
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		int nGetFaceNum;
		if (!pRecordset->adoEOF)
		{
			CString csFaceNum = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("num")->Value;
			nGetFaceNum = atoi(csFaceNum);
			if (nGetFaceNum == 0)
			{
				PersonsIndex = 1;

				csTableName.Format("PersonsList_%s_%d", csDeviceId, PersonsIndex);
				if (!InsertPersonsListCount(csTableName, PersonsIndex, csDeviceId))
					return FALSE;

				if (!((CButton *)GetDlgItem(IDC_RADIO_MYSQL))->GetCheck())
					return CreateListAndImgToSqlServer(PersonsIndex, csDeviceId);
				else
					return CreateTableToMySql(PersonsIndex, csDeviceId);
			}
			else
			{
				csSql.Format("SELECT max(PersonsIndex) as nindex  FROM PersonsListCount where DeviceId = '%s'", csDeviceId);
				pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
				if (!pRecordset->adoEOF)
				{
					CString csIndex = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("nindex")->Value;

					csSql.Format("SELECT *  FROM PersonsListCount where PersonsIndex = '%s' and DeviceId = '%s'", csIndex, csDeviceId);
					pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
					if (!pRecordset->adoEOF)
					{
						CString csUsedConut = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("UsedCount")->Value;
						PersonsIndex = atoi(csIndex);

						if (atoi(csUsedConut) >= m_nContrastServerDB_Capacity)
						{
							PersonsIndex += 1;
							csTableName.Format("PersonsList_%s_%d", csDeviceId, PersonsIndex);
							if (!InsertPersonsListCount(csTableName, PersonsIndex, csDeviceId))
								return FALSE;

							if (!((CButton *)GetDlgItem(IDC_RADIO_MYSQL))->GetCheck())
								return CreateListAndImgToSqlServer(PersonsIndex, csDeviceId);
							else
								return CreateTableToMySql(PersonsIndex, csDeviceId);
						}

						return TRUE;
					}
				}
			}
		}
	}
	catch (...)
	{
		return FALSE;
	}

	return TRUE;
}
//�洢ע�����Ա�����ݿ�
BOOL CBatchAddDlg::SaveToSqlSrver(CString csFaceBdId, CString csFaceDbPersonId, CString csName, CString csGender, CString csBirthday, CString csAge, CString csNational, CString csPhone, CString csAddr, CString csDeviceID, CString csIdcard, CString csPresentId)
{
	int  nFlag = 0; //�����жϱ�־
	int  PersonsIndex;
	CString csPersonId;
	try
	{
		CString csTableName, csSql;
		_RecordsetPtr pRecordset;
		//��ȡ����ı�������������Դ����������趨ֵ���½�
		if (!GetPersonsListName(csDeviceID, PersonsIndex))
			return FALSE;

		CString  csImgEndPath, csPhotoPath, csEndFeaturePath;
		csEndFeaturePath.Format("%s//imgCurFaceFeature//%s.txt", g_csSavePath, csName);
		csPhotoPath.Format("%s//imgPhotoJpg//%s.jpg", g_csSavePath, csName);//ԭ��Ƭ
		csImgEndPath.Format("%s//imgCurFaceJpg//%s.jpg", g_csSavePath, csPresentId);//������Ƭ

		COleDateTime coTime;
		coTime = COleDateTime::GetCurrentTime();
		CString csTime = coTime.Format("%Y-%m-%d %H:%M:%S");

		//��ȡ��ǰʹ���������жϸ���PersonsListCount prid�ĸ����ֶ�
		csTableName.Format("PersonsList_%s_%d", csDeviceID, PersonsIndex);
		int nTableSumStart = GetTableSum(csTableName);
		//����List
		if (((CButton *)GetDlgItem(IDC_RADIO_MYSQL))->GetCheck())
		{
			csSql.Format("insert into PersonsList_%s_%d (FaceDBId, FaceDBPersonId, Name, Gender, Birthday, Age, IdCard,National, Phone, PresentAddress, CreateTime, DelFlag, Remark, DeviceId)values('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')", csDeviceID, PersonsIndex, csFaceBdId, csFaceDbPersonId, csName, csGender, csBirthday, csAge, csIdcard, csNational, csPhone, csAddr, csTime, "0", "", csDeviceID);
		}
		else
		{
			csSql.Format("insert into PersonsList_%s_%d (FaceDBId, FaceDBPersonId, Name, Gender, Birthday, Age, IdCard,[National], Phone, PresentAddress, CreateTime, DelFlag, Remark, DeviceId)values('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')", csDeviceID, PersonsIndex, csFaceBdId, csFaceDbPersonId, csName, csGender, csBirthday, csAge, csIdcard, csNational, csPhone, csAddr, csTime, "0", "", csDeviceID);
		}
		g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);

		nFlag = 1;

		//��ȡlist��id
		CString  csType;
		csSql.Format("SELECT * from PersonsList_%s_%d where Name = '%s' and IdCard  = '%s' and FaceDBPersonId = '%s' and DeviceId = '%s'", csDeviceID, PersonsIndex, csName, csIdcard, csFaceDbPersonId, csDeviceID);
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		if (!pRecordset->adoEOF)
			csPersonId = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("PersonId")->Value;
		else
			return FALSE;

		if (m_bFaceBorW)
			csType = "1";
		else
			csType = "0";

		//��img��
		csSql.Format("insert into PhotoInfo_%s_%d(PersonId, Type,IdCard ,PhotoPath ,FacePhotoPath,Feature,DelFlag,CreateTime,DeviceId)values('%s','%s','%s','%s','%s','%s','%s','%s','%s')", csDeviceID, PersonsIndex, csPersonId, csType, csIdcard, csPhotoPath, csImgEndPath,  csEndFeaturePath, "0", csTime, csDeviceID);
		g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		nFlag = 2;

		//��ȡ��ǰ���ڴ洢���� PersonsListCount ���е�ʹ������С
		csTableName.Format("PersonsList_%s_%d", csDeviceID, PersonsIndex);
		int nTableSum = GetTableSum(csTableName);
		csSql.Format("UPDATE PersonsListCount SET UsedCount = '%d' where PersonsIndex = '%d' and  DeviceId ='%s';", nTableSum, PersonsIndex, csDeviceID);
		g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);

		//���±���pressid�ķ�Χ
		if (nTableSumStart == 0)
		{
			csSql.Format("UPDATE PersonsListCount SET Range_Start = '%s' where PersonsIndex = '%d' and  DeviceId ='%s';", csFaceDbPersonId, PersonsIndex, csDeviceID);
		}
		else
		{
			csSql.Format("UPDATE PersonsListCount SET Range_End = '%s' where PersonsIndex = '%d' and  DeviceId ='%s';", csFaceDbPersonId, PersonsIndex, csDeviceID);
		}
		g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);

		return TRUE;
	}
	catch (...)
	{
		if (nFlag == 1)
		{
			if (!csPersonId.IsEmpty())
			{
				CString csSql;
				csSql.Format("delete from PersonsList_%s_%d where PersonId = '%s'", csDeviceID, PersonsIndex, csPersonId);
				g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
			}
			else
			{
				CString csSql;
				CString  csType;
				csSql.Format("delete from PersonsList_%s_%d where Name = '%s' and IdCard  = '%s' and FaceDBPersonId = '%s' and DeviceId = '%s'", csDeviceID, PersonsIndex, csName, csIdcard, csFaceDbPersonId, csDeviceID);
				g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
			}
		}
		return FALSE;
	}
	return TRUE;
}
//��ȡ�����ļ�
BOOL CBatchAddDlg::GetParameterIni()
{
	try
	{
		int nError;
		CString csIniPath;
		if (((CButton *)GetDlgItem(IDC_RADIO_MYSQL))->GetCheck())
			csIniPath = "./Parameter_MySql.ini";
		else
			csIniPath = "./Parameter_SqlServer.ini";

		nError = GetPrivateProfileString(("SqlConfiguration"), "Sql_ServerAdd", (""), m_csContrastServerDB_Ip.GetBuffer(MAX_PATH), MAX_PATH, csIniPath);
		if (nError <= 0)
		{
			return FALSE;
		}
		nError = GetPrivateProfileString(("SqlConfiguration"), "Sql_Database", (""), m_csContrastServerDB_Database.GetBuffer(MAX_PATH), MAX_PATH, csIniPath);
		if (nError <= 0)
		{
			return FALSE;
		}
		nError = GetPrivateProfileString(("SqlConfiguration"), "Sql_Name", (""), m_csContrastServerDB_Name.GetBuffer(MAX_PATH), MAX_PATH, csIniPath);
		if (nError <= 0)
		{
			return FALSE;
		}

		nError = GetPrivateProfileString(("SqlConfiguration"), "Sql_Pwd", (""), m_csContrastServerDB_Pwd.GetBuffer(MAX_PATH), MAX_PATH, csIniPath);
		if (nError <= 0)
		{
			return FALSE;
		}
		nError = GetPrivateProfileString(("SqlConfiguration"), "Sql_Capacity ", (""), m_csContrastServerDB_Capacity.GetBuffer(MAX_PATH), MAX_PATH, csIniPath);
		if (nError <= 0)
		{
			return FALSE;
		}
		m_nContrastServerDB_Capacity = atoi(m_csContrastServerDB_Capacity);
	}
	catch (...)
	{
		return FALSE;
	}
	return TRUE;
}
//��ʼ�����ݿ�����
BOOL CBatchAddDlg::InitDatabase()
{
	try
	{
		CString csSqlConnect;
		if (!((CButton *)GetDlgItem(IDC_RADIO_MYSQL))->GetCheck())
		{
			csSqlConnect.Format("Driver={sql server};server=%s;uid=%s;pwd=%s;database=%s;",
				m_csContrastServerDB_Ip, m_csContrastServerDB_Name, m_csContrastServerDB_Pwd, m_csContrastServerDB_Database);
		}
		else
		{
			csSqlConnect.Format("Driver={MySQL ODBC 5.1 Driver};Server=%s;Database=%s;uid=%s;pwd=%s;",
				m_csContrastServerDB_Ip, m_csContrastServerDB_Database, m_csContrastServerDB_Name, m_csContrastServerDB_Pwd);
		}

		g_pConnect.CreateInstance(__uuidof(Connection));
		g_pConnect->CursorLocation = adUseClient; //�α�����     
		_bstr_t strConnect = csSqlConnect;
		g_pConnect->Open(strConnect, "", "", adModeUnknown);
	}
	catch (_com_error &e)
	{
		CString csError = e.Description();
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 0, csError, PROGRAM_NAME);
		return FALSE;
	}
	return  TRUE;
}
//��ȡdevice�������ݴ����ڽṹ���ڲ��������̵߳ȴ�ע��
BOOL CBatchAddDlg::InitDeviceInfo()
{
	try
	{
		CString csSql;
		int nDeviceIndex = 0;
		_RecordsetPtr pRecordset;

		if (g_DeviceInfo != NULL)
		{
			g_bFlag = FALSE;
			for (int i = 0; i < g_DeviceSum;i++)
			{
				WaitForSingleObject(g_DeviceInfo[i].hThand->m_hThread, INFINITE);
			}
			delete[]g_DeviceInfo;
		}
		if (((CButton*)GetDlgItem(IDC_RADIO_DAN))->GetCheck())
			csSql.Format("select * from Device where  IsFaceServer = '%d' and DelFlag = '0';", ISFACESERVER_ACTIVITY);
		if (((CButton*)GetDlgItem(IDC_RADIO_JING))->GetCheck())
			csSql.Format("select * from Device where  IsFaceServer = '%d' and DelFlag = '0';", ISFACESERVER_IMMOBILI);


		g_DeviceSum = _GetExecuteResults(pRecordset, g_pConnect, csSql);
		if (g_DeviceSum == 0)
			return FALSE;
	
		g_DeviceInfo = new DEVICE_INFO[g_DeviceSum];
		g_bFlag = TRUE;

		while (!pRecordset->adoEOF)
		{
			g_DeviceInfo[nDeviceIndex].nIndex = nDeviceIndex;
			g_DeviceInfo[nDeviceIndex].csDeviceId = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("DeviceId")->Value;
			g_DeviceInfo[nDeviceIndex].DeviceName = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("DeviceName")->Value;
			g_DeviceInfo[nDeviceIndex].csDescription = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("Description")->Value;
			g_DeviceInfo[nDeviceIndex].csOnlyId = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("OnlyId")->Value;
			g_DeviceInfo[nDeviceIndex].csDeviceIp = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("IP")->Value;
			g_DeviceInfo[nDeviceIndex].csType = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("Type")->Value;
			g_DeviceInfo[nDeviceIndex].csIsFaceServer = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("IsFaceServer")->Value;
			g_DeviceInfo[nDeviceIndex].csAddr = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("Addr")->Value;
			g_DeviceInfo[nDeviceIndex].csRemark = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("Remark")->Value;
			g_DeviceInfo[nDeviceIndex].csWhiteFacedbs = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("WhiteFaceDbs")->Value;
			g_DeviceInfo[nDeviceIndex].csBlaekFacedbs = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("BlackFaceDbs")->Value;

			CString csOnline = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("IsOnline")->Value;
			g_DeviceInfo[nDeviceIndex].bIsonline = csOnline == "1" ? TRUE : FALSE;
			g_DeviceInfo[nDeviceIndex].bIsStatic = g_DeviceInfo[nDeviceIndex].csIsFaceServer == "0" ? TRUE : FALSE;
			g_DeviceInfo[nDeviceIndex].bIsWork = FALSE;

			if (!g_DeviceInfo[nDeviceIndex].csWhiteFacedbs.IsEmpty())
			{
				if (!GetDeviceFaceNmae(g_DeviceInfo[nDeviceIndex].csDeviceId, nDeviceIndex, g_DeviceInfo[nDeviceIndex].csWhiteFacedbs, 0))
				{
					delete[]g_DeviceInfo;
					return FALSE;
				}
			}

			if (!g_DeviceInfo[nDeviceIndex].csBlaekFacedbs.IsEmpty())
			{
				if (!GetDeviceFaceNmae(g_DeviceInfo[nDeviceIndex].csDeviceId, nDeviceIndex, g_DeviceInfo[nDeviceIndex].csBlaekFacedbs, 1))
				{
					delete[]g_DeviceInfo;
					return FALSE;
				}
			}

			g_DeviceInfo[nDeviceIndex].hThand = AfxBeginThread(&ThreadAddFace, (LPVOID)nDeviceIndex);
			nDeviceIndex++;
			pRecordset->MoveNext();
		}
	}
	catch (_com_error &e)
	{
		delete[]g_DeviceInfo;
		CString csError = e.Description();
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 0, csError, PROGRAM_NAME);
		return FALSE;
	}

	return TRUE;
}
//��device���ڵ�faces��ִ���ṹ����
BOOL CBatchAddDlg::GetDeviceFaceNmae(CString csDeviceId, int nDeviceIndex, CString csFacedbs, int nFaceType)
{
	try
	{
		CString csSql;
		_RecordsetPtr pRecordset;
		int nGetFaceNum = 0;
		CStringArray arFacedbs;
		int nFaceNum = __BreakCString(csFacedbs, arFacedbs, "+");
		if (nFaceType == 0)
		{
			g_DeviceInfo[nDeviceIndex].nWhiteFacedbs = nFaceNum;
		}
		else if (nFaceType == 1)
		{
			g_DeviceInfo[nDeviceIndex].nBlaekFacedbs = nFaceNum;
		}
		if (nFaceNum == 0)
			return TRUE;

		for (int i = 0; i < nFaceNum; i++)
		{
			csSql.Format("SELECT * FROM facedb WHERE DeviceId = '%s' and FaceDBFaceDBId = '%s' and Type = '%d'and DelFlag = '0'",
				csDeviceId, arFacedbs.GetAt(i), nFaceType);
			nGetFaceNum = _GetExecuteResults(pRecordset, g_pConnect, csSql);
			if (nGetFaceNum == 0)
				return FALSE;

			CString csFaceName = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("Name")->Value;

			if (nFaceType == 0)
			{
				g_DeviceInfo[nDeviceIndex].mapWhiteFacedb.insert(pair< CString, CString>(arFacedbs.GetAt(i), csFaceName));
			}
			else if (nFaceType == 1)
			{
				g_DeviceInfo[nDeviceIndex].mapBlackFacedb.insert(pair< CString, CString>(arFacedbs.GetAt(i), csFaceName));
			}
		}

	}
	catch (_com_error &e)
	{
		CString csError = e.Description();
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 0, csError, PROGRAM_NAME);
		return FALSE;
	}

	return TRUE;
}
//�������ݿ�
void CBatchAddDlg::OnBnClickedButtonConnectdb()
{
	UpdateData();

	if (!GetParameterIni())
	{
		MessageBox("�����ļ���ȡʧ��", "��ʾ");
		return;
	}

	if (!InitDatabase())
	{
		MessageBox("���ݿ�����ʧ��", "��ʾ");
	}
	else
	{
		if (!InitDeviceInfo())
		{
			MessageBox("��ȡһ�����Ϣʧ��", "��ʾ");
			return;
		}
		ShowBalackTree();
		ShowWhiteTree();
		m_bInitDb = TRUE;
		//GetDlgItem(IDC_BUTTON_CONNECTDB)->EnableWindow(FALSE);
	}
}
//��ʾ��������
BOOL CBatchAddDlg::ShowBalackTree()
{
	try
	{
		m_wndBlackTree.DeleteAllItems();
		m_hBlackRoot = m_wndBlackTree.InsertItem("������������");
		THREE_INFO *pRootInfo = new THREE_INFO();
		pRootInfo->nFlag = 0;
		m_wndBlackTree.SetItemData(m_hBlackRoot, (DWORD)pRootInfo);

		for (int i = 0; i < g_DeviceSum; i++)
		{
			HTREEITEM hIpTerr = m_wndBlackTree.InsertItem(g_DeviceInfo[i].csDeviceIp, m_hBlackRoot);
			THREE_INFO *pIpInfo = new THREE_INFO();
			pIpInfo->nFlag = 1;
			pIpInfo->csDeviceIp = g_DeviceInfo[i].csDeviceIp;
			pIpInfo->csDeviceId = g_DeviceInfo[i].csDeviceId;
			pIpInfo->nDeviceIndex = i;
			m_wndBlackTree.SetItemData(hIpTerr, (DWORD)pIpInfo);

			map<CString, CString>::iterator iter;
			for (iter = g_DeviceInfo[i].mapBlackFacedb.begin(); iter != g_DeviceInfo[i].mapBlackFacedb.end(); ++iter)
			{
				HTREEITEM hFaceTerr = m_wndBlackTree.InsertItem(iter->second, hIpTerr);
				THREE_INFO *pFaceInfo = new THREE_INFO();
				pFaceInfo->csFaceID = iter->first;
				pFaceInfo->csType = BLACK_TYPE;
				pFaceInfo->csDeviceIp = g_DeviceInfo[i].csDeviceIp;
				pFaceInfo->csDeviceId = g_DeviceInfo[i].csDeviceId;
				pFaceInfo->DeviceName = iter->second;
				pFaceInfo->nFlag = 2;
				pFaceInfo->nDeviceIndex = i;
				m_wndBlackTree.SetItemData(hFaceTerr, (DWORD)pFaceInfo);
			}
		}
		__ExpandAllNode(m_hBlackRoot, m_wndBlackTree);
	}
	catch (...)
	{
		return FALSE;
	}
	return TRUE;
}
//��ʾ��������
BOOL CBatchAddDlg::ShowWhiteTree()
{
	try
	{
		m_wndWhiteTree.DeleteAllItems();
		m_hWhiteRoot = m_wndWhiteTree.InsertItem("������������");
		THREE_INFO *pRootInfo;
		pRootInfo = new THREE_INFO();
		pRootInfo->nFlag = 0;
		m_wndWhiteTree.SetItemData(m_hWhiteRoot, (DWORD)pRootInfo);

		for (int i = 0; i < g_DeviceSum; i++)
		{
			HTREEITEM hIpTerr = m_wndWhiteTree.InsertItem(g_DeviceInfo[i].csDeviceIp, m_hWhiteRoot);
			THREE_INFO *pIpInfo = new THREE_INFO();
			pIpInfo->nFlag = 1;
			pIpInfo->csDeviceIp = g_DeviceInfo[i].csDeviceIp;
			pIpInfo->csDeviceId = g_DeviceInfo[i].csDeviceId;
			m_wndWhiteTree.SetItemData(hIpTerr, (DWORD)pIpInfo);

			map<CString, CString>::iterator iter;
			for (iter = g_DeviceInfo[i].mapWhiteFacedb.begin(); iter != g_DeviceInfo[i].mapWhiteFacedb.end(); ++iter)
			{
				HTREEITEM hFaceTerr = m_wndWhiteTree.InsertItem(iter->second, hIpTerr);
				THREE_INFO *pFaceInfo = new THREE_INFO();
				pFaceInfo->csFaceID = iter->first;
				pFaceInfo->csType = WHITE_TYPE;
				pFaceInfo->csDeviceIp = g_DeviceInfo[i].csDeviceIp;
				pFaceInfo->csDeviceId = g_DeviceInfo[i].csDeviceId;
				pFaceInfo->DeviceName = iter->second;
				pFaceInfo->nFlag = 2;
				pFaceInfo->nDeviceIndex = i;

				m_wndWhiteTree.SetItemData(hFaceTerr, (DWORD)pFaceInfo);
			}
		}
		__ExpandAllNode(m_hWhiteRoot, m_wndWhiteTree);
	}
	catch (...)
	{
		return FALSE;
	}
	return TRUE;
}
//�����������
void CBatchAddDlg::OnNMClickTreeBlack(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPoint pt;
	GetCursorPos(&pt);
	m_wndBlackTree.ScreenToClient(&pt);
	UINT uFlags = 0;
	HTREEITEM hChildSelect = NULL;
	m_hBlackSelect = m_wndBlackTree.HitTest(pt, &uFlags);
	if (NULL != m_hBlackSelect)
	{
		m_bFaceBorW = TRUE;

		if (uFlags & TVHT_ONITEMSTATEICON)//�����ѡ��
		{
			m_wndBlackTree.SelectItem(m_hBlackSelect);
			if (!m_wndBlackTree.GetCheck(m_hBlackSelect))
			{
				hChildSelect = m_wndBlackTree.GetParentItem(m_hBlackSelect);
			}
		}
		else if (uFlags & TVHT_ONITEMLABEL)//�������
		{
			if (!m_wndBlackTree.GetCheck(m_hBlackSelect))
			{
				m_wndBlackTree.SetCheck(m_hBlackSelect, TRUE);
				hChildSelect = m_wndBlackTree.GetParentItem(m_hBlackSelect);
			}
			else
			{
				m_wndBlackTree.SetCheck(m_hBlackSelect, FALSE);
			}
		}

		if (NULL != hChildSelect && hChildSelect != m_hBlackRoot)
		{
			if (!m_wndBlackTree.GetCheck(hChildSelect))
				m_wndBlackTree.SetCheck(hChildSelect, TRUE);
		}
	}


	*pResult = 0;
}
//�����������
void CBatchAddDlg::OnNMClickTreeWhite(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPoint pt;
	GetCursorPos(&pt);
	m_wndWhiteTree.ScreenToClient(&pt);
	UINT uFlags = 0;
	HTREEITEM hChildSelect = NULL;
	m_hWhiteSelect = m_wndWhiteTree.HitTest(pt, &uFlags);
	if (NULL != m_hWhiteSelect)
	{
		m_bFaceBorW = FALSE;

		if (uFlags & TVHT_ONITEMSTATEICON)//�����ѡ��
		{
			m_wndWhiteTree.SelectItem(m_hWhiteSelect);
			if (!m_wndWhiteTree.GetCheck(m_hWhiteSelect))
			{
				hChildSelect = m_wndWhiteTree.GetParentItem(m_hWhiteSelect);
			}
		}
		else if (uFlags & TVHT_ONITEMLABEL)//�������
		{
			if (!m_wndWhiteTree.GetCheck(m_hWhiteSelect))
			{
				m_wndWhiteTree.SetCheck(m_hWhiteSelect, TRUE);
				hChildSelect = m_wndWhiteTree.GetParentItem(m_hWhiteSelect);
			}
			else
			{
				m_wndWhiteTree.SetCheck(m_hWhiteSelect, FALSE);
			}
		}

		if (NULL != hChildSelect && hChildSelect != m_hWhiteRoot)
		{
			if (!m_wndWhiteTree.GetCheck(hChildSelect))
				m_wndWhiteTree.SetCheck(hChildSelect, TRUE);
		}
	}
	*pResult = 0;
}
//�Ҽ���������
void CBatchAddDlg::OnNMRClickTreeBlack(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPoint pt;
	GetCursorPos(&pt);
	m_wndBlackTree.ScreenToClient(&pt);
	UINT uFlags = 0;
	HTREEITEM hChildSelect = NULL;
	m_hBlackSelect = m_wndBlackTree.HitTest(pt, &uFlags);
	if (m_hBlackSelect != NULL )
	{
		m_bFaceBorW = TRUE;
		m_wndBlackTree.SelectItem(m_hBlackSelect);
		m_wndBlackTree.SetCheck(m_hBlackSelect, TRUE);
		CMenu menu;
		menu.LoadMenu(IDR_MENU1);
		CPoint point;
		GetCursorPos(&point);
		CMenu* pSubMenu = menu.GetSubMenu(0);
		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
	}

	*pResult = 0;
}
//�Ҽ���������
void CBatchAddDlg::OnNMRClickTreeWhite(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPoint pt;
	GetCursorPos(&pt);
	m_wndWhiteTree.ScreenToClient(&pt);
	UINT uFlags = 0;
	HTREEITEM hChildSelect = NULL;
	m_hWhiteSelect = m_wndWhiteTree.HitTest(pt, &uFlags);
	if (m_hWhiteSelect != NULL)
	{
		m_bFaceBorW = FALSE;
		m_wndWhiteTree.SelectItem(m_hWhiteSelect);
		m_wndWhiteTree.SetCheck(m_hWhiteSelect, TRUE);
		CMenu menu;
		menu.LoadMenu(IDR_MENU1);
		CPoint point;
		GetCursorPos(&point);
		CMenu* pSubMenu = menu.GetSubMenu(0);
		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
	}
	*pResult = 0;
}
//���������
void CBatchAddDlg::OnAddface()
{
	THREE_INFO * pNode = NULL;
	CString FaceDBId, csIp;
	CString csFeacID;
	try
	{
		if (m_bFaceBorW)
		{
			pNode = (THREE_INFO *)m_wndBlackTree.GetItemData(m_hBlackSelect);
			if (pNode == NULL)
				return;
		}
		else
		{
			pNode = (THREE_INFO *)m_wndWhiteTree.GetItemData(m_hWhiteSelect);
			if (pNode == NULL)
				return;
		}
		if (pNode->nFlag == 0)
		{
			MessageBox("ѡ��һ���", "��ʾ");
			return;
		}

		AddFace dlg;
		if (dlg.DoModal() == IDOK)
		{
			csIp = pNode->csDeviceIp;
			CString csFaceNmae = dlg.m_csFaceName;
			CString csTemp;
			csTemp.Format("�Ƿ���:%s��� %s�� ?", pNode->csDeviceIp, csFaceNmae);
			if (MessageBox(csTemp, "��ʾ", MB_OKCANCEL) != IDOK)
				return;

			//�жϿ��Ƿ����
			CString csSql, csType;
			_RecordsetPtr pRecordset;
			if (m_bFaceBorW == TRUE)
				csType = "1";
			else
				csType = "0";

			csSql.Format("select *  from FaceDB  where DeviceId = '%s' and Type = '%s' and name = '%s';", pNode->csDeviceId, csType, csFaceNmae);
			if (_GetExecuteResults(pRecordset, g_pConnect, csSql) > 0)
			{
				MessageBox("�����Ѵ���", "��ʾ");
				return;
			}

			//����faceDb
			csSql.Format("insert into FaceDB (DeviceId, Name, TotalCount,Type)values('%s','%s','%s','%s')", pNode->csDeviceId, csFaceNmae, m_csContrastServerDB_Capacity, csType);
			g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);

			//��ȡ����Id
			csSql.Format("select *  from FaceDB  where DeviceId = '%s' and Type = '%s' and name = '%s';", pNode->csDeviceId, csType, csFaceNmae);
			if (_GetExecuteResults(pRecordset, g_pConnect, csSql) == 0)
			{
				MessageBox("��ȡ���ؿ�IDʧ��", "��ʾ");
				return;
			}
			FaceDBId = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("FaceDBId")->Value;

			//ע��
			char pSession[37];
			int m_result = DeviceLogin(pNode->csDeviceIp, pSession);
			char chJson[800 * 1024];
			int result;
			if (m_bFaceBorW)
				result = FASAddBlack(pNode->csDeviceIp, pSession, FaceDBId, chJson, 800 * 1024);
			else
				result = FASAddWhite(pNode->csDeviceIp, pSession, FaceDBId, chJson, 800 * 1024);

			if (result == 0)
			{
				int s(0), e(0);
				CString strRecv = A2T(chJson);
				s = strRecv.Find(_T("\"id\":"));
				s = s + 5;
				e = strRecv.Find(_T(","), s);
				csFeacID = strRecv.Mid(s, e - s);
				//����facedb�ڵ�һ�����ʵid
				csSql.Format("UPDATE FaceDB SET FaceDBFaceDBId = '%s' where DeviceId = '%s' and Type = '%s' and name = '%s';", csFeacID, pNode->csDeviceId, csType, csFaceNmae);
				g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);

				DeviceLogout(pNode->csDeviceIp, pSession);

				//����device���ڼ��ֶ�
				if (((CButton*)GetDlgItem(IDC_RADIO_DAN))->GetCheck())
					csSql.Format("select *  FROM Device where  IsFaceServer = '%d' and deviceid ='%s'", ISFACESERVER_ACTIVITY, pNode->csDeviceId);
				if (((CButton*)GetDlgItem(IDC_RADIO_JING))->GetCheck())
					csSql.Format("select *  FROM Device where  IsFaceServer = '%d' and deviceid ='%s'", ISFACESERVER_IMMOBILI, pNode->csDeviceId);

				if (_GetExecuteResults(pRecordset, g_pConnect, csSql) > 0)
				{
					if (m_bFaceBorW)
					{
						CString csFaceDBs = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("BlackFaceDbs")->Value;
						if (csFaceDBs.IsEmpty())
						{
							csFaceDBs = csFeacID;
						}
						else
						{
							csFaceDBs += "+";
							csFaceDBs += csFeacID;
						}
						csSql.Format("UPDATE Device SET BlackFaceDbs = '%s' where DeviceId = '%s';", csFaceDBs, pNode->csDeviceId);
						g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
					}
					else
					{
						CString csFaceDBs = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("WhiteFaceDbs")->Value;
						if (csFaceDBs.IsEmpty())
						{
							csFaceDBs = csFeacID;
						}
						else
						{
							csFaceDBs += "+";
							csFaceDBs += csFeacID;
						}
						csSql.Format("UPDATE Device SET WhiteFaceDbs = '%s' where DeviceId = '%s';", csFaceDBs, pNode->csDeviceId);
						g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
					}
				}

				OnUpdatatree();
			}
		}
	}
	catch (...)
	{
		//������
		if (!FaceDBId.IsEmpty())
		{
			CString csSql;
			csSql.Format("delete from FaceDB where FaceDBId = '%s';", FaceDBId);
			g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		}

		if (!csFeacID.IsEmpty())
		{
			char pSession1[37];
			int m_result = DeviceLogin(csIp, pSession1);
			char chJson1[800 * 1024];
			int result;
			if (m_bFaceBorW)
				result = FASDeleteBlack(csIp, pSession1, csFeacID, chJson1, 800 * 1024);
			else
				result = FASDeleteWhite(csIp, pSession1, csFeacID, chJson1, 800 * 1024);

			if (result != 0)
			{
				CString csTemp;
				csTemp.Format("������_%s_ɾ����_%s_ʧ��", pNode->csDeviceIp, pNode->csFaceID);
				DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 0, csTemp, PROGRAM_NAME);
			}
			DeviceLogout(csIp, pSession1);
		}
	}
}
//ɾ��������
void CBatchAddDlg::OnDeleteface()
{
	THREE_INFO * pNode;
	try
	{
		if (m_bFaceBorW)
		{
			pNode = (THREE_INFO *)m_wndBlackTree.GetItemData(m_hBlackSelect);
			if (pNode == NULL)
				return;
		}
		else
		{
			pNode = (THREE_INFO *)m_wndWhiteTree.GetItemData(m_hWhiteSelect);
			if (pNode == NULL)
				return;
		}
		if (pNode->nFlag != 2)
		{
			MessageBox("��ѡ���", "��ʾ");
			return;
		}

		CString csTemp;
		csTemp.Format("�Ƿ�ɾ����%s ?", pNode->DeviceName);
		if (MessageBox(csTemp, "��ʾ", MB_OKCANCEL) == IDOK)
		{

			CString csSql;
			//ɾ���������ڿ�
			char pSession1[37];
			int m_result = DeviceLogin(pNode->csDeviceIp, pSession1);
			char chJson1[800 * 1024];
			int result;
			if (m_bFaceBorW)
				result = FASDeleteBlack(pNode->csDeviceIp, pSession1, pNode->csFaceID, chJson1, 800 * 1024);
			else
				result = FASDeleteWhite(pNode->csDeviceIp, pSession1, pNode->csFaceID, chJson1, 800 * 1024);

			if (result != 0)
			{
				CString csTemp;
				csTemp.Format("������_%s_ɾ����_%s_ʧ��", pNode->csDeviceIp, pNode->csFaceID);
				MessageBox(csTemp, "��ʾ");
				DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 0, csTemp, PROGRAM_NAME);
				//return;
			}
			//Ԥ��ɾ���ӱ�������
			if (ClearDeviceInFaceId(pNode->csDeviceId, pNode->csFaceID))
			{
				//�޸�device����facedbs�ֶ�
				if(!ClearDeviceFaceDbs(pNode->csDeviceId, pNode->csFaceID))
					MessageBox("���ݿ�������ɾ��ʧ��", "��ʾ");
			}
			else
			{
				MessageBox("���ݿ�����Աɾ��ʧ��", "��ʾ");
			}

			OnUpdatatree();
		}

	}
	catch (...)
	{
	}
}
//�༭������
void CBatchAddDlg::OnEditface()
{
	THREE_INFO * pNode;
	try
	{
		if (m_bFaceBorW)
		{
			pNode = (THREE_INFO *)m_wndBlackTree.GetItemData(m_hBlackSelect);
			if (pNode == NULL)
				return;
		}
		else
		{
			pNode = (THREE_INFO *)m_wndWhiteTree.GetItemData(m_hWhiteSelect);
			if (pNode == NULL)
				return;
		}
		if (pNode->nFlag != 2)
		{
			MessageBox("��ѡ���", "��ʾ");
			return;
		}

		AddFace dlg;
		if (dlg.DoModal() == IDOK)
		{
			CString csFaceNmae = dlg.m_csFaceName;
			CString csTemp;
			csTemp.Format("�Ƿ񽫣�%s �޸�Ϊ:%s", pNode->DeviceName, csFaceNmae);
			if (MessageBox(csTemp, "��ʾ", MB_OKCANCEL) == IDOK)
			{
				CString csSql;
				_RecordsetPtr pRecordset;
				csSql.Format("UPDATE FaceDB SET Name = '%s' where DeviceId = '%s' and FaceDBFaceDBId = '%s';", csFaceNmae, pNode->csDeviceId, pNode->csFaceID);
				g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
				OnUpdatatree();
			}
		}
	}
	catch (...)
	{
	}
}
//ˢ����
void CBatchAddDlg::OnUpdatatree()
{
	if (!InitDeviceInfo())
	{
		MessageBox("��ȡһ�����Ϣʧ��", "��ʾ");
		return;
	}
	ShowBalackTree();
	ShowWhiteTree();
}
//��ʾlist
void CBatchAddDlg::SetListData(CString csPath, CString csResults, CString csIp)
{
	COleDateTime coTime;
	coTime = COleDateTime::GetCurrentTime();
	CString strCurTime = coTime.Format("%Y-%m-%d %H:%M:%S");
	m_wndList.InsertItem(0, "");
	m_wndList.SetItemText(0, 0, csPath);
	m_wndList.SetItemText(0, 1, strCurTime);
	m_wndList.SetItemText(0, 2, csIp);
	m_wndList.SetItemText(0, 3, csResults);
}
//�ļ���ѡ��
void CBatchAddDlg::OnBnClickedButtonSelect()
{
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
	if (SHGetPathFromIDList(pidl, szDisplayName))
	{
		CString strPath(szDisplayName);
		m_csImgPath = strPath;
		GetDlgItem(IDC_STATIC_PATH)->SetWindowText(strPath);
		m_bSelectPath = TRUE;
	}
}
//�жϺ�������ѡ��
int CBatchAddDlg::TraversalBlackTree()
{
	CString csOutInfo = "�Ƿ���";
	BOOL bSelect = FALSE;

	HTREEITEM hDeviceIp = m_wndBlackTree.GetChildItem(m_hBlackRoot);
	if (NULL == hDeviceIp) return FALSE;
	do
	{
		if (m_wndBlackTree.GetCheck(hDeviceIp))
		{
			HTREEITEM hDeviceFace = m_wndBlackTree.GetChildItem(hDeviceIp);
			if (NULL == hDeviceFace) return FALSE;
			do
			{
				if (m_wndBlackTree.GetCheck(hDeviceFace))
				{
					THREE_INFO * pNode = (THREE_INFO *)m_wndBlackTree.GetItemData(hDeviceFace);
					if (pNode != NULL)
					{
						bSelect = TRUE;
						g_RegistrationSum++;
						g_DeviceInfo[pNode->nDeviceIndex].csFacedb = pNode->csFaceID;
						g_DeviceInfo[pNode->nDeviceIndex].bBlackOrWhite = TRUE;
						g_DeviceInfo[pNode->nDeviceIndex].bIsWork = TRUE;
						CString CsTemp;
						CsTemp.Format("%s��%s,", pNode->csDeviceIp, pNode->DeviceName);
						csOutInfo += CsTemp;
						CsTemp.Format("%d", pNode->nDeviceIndex);
						g_arRegistrationIndex.Add(CsTemp);
						break;
					}
				}
			} while ((hDeviceFace = m_wndBlackTree.GetNextSiblingItem(hDeviceFace)) != NULL);

		}
	} while ((hDeviceIp = m_wndBlackTree.GetNextSiblingItem(hDeviceIp)) != NULL);
	csOutInfo = csOutInfo.Mid(0, csOutInfo.GetLength() - 1);
	csOutInfo += "ע�������?";
	if (!bSelect)
		return 2;

	if (MessageBox(csOutInfo, "��ʾ", MB_OKCANCEL) == IDOK)
		return 0;
	else
		return 1;
}
///�жϰ�������ѡ��
int CBatchAddDlg::TraversalWhiteTree()
{
	CString csOutInfo = "�Ƿ���";
	BOOL bSelect = FALSE;
	HTREEITEM hDeviceIp = m_wndWhiteTree.GetChildItem(m_hWhiteRoot);
	if (NULL == hDeviceIp) return FALSE;
	do
	{
		if (m_wndWhiteTree.GetCheck(hDeviceIp))
		{
			HTREEITEM hDeviceFace = m_wndWhiteTree.GetChildItem(hDeviceIp);
			if (NULL == hDeviceFace) return FALSE;
			do
			{
				if (m_wndWhiteTree.GetCheck(hDeviceFace))
				{
					THREE_INFO * pNode = (THREE_INFO *)m_wndWhiteTree.GetItemData(hDeviceFace);
					if (pNode != NULL)
					{
						bSelect = TRUE;
						g_RegistrationSum++;
						g_DeviceInfo[pNode->nDeviceIndex].csFacedb = pNode->csFaceID;
						g_DeviceInfo[pNode->nDeviceIndex].bBlackOrWhite = FALSE;
						CString CsTemp;
						CsTemp.Format("%s��%s,", pNode->csDeviceIp, pNode->DeviceName);
						csOutInfo += CsTemp;
						CsTemp.Format("%d", pNode->nDeviceIndex);
						g_arRegistrationIndex.Add(CsTemp);
						break;
					}
				}
			} while ((hDeviceFace = m_wndWhiteTree.GetNextSiblingItem(hDeviceFace)) != NULL);

		}
	} while ((hDeviceIp = m_wndWhiteTree.GetNextSiblingItem(hDeviceIp)) != NULL);

	if (!bSelect)
		return 2;

	csOutInfo = csOutInfo.Mid(0, csOutInfo.GetLength() - 1);
	csOutInfo += "ע�������?";

	if (MessageBox(csOutInfo, "��ʾ", MB_OKCANCEL) == IDOK)
		return 0;
	else
		return 1;

}
//ѭ���ļ�����ͼƬ��ӵ�����
void CBatchAddDlg::BayesCategoryTest(CString tp)
{

	CFileFind fileFinder;
	CString filePath = tp + _T("\\*.*");

	BOOL bFinished = fileFinder.FindFile(filePath);
	int nInxex = 0;
	while (bFinished)  //ÿ��ѭ����Ӧһ�����Ŀ¼
	{
		try
		{
			bFinished = fileFinder.FindNextFile();
			CString fileName = fileFinder.GetFileName();

			if (fileFinder.IsDirectory() && !fileFinder.IsDots())  //����Ŀ¼��ݹ���ô˷���
			{
				BayesCategoryTest(fileFinder.GetFilePath());
			}
			else  //���ж��Ƿ�Ϊjpg�ļ�
			{
				if (fileName.GetLength() < 4)
				{
					continue;
				}
				int dotPos = fileName.ReverseFind('.');
				CString csFeature = tp + "//" + fileName;
				CString csName = fileName.Mid(0, dotPos);
				CString fileExt = fileName.Right(fileName.GetLength() - dotPos);
				if (fileExt == _T(".jpg"))  //����jpg�ļ���ʼ���
				{
					if (nInxex >= g_RegistrationSum)
						nInxex = 0;

					int nDeviceIndex = atoi(g_arRegistrationIndex.GetAt(nInxex));
					EnterCriticalSection(&cSECTION);
					if (g_DeviceInfo[nDeviceIndex].bIsWork)
					{
						g_DeviceInfo[nDeviceIndex].arFacePath.Add(csFeature);
					}
					nInxex++;
					LeaveCriticalSection(&cSECTION);
				}
			}
		}
		catch (CException* e)
		{
		}
	}
	fileFinder.Close();
}
//�жϹ����Ƿ����
BOOL CBatchAddDlg::VerdictCombox()
{
	UpdateData();
	if (
		m_wndComName.GetCurSel() == m_wndComIdcard.GetCurSel() ||
		m_wndComName.GetCurSel() == m_wndComGender.GetCurSel() ||
		m_wndComName.GetCurSel() == m_wndComBitrhday.GetCurSel() ||
		m_wndComName.GetCurSel() == m_wndComNational.GetCurSel() ||
		m_wndComName.GetCurSel() == m_wndComAddress.GetCurSel() ||
		m_wndComName.GetCurSel() == m_wndComPhone.GetCurSel()
		)
	{
		return FALSE;
	}

	int nSelect[8];
	memset(nSelect, 0, 8);
	g_FlagSum = 0;
	if (m_wndComName.GetCurSel() > 0)
	{
		nSelect[m_wndComName.GetCurSel() - 1] = 1;
		g_FlagSum++;
	}

	if (m_wndComIdcard.GetCurSel() > 0)
	{
		nSelect[m_wndComIdcard.GetCurSel() - 1] = 1;
		g_FlagSum++;
	}

	if (m_wndComGender.GetCurSel() > 0)
	{
		nSelect[m_wndComGender.GetCurSel() - 1] = 1;
		g_FlagSum++;
	}

	if (m_wndComBitrhday.GetCurSel() > 0)
	{
		nSelect[m_wndComBitrhday.GetCurSel() - 1] = 1;
		g_FlagSum++;
	}

	if (m_wndComNational.GetCurSel() > 0)
	{
		nSelect[m_wndComNational.GetCurSel() - 1] = 1;
		g_FlagSum++;
	}

	if (m_wndComAddress.GetCurSel() > 0)
	{
		nSelect[m_wndComAddress.GetCurSel() - 1] = 1;
		g_FlagSum++;
	}

	if (m_wndComPhone.GetCurSel() > 0)
	{
		nSelect[m_wndComPhone.GetCurSel() - 1] = 1;
		g_FlagSum++;
	}


	if (g_FlagSum >= 2)
	{
		if (m_wndComName.GetCurSel() == 0 && m_wndComIdcard.GetCurSel() == 0)
			return FALSE;

		if (m_wndComSeparator.GetCurSel() == 0)
			return FALSE;
	}

	for (int i = 0; i < g_FlagSum; i++)
	{
		if (nSelect[i] != 1)
		{
			return FALSE;
		}
	}

	return TRUE;
}
//����ͼƬ�ļ���
void CBatchAddDlg::CreateImgFile()
{
	try
	{
		TCHAR pFileName[MAX_PATH];
		int nPos = GetCurrentDirectory(MAX_PATH, pFileName);
		CString csFullPath(pFileName);
		COleDateTime cTime = COleDateTime::GetCurrentTime();
		CString csTime = cTime.Format(("%Y%m%d"));
		csFullPath.Replace("\\", "//");
		CString csRootFile, csImgRootName, csImageNmae, csFaceName, csErrorName,csPhotoName;
		csRootFile.Format("%s//RegistrationInfo", csFullPath);
		BOOL IsD = PathIsDirectory(csRootFile);
		if (!IsD)
			CreateDirectory(csRootFile, NULL);

		csImgRootName.Format("%s//%s", csRootFile, csTime);
		IsD = PathIsDirectory(csImgRootName);
		if (!IsD)
			CreateDirectory(csImgRootName, NULL);

		csImageNmae.Format("%s//imgCurFaceJpg", csImgRootName);
		IsD = PathIsDirectory(csImageNmae);
		if (!IsD)
			CreateDirectory(csImageNmae, NULL);

		csFaceName.Format("%s//imgCurFaceFeature", csImgRootName);
		IsD = PathIsDirectory(csFaceName);
		if (!IsD)
			CreateDirectory(csFaceName, NULL);

		csErrorName.Format("%s//ErrorImg", csImgRootName);
		IsD = PathIsDirectory(csErrorName);
		if (!IsD)
			CreateDirectory(csErrorName, NULL);

		csPhotoName.Format("%s//imgPhotoJpg", csImgRootName);
		IsD = PathIsDirectory(csPhotoName);
		if (!IsD)
			CreateDirectory(csPhotoName, NULL);

		g_csSavePath = csImgRootName;
	}
	catch (...)
	{
	}
}
//�ƶ���Ƭ
BOOL CBatchAddDlg::MoveImg(CString csImgPath, int nType)
{
	try
	{
		CString csTargetPath;
		int nFind = csImgPath.ReverseFind('//');
		CString csFaceName = csImgPath.Mid(nFind + 1, csImgPath.GetLength());

		if (nType == 0)
			csTargetPath.Format("%s//imgCurFaceJpg//%s", g_csSavePath, csFaceName);//��ȡ����ͼ
		else if (nType == 1)
			csTargetPath.Format("%s//imgCurFaceFeature//%s", g_csSavePath, csFaceName);
		else if (nType == 2)
			csTargetPath.Format("%s//ErrorImg//%s", g_csSavePath, csFaceName);
		else if (nType == 3)
			csTargetPath.Format("%s//imgPhotoJpg//%s", g_csSavePath, csFaceName);//ԭͼ

		if (MoveFile(csImgPath, csTargetPath) == 0)
			return FALSE;

		return TRUE;
	}
	catch (...)
	{
		return FALSE;
	}
	return FALSE;
}
//ע��
void CBatchAddDlg::OnBnClickedButtonAdd()
{
	if (!m_bInitDb)
	{
		MessageBox("δ�������ݿ�", "��ʾ");
		return;
	}
	if (m_bAddFalg)
	{
		MessageBox("����ע��", "��ʾ");
		return;
	}

	if (!m_bSelectPath)
	{
		MessageBox("��ѡ���ļ�", "��ʾ", MB_OKCANCEL);
		return;
	}


	if (!VerdictCombox())
	{
		MessageBox("������������", "��ʾ");
		return;
	}

	CreateImgFile();

	int nReturn = -1;
	if (m_bFaceBorW)
	{
		nReturn = TraversalBlackTree();
		if (nReturn == 1)
		{
			if (MessageBox("ȡ���·�", "��ʾ", MB_OKCANCEL) == IDOK)
				return;
		}
		else if (nReturn == 2)
		{
			MessageBox("δѡ���", "��ʾ", MB_OKCANCEL);
			return;
		}
		hGetImgPath = AfxBeginThread(&ThreadGetImgPath, NULL);
		m_bSelectPath = FALSE;
		m_bAddFalg = TRUE;
		GetDlgItem(IDC_STATIC_ZHUANGTAI)->SetWindowText("ע�Ὺʼ");
		SetTimer(1, 2000, NULL);
	}
	else
	{
		nReturn = TraversalWhiteTree();
		if (nReturn == 1)
		{
			if (MessageBox("ȡ���·�", "��ʾ", MB_OKCANCEL) == IDOK)
				return;
		}
		else if (nReturn == 2)
		{
			MessageBox("δѡ���", "��ʾ", MB_OKCANCEL);
			return;
		}
		hGetImgPath = AfxBeginThread(&ThreadGetImgPath, NULL);
		m_bSelectPath = FALSE;
		m_bAddFalg = TRUE;
		GetDlgItem(IDC_STATIC_ZHUANGTAI)->SetWindowText("ע�Ὺʼ");
		SetTimer(1, 2000, NULL);
	}

}
//�ж�ע��״̬
void CBatchAddDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		BOOL bFlag = TRUE;
		for (int i = 0; i < g_RegistrationSum; i++)
		{
			int nDeviceIndex = atoi(g_arRegistrationIndex.GetAt(i));
			int nNum = g_DeviceInfo[nDeviceIndex].arFacePath.GetSize();
			if (nNum != 0)
			{
				bFlag = FALSE;
			}
		}

		if (bFlag)
		{
			KillTimer(1);
			GetDlgItem(IDC_STATIC_ZHUANGTAI)->SetWindowText("ע�����");
			m_bAddFalg = FALSE;
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}
//list��ʾ
void CBatchAddDlg::OnLvnGetInfoTipList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVGETINFOTIP pGetInfoTip = reinterpret_cast<LPNMLVGETINFOTIP>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString csTip = m_wndList.GetItemText(pGetInfoTip->iItem, 0);
	_tcscpy(pGetInfoTip->pszText, csTip);
	*pResult = 0;
}
//��ӷ�����
void CBatchAddDlg::OnAddserver()
{
	THREE_INFO * pNode;
	try
	{
		if (m_bFaceBorW)
		{
			pNode = (THREE_INFO *)m_wndBlackTree.GetItemData(m_hBlackSelect);
			if (pNode == NULL)
				return;
		}
		else
		{
			pNode = (THREE_INFO *)m_wndWhiteTree.GetItemData(m_hWhiteSelect);
			if (pNode == NULL)
				return;
		}
		CAddServer dlg;
		if (dlg.DoModal() == IDOK)
		{
			COleDateTime coTime;
			coTime = COleDateTime::GetCurrentTime();
			CString csTime = coTime.Format("%Y-%m-%d %H:%M:%S");

			CString csSql;
			csSql.Format("insert into device(DeviceName, Description, OnlyId, IP, Addr, Remark,Type,IsFaceServer, CreateTime, LastTime)values('%s','%s','%s','%s','%s','%s','%d','%d','%s','%s')", dlg.m_csName, dlg.m_csDescription,dlg.m_csOnlyId,dlg.m_csIp,dlg.m_csAddr,dlg.m_csMemark,dlg.m_nInOrOut,dlg.m_nStatic,csTime,csTime);
			g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
			OnUpdatatree();
		}
		
	}
	catch (...)
	{
	}
}
//�༭������
void CBatchAddDlg::OnEditserver()
{
	THREE_INFO * pNode;
	try
	{
		if (m_bFaceBorW)
		{
			pNode = (THREE_INFO *)m_wndBlackTree.GetItemData(m_hBlackSelect);
			if (pNode == NULL)
				return;
		}
		else
		{
			pNode = (THREE_INFO *)m_wndWhiteTree.GetItemData(m_hWhiteSelect);
			if (pNode == NULL)
				return;
		}
		if (pNode->nFlag != 1)
		{
			MessageBox("��ѡ�������", "��ʾ");
			return;
		}

		CAddServer dlg(NULL,1);
		dlg.m_csName = g_DeviceInfo[pNode->nDeviceIndex].DeviceName;
		dlg.m_csDescription = g_DeviceInfo[pNode->nDeviceIndex].csDescription;
		dlg.m_csOnlyId = g_DeviceInfo[pNode->nDeviceIndex].csOnlyId;
		dlg.m_csIp = g_DeviceInfo[pNode->nDeviceIndex].csDeviceIp;
		dlg.m_csAddr = g_DeviceInfo[pNode->nDeviceIndex].csAddr;
		dlg.m_csMemark = g_DeviceInfo[pNode->nDeviceIndex].csRemark;

		if (atoi(g_DeviceInfo[pNode->nDeviceIndex].csType) == 0)
			dlg.m_bInorOut = TRUE;
		else
			dlg.m_bInorOut = FALSE;

		dlg.m_bStatic = g_DeviceInfo[pNode->nDeviceIndex].bIsStatic;
		if (dlg.DoModal() == IDOK)
		{
			COleDateTime coTime;
			coTime = COleDateTime::GetCurrentTime();
			CString csTime = coTime.Format("%Y-%m-%d %H:%M:%S");

			CString csSql;
			csSql.Format(" UPDATE device set DeviceName = '%s',Description = '%s',OnlyId = '%s',IP = '%s',Addr = '%s',Remark = '%s',Type = '%d',IsFaceServer = '%d',LastTime = '%s'where DeviceId = '%s'", dlg.m_csName, dlg.m_csDescription, dlg.m_csOnlyId, dlg.m_csIp, dlg.m_csAddr, dlg.m_csMemark, dlg.m_nInOrOut, dlg.m_nStatic, csTime, g_DeviceInfo[pNode->nDeviceIndex].csDeviceId);
			g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
			OnUpdatatree();
		}
	}
	catch (...)
	{
	}
}
//ɾ��������
void CBatchAddDlg::OnDeleteserver()
{
	THREE_INFO * pNode;
	try
	{
		if (m_bFaceBorW)
		{
			pNode = (THREE_INFO *)m_wndBlackTree.GetItemData(m_hBlackSelect);
			if (pNode == NULL)
				return;
		}
		else
		{
			pNode = (THREE_INFO *)m_wndWhiteTree.GetItemData(m_hWhiteSelect);
			if (pNode == NULL)
				return;
		}
		if (pNode->nFlag != 1)
		{
			MessageBox("��ѡ�������", "��ʾ");
			return;
		}
		COleDateTime coTime;
		coTime = COleDateTime::GetCurrentTime();
		CString csTime = coTime.Format("%Y-%m-%d %H:%M:%S");
		CString csSql;
		map<CString, CString>::iterator iter;

		for (iter = g_DeviceInfo[pNode->nDeviceIndex].mapBlackFacedb.begin(); iter != g_DeviceInfo[pNode->nDeviceIndex].mapBlackFacedb.end(); ++iter)
		{
			//Ԥ��ɾ���ӱ�������
			if (ClearDeviceInFaceId(pNode->csDeviceId, iter->first))
			{
				//�޸�device����facedbs�ֶ�
				if (!ClearDeviceFaceDbs(pNode->csDeviceId, iter->first))
				{
					MessageBox("���ݿ�������ɾ��ʧ��", "��ʾ");
					return;
				}
			}
			else
			{
				MessageBox("���ݿ�����Աɾ��ʧ��", "��ʾ");
				return;
			}
		}

		csSql.Format(" DELETE FROM device where DeviceId = '%s'",  g_DeviceInfo[pNode->nDeviceIndex].csDeviceId);
		g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);

		OnUpdatatree();
	}
	catch (...)
	{
	}
}
//ɾ����������facedbs�ֶ���faceid
BOOL CBatchAddDlg::ClearDeviceFaceDbs(CString csDevicId, CString csFaceId)
{
	try
	{
		CString csSql;
		_RecordsetPtr pRecordset;
		csSql.Format("DELETE FROM FaceDB  where FaceDBFaceDBId = '%s' and DeviceId = '%s';", csFaceId, csDevicId);
		g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);

		if (((CButton*)GetDlgItem(IDC_RADIO_DAN))->GetCheck())
			csSql.Format("select *  FROM Device where  IsFaceServer = '%d' and deviceid ='%s'", ISFACESERVER_ACTIVITY, csDevicId);
		if (((CButton*)GetDlgItem(IDC_RADIO_JING))->GetCheck())
			csSql.Format("select *  FROM Device where  IsFaceServer = '%d' and deviceid ='%s'", ISFACESERVER_IMMOBILI, csDevicId);

		if (_GetExecuteResults(pRecordset, g_pConnect, csSql) > 0)
		{
			if (m_bFaceBorW)
			{
				CString csFaceDBs = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("BlackFaceDbs")->Value;
				if (!csFaceDBs.IsEmpty())
				{
					CString csTemp;

					if (csFaceDBs.Find(csFaceId) == 0)
					{
						csTemp = csFaceId;
					}
					else
					{
						csTemp += "+";
						csTemp += csFaceId;
					}

					csFaceDBs.Replace(csTemp, "");

					if (csFaceDBs.Find("+") == 0)
						csFaceDBs = csFaceDBs.Mid(1, csFaceDBs.GetLength());
				}
				csSql.Format("UPDATE Device SET BlackFaceDbs = '%s' where DeviceId = '%s';", csFaceDBs, csDevicId);
				g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
			}
			else
			{
				CString csFaceDBs = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("WhiteFaceDbs")->Value;
				if (!csFaceDBs.IsEmpty())
				{
					CString csTemp;
					if (csFaceDBs.Find(csFaceId) == 0)
					{
						csTemp = csFaceId;
					}
					else
					{
						csTemp += "+";
						csTemp += csFaceId;
					}
					csTemp += csFaceId;
					csFaceDBs.Replace(csTemp, "");
					if (csFaceDBs.Find("+") == 0)
						csFaceDBs = csFaceDBs.Mid(1, csFaceDBs.GetLength());
				}
				csSql.Format("UPDATE Device SET WhiteFaceDbs = '%s' where DeviceId = '%s';", csFaceDBs, csDevicId);
				g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
			}
		}
	}
	catch (...)
	{
		return FALSE;
	}
	return TRUE;
}
//ɾ���������ݿ��ڷ�����ע������
BOOL CBatchAddDlg::ClearDeviceInFaceId(CString csDevicId, CString csFaceId)
{
	try
	{
		CString csSql;
		_RecordsetPtr pRecordset;
		csSql.Format("SELECT * FROM personslistcount WHERE DeviceId = '%s'", csDevicId);
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		while(!pRecordset->adoEOF)
		{
			CString csFaceName = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("DataBaseName")->Value;
			
			CString csPhotoInfName = csFaceName;
			csPhotoInfName.Replace("personslist", "photoinfo");
			csPhotoInfName.Replace("PersonsList", "PhotoInfo");
			//ɾ��img��
			csSql.Format("DELETE a FROM  %s a, %s b WHERE b.FaceDBId = '%s'", csPhotoInfName,csFaceName, csFaceId);
			g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
			//ɾ��list��
			csSql.Format("DELETE FROM  %s WHERE FaceDBId = '%s'", csFaceName, csFaceId);
			g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);


			int nTableSum = GetTableSum(csFaceName);
			csSql.Format("UPDATE PersonsListCount SET UsedCount = '%d' where DataBaseName ='%s';", nTableSum, csFaceName);

			g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
			pRecordset->MoveNext();
		}
	}
	catch (...)
	{
		return FALSE;
	}
	return TRUE;
}
