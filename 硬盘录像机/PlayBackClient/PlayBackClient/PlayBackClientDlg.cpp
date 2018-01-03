
// PlayBackClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PlayBackClient.h"
#include "PlayBackClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

UINT ThreadPalyDownLoadVideo(LPVOID lparam);
UINT ThreadDownLoadVideo(LPVOID lparam);
static ShowListData m_SListData[MAX_TEXT_SHOW_SUM];

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


// CPlayBackClientDlg 对话框



CPlayBackClientDlg::CPlayBackClientDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CPlayBackClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nSelectIndex = -1;
	m_nProgressNum = 1;
	m_nProgressMax = 1;
	m_bIsPayVideo = FALSE;
	m_bIsDownlod = FALSE;
}

void CPlayBackClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//	DDX_Control(pDX, IDC_BUTTON1, m_wndClose);

}

BEGIN_MESSAGE_MAP(CPlayBackClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_MBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

int __DecodeBase64(const char* pSrc, unsigned char* pDst, int nSrcLen)
{
	char DeBase64Tab[] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		62,        // '+'
		0, 0, 0,
		63,        // '/'
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61,        // '0'-'9'
		0, 0, 0, 0, 0, 0, 0,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,        // 'A'-'Z'
		0, 0, 0, 0, 0, 0,
		26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
		39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,        // 'a'-'z'
	};

	int nDstLen;            // 输出的字符计数
	int nValue;             // 解码用到的长整数
	int i;

	i = 0;
	nDstLen = 0;

	// 取4个字符，解码到一个长整数，再经过移位得到3个字节
	while (i < nSrcLen)
	{
		if (*pSrc != '\r' && *pSrc != '\n')
		{
			nValue = DeBase64Tab[*pSrc++] << 18;
			nValue += DeBase64Tab[*pSrc++] << 12;
			*pDst++ = (nValue & 0x00ff0000) >> 16;
			nDstLen++;

			if (*pSrc != '=')
			{
				nValue += DeBase64Tab[*pSrc++] << 6;
				*pDst++ = (nValue & 0x0000ff00) >> 8;
				nDstLen++;

				if (*pSrc != '=')
				{
					nValue += DeBase64Tab[*pSrc++];
					*pDst++ = nValue & 0x000000ff;
					nDstLen++;
				}
			}

			i += 4;
		}
		else        // 回车换行，跳过
		{
			pSrc++;
			i++;
		}
	}
	// 输出加个结束符
	*pDst = '\0';

	return nDstLen;
}

// CPlayBackClientDlg 消息处理程序

BOOL CPlayBackClientDlg::OnInitDialog()
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


	CString csFile = GetiniPath();
	CString csTemp;
	csTemp = csFile +VIDEO_PATH;
	if (!PathIsDirectoryW(csTemp.AllocSysString()))
	{
		CreateDirectoryW(csTemp.AllocSysString(), NULL);
	}
	csTemp = csFile + IMG_PATH;
	if (!PathIsDirectoryW(csTemp.AllocSysString()))
	{
		CreateDirectoryW(csTemp.AllocSysString(), NULL);
	}

	MoveWindow(CRect(0, 0, WIDTH, HEIGHT), false);
	GetDlgItem(IDC_STATIC_VIDEO)->MoveWindow(CRect(528, 87, 528 + 348, 87 + 335), false);

	m_rDlgRect.SetRect(0, 0, WIDTH, HEIGHT);

	m_rLeftRect.SetRect(WIDTH * 274 / 1024, HEIGHT * 674 / 768, WIDTH * 296 / 1024, HEIGHT * 696 / 768);
	m_rRightRect.SetRect(WIDTH * 344 / 1024, HEIGHT * 674 / 768, WIDTH * 366 / 1024, HEIGHT * 696 / 768);
	m_rCloseRect.SetRect(WIDTH * 994 / 1024, HEIGHT * 12 / 768, WIDTH * 1001 / 1024, HEIGHT * 28 / 768);
	m_rPlayBackRect.SetRect(WIDTH * 663 / 1024, HEIGHT * 445 / 768, WIDTH * 753 / 1024, HEIGHT * 479 / 768);

	if (InitDataBase())
	{
		InitListRect();
		GetListMaxSum();
		UpdataListData(1);
	}
	else
	{
		MessageBox("数据库链接失败", "错误");
	}

	m_HPlayVideoEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_TPlayVideo = AfxBeginThread(&ThreadPalyDownLoadVideo, NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
// 文件路径
CString CPlayBackClientDlg::GetiniPath()
{
	CString strIp;
	TCHAR pFileName[MAX_PATH];
	int nPos = GetCurrentDirectory(MAX_PATH, pFileName);
	CString csFullPath(pFileName);

	return csFullPath;
}

BOOL CPlayBackClientDlg::InitDataBase()
{
	try
	{
		CString csFullPath = GetiniPath();
		csFullPath += DBINI_PATH;

		CString csSqlIP, csSqlDatabase;
		GetPrivateProfileString(("SqlConfiguration"), "Tcp_ServerIP", (""), m_csTcpIP.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);
		GetPrivateProfileString(("SqlConfiguration"), "Sql_ServerAdd", (""), csSqlIP.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);
		GetPrivateProfileString(("SqlConfiguration"), "Sql_Database", (""), csSqlDatabase.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);

		CString csSqlConnect;
		csSqlConnect.Format("Driver={MySQL ODBC 5.1 Driver};Server=%s;Database=%s;uid=root;pwd=cr;", csSqlIP, csSqlDatabase);
		m_pConnect.CreateInstance(__uuidof(Connection));
		_bstr_t strConnect = csSqlConnect;
		m_pConnect->Open(strConnect, "", "", adModeUnknown);
	}
	catch (_com_error e)
	{
		return FALSE;
	}
	return TRUE;
}

void CPlayBackClientDlg::InitListRect()
{
	//区域起始左、上 65 145
	int iAreaStartLeft = WIDTH * 67 / 1024;
	int iAreaStartTop = HEIGHT * 145 / 768;

	//列表的大小316*32
	int iAreaWidth = WIDTH * 312 / 1024;
	int iAreaHeight = HEIGHT * 29 / 768;

	//两条数据间隔距离
	int iAreaJGWidth = WIDTH * 0 / 1024;
	int iAreaJGHeight = HEIGHT * 3 / 768;

	//定义16个区域
	int i = 0;
	for (i = 0; i < 16; i++)
	{
		//初始化区域
		m_SListData[i].rListRect.SetRect(
			iAreaStartLeft,
			iAreaStartTop + (iAreaHeight + iAreaJGHeight) * i,
			iAreaStartLeft + iAreaWidth,
			iAreaStartTop + iAreaHeight + (iAreaHeight + iAreaJGHeight) * i);

		//初始化 文字坐标
		PointF pointName(iAreaStartLeft + 10, iAreaStartTop + (iAreaHeight + iAreaJGHeight) * i + 7);
		m_SListData[i].pointName = pointName;
		//初始化 时间坐标
		PointF pointTime(iAreaStartLeft + 145, iAreaStartTop + (iAreaHeight + iAreaJGHeight) * i + 7);
		m_SListData[i].pointTime = pointTime;
	}
}

BOOL CPlayBackClientDlg::GetListMaxSum()
{
	CString csSql;
	m_nProgressNum = 1;
	m_nProgressMax = 1;
	_RecordsetPtr pRecordset;
	csSql = "select count(1) from warninginfo where isstorage = '2'";
	try
	{
		pRecordset = m_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);

		if (!pRecordset->adoEOF)
		{
			CString csEmpInfoName = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("count(1)")->Value;
			int nSum = atoi(csEmpInfoName);

			if (nSum > MAX_TEXT_SHOW_SUM)
			{
				m_nProgressMax = nSum / MAX_TEXT_SHOW_SUM;
				if (nSum % MAX_TEXT_SHOW_SUM != 0)
					m_nProgressMax++;
			}
		}
	}
	catch (...)
	{
		return FALSE;
	}

	return  TRUE;
}

void CPlayBackClientDlg::UpdataListData(int nPagesNum)
{
	try
	{
		//ZeroMemory(m_SListData, MAX_TEXT_SHOW_SUM);
		CString csSql;
		int nStart;
		int nIndex = 0;
		_RecordsetPtr pRecordset;
		_RecordsetPtr pRecordset1;
		_RecordsetPtr pRecordset2;
		CString csSnapshotID, csWarningID, csIdName, csSimilar;

		nStart = (nPagesNum - 1) * MAX_TEXT_SHOW_SUM;
		csSql.Format("select * from warninginfo where isstorage = '2' limit %d,%d", nStart, MAX_TEXT_SHOW_SUM);

		pRecordset = m_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		while (!pRecordset->adoEOF)
		{
			csSnapshotID = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("snapshotid")->Value;
			csWarningID = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("id")->Value;
			csIdName = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("id_card")->Value;
			csSimilar = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("similarity")->Value;
			m_SListData[nIndex].csSnapshotID = csSnapshotID;
			m_SListData[nIndex].csWarningID = csWarningID;
			m_SListData[nIndex].csIdCard = csIdName;
			m_SListData[nIndex].csSimilar = csSimilar;



			csSql.Format("select * FROM snapshotinfo WHERE id = '%s'", csSnapshotID);
			pRecordset1 = m_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
			if (!pRecordset1->adoEOF)
			{
				m_SListData[nIndex].strVideoFile = (TCHAR *)(_bstr_t)pRecordset1->GetFields()->GetItem("imgpath")->Value;
				m_SListData[nIndex].strTime = (TCHAR *)(_bstr_t)pRecordset1->GetFields()->GetItem("alarmtime")->Value;
				CString csFile = GetiniPath();
				csFile += VIDEO_PATH;
				csFile += m_SListData[nIndex].strVideoFile;
				if (!PathIsDirectoryW(csFile.AllocSysString()))
				{
					CreateDirectoryW(csFile.AllocSysString(), NULL);
					m_SListData[nIndex].bIsDownlod = FALSE;
				}
				else
				{
					CString csVideo;
					csVideo.Format("%s\\%s.avi", csFile, csWarningID);
					if (_access(csVideo.GetBuffer(), 0) != -1)
						m_SListData[nIndex].bIsDownlod = TRUE;
					else
						m_SListData[nIndex].bIsDownlod = FALSE;
				}
			}


			csSql.Format("select * FROM employeeinfo WHERE idcard = '%s'", csIdName);
			pRecordset2 = m_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
			if (!pRecordset2->adoEOF)
			{
				m_SListData[nIndex].csName = (TCHAR *)(_bstr_t)pRecordset2->GetFields()->GetItem("name")->Value;
				if (m_SListData[nIndex].csName.GetLength() > 16)
				{
					m_SListData[nIndex].csNameTemp = m_SListData[nIndex].csName.Mid(0, 14);
					m_SListData[nIndex].csNameTemp += "...";
				}
				m_SListData[nIndex].csSex = (TCHAR *)(_bstr_t)pRecordset2->GetFields()->GetItem("sex")->Value;
				if (m_SListData[nIndex].csSex == "male")
					m_SListData[nIndex].csSex = "男";
				else
					m_SListData[nIndex].csSex = "女";
				m_SListData[nIndex].csAddress = (TCHAR *)(_bstr_t)pRecordset2->GetFields()->GetItem("address")->Value;
				m_SListData[nIndex].csPhone = (TCHAR *)(_bstr_t)pRecordset2->GetFields()->GetItem("phone")->Value;
			}
			nIndex++;
			pRecordset->MoveNext();
		}
	}
	catch (CException* e)
	{
	}
}

void CPlayBackClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPlayBackClientDlg::ShowVideo(IplImage *m_img, UINT ID)
{
	try
	{
		if (m_img == NULL || ID == NULL)
			return;
		CDC* pDC = GetDlgItem(ID)->GetDC();
		HDC hDC = pDC->GetSafeHdc();
		CRect rect;
		GetDlgItem(ID)->GetClientRect(&rect);
		SetRect(rect, rect.left, rect.top, rect.right, rect.bottom);
		m_cimg.CopyOf(m_img); //m_cimg 就是一个CvvImage 对象
		m_cimg.DrawToHDC(hDC, &rect);  //显示图像
		ReleaseDC(pDC);
	}
	catch (CException* e)
	{
	}
}


void CPlayBackClientDlg::OnPaint()
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
		CString csBk;

		if (m_bIsPayVideo)
			csBk = L"Resources/bj1.jpg";
		else
			csBk = L"Resources/bj2.jpg";

		USES_CONVERSION;
		WCHAR* pBufbj = T2W((LPCTSTR)csBk);
		Image img_bk(pBufbj);
		g.DrawImage(&img_bk, 0, 0, m_rDlgRect.Width(), m_rDlgRect.Height());

		Gdiplus::Font fontParentInfo(L"微软雅黑", WIDTH / 70, FontStyleRegular, UnitPixel);
		SolidBrush parentinfoBrush(Color(255, 50, 50, 50));//黑色
		SolidBrush parentinfoBai(Color(255, 240, 240, 240));//白色
		Pen ChoicePen(Color(255, 26, 70, 250), 1);

		//加载列表
		for (int i = 0; i < MAX_TEXT_SHOW_SUM; i++)
		{
			g.DrawString(m_SListData[i].csNameTemp.AllocSysString(), -1, &fontParentInfo, m_SListData[i].pointName, &parentinfoBrush);
			g.DrawString(m_SListData[i].strTime.AllocSysString(), -1, &fontParentInfo, m_SListData[i].pointTime, &parentinfoBrush);
		}
		//页码
		PointF pointPage(297, 675);
		CString csPage;
		csPage.Format("%d/%d", m_nProgressNum, m_nProgressMax);
		g.DrawString(csPage.AllocSysString(), -1, &fontParentInfo, pointPage, &parentinfoBrush);

		//选择话底线
		if (m_nSelectIndex != -1)
		{
			PointF pointSelectStart(m_SListData[m_nSelectIndex].rListRect.left, m_SListData[m_nSelectIndex].rListRect.top + 29);
			PointF pointSelectEnd(m_SListData[m_nSelectIndex].rListRect.left + m_SListData[m_nSelectIndex].rListRect.Width(), m_SListData[m_nSelectIndex].rListRect.top + 29);
			g.DrawLine(&ChoicePen, pointSelectStart, pointSelectEnd);

			//照片画框
			CString csName, csSex, csAddress, csPhone, csSimilar, strTime, csImg;
			Pen gPen(Color(255, 215, 215, 215), 2);
			g.DrawRectangle(&gPen, Gdiplus::Rect(509, 534, 129, 166));
			//显示照片
			CString csFile = GetiniPath();
			csFile += IMG_PATH;
			csImg.Format("%s%s.jpg", csFile, m_SListData[m_nSelectIndex].csWarningID);
			USES_CONVERSION;
			WCHAR* pBuf = T2W((LPCTSTR)csImg);
			Image imgpeople(pBuf);
			g.DrawImage(&imgpeople, 510, 535, 128, 165);
			//显示详细信息
			PointF  pointName(650, 557);
			PointF  pointSex(650, 577);
			PointF  pointAddr(650, 597);
			PointF  pointPhone(650, 617);
			PointF  poinSimilar(650, 637);
			PointF  pointTime(650, 657);
			csName.Format("姓名：%s", m_SListData[m_nSelectIndex].csName);
			csSex.Format("姓别：%s", m_SListData[m_nSelectIndex].csSex);
			csAddress.Format("籍贯：%s", m_SListData[m_nSelectIndex].csAddress);
			csPhone.Format("电话：%s", m_SListData[m_nSelectIndex].csPhone);
			csSimilar.Format("相似度：%s", m_SListData[m_nSelectIndex].csSimilar);
			strTime.Format("报警时间：%s", m_SListData[m_nSelectIndex].strTime);

			g.DrawString(csName.AllocSysString(), -1, &fontParentInfo, pointName, &parentinfoBai);
			g.DrawString(csSex.AllocSysString(), -1, &fontParentInfo, pointSex, &parentinfoBai);
			g.DrawString(csAddress.AllocSysString(), -1, &fontParentInfo, pointAddr, &parentinfoBai);
			g.DrawString(csPhone.AllocSysString(), -1, &fontParentInfo, pointPhone, &parentinfoBai);
			g.DrawString(csSimilar.AllocSysString(), -1, &fontParentInfo, poinSimilar, &parentinfoBai);
			g.DrawString(strTime.AllocSysString(), -1, &fontParentInfo, pointTime, &parentinfoBai);
		}
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPlayBackClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPlayBackClientDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	try
	{
		if (m_rPlayBackRect.PtInRect(point))
		{
			if (m_nSelectIndex != -1)
			{
				if (m_bIsPayVideo)
				{
					m_bIsPayVideo = FALSE;
				}
				else
				{
					CString csFile = GetiniPath();
					csFile += VIDEO_PATH;
					csFile += m_SListData[m_nSelectIndex].strVideoFile;
					CString csVideo;
					csVideo.Format("%s\\%s.avi", csFile, m_SListData[m_nSelectIndex].csWarningID);
					if (_access(csVideo.GetBuffer(), 0) != -1)
					{
						SetEvent(m_HPlayVideoEvent);
						m_bIsPayVideo = TRUE;
					}
					else
					{
						MessageBox("文件不存在请先下载", "错误");
					}
				}
				Invalidate(false);
			}
		}
		else if (m_rRightRect.PtInRect(point))
		{
			if (m_nProgressNum < m_nProgressMax)
			{
				m_nProgressNum++;
				UpdataListData(m_nProgressNum);
				Invalidate(false);
			}
		}
		else if (m_rLeftRect.PtInRect(point))
		{
			if (m_nProgressNum > 1)
			{
				m_nProgressNum--;
				UpdataListData(m_nProgressNum);
				Invalidate(false);
			}
		}
		else if (m_rCloseRect.PtInRect(point))
		{
			m_bIsPayVideo = FALSE;
			PostMessage(WM_CLOSE);
		}
		else
		{
			BOOL  bFind = FALSE;
			for (int i = 0; i < MAX_TEXT_SHOW_SUM; i++)
			{
				if (m_SListData[i].rListRect.PtInRect(point))
				{
					bFind = TRUE;
					m_nSelectIndex = i;

					_RecordsetPtr pRecordset;
					CString csSql, m_csImage, csImgPath;
					csSql.Format("select * FROM imginfo WHERE idcard = '%s'", m_SListData[m_nSelectIndex].csIdCard);
					pRecordset = m_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
					if (!pRecordset->adoEOF)
					{
						m_csImage = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("img")->Value;
						char *pImg = (LPSTR)(LPCTSTR)m_csImage;
						int nImgLen = strlen(pImg);

						BYTE *impDst = new BYTE[nImgLen];
						//解码并显示
						int nLen = __DecodeBase64(pImg, impDst, nImgLen);
						if (nLen <= 0)
						{
							return;
						}

						CString csFile = GetiniPath();
						csFile += IMG_PATH;
						csFile += m_SListData[m_nSelectIndex].csWarningID;
						csFile += ".jpg";

						CFile fileJPG;
						fileJPG.Open(csFile, CFile::modeCreate | CFile::modeWrite);
						fileJPG.Write(impDst, nLen);
						fileJPG.Close();
						delete[] impDst;
					}
					Invalidate(false);
					break;

				}
			}
			if (!bFind)
			{
				PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
			}
		}
	}
	catch (CException* e)
	{
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}


UINT ThreadDownLoadVideo(LPVOID lparam)
{
	CPlayBackClientDlg *pWnd = (CPlayBackClientDlg *)AfxGetApp()->GetMainWnd();
	try
	{
		int nIdex = (int)lparam;
		pWnd->m_bIsDownlod = TRUE;
		WSADATA            wsd;            //WSADATA变量
		SOCKET             sHost;          //服务器套接字
		SOCKADDR_IN        servAddr;       //服务器地址
		int retVal;
		WSAStartup(MAKEWORD(2, 2), &wsd);
		sHost = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		//设置服务器地址
		servAddr.sin_family = AF_INET;//地址家族
		servAddr.sin_addr.s_addr = inet_addr(pWnd->m_csTcpIP);//IP地址
		servAddr.sin_port = htons((short)TCPPORT);//服务端口号
		int    nServAddlen = sizeof(servAddr);//填充该结构的大小
		retVal = connect(sHost, (LPSOCKADDR)&servAddr, sizeof(servAddr));
		char buf[10000000];
		ZeroMemory(buf, sizeof(buf));

		CString csSend;
		csSend.Format("%s%s", m_SListData[nIdex].csWarningID, m_SListData[nIdex].strVideoFile);
		retVal = send(sHost, csSend.GetBuffer(), csSend.GetLength(), 0);
		if (retVal == -1)
		{
			return -1;
		}
		retVal = recv(sHost, buf, sizeof(buf), 0);
		if (retVal != -1)
		{
			CString csFile = pWnd->GetiniPath();
			csFile += VIDEO_PATH;
			CString csVideo;
			csVideo.Format("%s%s\\%s.avi", csFile, m_SListData[nIdex].strVideoFile, m_SListData[nIdex].csWarningID);
			CFile fileJPG;
			fileJPG.Open(csVideo, CFile::modeCreate | CFile::modeWrite);
			fileJPG.Write(buf, retVal);
			Sleep(10);
			fileJPG.Close();
		}
		closesocket(sHost);
		pWnd->m_bIsDownlod = FALSE;
		Sleep(10);

		if (!pWnd->m_bIsPayVideo)
			SetEvent(pWnd->m_HPlayVideoEvent);
	}
	catch (CException* e)
	{
		pWnd->m_bIsDownlod = FALSE;
		return -1;
	}

	return 0;
}

UINT ThreadPalyDownLoadVideo(LPVOID lparam)
{
	try
	{
		CPlayBackClientDlg *pWnd = (CPlayBackClientDlg *)AfxGetApp()->GetMainWnd();
		DWORD dReturn;
		while (true)
		{
			dReturn = WaitForSingleObject(pWnd->m_HPlayVideoEvent, INFINITE);
			if (WAIT_OBJECT_0 == dReturn)
			{
				TRACE("Play\n");
				CString csVideoName;

				CString csFile = pWnd->GetiniPath();
				csFile += VIDEO_PATH;
				CString csVideo;
				csVideo.Format("%s%s\\%s.avi", csFile, m_SListData[pWnd->m_nSelectIndex].strVideoFile, m_SListData[pWnd->m_nSelectIndex].csWarningID);
				CvCapture* capture = cvCreateFileCapture(csVideo);
				IplImage* frame;
				while (pWnd->m_bIsPayVideo)
				{
					frame = cvQueryFrame(capture);
					if (!frame) break;
					pWnd->ShowVideo(frame, 1001);
					Sleep(27);
				}
				pWnd->m_bIsPayVideo = FALSE;
				pWnd->Invalidate(false);
				cvReleaseCapture(&capture);
				TRACE("StopPlay\n");
			}
		}
	}
	catch (CException* e)
	{
		return -1;
	}

	return 0;
}

void CPlayBackClientDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	for (int i = 0; i < MAX_TEXT_SHOW_SUM; i++)
	{
		if (m_SListData[i].rListRect.PtInRect(point))
		{
			m_TDownlod = AfxBeginThread(&ThreadDownLoadVideo, (LPVOID)i);
		}
	}
	CDialogEx::OnLButtonDblClk(nFlags, point);
}

BOOL CPlayBackClientDlg::PreTranslateMessage(MSG* pMsg)
{
	// 	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_RETURN)    
	// 		return TRUE; 
	if (pMsg->message == WM_KEYDOWN&&pMsg->wParam == VK_ESCAPE)
	{
		m_bIsPayVideo = FALSE;
		Sleep(5);
	}
	return CDialog::PreTranslateMessage(pMsg);
}