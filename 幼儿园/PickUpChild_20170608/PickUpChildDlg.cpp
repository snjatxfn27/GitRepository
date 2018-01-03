
// PickUpChildDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PickUpChild.h"
#include "PickUpChildDlg.h"
#include "afxdialogex.h"
#include "ChildManagerDlg.h"
#include "ParentManagerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WIDTH 1024
#define HEIGHT 768
CEvent _WaitConnect[MAX_VIDEO_THREAD];


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

UINT PlayerThread(void* param)
{
	THREAD_PARAM * pParam = (THREAD_PARAM *)param;
	int nSelfID = pParam->nID;
	CPickUpChildDlg *pParent = (CPickUpChildDlg*)(pParam->pParent);

	pParent->m_aThreadDataArray[nSelfID].nPlayerStatus = VIDEO_CONNECTING;
	pParent->ShowImage(pParent->m_matBlank, nSelfID, 0.5,
		"Loading...... please wait\n" + pParent->m_aThreadDataArray[nSelfID].sAddress);

	cv::Mat matFrame;
	pParent->m_aThreadDataArray[nSelfID].capture.open(pParent->m_aThreadDataArray[nSelfID].sAddress);
	//cv::VideoCapture capture("rtsp://218.204.223.237:554/live/1/66251FC11353191F/e7ooqwcfbqjoo80j.sdp");
	//cv::VideoCapture capture(0);
	if (pParent->m_aThreadDataArray[nSelfID].capture.isOpened())
	{
		pParent->m_aThreadDataArray[nSelfID].nPlayerStatus = VIDEO_PLAYING;
		_WaitConnect[nSelfID].SetEvent();

		while ((pParent->m_nActiveThreadID == nSelfID) && (!pParent->m_bStopFlag))
		{
			if (pParent->m_aThreadDataArray[nSelfID].capture.read(matFrame))
			{
				pParent->m_aThreadDataArray[nSelfID].nFrameCount++;
				if (pParent->m_aThreadDataArray[nSelfID].matFrame.empty())
					pParent->m_aThreadDataArray[nSelfID].matFrame = matFrame.clone();
			}
			Sleep(5);
		}
		//pParent->ShowImage(pParent->m_matBlank, nSelfID, "play finished");
	}
	else
	{
		pParent->ShowImage(pParent->m_matBlank, nSelfID, 0.5, "Can not connect to carama!!");
		pParent->m_aThreadDataArray[nSelfID].nPlayerStatus = VIDEO_STOPED;
		_WaitConnect[nSelfID].SetEvent();
		return -1;
	}

	return 0;
}
UINT ShowThread(void* param)
{
	THREAD_PARAM * pParam = (THREAD_PARAM *)param;
	int nSelfID = pParam->nID;
	CPickUpChildDlg *pParent = (CPickUpChildDlg*)(pParam->pParent);

	cv::Mat matFrame;
	//////////
	std::string sOutputstr;
	std::stringstream ss;
	ss << nSelfID;
	ss >> sOutputstr;
	sOutputstr = pParent->m_aThreadDataArray[nSelfID].sAddress + "\nCurrent Thread ID:" + sOutputstr;
	///////////

	while ((pParent->m_nActiveThreadID == nSelfID) && (!pParent->m_bStopFlag))
	{
		if (pParent->m_aThreadDataArray[nSelfID].matFrame.data)
		{
			matFrame = pParent->m_aThreadDataArray[nSelfID].matFrame.clone();
			pParent->m_aThreadDataArray[nSelfID].matFrame.release();

			// if face detect
			//			if (pParent->m_bDetectFace) pParent->DetectFace(matFrame);

			// if erase background
			if (pParent->m_bEraseBkg) pParent->EraseBkg(matFrame);

			if (pParent->m_aThreadDataArray[nSelfID].nFrameCount < 80) pParent->ShowImage(matFrame, nSelfID, 0.5, sOutputstr);
			else pParent->ShowImage(matFrame, nSelfID);
		}
		Sleep(1);
	}

	if (pParent->m_bStopFlag)
	{
		//		pParent->m_pTarget->Invalidate();
		pParent->InvalidateRect(pParent->m_rShowRect, false);
	}

	return 0;
}

UINT WatcherThread(void* param)
{
	THREAD_PARAM * pParam = (THREAD_PARAM *)param;
	int nSelfID = pParam->nID;
	CPickUpChildDlg *pParent = (CPickUpChildDlg*)(pParam->pParent);
	int nConnectTimeOut = pParent->m_nConnectTimeOut;
	int nStreamTimeOut = pParent->m_nStreamTimeOut;

	int nLastFrameCount = 0;
	BOOL bStreamStoped = false;
	BOOL bStreamLost = false;

	_WaitConnect[nSelfID].ResetEvent();
	WaitForSingleObject(_WaitConnect[nSelfID], nConnectTimeOut);

	if (pParent->m_aThreadDataArray[nSelfID].nPlayerStatus == VIDEO_CONNECTING)
	{
		pParent->ShowImage(pParent->m_matBlank, nSelfID, 0.6, "Can not connect to carama!");
		TerminateThread(pParent->m_aThreadDataArray[nSelfID].hPlayer, 0);
		pParent->m_aThreadDataArray[nSelfID].nPlayerStatus = VIDEO_STOPED;
		pParent->m_aThreadDataArray[nSelfID].hPlayer = 0;
		pParent->m_aThreadDataArray[nSelfID].hWatcher = 0;
		pParent->m_aThreadDataArray[nSelfID].nFrameCount = 0;
		pParent->m_aThreadDataArray[nSelfID].sAddress = "";
		return 0;
	}

	while ((pParent->m_nActiveThreadID == nSelfID) && (!pParent->m_bStopFlag))
	{
		Sleep(nStreamTimeOut);
		if (nLastFrameCount == pParent->m_aThreadDataArray[nSelfID].nFrameCount)
		{
			if (bStreamStoped)
			{
				pParent->ShowImage(pParent->m_matBlank, nSelfID, 0.6, "Video stream lost!");
				bStreamLost = true;
			}
			else
			{
				bStreamStoped = true;
			}
		}
		else
		{
			bStreamLost = false;
			bStreamStoped = false;
			nLastFrameCount = pParent->m_aThreadDataArray[nSelfID].nFrameCount;
		}
	}
	if (bStreamLost) TerminateThread(pParent->m_aThreadDataArray[nSelfID].hPlayer, 0);
	pParent->m_aThreadDataArray[nSelfID].nPlayerStatus = VIDEO_STOPED;
	pParent->m_aThreadDataArray[nSelfID].hPlayer = 0;
	pParent->m_aThreadDataArray[nSelfID].hWatcher = 0;
	pParent->m_aThreadDataArray[nSelfID].nFrameCount = 0;
	pParent->m_aThreadDataArray[nSelfID].sAddress = "";

	return 0;
}

// CPickUpChildDlg 对话框



CPickUpChildDlg::CPickUpChildDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPickUpChildDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPickUpChildDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPickUpChildDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CPickUpChildDlg 消息处理程序

BOOL CPickUpChildDlg::OnInitDialog()
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

	MoveWindow(CRect(0, 0, WIDTH,HEIGHT), false);
	m_rClient.SetRect(0, 0, WIDTH, HEIGHT);
	m_rChildManagerButton.SetRect(WIDTH * 12 / 1024, HEIGHT * 649 / 768, WIDTH * 183 / 1024, HEIGHT * 673 / 768);
	m_rParentManagerButton.SetRect(WIDTH * 12 / 1024, HEIGHT * 612 / 768, WIDTH * 183 / 1024, HEIGHT * 637 / 768);
	m_rConfirmButton.SetRect(WIDTH * 933 / 1024, HEIGHT * 336 / 768, WIDTH * 1005 / 1024, HEIGHT * 370 / 768);
	InitInChildArea();
	InitOutChildArea();
	InitDisParentArea();
	m_uiCurInPage = 0;
	RefreshInChildArea(m_uiCurInPage);
	m_uiCurOutPage = 0;
	RefreshOutChildArea(m_uiCurOutPage);
	m_iCurSelInNum = -1;

	m_uiTodayAllCount = m_uiEndCount = m_uiInCount = m_uiOutCount = 0;
	RefreshCount();
	RefreshDisParent(-1);
	RefreshHistory();

	m_rShowRect.SetRect(WIDTH * 9 / 1024, HEIGHT * 46 / 768, WIDTH * 186 / 1024, HEIGHT * 222 / 768);
	InitPlayEnviroment();
	Play("rtsp://admin:unisib1000@192.168.2.100:554/h264/ch1/main/av_stream");

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPickUpChildDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPickUpChildDlg::OnPaint()
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
		Image img_bk(L"img/bk.jpg");
		
		g.DrawImage(&img_bk, 0, 0, m_rClient.Width(), m_rClient.Height());
		Pen grayPen(Color(255, 215, 215, 215), 1);
		Pen redPen(Color(255, 255, 0, 0), 3);
		SolidBrush nameBrush(Color(255, 0, 0, 0));
		Gdiplus::Font font(L"新宋体", WIDTH/70, FontStyleRegular, UnitPixel);
		g.SetTextRenderingHint(TextRenderingHintClearTypeGridFit);
		int i = 0;
		//画在校学生框格
		for (i = 0; i < 21; i++)
		{
			if (i == m_iCurSelInNum)
				g.DrawRectangle(&redPen, Gdiplus::Rect(m_curInChild[i].rInChildRect.left, m_curInChild[i].rInChildRect.top, m_curInChild[i].rInChildRect.Width(), m_curInChild[i].rInChildRect.Height()));
			else
				g.DrawRectangle(&grayPen, Gdiplus::Rect(m_curInChild[i].rInChildRect.left, m_curInChild[i].rInChildRect.top, m_curInChild[i].rInChildRect.Width(), m_curInChild[i].rInChildRect.Height()));
		}
		//画在校学生图片
		for (i = 0; i < 21; i++)
		{
			if (m_curInChild[i].iCurInID == -1)
				break;
			Image img_in(m_curInChild[i].strPhotoName);
			g.DrawImage(&img_in, m_curInChild[i].rInChildRect.left, m_curInChild[i].rInChildRect.top, m_curInChild[i].rInChildRect.Width(), m_curInChild[i].rInChildRect.Width());
			PointF pointF(m_curInChild[i].rInChildRect.left + m_curInChild[i].rInChildRect.Width()/20, m_curInChild[i].rInChildRect.top + m_curInChild[i].rInChildRect.Width()*11/10);
			g.DrawString(m_curInChild[i].strName.AllocSysString(), -1, &font, pointF, &nameBrush);
		}
		//画离校学生框格
		for (i = 0; i < 21; i++)
			g.DrawRectangle(&grayPen, Gdiplus::Rect(m_curOutChild[i].rOutChildRect.left, m_curOutChild[i].rOutChildRect.top, m_curOutChild[i].rOutChildRect.Width(), m_curOutChild[i].rOutChildRect.Height()));
		//画离校学生图片
		for (i = 0; i < 21; i++)
		{
			if (m_curOutChild[i].iCurOutID == -1)
				break;
			Image img_in(m_curOutChild[i].strPhotoName);
			g.DrawImage(&img_in, m_curOutChild[i].rOutChildRect.left, m_curOutChild[i].rOutChildRect.top, m_curOutChild[i].rOutChildRect.Width(), m_curOutChild[i].rOutChildRect.Width());
			PointF pointF(m_curOutChild[i].rOutChildRect.left + m_curOutChild[i].rOutChildRect.Width() / 20, m_curOutChild[i].rOutChildRect.top + m_curOutChild[i].rOutChildRect.Width() * 11 / 10);
			g.DrawString(m_curOutChild[i].strName.AllocSysString(), -1, &font, pointF, &nameBrush);
		}
		//画家长图片
		float alpha = 0.5;
		ColorMatrix cm = {
			1, 0, 0, 0, 0,
			0, 1, 0, 0, 0,
			0, 0, 1, 0, 0,
			0, 0, 0, alpha, 0,
			0, 0, 0, 0, 1 };
		ImageAttributes imageAttr;
		imageAttr.SetColorMatrix(&cm);
		m_curDisParent[1].isRight = true;
		for (i = 0; i < 3; i++)
		{
			if (m_curDisParent[i].iID == -1)
				break;
			Image img_disparent(m_curDisParent[i].strPhotoName);
			if (!m_curDisParent[i].isRight)
			{
				int w = img_disparent.GetWidth();
				int h = img_disparent.GetHeight();
				Gdiplus::Rect rect(m_curDisParent[i].rCurDisParent.left, m_curDisParent[i].rCurDisParent.top, m_curDisParent[i].rCurDisParent.Width(), m_curDisParent[i].rCurDisParent.Height());
				g.DrawImage(&img_disparent, rect, 0, 0, w, h, UnitPixel, &imageAttr);    //透明alpha 
			}
			else
				g.DrawImage(&img_disparent, m_curDisParent[i].rCurDisParent.left, m_curDisParent[i].rCurDisParent.top, m_curDisParent[i].rCurDisParent.Width(), m_curDisParent[i].rCurDisParent.Height());
		}
		//写识别成功的家长信息
		SolidBrush parentinfoBrush(Color(255, 50, 50, 50));
		PointF pointFparentinfo1(WIDTH * 780 / 1024, HEIGHT * 330 / 768);
		PointF pointFparentinfo2(WIDTH * 780 / 1024, HEIGHT * 357 / 768);
		PointF pointFparentinfo3(WIDTH * 780 / 1024, HEIGHT * 384 / 768);
		PointF pointFparentinfo4(WIDTH * 780 / 1024, HEIGHT * 411 / 768);
/*		LOGFONTW lfont;
		memset(&lfont, 0, sizeof(lfont));
		lfont.lfHeight = -15, //   nHeight   注意使用负值，表示character height,  正值表示 cell height  
		lfont.lfWidth = 0,  //   nWidth  
		    //0,     //   nEscapement  
		    //0,     //   nOrientation  
		lfont.lfWeight = FW_NORMAL,  //nWeight  
		lfont.lfItalic = FALSE,      //bItalic  
		lfont.lfUnderline = FALSE,   //   bUnderline  
		lfont.lfStrikeOut = 0,         //   cStrikeOut  
		lfont.lfCharSet = DEFAULT_CHARSET,   //   nCharSet  
		lfont.lfOutPrecision = OUT_DEFAULT_PRECIS,//   nOutPrecision  
		lfont.lfClipPrecision = CLIP_DEFAULT_PRECIS,  //   nClipPrecision  
		lfont.lfQuality = CLEARTYPE_QUALITY,      //   nQuality  
		lfont.lfPitchAndFamily = DEFAULT_PITCH | FF_SWISS,//   nPitchAndFamily  
		wcscpy_s(lfont.lfFaceName, (L"微软雅黑"));  //   lpszFacename 
		Gdiplus::Font fontParentInfo(hDC, &lfont);*/
		Gdiplus::Font fontParentInfo(L"微软雅黑", WIDTH / 80, FontStyleRegular, UnitPixel);
		for (i = 0; i < 3; i++)
		{
			if (m_curDisParent[i].iID!=-1 && m_curDisParent[i].isRight)
			{
				CString strParentName, strParentRelation, strChildClass, strParentPhone;
				strParentName.Format(L"家长姓名：%s", m_curDisParent[i].strName);
				strParentRelation.Format(L"与学生关系：%s", m_curDisParent[i].strRelation);
				strChildClass.Format(L"学生班级：%s", m_curDisParent[i].strClassName);
				strParentPhone.Format(L"联系方式：%s", m_curDisParent[i].strPhone);
				g.DrawString(strParentName.AllocSysString(), -1, &fontParentInfo, pointFparentinfo1, &parentinfoBrush);
				g.DrawString(strParentRelation.AllocSysString(), -1, &fontParentInfo, pointFparentinfo2, &parentinfoBrush);
				g.DrawString(strChildClass.AllocSysString(), -1, &fontParentInfo, pointFparentinfo3, &parentinfoBrush);
				g.DrawString(strParentPhone.AllocSysString(), -1, &fontParentInfo, pointFparentinfo4, &parentinfoBrush);
				break;
			}
		}
		//写接送记录
		PointF history1(WIDTH * 780 / 1024, HEIGHT * 493 / 768);
		PointF history2(WIDTH * 780 / 1024, HEIGHT * 523 / 768);
		PointF history3(WIDTH * 780 / 1024, HEIGHT * 553 / 768);
		PointF history4(WIDTH * 780 / 1024, HEIGHT * 583 / 768);
		PointF history5(WIDTH * 780 / 1024, HEIGHT * 613 / 768);
		PointF history6(WIDTH * 780 / 1024, HEIGHT * 643 / 768);
		PointF history7(WIDTH * 780 / 1024, HEIGHT * 673 / 768);
		PointF history8(WIDTH * 780 / 1024, HEIGHT * 703 / 768);
		PointF history9(WIDTH * 780 / 1024, HEIGHT * 733 / 768);
		g.DrawString(m_strHistory[0].AllocSysString(), -1, &fontParentInfo, history1, &parentinfoBrush);
		g.DrawString(m_strHistory[1].AllocSysString(), -1, &fontParentInfo, history2, &parentinfoBrush);
		g.DrawString(m_strHistory[2].AllocSysString(), -1, &fontParentInfo, history3, &parentinfoBrush);
		g.DrawString(m_strHistory[3].AllocSysString(), -1, &fontParentInfo, history4, &parentinfoBrush);
		g.DrawString(m_strHistory[4].AllocSysString(), -1, &fontParentInfo, history5, &parentinfoBrush);
		g.DrawString(m_strHistory[5].AllocSysString(), -1, &fontParentInfo, history6, &parentinfoBrush);
		g.DrawString(m_strHistory[6].AllocSysString(), -1, &fontParentInfo, history7, &parentinfoBrush);
		g.DrawString(m_strHistory[7].AllocSysString(), -1, &fontParentInfo, history8, &parentinfoBrush);
		g.DrawString(m_strHistory[8].AllocSysString(), -1, &fontParentInfo, history9, &parentinfoBrush);
		//写4种人数
		SolidBrush num1Brush(Color(255, 98, 169, 225));
		SolidBrush num2Brush(Color(255, 235, 168, 81));
		SolidBrush num3Brush(Color(255, 121, 165, 130));
		SolidBrush num4Brush(Color(255, 223, 96, 103));
		PointF pointFnum1(WIDTH * 847 / 1024, HEIGHT * 60 / 768);
		PointF pointFnum2(WIDTH * 977 / 1024, HEIGHT * 60 / 768);
		PointF pointFnum3(WIDTH * 847 / 1024, HEIGHT * 119 / 768);
		PointF pointFnum4(WIDTH * 977 / 1024, HEIGHT * 119 / 768);
		Gdiplus::Font fontNum(L"黑体", WIDTH / 70, FontStyleRegular, UnitPixel);
		CString strTodayAllCount,strEndCount,strInCount,strOutCount;
		strTodayAllCount.Format(L"%03d", m_uiTodayAllCount);
		strEndCount.Format(L"%03d", m_uiEndCount);
		strInCount.Format(L"%03d", m_uiInCount);
		strOutCount.Format(L"%03d", m_uiOutCount);
		g.DrawString(strTodayAllCount.AllocSysString(), -1, &fontNum, pointFnum1, &num1Brush);
		g.DrawString(strEndCount.AllocSysString(), -1, &fontNum, pointFnum2, &num2Brush);
		g.DrawString(strInCount.AllocSysString(), -1, &fontNum, pointFnum3, &num3Brush);
		g.DrawString(strOutCount.AllocSysString(), -1, &fontNum, pointFnum4, &num4Brush);

		g.ReleaseHDC(hDC);
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPickUpChildDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPickUpChildDlg::InitInChildArea()
{
	//区域宽、高
	int iAreaWidth = WIDTH * 68 / 1024;
	int iAreaHeight = HEIGHT * 93 / 768;
	//区域宽间隔、高间隔
	int iAreaJGWidth = WIDTH * 11 / 1024;
	int iAreaJGHeight = HEIGHT * 11 / 768;
	//区域起始左、上
	int iAreaStartLeft = WIDTH * 203 / 1024;
	int iAreaStartTop = HEIGHT * 73 / 768;
	//定义21个区域
	int i = 0;
	for (i = 0; i < 21; i++)
	{
		m_curInChild[i].rInChildRect.SetRect(iAreaStartLeft + (iAreaWidth + iAreaJGWidth)*(i % 7),
			iAreaStartTop + (iAreaHeight + iAreaJGHeight)*(i / 7),
			iAreaStartLeft + (iAreaWidth + iAreaJGWidth)*(i % 7) + iAreaWidth,
			iAreaStartTop + (iAreaHeight + iAreaJGHeight)*(i / 7) + iAreaHeight);
	}
}

void CPickUpChildDlg::RefreshInChildArea(unsigned int uiCurPage)
{
	int i = 0;
	for (i = 0; i < 21; i++)
		m_curInChild[i].iCurInID = -1;
	_ConnectionPtr conn;
	conn.CreateInstance(__uuidof(Connection));
	CString strConn;
	strConn.Format(_T("Driver={MySQL ODBC 5.1 Driver}; Server = %s; Database = %s;uid=%s;pwd=%s;"), theApp.m_strDBIP, theApp.m_strDBName, theApp.m_strDBUsername, theApp.m_strDBPassword);
	try
	{
		conn->Open(_bstr_t(strConn), "", "", adModeUnknown);
	}
	catch (_com_error e)
	{
		return;
	}
	_RecordsetPtr pSet;
	pSet.CreateInstance(__uuidof(Recordset));
	CString strSQL;
	strSQL.Format(_T("select * from child where ishere=1 and todayishere=1 limit %d,21"), uiCurPage*21);
	pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	int num = 0;
	while (!pSet->adoEOF)
	{
		m_curInChild[num].iCurInID = getsqldouble("id");
		m_curInChild[num].strPhotoName.Format(L"child_photo/%d.jpg", m_curInChild[num].iCurInID);
		m_curInChild[num].strName = getsqlstr("name");
		pSet->MoveNext();
		num++;
	}
	pSet->Close();
	conn->Close();
}

void CPickUpChildDlg::InitOutChildArea()
{
	//区域宽、高
	int iAreaWidth = WIDTH * 68 / 1024;
	int iAreaHeight = HEIGHT * 93 / 768;
	//区域宽间隔、高间隔
	int iAreaJGWidth = WIDTH * 11 / 1024;
	int iAreaJGHeight = HEIGHT * 11 / 768;
	//区域起始左、上
	int iAreaStartLeft = WIDTH * 203 / 1024;
	int iAreaStartTop = HEIGHT * 425 / 768;
	//定义21个区域
	int i = 0;
	for (i = 0; i < 21; i++)
	{
		m_curOutChild[i].rOutChildRect.SetRect(iAreaStartLeft + (iAreaWidth + iAreaJGWidth)*(i % 7),
			iAreaStartTop + (iAreaHeight + iAreaJGHeight)*(i / 7),
			iAreaStartLeft + (iAreaWidth + iAreaJGWidth)*(i % 7) + iAreaWidth,
			iAreaStartTop + (iAreaHeight + iAreaJGHeight)*(i / 7) + iAreaHeight);
	}
}

void CPickUpChildDlg::RefreshOutChildArea(unsigned int uiCurPage)
{
	int i = 0;
	for (i = 0; i < 21; i++)
		m_curOutChild[i].iCurOutID = -1;
	_ConnectionPtr conn;
	conn.CreateInstance(__uuidof(Connection));
	CString strConn;
	strConn.Format(_T("Driver={MySQL ODBC 5.1 Driver}; Server = %s; Database = %s;uid=%s;pwd=%s;"), theApp.m_strDBIP, theApp.m_strDBName, theApp.m_strDBUsername, theApp.m_strDBPassword);
	try
	{
		conn->Open(_bstr_t(strConn), "", "", adModeUnknown);
	}
	catch (_com_error e)
	{
		return;
	}
	_RecordsetPtr pSet;
	pSet.CreateInstance(__uuidof(Recordset));
	CString strSQL;
	strSQL.Format(_T("select * from child where ishere=0 and todayishere=1 limit %d,21"), uiCurPage * 21);
	pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	int num = 0;
	while (!pSet->adoEOF)
	{
		m_curOutChild[num].iCurOutID = getsqldouble("id");
		m_curOutChild[num].strPhotoName.Format(L"child_photo/%d.jpg", m_curOutChild[num].iCurOutID);
		m_curOutChild[num].strName = getsqlstr("name");
		pSet->MoveNext();
		num++;
	}
	pSet->Close();
	conn->Close();
}
void CPickUpChildDlg::RefreshCount()
{
	_ConnectionPtr conn;
	conn.CreateInstance(__uuidof(Connection));
	CString strConn;
	strConn.Format(_T("Driver={MySQL ODBC 5.1 Driver}; Server = %s; Database = %s;uid=%s;pwd=%s;"), theApp.m_strDBIP, theApp.m_strDBName, theApp.m_strDBUsername, theApp.m_strDBPassword);
	try
	{
		conn->Open(_bstr_t(strConn), "", "", adModeUnknown);
	}
	catch (_com_error e)
	{
		return;
	}
	_RecordsetPtr pSet;
	pSet.CreateInstance(__uuidof(Recordset));
	CString strSQL;
	strSQL.Format(_T("%s"),L"select count(id) as num from child where todayishere=1");
	pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	m_uiTodayAllCount = getsqlint("num");
	pSet->Close();
	strSQL.Format(_T("%s"), L"select count(id) as num from child where todayishere=1 and isend=1");
	pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	m_uiEndCount = getsqlint("num");
	pSet->Close();
	strSQL.Format(_T("%s"), L"select count(id) as num from child where todayishere=1 and ishere=0");
	pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	m_uiInCount = getsqlint("num");
	pSet->Close();
	strSQL.Format(_T("%s"), L"select count(id) as num from child where todayishere=1 and ishere=1");
	pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	m_uiOutCount = getsqlint("num");
	pSet->Close();
	conn->Close();
}

void CPickUpChildDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (m_rChildManagerButton.PtInRect(point))
	{
		CChildManagerDlg dlg;
		dlg.DoModal();
	}
	else if (m_rParentManagerButton.PtInRect(point))
	{
		CParentManagerDlg dlg;
		dlg.DoModal();
	}
	else if (m_rConfirmButton.PtInRect(point))
	{
		SetChildStatus(m_curInChild[m_iCurSelInNum].iCurInID, 0, -1, -1);
		int i;
		for (i = 0; i < 3; i++)
		{
			if (m_curDisParent[i].isRight)
			{
				AddHistory(m_curInChild[m_iCurSelInNum].iCurInID, m_curDisParent[i].iID);
				break;
			}
		}
		RefreshHistory();
		RefreshInChildArea(0);
		RefreshOutChildArea(0);
		RefreshCount();
		m_iCurSelInNum = -1;
		RefreshDisParent(-1);
		Invalidate(false);
	}
	int i = 0;
	for (i = 0; i < 21; i++)
	{
		if (m_curInChild[i].rInChildRect.PtInRect(point))
		{
			m_iCurSelInNum = i;
			RefreshDisParent(m_curInChild[i].iCurInID);
			Invalidate(false);
			break;
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CPickUpChildDlg::InitDisParentArea()
{
	m_curDisParent[0].rCurDisParent.SetRect(WIDTH * 766 / 1024, HEIGHT * 210 / 768, WIDTH * 836 / 1024, HEIGHT * 303 / 768);
	m_curDisParent[1].rCurDisParent.SetRect(WIDTH * 846 / 1024, HEIGHT * 210 / 768, WIDTH * 916 / 1024, HEIGHT * 303 / 768);
	m_curDisParent[2].rCurDisParent.SetRect(WIDTH * 926 / 1024, HEIGHT * 210 / 768, WIDTH * 996 / 1024, HEIGHT * 303 / 768);
}

void CPickUpChildDlg::RefreshDisParent(int iChildID)
{
	int i = 0;
	for (i = 0; i < 3; i++)
	{
		m_curDisParent[i].iID = -1;
		m_curDisParent[i].isRight = false;
	}
	if (iChildID == -1)
		return;

	_ConnectionPtr conn;
	conn.CreateInstance(__uuidof(Connection));
	CString strConn;
	strConn.Format(_T("Driver={MySQL ODBC 5.1 Driver}; Server = %s; Database = %s;uid=%s;pwd=%s;"), theApp.m_strDBIP, theApp.m_strDBName, theApp.m_strDBUsername, theApp.m_strDBPassword);
	try
	{
		conn->Open(_bstr_t(strConn), "", "", adModeUnknown);
	}
	catch (_com_error e)
	{
		return;
	}
	_RecordsetPtr pSet;
	pSet.CreateInstance(__uuidof(Recordset));
	CString strSQL;
	strSQL.Format(_T("select parent.id as id,parent.name as name,parent.relation as relation,parent.phone as phone,class.name as classname from parent left join child on child.id=parent.childid left join class on class.id=child.classid where childid=%d"), iChildID);
	pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	int num = 0;
	while (!pSet->adoEOF)
	{
		m_curDisParent[num].iID = getsqldouble("id");
		m_curDisParent[num].strPhotoName.Format(L"parent_photo/%d.jpg", m_curDisParent[num].iID);
		m_curDisParent[num].strName = getsqlstr("name");
		m_curDisParent[num].strRelation = getsqlstr("relation");
		m_curDisParent[num].strPhone = getsqlstr("phone");
		m_curDisParent[num].strClassName = getsqlstr("classname");
		pSet->MoveNext();
		num++;
	}
	pSet->Close();
	conn->Close();
}

void CPickUpChildDlg::RefreshHistory()
{
	int i = 0;
	for (i = 0; i < 9; i++)
		m_strHistory[i].Empty();

	_ConnectionPtr conn;
	conn.CreateInstance(__uuidof(Connection));
	CString strConn;
	strConn.Format(_T("Driver={MySQL ODBC 5.1 Driver}; Server = %s; Database = %s;uid=%s;pwd=%s;"), theApp.m_strDBIP, theApp.m_strDBName, theApp.m_strDBUsername, theApp.m_strDBPassword);
	try
	{
		conn->Open(_bstr_t(strConn), "", "", adModeUnknown);
	}
	catch (_com_error e)
	{
		return;
	}
	_RecordsetPtr pSet;
	pSet.CreateInstance(__uuidof(Recordset));
	CString strSQL;
	strSQL.Format(_T("%s"), L"select child.name as childname,parent.name as parentname,history.pickuptime from history left join child on child.id=history.childid left join parent on parent.id=history.parentid order by pickuptime desc limit 0,9");
	pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	int num = 0;
	while (!pSet->adoEOF)
	{
		CString strChildName,strPickUpTime,strParentName;
		strChildName = getsqlstr("childname");
		strPickUpTime = getsqlstr("pickuptime");
		strParentName = getsqlstr("parentname");
		m_strHistory[num].Format(L"%s %s 已被%s接走", strChildName, strPickUpTime, strParentName);
		pSet->MoveNext();
		num++;
	}
	pSet->Close();
	conn->Close();
}

void CPickUpChildDlg::SetChildStatus(int iChildID, int iIsHere, int iTodayIsHere, int iIsEnd)
{
	_ConnectionPtr conn;
	conn.CreateInstance(__uuidof(Connection));
	CString strConn;
	strConn.Format(_T("Driver={MySQL ODBC 5.1 Driver}; Server = %s; Database = %s;uid=%s;pwd=%s;"), theApp.m_strDBIP, theApp.m_strDBName, theApp.m_strDBUsername, theApp.m_strDBPassword);
	try
	{
		conn->Open(_bstr_t(strConn), "", "", adModeUnknown);
	}
	catch (_com_error e)
	{
		return;
	}
	_RecordsetPtr pSet;
	pSet.CreateInstance(__uuidof(Recordset));
	CString strSQL;
	strSQL.Format(_T("%s%d"), L"select * from child where id=",iChildID);
	pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	if (!pSet->adoEOF)
	{
		CString strTemp;
		if (iIsHere != -1)
		{
			strTemp.Format(L"%d", iIsHere);
			pSet->put_Collect(_variant_t("ishere"), _variant_t(strTemp));
		}
		if (iTodayIsHere != -1)
		{
			strTemp.Format(L"%d", iTodayIsHere);
			pSet->put_Collect(_variant_t("todayishere"), _variant_t(strTemp));
		}
		if (iIsEnd != -1)
		{
			strTemp.Format(L"%d", iIsEnd);
			pSet->put_Collect(_variant_t("isend"), _variant_t(strTemp));
		}
		pSet->Update();
	}
	pSet->Close();
	conn->Close();
}

void CPickUpChildDlg::AddHistory(int iChildID, int iParentID)
{
	_ConnectionPtr conn;
	conn.CreateInstance(__uuidof(Connection));
	CString strConn;
	strConn.Format(_T("Driver={MySQL ODBC 5.1 Driver}; Server = %s; Database = %s;uid=%s;pwd=%s;"), theApp.m_strDBIP, theApp.m_strDBName, theApp.m_strDBUsername, theApp.m_strDBPassword);
	try
	{
		conn->Open(_bstr_t(strConn), "", "", adModeUnknown);
	}
	catch (_com_error e)
	{
		return;
	}
	_RecordsetPtr pSet;
	pSet.CreateInstance(__uuidof(Recordset));
	CString strSQL;
	strSQL.Format(_T("%s"), L"select * from history limit 0,1");
	pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	CString strChildID, strParentID;
	strChildID.Format(L"%d", iChildID);
	strParentID.Format(L"%d", iParentID);
	pSet->AddNew();
	pSet->put_Collect(_variant_t("childid"), _variant_t(strChildID));
	pSet->put_Collect(_variant_t("parentid"), _variant_t(strParentID));
	pSet->Update();
	pSet->Close();
	conn->Close();
}

void CPickUpChildDlg::ShowImage(Mat& matSource, int nID)
{
	//	if ((nID != m_nActiveThreadID) || m_bStopFlag) return;

	CRect rect;
	CImage imgDst;

	// Get Display area rect
	cv::Size winSize(m_rShowRect.right, m_rShowRect.bottom);

	// Resize the source to the size of the destination image if necessary
	//cv::Mat frame(winSize, CV_8UC3);
	Mat frame;
	if (matSource.size() != winSize)
		cv::resize(matSource, frame, winSize);
	else
		frame = matSource.clone();

	//	m_pTarget->GetClientRect(&rect);
	SetRect(rect, m_rShowRect.left, m_rShowRect.top, m_rShowRect.right, m_rShowRect.bottom);

	if (Mat2CImage(&frame, imgDst) == 0)
	{
		//Mat2CImage(&frame, imgDst);
		//		CDC * pDC = m_pTarget->GetDC();
		CDC * pDC = GetDC();

		imgDst.Draw(pDC->GetSafeHdc(), rect);
		frame.release();
		ReleaseDC(pDC);
		imgDst.Destroy();
	}
}

void CPickUpChildDlg::ShowImage(cv::Mat& matSource, int nID, float fFontSize, std::string sSubtitle)
{
	if ((nID != m_nActiveThreadID) || m_bStopFlag) return;

	CRect rect;
	CImage imgDst;

	// Get Display area rect
	cv::Size winSize(m_rShowRect.right, m_rShowRect.bottom);

	// Resize the source to the size of the destination image if necessary
	cv::Mat frame;
	if (matSource.size() != winSize) cv::resize(matSource, frame, winSize);
	else frame = matSource.clone();

	// if face detect
	//if (m_bDetectFace) DetectFace(frame);

	// if erase background
	//if (m_bEraseBkg) EraseBkg(frame);

	// Add subtitle to frame
	OutputSubtitle(frame, fFontSize, sSubtitle);

	// Convert to CImage for display
	Mat2CImage(&frame, imgDst);
	//	MatToCImage(frame, imgDst);

	// Display frame
	CDC *pDC = GetDC();
	imgDst.Draw(pDC->GetSafeHdc(), m_rShowRect);
	frame.release();
	ReleaseDC(pDC);
	imgDst.Destroy();
}
void CPickUpChildDlg::OutputSubtitle(cv::Mat& matOriginal, float fFontSize, std::string sSubtitle)
{
	int nLineCount = 0;
	std::string::size_type nFindPos = 0;
	std::string sText = sSubtitle;
	std::string sLines;

	// splite string by '\n' & add each line to frame
	nFindPos = sText.find('\n');
	while (nFindPos != std::string::npos)
	{
		sLines = sText.substr(0, nFindPos);
		putText(matOriginal, sLines, cvPoint(20, 30 + nLineCount * 60 * fFontSize),
			CV_FONT_HERSHEY_COMPLEX, fFontSize, cvScalar(0, 0, 0), 1, 8, false);
		putText(matOriginal, sLines, cvPoint(18, 30 + nLineCount * 60 * fFontSize - 2),
			CV_FONT_HERSHEY_COMPLEX, fFontSize, cvScalar(180, 180, 180), 1, 8, false);
		if (nFindPos < sText.length())
		{
			sLines.clear();
			sLines = sText.substr(nFindPos + 1, sText.length());
			sText = sLines;
			nFindPos = sText.find('\n');
		}
		else
		{
			sText.clear();
			break;
		}
		nLineCount++;
	}
	putText(matOriginal, sText, cvPoint(20, 30 + nLineCount * 60 * fFontSize),
		CV_FONT_HERSHEY_COMPLEX, fFontSize, cvScalar(0, 0, 0), 1, 8, false);
	putText(matOriginal, sText, cvPoint(18, 30 + nLineCount * 60 * fFontSize - 2),
		CV_FONT_HERSHEY_COMPLEX, fFontSize, cvScalar(180, 180, 180), 1, 8, false);
}

void CPickUpChildDlg::EraseBkg(cv::Mat& matFrame)
{
	cv::Mat foreground;
	cv::Mat Original = matFrame.clone();
	matFrame.release();
	m_mogBkg->apply(Original, foreground, 0.001);
	cv::erode(foreground, foreground, cv::Mat());
	cv::dilate(foreground, foreground, cv::Mat());
	matFrame = foreground.clone();
}
int CPickUpChildDlg::Mat2CImage(Mat *mat, CImage &img)
{
	if (!mat || mat->empty()) return -1;

	//重建CImage  
	img.Destroy();
	int nBPP = mat->channels() * 8;
	img.Create(mat->cols, mat->rows, nBPP);
	if (nBPP == 8)
	{
		static RGBQUAD pRGB[256];
		for (int i = 0; i < 256; i++)
			pRGB[i].rgbBlue = pRGB[i].rgbGreen = pRGB[i].rgbRed = i;
		img.SetColorTable(0, 256, pRGB);
	}
	uchar* psrc = mat->data;
	uchar* pdst = (uchar*)img.GetBits();
	int imgPitch = img.GetPitch();
	for (int y = 0; y < mat->rows; y++)
	{
		memcpy(pdst, psrc, mat->cols*mat->channels());//mat->step is incorrect for those images created by roi (sub-images!)  
		psrc += mat->step;
		pdst += imgPitch;
	}
	return 0;
}
int CPickUpChildDlg::Play(char * sAddress)
{
	int nID;

	if (m_nActiveThreadID > -1 && m_nActiveThreadID < MAX_VIDEO_THREAD)
	{
		if (m_aThreadDataArray[m_nActiveThreadID].sAddress.compare(sAddress) == 0
			&& m_aThreadDataArray[m_nActiveThreadID].nPlayerStatus != VIDEO_STOPED)  return 1; /*already in process*/
	}

	nID = FindAviableID();
	if (nID == -1) return 2; /*no more thread aviable*/

	m_nActiveThreadID = nID;

	m_bStopFlag = false;
	m_pThreadParam.nID = m_nActiveThreadID;
	m_pThreadParam.pParent = this;
	m_aThreadDataArray[m_nActiveThreadID].sAddress = sAddress;
	m_aThreadDataArray[m_nActiveThreadID].capture.release();
	m_aThreadDataArray[m_nActiveThreadID].matFrame.release();
	m_aThreadDataArray[m_nActiveThreadID].hPlayer = AfxBeginThread(PlayerThread, &m_pThreadParam);
	m_aThreadDataArray[m_nActiveThreadID].hWatcher = AfxBeginThread(WatcherThread, &m_pThreadParam);
	m_aThreadDataArray[m_nActiveThreadID].hShow = AfxBeginThread(ShowThread, &m_pThreadParam);

	return 0;
}

int CPickUpChildDlg::FindAviableID()
{
	int i = 0;

	while (i < MAX_VIDEO_THREAD)
	{
		if (m_aThreadDataArray[i].hPlayer == 0
			&& m_aThreadDataArray[i].hWatcher == 0
			&& m_aThreadDataArray[i].nPlayerStatus == VIDEO_STOPED)
		{
			return i;
		}
		i++;
	}

	return -1;
}
void CPickUpChildDlg::InitPlayEnviroment()
{
	m_bStopFlag = true;
	//	m_pTarget = pTarget;
	CRect rect;
	cv::Size winSize(m_rShowRect.right, m_rShowRect.bottom);
	m_matBlank.release();
	m_matBlank = cv::Mat(winSize, CV_8UC3, cvScalar(200, 0, 0));

	//	SetSubtitleFont(1, 1, 1);
	int i;
	for (i = 0; i < MAX_VIDEO_THREAD; i++)
	{
		m_aThreadDataArray[i].hPlayer = 0;
		m_aThreadDataArray[i].hWatcher = 0;
		m_aThreadDataArray[i].hShow = 0;
		m_aThreadDataArray[i].nFrameCount = 0;
		m_aThreadDataArray[i].nPlayerStatus = VIDEO_STOPED;
		m_aThreadDataArray[i].sAddress = "";
	}
	m_nActiveThreadID = -1;

	// load face detect file
	//	if (m_cascadeFace.load("d://haarcascade_frontalface_alt2.xml")) m_bCascadeLoaded = true;
	//	else m_bCascadeLoaded = false;


	// for erase background
	m_mogBkg = cv::createBackgroundSubtractorMOG2(500, 16, true);
}
