
// CrowdAnalyzDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CrowdAnalyz.h"
#include "CrowdAnalyzDlg.h"
#include "afxdialogex.h"
#define  DBINI_PATH			 "\\Parameter.ini"

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


// CCrowdAnalyzDlg �Ի���



CCrowdAnalyzDlg::CCrowdAnalyzDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCrowdAnalyzDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCrowdAnalyzDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCrowdAnalyzDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CCrowdAnalyzDlg ��Ϣ�������

BOOL CCrowdAnalyzDlg::OnInitDialog()
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
	m_dlgSSJK = NULL;
	m_dlgRLKGL = NULL;
	m_iCurDlg = 0;
	MoveWindow(0, 0, WIDTH, HEIGHT);
	m_rClient.SetRect(0, 0, WIDTH, HEIGHT);
	m_rMin.SetRect(1832 * WIDTH / 1920, 0, 1864 * WIDTH / 1920, 32 * HEIGHT / 1080);
	m_rClose.SetRect(1865 * WIDTH / 1920, 0, 1907 * WIDTH / 1920, 32 * HEIGHT / 1080);
	m_rToolBar.SetRect(0, 106 * HEIGHT / 1080, 1920 * WIDTH / 1920, 250 * HEIGHT / 1080);
	m_rChildDialog.SetRect(0, 250 * HEIGHT / 1080, 1920 * WIDTH / 1920, 1080 * HEIGHT / 1080);
	m_rSSJK.SetRect(135*WIDTH/1920, 110 * HEIGHT / 1080, 345 * WIDTH / 1920, 230 * HEIGHT / 1080);
	m_rRLKGL.SetRect(389 * WIDTH / 1920, 110 * HEIGHT / 1080, 594 * WIDTH / 1920, 230 * HEIGHT / 1080);

	m_rXTSZ.SetRect(1539 * WIDTH / 1920, 110 * HEIGHT / 1080, 1750 * WIDTH / 1920, 230 * HEIGHT / 1080);
	m_rSBGL.SetRect(1315 * WIDTH / 1920, 110 * HEIGHT / 1080, 1523 * WIDTH / 1920, 230 * HEIGHT / 1080);
	m_rLSJL.SetRect(1089 * WIDTH / 1920, 110 * HEIGHT / 1080, 1306 * WIDTH / 1920, 230 * HEIGHT / 1080);
	m_rSJFX.SetRect(871 * WIDTH / 1920, 110 * HEIGHT / 1080, 1058 * WIDTH / 1920, 230 * HEIGHT / 1080);
	
	//˫����
	CClientDC dc(this);
	pMemDC = new CDC;
	pBitmap = new CBitmap;
	pMemDC->CreateCompatibleDC(&dc);
	pBitmap->CreateCompatibleBitmap(&dc, WIDTH, HEIGHT);


	if (!InitDatabase())
	{
		MessageBox(_T("���ݿ�����ʧ���˳�"));
		EndDialog(IDCANCEL);//��һ�������Ч��
		return FALSE;
	}
	//��ʾӦ����ʾ���ӶԻ���
	DisplayChildDialog(m_iCurDlg);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CCrowdAnalyzDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCrowdAnalyzDlg::OnPaint()
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
		//����
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������
		CBitmap * pOldBitmap = pMemDC->SelectObject(pBitmap);
		pMemDC->PatBlt(0, 0, WIDTH, HEIGHT, PATCOPY);
		Graphics g(pMemDC->m_hDC);
		//���ܱ���
		Image img_bk(L"img/D-k1.jpg");
		g.DrawImage(&img_bk, 0, 0, m_rClient.Width(), m_rClient.Height());
		//��������
		if (m_iCurDlg == 0)
		{
			Image img_toolbar(L"img/ssjk.jpg");
			g.DrawImage(&img_toolbar, m_rToolBar.left, m_rToolBar.top, m_rToolBar.Width(), m_rToolBar.Height());
		}
		else if (m_iCurDlg == 1)
		{
			Image img_toolbar(L"img/rlkgl.jpg");
			g.DrawImage(&img_toolbar, m_rToolBar.left, m_rToolBar.top, m_rToolBar.Width(), m_rToolBar.Height());
		}
		else if (m_iCurDlg == 2)
		{
			Image img_toolbar(L"img/dtfxk.jpg");
			g.DrawImage(&img_toolbar, m_rToolBar.left, m_rToolBar.top, m_rToolBar.Width(), m_rToolBar.Height());
		}
		else if (m_iCurDlg == 3)
		{
			Image img_toolbar(L"img/sjfx.jpg");
			g.DrawImage(&img_toolbar, m_rToolBar.left, m_rToolBar.top, m_rToolBar.Width(), m_rToolBar.Height());
		}
		else if (m_iCurDlg == 4)
		{
			Image img_toolbar(L"img/lsjl.jpg");
			g.DrawImage(&img_toolbar, m_rToolBar.left, m_rToolBar.top, m_rToolBar.Width(), m_rToolBar.Height());
		}
		else if (m_iCurDlg == 5)
		{
			Image img_toolbar(L"img/sbgl.jpg");
			g.DrawImage(&img_toolbar, m_rToolBar.left, m_rToolBar.top, m_rToolBar.Width(), m_rToolBar.Height());
		}
		else if (m_iCurDlg == 6)
		{
			Image img_toolbar(L"img/xtsz.jpg");
			g.DrawImage(&img_toolbar, m_rToolBar.left, m_rToolBar.top, m_rToolBar.Width(), m_rToolBar.Height());
		}

		//����λ��
		int i = 0;
		Pen redPen(Color(255, 0, 0), 1);
//		g.DrawRectangle(&redPen, m_rSSJK.left, m_rSSJK.top, m_rSSJK.Width(), m_rSSJK.Height());
//		g.DrawRectangle(&redPen, m_rRLKGL.left, m_rRLKGL.top, m_rRLKGL.Width(), m_rRLKGL.Height());

		//��ʾ���棬���HDC
		dc.BitBlt(0, 0, WIDTH, HEIGHT, pMemDC, 0, 0, SRCCOPY);
		pMemDC->SelectObject(pOldBitmap);
		g.ReleaseHDC(pMemDC->m_hDC);

		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCrowdAnalyzDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCrowdAnalyzDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO:  �ڴ˴������Ϣ����������
	//����+
	delete pMemDC;
	delete pBitmap;
	DestoryCurDialog();
}
CString CCrowdAnalyzDlg::GetiniPath()
{
	CString strIp;
	TCHAR pFileName[MAX_PATH];
	int nPos = GetCurrentDirectory(MAX_PATH, pFileName);

	CString csFullPath(pFileName);
	csFullPath += DBINI_PATH;

	return csFullPath;
}
BOOL CCrowdAnalyzDlg::InitDatabase()
{
	CString csSqlConnect;

	CString csFullPath = GetiniPath();
	GetPrivateProfileString(_T("SqlConfiguration"), _T("Sql_ServerAdd"), _T(""), m_csSqlIP.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);
	GetPrivateProfileString(_T("SqlConfiguration"), _T("Sql_Database "), _T(""), m_csSqlData.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);
	GetPrivateProfileString(_T("SqlConfiguration"), _T("Sql_Name"), _T(""), m_csSqlName.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);
	GetPrivateProfileString(_T("SqlConfiguration"), _T("Sql_Pwd"), _T(""), m_csSqlPwd.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);

	//csSqlConnect = "Driver={MySQL ODBC 5.1 Driver};Server = 192.168.2.66; Database = hospitaldatasys; uid = root;pwd = cr;";
	csSqlConnect.Format(_T("Driver={MySQL ODBC 5.1 Driver};Server=%s;Database=%s;uid=%s;pwd=%s;"), m_csSqlIP, m_csSqlData, m_csSqlName, m_csSqlPwd);
	try
	{
		g_pConnect.CreateInstance(__uuidof(Connection));
		_bstr_t strConnect = csSqlConnect;
		g_pConnect->Open(strConnect, "", "", adModeUnknown);
	}
	catch (_com_error e)
	{
		return FALSE;
	}
	return  TRUE;
}
void CCrowdAnalyzDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//����
	if (m_rClose.PtInRect(point))
	{
		DestoryCurDialog();
		PostMessage(WM_CLOSE);
	}
	else if (m_rMin.PtInRect(point))
	{
		PostMessage(WM_SYSCOMMAND, SC_MINIMIZE);
	}
	else if (m_rSSJK.PtInRect(point))
	{
		if (m_iCurDlg == 0)
			return;
		else
		{
			DestoryCurDialog();
			DisplayChildDialog(0);
			m_iCurDlg = 0;
		}
		InvalidateRect(m_rToolBar, false);
	}
	else if (m_rRLKGL.PtInRect(point))
	{
		if (m_iCurDlg == 1)
			return;
		else
		{
			DestoryCurDialog();
			DisplayChildDialog(1);
			m_iCurDlg = 1;
		}
		InvalidateRect(m_rToolBar, false);
	}
	else if (m_rLSJL.PtInRect(point))
	{
		if (m_iCurDlg == 4)
			return;
		else
		{
			DestoryCurDialog();
			DisplayChildDialog(4);
			m_iCurDlg = 4;
		}
		InvalidateRect(m_rToolBar, false);
	}
	else if (m_rSJFX.PtInRect(point))
	{
		if (m_iCurDlg == 3)
			return;
		else
		{
			DestoryCurDialog();
			DisplayChildDialog(3);
			m_iCurDlg = 3;
		}
		InvalidateRect(m_rToolBar, false);
	}
	else if (m_rSBGL.PtInRect(point))
	{
		if (m_iCurDlg == 5)
			return;
		else
		{
			DestoryCurDialog();
			DisplayChildDialog(5);
			m_iCurDlg = 5;
		}
		InvalidateRect(m_rToolBar, false);
	}
	else if (m_rXTSZ.PtInRect(point))
	{
		if (m_iCurDlg == 6)
			return;
		else
		{
			DestoryCurDialog();
			DisplayChildDialog(6);
			m_iCurDlg = 6;
		}
		InvalidateRect(m_rToolBar, false);
	}
	else
	{
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));

	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CCrowdAnalyzDlg::DisplayChildDialog(int iIndex)
{
	if (iIndex == 0)
	{
		m_dlgSSJK = new CSSJKDlg;
		m_dlgSSJK->Create(IDD_DIALOG_SSJK, FALSE);
		m_dlgSSJK->MoveWindow(m_rChildDialog);
		m_dlgSSJK->ShowWindow(SW_SHOW);
	}
	else if (iIndex == 1)
	{
		m_dlgRLKGL = new CRLKGLDlg;
		m_dlgRLKGL->Create(IDD_DIALOG_RLKGL, FALSE);
		m_dlgRLKGL->MoveWindow(m_rChildDialog);
		m_dlgRLKGL->ShowWindow(SW_SHOW);
	}
	else if (iIndex == 3)
	{
		m_dlgSJLFX = new CSJFXDlg;
		m_dlgSJLFX->Create(IDD_DIALOG_SJFX, FALSE);
		m_dlgSJLFX->MoveWindow(m_rChildDialog);
		m_dlgSJLFX->ShowWindow(SW_SHOW);
	}
	else if (iIndex == 4)
	{
		m_dlgLSJL = new CLSJLDlg;
		m_dlgLSJL->Create(IDD_DIALOG_LSJL, FALSE);
		m_dlgLSJL->MoveWindow(m_rChildDialog);
		m_dlgLSJL->ShowWindow(SW_SHOW);
	}
	else if (iIndex == 5)
	{
		m_dlgSBGL = new CSBGLDlg;
		m_dlgSBGL->Create(IDD_DIALOG_SBGL, FALSE);
		m_dlgSBGL->MoveWindow(m_rChildDialog);
		m_dlgSBGL->ShowWindow(SW_SHOW);
	}
	else if (iIndex == 6)
	{
		m_dlgXTSZ = new CXTSZDlg;
		m_dlgXTSZ->Create(IDD_DIALOG_XTSZ, FALSE);
		m_dlgXTSZ->MoveWindow(m_rChildDialog);
		m_dlgXTSZ->ShowWindow(SW_SHOW);
	}
}
void CCrowdAnalyzDlg::DestoryCurDialog()
{
	if (m_iCurDlg == 0)
	{
		m_dlgSSJK->OnDestroy();
		delete m_dlgSSJK;
		m_dlgSSJK = NULL;
	}
	else if (m_iCurDlg == 1)
	{
		m_dlgRLKGL->OnDestroy();
		delete m_dlgRLKGL;
		m_dlgRLKGL = NULL;
	}
	else if (m_iCurDlg == 3)
	{
		m_dlgSJLFX->OnDestroy();
		delete m_dlgSJLFX;
		m_dlgSJLFX = NULL;
	}
	else if (m_iCurDlg == 4)
	{
		m_dlgLSJL->OnDestroy();
		delete m_dlgLSJL;
		m_dlgLSJL = NULL;
	}
	else if (m_iCurDlg == 5)
	{
		m_dlgSBGL->OnDestroy();
		delete m_dlgSBGL;
		m_dlgSBGL = NULL;
	}
	else if (m_iCurDlg == 6)
	{
		m_dlgXTSZ->OnDestroy();
		delete m_dlgXTSZ;
		m_dlgXTSZ = NULL;
	}
}

void CCrowdAnalyzDlg::SetLsjl()
{
	m_dlgLSJL = new CLSJLDlg;
	m_dlgLSJL->Create(IDD_DIALOG_LSJL, FALSE);
	m_dlgLSJL->MoveWindow(m_rChildDialog);
	m_dlgLSJL->m_csEndSql = m_csLSJLSql;
	m_dlgLSJL->m_csName = m_csLSJLIdcard;
	m_dlgLSJL->ShowXiangxi();
	//m_iCurDlg = 4;
}