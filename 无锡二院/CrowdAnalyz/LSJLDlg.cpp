// LSJLDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CrowdAnalyz.h"
#include "LSJLDlg.h"
#include "afxdialogex.h"
#include "CrowdAnalyzDlg.h"

#define  DBINI_PATH			 "\\Parameter.ini"
static _ConnectionPtr  g_pConnectMigration;

#define  IMG_PORT     10000

// CLSJLDlg 对话框

IMPLEMENT_DYNAMIC(CLSJLDlg, CDialogEx)

CLSJLDlg::CLSJLDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CLSJLDlg::IDD, pParent)
, m_csName(_T(""))
, m_csXiangsidu(_T(""))
, m_csXingbie(_T(""))
, m_csWeizhi(_T(""))
, m_csFanwei1(_T(""))
, m_csFanwei2(_T(""))
, m_csPaga(_T(""))
{

}

CLSJLDlg::~CLSJLDlg()
{
}

void CLSJLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_csName);
	DDX_Text(pDX, IDC_EDIT_XIANGSIDU, m_csXiangsidu);
	DDX_Control(pDX, IDC_EDIT_XINGBIE, m_comXingbie);
	DDX_Control(pDX, IDC_COMBO_WEIZHI, m_comWeizhi);
	DDX_Control(pDX, IDC_COMBO_FANWEI1, m_comFanwei1);
	DDX_Control(pDX, IDC_COMBO_FANWEI2, m_comFanwei2);
	DDX_CBString(pDX, IDC_EDIT_XINGBIE, m_csXingbie);
	DDX_CBString(pDX, IDC_COMBO_WEIZHI, m_csWeizhi);
	DDX_CBString(pDX, IDC_COMBO_FANWEI1, m_csFanwei1);
	DDX_CBString(pDX, IDC_COMBO_FANWEI2, m_csFanwei2);
	DDX_Text(pDX, IDC_EDIT_PAGA, m_csPaga);
}


BEGIN_MESSAGE_MAP(CLSJLDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CLSJLDlg 消息处理程序


void CLSJLDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	delete pMemDC;
	delete pBitmap;
}


void CLSJLDlg::OnPaint()
{
	CPaintDC dc(this);
	CBitmap * pOldBitmap = pMemDC->SelectObject(pBitmap);
	pMemDC->PatBlt(0, 0, m_rClient.Width(), m_rClient.Height(), PATCOPY);
	Graphics g(pMemDC->m_hDC);
	//画总背景
	Image img_bk(L"img/lsbj_bk.jpg");
	g.DrawImage(&img_bk, 0, 0, m_rClient.Width(), m_rClient.Height());
	if (m_bShowJpg)
	{
		Image img_bk1(L"img/rlzpt.jpg");
		Image imagezp(L"data/Lishizp.jpg");
		Image imagebx(L"data/Lishibx.jpg");
		g.DrawImage(&img_bk1, m_rTuXiang.left, m_rTuXiang.top, m_rTuXiang.Width(), m_rTuXiang.Height());
		g.DrawImage(&imagezp, m_rRenLian.left, m_rRenLian.top, m_rRenLian.Width(), m_rRenLian.Height());
		g.DrawImage(&imagebx, m_rBaxiang.left, m_rBaxiang.top, m_rBaxiang.Width(), m_rBaxiang.Height());

	}
	
	///ceshi
	Pen redPen(Color(255, 0, 0), 1);
	//g.DrawRectangle(&redPen, m_rXingming.left, m_rXingming.top, m_rXingming.Width(), m_rXingming.Height());
	//g.DrawRectangle(&redPen, m_rXingbie.left, m_rXingbie.top, m_rXingbie.Width(), m_rXingbie.Height());
	//g.DrawRectangle(&redPen, m_rWeizhi.left, m_rWeizhi.top, m_rWeizhi.Width(), m_rWeizhi.Height());
	//g.DrawRectangle(&redPen, m_rFanwei1.left, m_rFanwei1.top, m_rFanwei1.Width(), m_rFanwei1.Height());
	//	g.DrawRectangle(&redPen, m_rFanwei2.left, m_rFanwei2.top, m_rFanwei2.Width(), m_rFanwei2.Height());
	//	g.DrawRectangle(&redPen, m_rXiangsidu.left, m_rXiangsidu.top, m_rXiangsidu.Width(), m_rXiangsidu.Height());
	//g.DrawRectangle(&redPen, m_rSousuo.left, m_rSousuo.top, m_rSousuo.Width(), m_rSousuo.Height());
	//g.DrawRectangle(&redPen, m_rUp.left, m_rUp.top, m_rUp.Width(), m_rUp.Height());
	//g.DrawRectangle(&redPen, m_rNext.left, m_rNext.top, m_rNext.Width(), m_rNext.Height());
	//g.DrawRectangle(&redPen, m_rPaga.left, m_rPaga.top, m_rPaga.Width(), m_rPaga.Height());
	//g.DrawRectangle(&redPen, m_rRenLian.left, m_rRenLian.top, m_rRenLian.Width(), m_rRenLian.Height());
	//g.DrawRectangle(&redPen, m_rBaxiang.left, m_rBaxiang.top, m_rBaxiang.Width(), m_rBaxiang.Height());


	// 	for (int i = 0; i < 9; i++)
	// 	{
	// 		g.DrawRectangle(&redPen, m_DataList[i].rZP.left, m_DataList[i].rZP.top, m_DataList[i].rZP.Width(), m_DataList[i].rZP.Height());
	// 		g.DrawRectangle(&redPen, m_DataList[i].rXiangsidu.left, m_DataList[i].rXiangsidu.top, m_DataList[i].rXiangsidu.Width(), m_DataList[i].rXiangsidu.Height());
	// 		g.DrawRectangle(&redPen, m_DataList[i].rBX.left, m_DataList[i].rBX.top, m_DataList[i].rBX.Width(), m_DataList[i].rBX.Height());
	// 		g.DrawRectangle(&redPen, m_DataList[i].rIdcard.left, m_DataList[i].rIdcard.top, m_DataList[i].rIdcard.Width(), m_DataList[i].rIdcard.Height());
	// 		g.DrawRectangle(&redPen, m_DataList[i].rFenlei.left, m_DataList[i].rFenlei.top, m_DataList[i].rFenlei.Width(), m_DataList[i].rFenlei.Height());
	// 		g.DrawRectangle(&redPen, m_DataList[i].rBaojingyuan.left, m_DataList[i].rBaojingyuan.top, m_DataList[i].rBaojingyuan.Width(), m_DataList[i].rBaojingyuan.Height());
	// 		g.DrawRectangle(&redPen, m_DataList[i].rCZ.left, m_DataList[i].rCZ.top, m_DataList[i].rCZ.Width(), m_DataList[i].rCZ.Height());
	// 		g.DrawRectangle(&redPen, m_DataList[i].rPos.left, m_DataList[i].rPos.top, m_DataList[i].rPos.Width(), m_DataList[i].rPos.Height());
	// 		g.DrawRectangle(&redPen, m_DataList[i].rTime.left, m_DataList[i].rTime.top, m_DataList[i].rTime.Width(), m_DataList[i].rTime.Height());
	// 	}



	DisDatalist(&g);
	DisXiangqing(&g);

	//显示缓存，清除HDC
	dc.BitBlt(0, 0, m_rClient.Width(), m_rClient.Height(), pMemDC, 0, 0, SRCCOPY);
	pMemDC->SelectObject(pOldBitmap);
	g.ReleaseHDC(pMemDC->m_hDC);
}


HBRUSH CLSJLDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor)
	{
	case CTLCOLOR_EDIT:
		pDC->SetTextColor(m_colorEditText);
		pDC->SetBkColor(m_colorEditBK);
		return (HBRUSH)(m_pEditBkBrush->GetSafeHandle());

	default:
		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	}
}


BOOL CLSJLDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_colorEditText = RGB(212, 214, 222);
	m_colorEditBK = RGB(21, 25, 60);
	m_pEditBkBrush = new CBrush(RGB(21, 25, 60));
	m_bShowJpg = FALSE;
	InitRet();

	GetDlgItem(IDC_EDIT_NAME)->MoveWindow(m_rXingming.left, m_rXingming.top, m_rXingming.Width(), m_rXingming.Height());
	GetDlgItem(IDC_EDIT_XIANGSIDU)->MoveWindow(m_rXiangsidu.left, m_rXiangsidu.top, m_rXiangsidu.Width(), m_rXiangsidu.Height());
	GetDlgItem(IDC_EDIT_XINGBIE)->MoveWindow(m_rXingbie.left, m_rXingbie.top, m_rXingbie.Width(), m_rXingbie.Height());
	GetDlgItem(IDC_COMBO_WEIZHI)->MoveWindow(m_rWeizhi.left, m_rWeizhi.top, m_rWeizhi.Width(), m_rWeizhi.Height());
	GetDlgItem(IDC_COMBO_FANWEI1)->MoveWindow(m_rFanwei1.left, m_rFanwei1.top, m_rFanwei1.Width(), m_rFanwei1.Height());
	GetDlgItem(IDC_COMBO_FANWEI2)->MoveWindow(m_rFanwei2.left, m_rFanwei2.top, m_rFanwei2.Width(), m_rFanwei2.Height());
	GetDlgItem(IDC_EDIT_PAGA)->MoveWindow(m_rPaga.left, m_rPaga.top, m_rPaga.Width(), m_rPaga.Height());
	m_iDataLinstCount = 0;

	InitDatabaseMigration();
	GetDlgInfo();
	m_csPaga = _T("1");

	//双缓存
	CClientDC dc(this);
	pMemDC = new CDC;
	pBitmap = new CBitmap;
	pMemDC->CreateCompatibleDC(&dc);
	pBitmap->CreateCompatibleBitmap(&dc, WIDTH, HEIGHT);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
void CLSJLDlg::InitRet()
{
	m_rClient.SetRect(0, 0, 1920 * WIDTH / 1920, 830 * HEIGHT / 1080);


	m_rDataList.SetRect(56 * m_rClient.Width() / 1920, 179 * m_rClient.Height() / 830, 879 * m_rClient.Width() / 1920, 750 * m_rClient.Height() / 830);

	m_rXingming.SetRect(147 * m_rClient.Width() / 1920, 75 * m_rClient.Height() / 830, 259 * m_rClient.Width() / 1920, 93 * m_rClient.Height() / 830);
	m_rXingbie.SetRect(311 * m_rClient.Width() / 1920, 72 * m_rClient.Height() / 830, 430 * m_rClient.Width() / 1920, 96 * m_rClient.Height() / 830);
	m_rWeizhi.SetRect(508 * m_rClient.Width() / 1920, 72 * m_rClient.Height() / 830, 627 * m_rClient.Width() / 1920, 96 * m_rClient.Height() / 830);

	m_rFanwei1.SetRect(144 * m_rClient.Width() / 1920, 110 * m_rClient.Height() / 830, 264 * m_rClient.Width() / 1920, 136 * m_rClient.Height() / 830);
	m_rFanwei2.SetRect(313 * m_rClient.Width() / 1920, 110 * m_rClient.Height() / 830, 430 * m_rClient.Width() / 1920, 136 * m_rClient.Height() / 830);
	m_rXiangsidu.SetRect(512 * m_rClient.Width() / 1920, 113 * m_rClient.Height() / 830, 623 * m_rClient.Width() / 1920, 132 * m_rClient.Height() / 830);

	m_rSousuo.SetRect(710 * m_rClient.Width() / 1920, 109 * m_rClient.Height() / 830, 755 * m_rClient.Width() / 1920, 133 * m_rClient.Height() / 830);


	m_rUp.SetRect(372 * m_rClient.Width() / 1920, 751 * m_rClient.Height() / 830, 439 * m_rClient.Width() / 1920, 773 * m_rClient.Height() / 830);
	m_rNext.SetRect(535 * m_rClient.Width() / 1920, 751 * m_rClient.Height() / 830, 603 * m_rClient.Width() / 1920, 773 * m_rClient.Height() / 830);
	m_rPaga.SetRect(470 * m_rClient.Width() / 1920, 753 * m_rClient.Height() / 830, 504 * m_rClient.Width() / 1920, 770 * m_rClient.Height() / 830);

	m_rTuXiang.SetRect(900 * m_rClient.Width() / 1920, 180 * m_rClient.Height() / 830, 1432 * m_rClient.Width() / 1920, 472 * m_rClient.Height() / 830);
	m_rBaxiang.SetRect(1180 * m_rClient.Width() / 1920, 215 * m_rClient.Height() / 830, 1420 * m_rClient.Width() / 1920, 460 * m_rClient.Height() / 830);
	m_rRenLian.SetRect(910 * m_rClient.Width() / 1920, 215 * m_rClient.Height() / 830, 1152 * m_rClient.Width() / 1920, 460 * m_rClient.Height() / 830);

	for (int i = 0; i < 9; i++)
	{
		m_DataList[i].rZP.SetRect((61 * m_rClient.Width() / 1920), (183 + (63 * i))* m_rClient.Height() / 830, (120 * m_rClient.Width() / 1920), (240 + (63 * i))* m_rClient.Height() / 830);
		m_DataList[i].rXiangsidu.SetRect((125 * m_rClient.Width() / 1920), (183 + (63 * i))* m_rClient.Height() / 830, (183 * m_rClient.Width() / 1920), (240 + (63 * i))* m_rClient.Height() / 830);
		m_DataList[i].rBX.SetRect((188 * m_rClient.Width() / 1920), (183 + (63 * i))* m_rClient.Height() / 830, (248 * m_rClient.Width() / 1920), (240 + (63 * i))* m_rClient.Height() / 830);
		m_DataList[i].rIdcard.SetRect((253 * m_rClient.Width() / 1920), (183 + (63 * i))* m_rClient.Height() / 830, (335 * m_rClient.Width() / 1920), (240 + (63 * i))* m_rClient.Height() / 830);
		m_DataList[i].rFenlei.SetRect((340 * m_rClient.Width() / 1920), (183 + (63 * i))* m_rClient.Height() / 830, (420 * m_rClient.Width() / 1920), (240 + (63 * i))* m_rClient.Height() / 830);
		m_DataList[i].rBaojingyuan.SetRect((425 * m_rClient.Width() / 1920), (183 + (63 * i))* m_rClient.Height() / 830, (538 * m_rClient.Width() / 1920), (240 + (63 * i))* m_rClient.Height() / 830);
		m_DataList[i].rPos.SetRect((543 * m_rClient.Width() / 1920), (183 + (63 * i))* m_rClient.Height() / 830, (673 * m_rClient.Width() / 1920), (240 + (63 * i))* m_rClient.Height() / 830);
		m_DataList[i].rTime.SetRect((678 * m_rClient.Width() / 1920), (183 + (63 * i))* m_rClient.Height() / 830, (802 * m_rClient.Width() / 1920), (240 + (63 * i))* m_rClient.Height() / 830);
		m_DataList[i].rCZ.SetRect((817 * m_rClient.Width() / 1920), (195 + (63 * i))* m_rClient.Height() / 830, (877 * m_rClient.Width() / 1920), (225 + (63 * i))* m_rClient.Height() / 830);
	}

}
void CLSJLDlg::DisDatalist(Graphics *g)
{
	Image imagezp0(L"data/Lishizp_0.jpg");
	Image imagezp1(L"data/Lishizp_1.jpg");
	Image imagezp2(L"data/Lishizp_2.jpg");
	Image imagezp3(L"data/Lishizp_3.jpg");
	Image imagezp4(L"data/Lishizp_4.jpg");
	Image imagezp5(L"data/Lishizp_5.jpg");
	Image imagezp6(L"data/Lishizp_6.jpg");
	Image imagezp7(L"data/Lishizp_7.jpg");
	Image imagezp8(L"data/Lishizp_8.jpg");

	Image imagebx0(L"data/Lishibx_0.jpg");
	Image imagebx1(L"data/Lishibx_1.jpg");
	Image imagebx2(L"data/Lishibx_2.jpg");
	Image imagebx3(L"data/Lishibx_3.jpg");
	Image imagebx4(L"data/Lishibx_4.jpg");
	Image imagebx5(L"data/Lishibx_5.jpg");
	Image imagebx6(L"data/Lishibx_6.jpg");
	Image imagebx7(L"data/Lishibx_7.jpg");
	Image imagebx8(L"data/Lishibx_8.jpg");

	Image imageXQ(L"img/ssjk_xiangqing.png");

	if (m_iDataLinstCount >= 1)
	{
		g->DrawImage(&imagezp0, m_DataList[0].rZP.left, m_DataList[0].rZP.top, m_DataList[0].rZP.Width(), m_DataList[0].rZP.Height());
		g->DrawImage(&imagebx0, m_DataList[0].rBX.left, m_DataList[0].rBX.top, m_DataList[0].rBX.Width(), m_DataList[0].rBX.Height());
	}
	if (m_iDataLinstCount >= 2)
	{
		g->DrawImage(&imagezp1, m_DataList[1].rZP.left, m_DataList[1].rZP.top, m_DataList[1].rZP.Width(), m_DataList[1].rZP.Height());
		g->DrawImage(&imagebx1, m_DataList[1].rBX.left, m_DataList[1].rBX.top, m_DataList[1].rBX.Width(), m_DataList[1].rBX.Height());
	}
	if (m_iDataLinstCount >= 3)
	{
		g->DrawImage(&imagezp2, m_DataList[2].rZP.left, m_DataList[2].rZP.top, m_DataList[2].rZP.Width(), m_DataList[2].rZP.Height());
		g->DrawImage(&imagebx2, m_DataList[2].rBX.left, m_DataList[2].rBX.top, m_DataList[2].rBX.Width(), m_DataList[2].rBX.Height());
	}
	if (m_iDataLinstCount >= 4)
	{
		g->DrawImage(&imagezp3, m_DataList[3].rZP.left, m_DataList[3].rZP.top, m_DataList[3].rZP.Width(), m_DataList[3].rZP.Height());
		g->DrawImage(&imagebx3, m_DataList[3].rBX.left, m_DataList[3].rBX.top, m_DataList[3].rBX.Width(), m_DataList[3].rBX.Height());
	}
	if (m_iDataLinstCount >= 5)
	{
		g->DrawImage(&imagezp4, m_DataList[4].rZP.left, m_DataList[4].rZP.top, m_DataList[4].rZP.Width(), m_DataList[4].rZP.Height());
		g->DrawImage(&imagebx4, m_DataList[4].rBX.left, m_DataList[4].rBX.top, m_DataList[4].rBX.Width(), m_DataList[4].rBX.Height());
	}if (m_iDataLinstCount >= 6)
	{
		g->DrawImage(&imagezp5, m_DataList[5].rZP.left, m_DataList[5].rZP.top, m_DataList[5].rZP.Width(), m_DataList[5].rZP.Height());
		g->DrawImage(&imagebx5, m_DataList[5].rBX.left, m_DataList[5].rBX.top, m_DataList[5].rBX.Width(), m_DataList[5].rBX.Height());
	}if (m_iDataLinstCount >= 7)
	{
		g->DrawImage(&imagezp6, m_DataList[6].rZP.left, m_DataList[6].rZP.top, m_DataList[6].rZP.Width(), m_DataList[6].rZP.Height());
		g->DrawImage(&imagebx6, m_DataList[6].rBX.left, m_DataList[6].rBX.top, m_DataList[6].rBX.Width(), m_DataList[6].rBX.Height());
	}if (m_iDataLinstCount >= 8)
	{
		g->DrawImage(&imagezp7, m_DataList[7].rZP.left, m_DataList[7].rZP.top, m_DataList[7].rZP.Width(), m_DataList[7].rZP.Height());
		g->DrawImage(&imagebx7, m_DataList[7].rBX.left, m_DataList[7].rBX.top, m_DataList[7].rBX.Width(), m_DataList[7].rBX.Height());
	}if (m_iDataLinstCount >= 9)
	{
		g->DrawImage(&imagezp8, m_DataList[8].rZP.left, m_DataList[8].rZP.top, m_DataList[8].rZP.Width(), m_DataList[8].rZP.Height());
		g->DrawImage(&imagebx8, m_DataList[8].rBX.left, m_DataList[8].rBX.top, m_DataList[8].rBX.Width(), m_DataList[8].rBX.Height());
	}


	Gdiplus::Font font(L"方正细倩简体", m_rClient.Width() * 9 / 1000, FontStyleRegular, UnitPixel);
	SolidBrush percentBrush(Color(255, 212, 214, 222));
	StringFormat stringformat;
	stringformat.SetAlignment(StringAlignmentCenter);
	stringformat.SetLineAlignment(StringAlignmentCenter);

	for (int i = 0; i < m_iDataLinstCount; i++)
	{
		g->DrawString(m_DataList[i].csXiangsidu.AllocSysString(), -1, &font, RectF(m_DataList[i].rXiangsidu.left, m_DataList[i].rXiangsidu.top, m_DataList[i].rXiangsidu.Width(), m_DataList[i].rXiangsidu.Height()), &stringformat, &percentBrush);
		g->DrawString(m_DataList[i].csIdcard.AllocSysString(), -1, &font, RectF(m_DataList[i].rIdcard.left, m_DataList[i].rIdcard.top, m_DataList[i].rIdcard.Width(), m_DataList[i].rIdcard.Height()), &stringformat, &percentBrush);
		g->DrawString(m_DataList[i].csFenlei.AllocSysString(), -1, &font, RectF(m_DataList[i].rFenlei.left, m_DataList[i].rFenlei.top, m_DataList[i].rFenlei.Width(), m_DataList[i].rFenlei.Height()), &stringformat, &percentBrush);
		g->DrawString(m_DataList[i].csBaojingyuan.AllocSysString(), -1, &font, RectF(m_DataList[i].rBaojingyuan.left, m_DataList[i].rBaojingyuan.top, m_DataList[i].rBaojingyuan.Width(), m_DataList[i].rBaojingyuan.Height()), &stringformat, &percentBrush);
		g->DrawString(m_DataList[i].csPos.AllocSysString(), -1, &font, RectF(m_DataList[i].rPos.left, m_DataList[i].rPos.top, m_DataList[i].rPos.Width(), m_DataList[i].rPos.Height()), &stringformat, &percentBrush);
		g->DrawString(m_DataList[i].csTime.AllocSysString(), -1, &font, RectF(m_DataList[i].rTime.left, m_DataList[i].rTime.top, m_DataList[i].rTime.Width(), m_DataList[i].rTime.Height()), &stringformat, &percentBrush);
		g->DrawImage(&imageXQ, m_DataList[i].rCZ.left, m_DataList[i].rCZ.top, m_DataList[i].rCZ.Width(), m_DataList[i].rCZ.Height());
	}

}

void CLSJLDlg::DisXiangqing(Graphics *g)
{

}

CString CLSJLDlg::GetiniPath()
{
	CString strIp;
	TCHAR pFileName[MAX_PATH];
	int nPos = GetCurrentDirectory(MAX_PATH, pFileName);

	CString csFullPath(pFileName);
	csFullPath += DBINI_PATH;

	return csFullPath;
}

BOOL CLSJLDlg::InitDatabaseMigration()
{
	CString csSqlConnect;

	CString csFullPath = GetiniPath();
	GetPrivateProfileString(_T("SqlConfiguration"), _T("Sql_ServerAddMigration "), _T(""), m_csSqlIP.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);
	GetPrivateProfileString(_T("SqlConfiguration"), _T("Sql_DatabaseHistory"), _T(""), m_csSqlData.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);
	GetPrivateProfileString(_T("SqlConfiguration"), _T("Sql_Name"), _T(""), m_csSqlName.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);
	GetPrivateProfileString(_T("SqlConfiguration"), _T("Sql_Pwd"), _T(""), m_csSqlPwd.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);
	GetPrivateProfileString(_T("SqlConfiguration"), _T("Img_ServerAdd"), _T(""), m_csImgIP.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);

	csSqlConnect.Format(_T("Driver={MySQL ODBC 5.1 Driver};Server=%s;Database=%s;uid=%s;pwd=%s;"), m_csSqlIP, m_csSqlData, m_csSqlName, m_csSqlPwd);
	try
	{
		g_pConnectMigration.CreateInstance(__uuidof(Connection));
		_bstr_t strConnect = csSqlConnect;
		g_pConnectMigration->Open(strConnect, "", "", adModeUnknown);
	}
	catch (_com_error e)
	{
		return FALSE;
	}
	return  TRUE;
}

BOOL CLSJLDlg::GetMaxCurrent(CString csMaxCurrnet, CString &csStartTime, CString &csEndTime)
{
	try
	{
		CString csSql;
		_RecordsetPtr pRecordset;

		csSql.Format(_T("SELECT * FROM cycleinfo where currentcycle = '%s'"), csMaxCurrnet);
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		if (!pRecordset->adoEOF)
		{
			csStartTime = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("startdate")->Value;
			csEndTime = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("enddate")->Value;
			//csEndTime += " 00:00:00";
		}
		return TRUE;

	}
	catch (CException* e)
	{
	}
	return FALSE;
}


BOOL CLSJLDlg::GetDlgInfo()
{

	try
	{
		CString csSql, csTemp;
		_RecordsetPtr pRecordset;
		csSql = (_T("SELECT * FROM cycleinfo WHERE ismigration = 1"));
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		while (!pRecordset->adoEOF)
		{

			csTemp = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("startdate")->Value;
			m_comFanwei1.AddString(csTemp);
			csTemp = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("enddate")->Value;
			m_comFanwei2.AddString(csTemp);
			pRecordset->MoveNext();
		}

		csSql = (_T("SELECT * FROM camera"));
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		while (!pRecordset->adoEOF)
		{
			csTemp = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("addr")->Value;
			m_comWeizhi.AddString(csTemp);
			pRecordset->MoveNext();
		}
		m_nPagaNum = 1;
		m_nPagasMax = 1;


		return  TRUE;

	}
	catch (_com_error e)
	{
	}
	return FALSE;
}
BOOL CLSJLDlg::GetDataList()
{
	try
	{
		UpdateData();
		USES_CONVERSION;

		CString csStartTime, csEditTime, csTime, csTime1;

		if (m_csFanwei1.IsEmpty() || m_csFanwei2.IsEmpty())
		{
			MessageBox(_T("时间不可为空"));
			return FALSE;
		}
// 		if (!GetMaxCurrent(m_csFanwei1, csStartTime, csTime))
// 			return FALSE;
// 
// 		if (!GetMaxCurrent(m_csFanwei2, csTime, csEditTime))
// 			return FALSE;

		m_iDataLinstCount = 0;

		CString csSql, csTemp, csBlack, csDyclict, csEndSql, csUrrect;
		_RecordsetPtr pRecordset;
		BOOL bFlag = FALSE;
		csSql.Format(_T("SELECT * FROM cycleinfo WHERE enddate >= '%s'  and  enddate <= '%s'"), m_csFanwei1, m_csFanwei2);
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);

		while (!pRecordset->adoEOF)
		{
			bFlag = TRUE;
			csTemp = L"";
			csUrrect = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("currentcycle")->Value;
			csStartTime = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("startdate")->Value;
			csEditTime = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("enddate")->Value;

			csBlack = GetCurrectSql(csUrrect, csStartTime, csEditTime, 1);
			csDyclict = GetCurrectSql(csUrrect, csStartTime, csEditTime, 2);
			if (csBlack.IsEmpty() && !csDyclict.IsEmpty())
			{
				csTemp += csDyclict;
				csTemp += L"union";
			}
			else if (!csBlack.IsEmpty() && csDyclict.IsEmpty())
			{
				csTemp += csBlack;
				csTemp += L"union";
			}
			else if (csBlack.IsEmpty() && csDyclict.IsEmpty())
			{
				csTemp = L"";
			}
			else
			{
				csTemp.Format(_T("%s  union %s union"), csBlack, csDyclict);
			}

			csEndSql += csTemp;
			pRecordset->MoveNext();
		}

		if (bFlag)
		{
			m_csEndSql = csEndSql.Mid(0, csEndSql.GetLength() - 5);
			return SetDataList(1);
		}
	}
	catch (_com_error e)
	{
	}
	return FALSE;

}

CString CLSJLDlg::GetCurrectSql(CString csTable, CString csStartTime, CString csEditTime, int nType)
{
	UpdateData(TRUE);
	CString csSql, csTemp, csName;
	_RecordsetPtr pRecordset;

	if (nType == 1)
	{
		csName.Format(_T("blacklist_%s"), csTable);
	}
	else if (nType == 2)
	{
		csName.Format(_T("dynamic_%s"), csTable);
	}

	csTemp.Format(_T(" SELECT table_name FROM information_schema.TABLES WHERE table_name ='%s'"), csName);
	pRecordset = g_pConnectMigration->Execute(_bstr_t(csTemp), NULL, adCmdText);
	if (pRecordset->adoEOF)
	{
		return L"";
	}


	csEditTime += " 23:59:59";

	csTemp.Format(_T(" select * from %s where %s.alarmtime >='%s'  and %s.alarmtime<='%s'"), csName, csName, csStartTime, csName, csEditTime);
	csSql += csTemp;
	// 	if (!m_csXingbie.IsEmpty())
	// 	{
	// 		csTemp.Format(_T("select * from %s left join employeeinfo on %s.id_card=employeeinfo.id_card where employeeinfo.sex = '%s' and %s.alarmtime >='%s'  and %s.alarmtime<='%s'"), csName, csName, m_csXingbie,csName, csStartTime, csName, csEditTime);
	// 		csSql += csTemp;
	// 	}

	if (!m_csName.IsEmpty())
	{
		csTemp.Format(_T("and %s.id_card = '%s'"), csName, m_csName);
		csSql += csTemp;
	}


	if (!m_csXiangsidu.IsEmpty())
	{
		csTemp.Format(_T("and %s.similarity >= '%s'"), csName, m_csXiangsidu);
		csSql += csTemp;
	}


	if (!m_csWeizhi.IsEmpty())
	{
		try
		{
			csTemp.Format(_T("SELECT * FROM camera WHERE addr = '%s'"), m_csWeizhi);
			pRecordset = g_pConnect->Execute(_bstr_t(csTemp), NULL, adCmdText);
			if (!pRecordset->adoEOF)
			{
				CString csCameraid = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("cameraid")->Value;
				csTemp.Format(_T("and %s.cameraid = '%s'"), csName, csCameraid);
				csSql += csTemp;
			}
		}
		catch (CException* e)
		{
		}

	}
	return csSql;
}

BOOL CLSJLDlg::GetempInfo(CString csIdcard, int nIndex)
{
	try
	{
		_RecordsetPtr pRecordset;
		CString csSql, csTemp;
		csSql.Format(_T("SELECT * FROM employeeinfo WHERE id_card = '%s'"), csIdcard);
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		if (!pRecordset->adoEOF)
		{
			m_DataList[nIndex].csFenlei = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("type")->Value;
			csTemp = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("libraryid")->Value;
			m_DataList[nIndex].csBaojingyuan = csTemp == _T("1") ? _T("黑名单") : _T("动态黑名单");
		}
	}
	catch (...)
	{
		return FALSE;
	}

	return TRUE;
}
BOOL CLSJLDlg::SetDataList(int nIndex)
{

	try
	{
		_RecordsetPtr pRecordset;
		CString csSql, csTemp;
		int nStart;
		nStart = (nIndex - 1) * 9;
		csSql.Format(_T("%s limit %d,9"), m_csEndSql, nStart);
		pRecordset = g_pConnectMigration->Execute(_bstr_t(csSql), NULL, adCmdText);
		int nListIndex = 0;
		if (pRecordset->adoEOF)
		{
			MessageBox(_T("没有符合条件数据"));
			return FALSE;
		}
		while (!pRecordset->adoEOF)
		{
			m_DataList[nListIndex].csXiangsidu = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("similarity")->Value;
			m_DataList[nListIndex].csIdcard = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("id_card")->Value;
			m_DataList[nListIndex].csPos = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("cameraid")->Value;
			m_DataList[nListIndex].csTime = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("alarmtime")->Value;
			GetempInfo(m_DataList[nListIndex].csIdcard, nListIndex);

			GetIdcardImg(_T("Lishi"),
				(TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("id_card")->Value,
				(TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("snapshotid")->Value, nListIndex);

			nListIndex++;
			pRecordset->MoveNext();
		}
		m_iDataLinstCount = nListIndex;
	}
	catch (...)
	{
		return FALSE;
	}

	return TRUE;
}

void CLSJLDlg::SetUp()
{
	if (1 < m_nPagaNum)
	{
		m_nPagaNum--;
		m_csPaga.Format(_T("%d"), m_nPagaNum);
		UpdateData(FALSE);
		SetDataList(m_nPagaNum);
	}
	else
		MessageBox(_T("当前页为首页"));
}
void CLSJLDlg::SetNext()
{
	if (m_nPagaNum < m_nPagasMax)
	{
		m_nPagaNum++;
		m_csPaga.Format(_T("%d"), m_nPagaNum);
		UpdateData(FALSE);
		SetDataList(m_nPagaNum);
	}
	else
		MessageBox(_T("当前页为末页"));
}

BOOL CLSJLDlg::GetMaxPaga()
{
	CString csSql;
	m_nPagasMax = 1;
	m_nPagaNum = 1;
	csSql = m_csEndSql;
	csSql.Replace(_T("*"), _T("count(*)"));
	try
	{
		_RecordsetPtr pRecordset;
		USES_CONVERSION;

		pRecordset = g_pConnectMigration->Execute(_bstr_t(csSql), NULL, adCmdText);
		int nTemp = 0;
		while (!pRecordset->adoEOF)
		{
			CString csEmpInfoName = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("count(*)")->Value;
			int nSum = atoi(T2A(csEmpInfoName));
			nTemp += nSum;
			pRecordset->MoveNext();
		}

		if (nTemp > 9)
		{
			m_nPagasMax = nTemp / 9;
			if (nTemp % 9 != 0)
				m_nPagasMax++;
		}
	}
	catch (...)
	{
		return FALSE;
	}

	return  TRUE;
}

BOOL CLSJLDlg::GetIdcardImg(CString csImgName, CString csIdcard, CString csSnapshotid, int nIndex)
{
	try
	{
		CString csSql, csTemp;
		_RecordsetPtr pRecordset;
		//取抓拍图
		csSql.Format(_T("select * from snapshot WHERE Snapshotid = '%s';"), csSnapshotid);
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		if (!pRecordset->adoEOF)
		{
			CString csImgPath = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("FacePhotoName")->Value;
			int nStart = csImgPath.Find(_T("/Data"));
			csImgPath = csImgPath.Mid(nStart, csImgPath.GetLength() - nStart);

			csTemp.Format(_T("data/%szp_%d.jpg"), csImgName, nIndex);
			if (m_cTools.DownloadImageFromFileServer(m_csImgIP, IMG_PORT, csImgPath, csTemp) == -1)
			{
				SetNullJpg(csTemp);
			}
		}
		else
		{
			return FALSE;
		}
		//取靶向
		csSql.Format(_T("select * from imginfo WHERE id_card = '%s';"), csIdcard);
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		if (!pRecordset->adoEOF)
		{

			CString csImgPath = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("imgpath")->Value;
			int nStart = csImgPath.Find(_T("/Data"));
			csImgPath = csImgPath.Mid(nStart, csImgPath.GetLength() - nStart);

			csTemp.Format(_T("data/%sbx_%d.jpg"), csImgName, nIndex);
			if (m_cTools.DownloadImageFromFileServer(m_csImgIP, IMG_PORT, csImgPath, csTemp) == -1)
			{
				SetNullJpg(csTemp);
			}
		}
		else
		{
			csTemp.Format(_T("data/%sbx_%d.jpg"), csImgName, nIndex);
			SetNullJpg(csTemp);
		}

		return TRUE;

	}
	catch (CException* e)
	{
	}

	return FALSE;
}
void CLSJLDlg::SetNullJpg(CString csPath)
{
	CFile file;
	if (file.Open(csPath, CFile::modeCreate | CFile::modeWrite))
	{
		char jpgchar[36];
		memset(jpgchar, 0, 36);
		file.Write(jpgchar, 36);
		file.Close();
	}
}

void CLSJLDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (m_rSousuo.PtInRect(point))
	{
		if (GetDataList() && GetMaxPaga())
		{
			m_bShowJpg = FALSE;
			InvalidateRect(m_rDataList, false);
			InvalidateRect(m_rTuXiang, false);
		}
		m_csPaga = _T("1");
		UpdateData(FALSE);
	}
	else if (m_rUp.PtInRect(point))
	{
		m_bShowJpg = FALSE;
		SetUp();
		InvalidateRect(m_rDataList, false);
		InvalidateRect(m_rTuXiang, false);

	}
	else if (m_rNext.PtInRect(point))
	{
		m_bShowJpg = FALSE;
		SetNext();
		InvalidateRect(m_rDataList, false);
		InvalidateRect(m_rTuXiang, false);

	}

	for (int i = 0; i < m_iDataLinstCount; i++)
	{
		if (m_DataList[i].rCZ.PtInRect(point))
		{
			CString csTemp;
			csTemp.Format(_T("data/Lishizp_%d.jpg"), i);
			if (!CopyFile(csTemp, _T("data/Lishizp.jpg"), false))
				return;
			csTemp.Format(_T("data/Lishibx_%d.jpg"), i);
			if (!CopyFile(csTemp, _T("data/Lishibx.jpg"), false))
				return;
			m_bShowJpg = TRUE;
			InvalidateRect(m_rTuXiang, false);
		}
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}


BOOL CLSJLDlg::ShowXiangxi()
{
	try
	{
		SetTimer(1, 500, NULL);
	}
	catch (CException* e)
	{
		return FALSE;
	}
	return TRUE;
}

void CLSJLDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1)
	{
		KillTimer(1);
		CCrowdAnalyzDlg * m_pReceiver = (CCrowdAnalyzDlg*)GetParent();
		m_pReceiver->DestoryCurDialog();
		m_pReceiver->m_dlgLSJL->ShowWindow(SW_SHOW);
		//m_comFanwei1.SetCurSel(m_comFanwei1.FindString(-1,m_csFanwei1));
		UpdateData(FALSE);
		m_pReceiver->m_iCurDlg = 4;
		m_pReceiver->InvalidateRect(m_pReceiver->m_rToolBar, false);
		if (GetMaxPaga())
		{
			SetDataList(1);
		}
	}
	

	CDialogEx::OnTimer(nIDEvent);
}
