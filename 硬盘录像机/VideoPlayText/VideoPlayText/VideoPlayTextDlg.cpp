
// VideoPlayTextDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoPlayText.h"
#include "VideoPlayTextDlg.h"
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


// CVideoPlayTextDlg 对话框



CVideoPlayTextDlg::CVideoPlayTextDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVideoPlayTextDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVideoPlayTextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
#include "VideoPlay.h"
BEGIN_MESSAGE_MAP(CVideoPlayTextDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(DOWNLOADEND, OnMyMsg)
	ON_MESSAGE(DOWNLOADERROR, OnMyMsg1)
	ON_BN_CLICKED(IDOK, &CVideoPlayTextDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDOK2, &CVideoPlayTextDlg::OnBnClickedOk2)
	ON_BN_CLICKED(IDOK3, &CVideoPlayTextDlg::OnBnClickedOk3)
END_MESSAGE_MAP()


// CVideoPlayTextDlg 消息处理程序
LRESULT CVideoPlayTextDlg::OnMyMsg(WPARAM wParam, LPARAM lParam)
{
		AfxMessageBox("消息已经收到！");
// 	COPYDATASTRUCT *p = NULL;
// 	p = (COPYDATASTRUCT*)lParam;
// 	CString strTemp;
// 	memcpy(strTemp.GetBuffer((int)p->cbData), p->lpData, p->cbData);
// 	strTemp.ReleaseBuffer();
	return 0;

}

LRESULT CVideoPlayTextDlg::OnMyMsg1(WPARAM wParam, LPARAM lParam)
{
	AfxMessageBox("1111111111111111！");
	// 	COPYDATASTRUCT *p = NULL;
	// 	p = (COPYDATASTRUCT*)lParam;
	// 	CString strTemp;
	// 	memcpy(strTemp.GetBuffer((int)p->cbData), p->lpData, p->cbData);
	// 	strTemp.ReleaseBuffer();
	return 0;

}
BOOL CVideoPlayTextDlg::OnInitDialog()
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CVideoPlayTextDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CVideoPlayTextDlg::OnPaint()
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
HCURSOR CVideoPlayTextDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LONG S1, S2;

#include "VideoPlay.h"
void CVideoPlayTextDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
//	CDialogEx::OnOK();
	VideoPlay dlg;
	CString csIp;
	int nidex;
	dlg.InitVideoPlay(this->m_hWnd);
	dlg.LoginVideoPlay("192.168.2.110", "8000", "admin", "unisib1000");
	dlg.PlayBackByTime(33, "20170609152000", "20170609152400", GetDlgItem(IDC_STATIC_PLAY)->m_hWnd);
	//dlg.SetMwssssss(this->m_hWnd);

//	dlg.LoginVideoPlay("192.168.2.110", "8000", "admin", "unisib1000");
//	dlg.DoGetDeviceResoureCfg();
//	CStringArray aridex;
//	int ttt = dlg.GetDevCameraSum(aridex);
//	int tttttt = dlg.GetDevCameraSum(aridex,TRUE);
//	int sum = aridex.GetSize();
//	for (int i = 0; i < aridex.GetSize();i++)
//	{
//		CString sssssss = aridex.GetAt(i);
//		BOOL bbbbbbbbbb;
//		dlg.GetDevCameraNumData(atoi(aridex.GetAt(i)) - 1, csIp, bbbbbbbbbb);
//		int tttt = 0;
//	}
////	dlg.SetPlay(GetDlgItem(IDC_STATIC_PLAY)->m_hWnd);
//	dlg.StartPlay(atoi(aridex.GetAt(0)), GetDlgItem(IDC_STATIC_PLAY)->m_hWnd, S1);
//	//dlg.StartPlay(atoi(aridex.GetAt(1)), GetDlgItem(IDC_STATIC_PLAY2)->m_hWnd, S2);
////	dlg.StartRecord(atoi(aridex.GetAt(0)), S1,"D:\\");
//	dlg.CapturePictureBmp(atoi(aridex.GetAt(0)), S1, "", "222");
//
//	BOOL ffffffffffffffffff = dlg.CapturePictureJpg(atoi(aridex.GetAt(0)), S1,0,0, "D:\\", "1111");
	int tttttttttttttt = 0;
}

VideoPlay dlg1111;
void ssss(int &tt)
{
	tt = 10;
}
void CVideoPlayTextDlg::OnBnClickedOk2()
{
	int sumd = 12222;
	ssss(sumd);
	int tttttttttttt = 0;
}


UINT GetDownloadPosThread(LPVOID lpam)
{
	GETDOWNLOAD* GetDownload = (GETDOWNLOAD*)lpam;

	CString sssss = GetDownload->csFileNam;
	return 0;
}
GETDOWNLOAD GetDownload;
int  GetDownloadPos(int nCurChanIndex, CString csStartTime, CString csEndTime, CString csFileNam, CString csFilPath)
{
// 	GETDOWNLOAD *GetDownload = new GETDOWNLOAD;
// 	GetDownload->StartTime.dwYear = atoi(csStartTime.Mid(0, 4));
// 	GetDownload->StartTime.dwMonth = atoi(csStartTime.Mid(4, 2));
// 	GetDownload->StartTime.dwDay = atoi(csStartTime.Mid(6, 2));
// 	GetDownload->StartTime.dwHour = atoi(csStartTime.Mid(8, 2));
// 	GetDownload->StartTime.dwMinute = atoi(csStartTime.Mid(10, 2));
// 	GetDownload->StartTime.dwSecond = atoi(csStartTime.Mid(12, 2));
// 
// 	GetDownload->StopTime.dwYear = atoi(csEndTime.Mid(0, 4));
// 	GetDownload->StopTime.dwMonth = atoi(csEndTime.Mid(4, 2));
// 	GetDownload->StopTime.dwDay = atoi(csEndTime.Mid(6, 2));
// 	GetDownload->StopTime.dwHour = atoi(csEndTime.Mid(8, 2));
// 	GetDownload->StopTime.dwMinute = atoi(csEndTime.Mid(10, 2));
// 	GetDownload->StopTime.dwSecond = atoi(csEndTime.Mid(12, 2));
// 
// 	GetDownload->nCurChanIndex = nCurChanIndex;
// 	GetDownload->csFileNam = csFileNam;
// 	GetDownload->csFilPath = csFilPath;
	


	GetDownload.StartTime.dwYear = atoi(csStartTime.Mid(0, 4));
	GetDownload.StartTime.dwMonth = atoi(csStartTime.Mid(4, 2));
	GetDownload.StartTime.dwDay = atoi(csStartTime.Mid(6, 2));
	GetDownload.StartTime.dwHour = atoi(csStartTime.Mid(8, 2));
	GetDownload.StartTime.dwMinute = atoi(csStartTime.Mid(10, 2));
	GetDownload.StartTime.dwSecond = atoi(csStartTime.Mid(12, 2));

	GetDownload.StopTime.dwYear = atoi(csEndTime.Mid(0, 4));
	GetDownload.StopTime.dwMonth = atoi(csEndTime.Mid(4, 2));
	GetDownload.StopTime.dwDay = atoi(csEndTime.Mid(6, 2));
	GetDownload.StopTime.dwHour = atoi(csEndTime.Mid(8, 2));
	GetDownload.StopTime.dwMinute = atoi(csEndTime.Mid(10, 2));
	GetDownload.StopTime.dwSecond = atoi(csEndTime.Mid(12, 2));
	int tttt= nCurChanIndex;
	GetDownload.nCurChanIndex = nCurChanIndex;
//	GetDownload.csFileNam = csFileNam;
	//GetDownload.csFilPath = csFilPath;
	AfxBeginThread(&GetDownloadPosThread, &GetDownload);
	//delete GetDownload;
	return 0;
}


void CVideoPlayTextDlg::OnBnClickedOk3()
{
  //GetDownloadPos(111,"2017 12 11 10 09 08","20160102030405","2222222","3333333333");
	CString csIp;
	int nidex;
	dlg1111.InitVideoPlay(this->m_hWnd);
	dlg1111.LoginVideoPlay("192.168.2.99", "8000", "admin", "unisib100");
	dlg1111.DoGetDeviceResoureCfg();
	CStringArray aridex;
	int ttt = dlg1111.GetDevCameraSum(aridex);

// 	int sum = aridex.GetSize();
// 	for (int i = 0; i < aridex.GetSize(); i++)
// 	{
// 		CString sssssss = aridex.GetAt(i);
// 		BOOL bbbbbbbbbb;
// 		int ttttttttttttttttt = 0;
// 		dlg1111.GetDevCameraNumDataToIP(atoi(aridex.GetAt(i)) - 1, csIp, bbbbbbbbbb);
// 		int tldkjlk = atoi(aridex.GetAt(i)) - 1;
// 		BOOL ssssssssss = dlg1111.GetDevCameraNumDataToIndex(ttttttttttttttttt, csIp, bbbbbbbbbb);
// 		int tttt = 0;
// 	}
	BOOL bb;

	int t1 = atoi(aridex.GetAt(0));
	int t2 = atoi(aridex.GetAt(1));
	dlg1111.GetDevCameraNumDataToIP(32, csIp, bb);
	dlg1111.GetDevCameraNumDataToIP(t2, csIp, bb);

	dlg1111.GetDevCameraNumDataToIndex(nidex, "192.168.2.102", bb);
	dlg1111.StartPlay(nidex , GetDlgItem(IDC_STATIC_PLAY)->m_hWnd, S1);
	dlg1111.GetDownloadPos(nidex  , 22, "20170615101417", "20170615101447", "cesehi", "");
}
