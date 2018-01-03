
// WinGUIClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WinGUIClient.h"
#include "WinGUIClientDlg.h"
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


// CWinGUIClientDlg �Ի���



CWinGUIClientDlg::CWinGUIClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWinGUIClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWinGUIClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWinGUIClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CWinGUIClientDlg ��Ϣ�������

BOOL CWinGUIClientDlg::OnInitDialog()
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

	//��󻯴���
	int cx = GetSystemMetrics(SM_CXFULLSCREEN);
	int cy = GetSystemMetrics(SM_CYFULLSCREEN);
	CRect rt;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rt, 0);
	cy = rt.bottom;
	HWND hWnd = AfxGetMainWnd()->m_hWnd;
	::MoveWindow(hWnd, 0, 0, cx, cy, TRUE);

	GetClientRect(&m_rClient);

	//����������ߴ�
	InitClients();

	//���и����ֶԻ���
	RunDialogs();


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CWinGUIClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CWinGUIClientDlg::OnPaint()
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
HCURSOR CWinGUIClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CWinGUIClientDlg::InitClients()
{
	m_rTop.top = m_rClient.top;
	m_rTop.bottom = m_rClient.Height() * 14 / 100;
	m_rTop.left = m_rClient.left;
	m_rTop.right = m_rClient.right;

	m_rDevice.top = m_rClient.Height() * 14 / 100;
	m_rDevice.bottom = m_rClient.Height() * 73 / 100;
	m_rDevice.left = m_rClient.left;
	m_rDevice.right = m_rClient.Width() * 24 / 100;
	
	m_rVideo.top = m_rClient.Height() * 14 / 100;
	m_rVideo.bottom = m_rClient.Height() * 80 / 100;
	m_rVideo.left = m_rClient.Width() * 24 / 100;
	m_rVideo.right = m_rClient.Width() * 80 / 100;
	theApp.m_rectVideo.SetRect(0, 0, m_rVideo.Width(), m_rVideo.Height());

	m_rBottom.top = m_rClient.Height() * 96 / 100;
	m_rBottom.bottom = m_rClient.bottom;
	m_rBottom.left = m_rClient.left;
	m_rBottom.right = m_rClient.right;

	m_rRTAlarm.top = m_rClient.Height() * 73 / 100;
	m_rRTAlarm.bottom = m_rClient.Height() * 96 / 100;
	m_rRTAlarm.left = m_rClient.left;
	m_rRTAlarm.right = m_rClient.Width() * 24 / 100;

	m_rRTAlarmInfo.top = m_rClient.Height() * 80 / 100;
	m_rRTAlarmInfo.bottom = m_rClient.Height() * 96 / 100;
	m_rRTAlarmInfo.left = m_rClient.Width() * 24 / 100;
	m_rRTAlarmInfo.right = m_rClient.Width() * 80 / 100;

	m_rHistory.top = m_rClient.Height() * 14 / 100;
	m_rHistory.bottom = m_rClient.Height() * 96 / 100;
	m_rHistory.left = m_rClient.Width() * 80 / 100;
	m_rHistory.right = m_rClient.Width() * 100 / 100;
}

void CWinGUIClientDlg::RunDialogs()
{
	//�����Ի���
	m_dlgTop = new CTopDlg;
	m_dlgTop->Create(IDD_DIALOG_TOP, FALSE);
	m_dlgTop->MoveWindow(m_rTop);
	m_dlgTop->m_rClient.top = m_dlgTop->m_rClient.left = 0;
	m_dlgTop->m_rClient.bottom = m_rTop.Height();
	m_dlgTop->m_rClient.right = m_rTop.Width();
	m_dlgTop->ShowWindow(SW_SHOW);

	//����豸�Ի���
	m_dlgDevice = new CDeviceDlg;
	m_dlgDevice->Create(IDD_DIALOG_DEVICE, FALSE);
	m_dlgDevice->MoveWindow(m_rDevice);
	m_dlgDevice->m_rClient.top = m_dlgDevice->m_rClient.left = 0;
	m_dlgDevice->m_rClient.bottom = m_rDevice.Height();
	m_dlgDevice->m_rClient.right = m_rDevice.Width();
	m_dlgDevice->ShowWindow(SW_SHOW);

	//��Ƶ�Ի���
	m_dlgVideo = new CVideoDlg;
	m_dlgVideo->Create(IDD_DIALOG_VIDEO, FALSE);
	m_dlgVideo->MoveWindow(m_rVideo);
	m_dlgVideo->m_rClient.top = m_dlgVideo->m_rClient.left = 0;
	m_dlgVideo->m_rClient.bottom = m_rVideo.Height();
	m_dlgVideo->m_rClient.right = m_rVideo.Width();
	m_dlgVideo->ShowWindow(SW_SHOW);

	//�ײ��Ի���
	m_dlgBottom = new CBottomDlg;
	m_dlgBottom->Create(IDD_DIALOG_BOTTOM, FALSE);
	m_dlgBottom->MoveWindow(m_rBottom);
	m_dlgBottom->m_rClient.top = m_dlgBottom->m_rClient.left = 0;
	m_dlgBottom->m_rClient.bottom = m_rBottom.Height();
	m_dlgBottom->m_rClient.right = m_rBottom.Width();
	m_dlgBottom->ShowWindow(SW_SHOW);

	//ʵʱ�����Ի���
	m_dlgRTAlarm = new CRTAlarmDlg;
	m_dlgRTAlarm->Create(IDD_DIALOG_RTALARM, FALSE);
	m_dlgRTAlarm->MoveWindow(m_rRTAlarm);
	m_dlgRTAlarm->m_rClient.top = m_dlgRTAlarm->m_rClient.left = 0;
	m_dlgRTAlarm->m_rClient.bottom = m_rRTAlarm.Height();
	m_dlgRTAlarm->m_rClient.right = m_rRTAlarm.Width();
	m_dlgRTAlarm->ShowWindow(SW_SHOW);

	//ʵʱ������Ϣ�Ի���
	m_dlgRTAlarmInfo = new CRTAlarmInfoDlg;
	m_dlgRTAlarmInfo->Create(IDD_DIALOG_RTALARMINFO, FALSE);
	m_dlgRTAlarmInfo->MoveWindow(m_rRTAlarmInfo);
	m_dlgRTAlarmInfo->m_rClient.top = m_dlgRTAlarmInfo->m_rClient.left = 0;
	m_dlgRTAlarmInfo->m_rClient.bottom = m_rRTAlarmInfo.Height();
	m_dlgRTAlarmInfo->m_rClient.right = m_rRTAlarmInfo.Width();
	m_dlgRTAlarmInfo->ShowWindow(SW_SHOW);

	//������ʷ�Ի���
	m_dlgHistory = new CHistoryDlg;
	m_dlgHistory->Create(IDD_DIALOG_HISTORY, FALSE);
	m_dlgHistory->MoveWindow(m_rHistory);
	m_dlgHistory->m_rClient.top = m_dlgHistory->m_rClient.left = 0;
	m_dlgHistory->m_rClient.bottom = m_rHistory.Height();
	m_dlgHistory->m_rClient.right = m_rHistory.Width();
	m_dlgHistory->ShowWindow(SW_SHOW);
}