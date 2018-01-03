// SSJKDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CrowdAnalyz.h"
#include "SSJKDlg.h"
#include "afxdialogex.h"
#include "Tools.h"
#include "CrowdAnalyzDlg.h"
#define  DBINI_PATH			 "\\Parameter.ini"
static struct CameraInfo* g_CameraArrInfo;

CStringArray g_arImgData;

#define  HEI_MING_DAN		_T("heimingdan")
#define  TONG_JI			_T("tongji")


#define  IMG_PORT     10000
// CSSJKDlg 对话框

IMPLEMENT_DYNAMIC(CSSJKDlg, CDialogEx)

CSSJKDlg::CSSJKDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CSSJKDlg::IDD, pParent)
{

}

CSSJKDlg::~CSSJKDlg()
{
}

void CSSJKDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CAMERA, m_wndCameraCom);
}


BEGIN_MESSAGE_MAP(CSSJKDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_CBN_SELCHANGE(IDC_COMBO_CAMERA, &CSSJKDlg::OnCbnSelchangeComboCamera)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CSSJKDlg 消息处理程序


BOOL CSSJKDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	m_rClient.SetRect(0, 0, 1920 * WIDTH / 1920, 830 * HEIGHT / 1080);
	m_rNum.SetRect(846 * m_rClient.Width() / 1920, 12 * m_rClient.Height() / 830, 1502 * m_rClient.Width() / 1920, 213 * m_rClient.Height() / 830);
	m_rSnap.SetRect(1557 * m_rClient.Width() / 1920, 14 * m_rClient.Height() / 830, 1877 * m_rClient.Width() / 1920, 803 * m_rClient.Height() / 830);
	m_rTongji.SetRect(44 * m_rClient.Width() / 1920, 14 * m_rClient.Height() / 830, 781 * m_rClient.Width() / 1920, 392 * m_rClient.Height() / 830);
	m_rAlarm.SetRect(44 * m_rClient.Width() / 1920, 426 * m_rClient.Height() / 830, 781 * m_rClient.Width() / 1920, 803 * m_rClient.Height() / 830);
	m_rToday.SetRect(846 * m_rClient.Width() / 1920, 236 * m_rClient.Height() / 830, 1502 * m_rClient.Width() / 1920, 309 * m_rClient.Height() / 830);
	m_rCamera.SetRect(983 * m_rClient.Width() / 1920, 250 * m_rClient.Height() / 830, 1144 * m_rClient.Width() / 1920, 270 * m_rClient.Height() / 830);
	m_rCameraStatus.SetRect(961 * m_rClient.Width() / 1920, 281 * m_rClient.Height() / 830, 1035 * m_rClient.Width() / 1920, 306 * m_rClient.Height() / 830);
	m_rCameraPos.SetRect(1096 * m_rClient.Width() / 1920, 281 * m_rClient.Height() / 830, 1275 * m_rClient.Width() / 1920, 306 * m_rClient.Height() / 830);
	m_rCameraVideo.SetRect(889 * m_rClient.Width() / 1920, 362 * m_rClient.Height() / 830, 1445 * m_rClient.Width() / 1920, 763 * m_rClient.Height() / 830);
	m_rCameraVideoClose.SetRect(1462 * m_rClient.Width() / 1920, 330 * m_rClient.Height() / 830, 1487 * m_rClient.Width() / 1920, 351 * m_rClient.Height() / 830);


	GetDlgItem(IDC_COMBO_CAMERA)->MoveWindow(m_rCamera.left, m_rCamera.top, m_rCamera.Width(), m_rCamera.Height());
	GetDlgItem(IDC_STATIC_PLAY)->MoveWindow(m_rCameraVideo.left, m_rCameraVideo.top, m_rCameraVideo.Width(), m_rCameraVideo.Height());



	int i = 0;
	for (i = 0; i < 8; i++)
	{
		m_rZhuapailiang[i].SetRect((1105 + 12 * i) * m_rClient.Width() / 1920, 51 * m_rClient.Height() / 830, (1114 + 12 * i) * m_rClient.Width() / 1920, 66 * m_rClient.Height() / 830);
		m_rBendiHei[i].SetRect((1105 + 12 * i) * m_rClient.Width() / 1920, 87 * m_rClient.Height() / 830, (1114 + 12 * i) * m_rClient.Width() / 1920, 102 * m_rClient.Height() / 830);
		m_rDongtaiHei[i].SetRect((1105 + 12 * i) * m_rClient.Width() / 1920, 129 * m_rClient.Height() / 830, (1114 + 12 * i) * m_rClient.Width() / 1920, 144 * m_rClient.Height() / 830);
		m_rDongtaiCun[i].SetRect((1105 + 12 * i) * m_rClient.Width() / 1920, 170 * m_rClient.Height() / 830, (1114 + 12 * i) * m_rClient.Width() / 1920, 185 * m_rClient.Height() / 830);
		m_rTodayZhuapai[i].SetRect((1375 + 12 * i) * m_rClient.Width() / 1920, 256 * m_rClient.Height() / 830, (1384 + 12 * i) * m_rClient.Width() / 1920, 271 * m_rClient.Height() / 830);
		m_rTodayAlarn[i].SetRect((1375 + 12 * i) * m_rClient.Width() / 1920, 286 * m_rClient.Height() / 830, (1384 + 12 * i) * m_rClient.Width() / 1920, 301 * m_rClient.Height() / 830);
	}

	for (i = 0; i < 10; i++)
	{
		m_rSnapImage[i].SetRect((1576 + (i % 2) * 147)* m_rClient.Width() / 1920, (36 + (i / 2) * 153)* m_rClient.Height() / 830, (1709 + (i % 2) * 147)* m_rClient.Width() / 1920, (171 + (i / 2) * 153)* m_rClient.Height() / 830);
	}

	for (i = 0; i < 5; i++)//统计分析结构体部分的位置
	{
		m_dongtaifenxi[i].rZP.SetRect((66 * m_rClient.Width() / 1920), (90 + (58 * i))* m_rClient.Height() / 830, (112 * m_rClient.Width() / 1920), (136 + (58 * i))* m_rClient.Height() / 830);
		m_dongtaifenxi[i].rBX.SetRect((190 * m_rClient.Width() / 1920), (90 + (58 * i))* m_rClient.Height() / 830, (236 * m_rClient.Width() / 1920), (136 + (58 * i))* m_rClient.Height() / 830);
		m_dongtaifenxi[i].rSim.SetRect((117 * m_rClient.Width() / 1920), (90 + (58 * i))* m_rClient.Height() / 830, (185 * m_rClient.Width() / 1920), (136 + (58 * i))* m_rClient.Height() / 830);
		m_dongtaifenxi[i].rNum.SetRect((250 * m_rClient.Width() / 1920), (90 + (58 * i))* m_rClient.Height() / 830, (370 * m_rClient.Width() / 1920), (136 + (58 * i))* m_rClient.Height() / 830);
		m_dongtaifenxi[i].rCount.SetRect((375 * m_rClient.Width() / 1920), (90 + (58 * i))* m_rClient.Height() / 830, (435 * m_rClient.Width() / 1920), (136 + (58 * i))* m_rClient.Height() / 830);
		m_dongtaifenxi[i].rPos.SetRect((440 * m_rClient.Width() / 1920), (90 + (58 * i))* m_rClient.Height() / 830, (590 * m_rClient.Width() / 1920), (136 + (58 * i))* m_rClient.Height() / 830);
		m_dongtaifenxi[i].rTime.SetRect((595 * m_rClient.Width() / 1920), (90 + (58 * i))* m_rClient.Height() / 830, (700 * m_rClient.Width() / 1920), (136 + (58 * i))* m_rClient.Height() / 830);
		m_dongtaifenxi[i].rOperate.SetRect((705 * m_rClient.Width() / 1920), (95 + (58 * i))* m_rClient.Height() / 830, (765 * m_rClient.Width() / 1920), (126 + (58 * i))* m_rClient.Height() / 830);
	}
	for (i = 0; i < 5; i++)//黑名单报警部分的位置
	{
		m_heimingdan[i].rZP.SetRect((66 * m_rClient.Width() / 1920), (501 + (58 * i))* m_rClient.Height() / 830, (112 * m_rClient.Width() / 1920), (547 + (58 * i))* m_rClient.Height() / 830);
		m_heimingdan[i].rBX.SetRect((190 * m_rClient.Width() / 1920), (501 + (58 * i))* m_rClient.Height() / 830, (236 * m_rClient.Width() / 1920), (547 + (58 * i))* m_rClient.Height() / 830);
		m_heimingdan[i].rSim.SetRect((117 * m_rClient.Width() / 1920), (501 + (58 * i))* m_rClient.Height() / 830, (185 * m_rClient.Width() / 1920), (547 + (58 * i))* m_rClient.Height() / 830);
		m_heimingdan[i].rNum.SetRect((250 * m_rClient.Width() / 1920), (501 + (58 * i))* m_rClient.Height() / 830, (370 * m_rClient.Width() / 1920), (547 + (58 * i))* m_rClient.Height() / 830);
		m_heimingdan[i].rCount.SetRect((375 * m_rClient.Width() / 1920), (501 + (58 * i))* m_rClient.Height() / 830, (435 * m_rClient.Width() / 1920), (547 + (58 * i))* m_rClient.Height() / 830);
		m_heimingdan[i].rPos.SetRect((440 * m_rClient.Width() / 1920), (501 + (58 * i))* m_rClient.Height() / 830, (590 * m_rClient.Width() / 1920), (547 + (58 * i))* m_rClient.Height() / 830);
		m_heimingdan[i].rTime.SetRect((595 * m_rClient.Width() / 1920), (501 + (58 * i))* m_rClient.Height() / 830, (700 * m_rClient.Width() / 1920), (547 + (58 * i))* m_rClient.Height() / 830);
		m_heimingdan[i].rOperate.SetRect((705 * m_rClient.Width() / 1920), (506 + (58 * i))* m_rClient.Height() / 830, (765 * m_rClient.Width() / 1920), (537 + (58 * i))* m_rClient.Height() / 830);
	}




	//双缓存
	CClientDC dc(this);
	pMemDC = new CDC;
	pBitmap = new CBitmap;
	pMemDC->CreateCompatibleDC(&dc);
	pBitmap->CreateCompatibleBitmap(&dc, m_rClient.Width(), m_rClient.Height());


	//////////////////////////////////

	//GetDlgItem(IDC_STATIC_PLAY)->ShowWindow(SW_HIDE);
	SetNullJpgAll();
	Start();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CSSJKDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	CBitmap * pOldBitmap = pMemDC->SelectObject(pBitmap);
	pMemDC->PatBlt(0, 0, m_rClient.Width(), m_rClient.Height(), PATCOPY);
	Graphics g(pMemDC->m_hDC);
	//画总背景
	Image img_bk(L"img/ssjk_bk.jpg");
	g.DrawImage(&img_bk, 0, 0, m_rClient.Width(), m_rClient.Height());

	//测试位置
	int i = 0;
	//Pen redPen(Color(255, 0, 0), 1);
	//g.DrawRectangle(&redPen, m_rToday.left, m_rToday.top, m_rToday.Width(), m_rToday.Height());
	//g.DrawRectangle(&redPen, m_rCameraStatus.left, m_rCameraStatus.top, m_rCameraStatus.Width(), m_rCameraStatus.Height());
	//g.DrawRectangle(&redPen, m_rCameraVideoClose.left, m_rCameraVideoClose.top, m_rCameraVideoClose.Width(), m_rCameraVideoClose.Height());
	//	for (i = 0; i < 8; i++)
	{
		//		g.DrawRectangle(&redPen, m_rTodayZhuapai[i].left, m_rTodayZhuapai[i].top, m_rTodayZhuapai[i].Width(), m_rTodayZhuapai[i].Height());
		//		g.DrawRectangle(&redPen, m_rTodayAlarn[i].left, m_rTodayAlarn[i].top, m_rTodayAlarn[i].Width(), m_rTodayAlarn[i].Height());
	}

	DisNum(&g);
	DisToday(&g);
	DisSnap(&g);
	DisTongji(&g);
	DisHeimingdan(&g);
	//显示缓存，清除HDC
	dc.BitBlt(0, 0, m_rClient.Width(), m_rClient.Height(), pMemDC, 0, 0, SRCCOPY);
	pMemDC->SelectObject(pOldBitmap);
	g.ReleaseHDC(pMemDC->m_hDC);
	// 不为绘图消息调用 CDialogEx::OnPaint()
}

void CSSJKDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO:  在此处添加消息处理程序代码
	//界面

	KillTimer(1);
	KillTimer(2);
	KillTimer(3);
	KillTimer(4);
	KillTimer(5);
	if (m_bIsPlaying)
	{
		StopPlay();
		DoLogout();
	}

	delete[]g_CameraArrInfo;
	delete pMemDC;
	delete pBitmap;

}

void CSSJKDlg::DisNum(Graphics *g)
{

	Image image0(L"img/0.png");
	Image image1(L"img/1.png");
	Image image2(L"img/2.png");
	Image image3(L"img/3.png");
	Image image4(L"img/4.png");
	Image image5(L"img/5.png");
	Image image6(L"img/6.png");
	Image image7(L"img/7.png");
	Image image8(L"img/8.png");
	Image image9(L"img/9.png");


	Image imageH0(L"img/0-H.png");
	Image imageH1(L"img/1-H.png");
	Image imageH2(L"img/2-H.png");
	Image imageH3(L"img/3-H.png");
	Image imageH4(L"img/4-H.png");
	Image imageH5(L"img/5-H.png");
	Image imageH6(L"img/6-H.png");
	Image imageH7(L"img/7-H.png");
	Image imageH8(L"img/8-H.png");
	Image imageH9(L"img/9-H.png");

	CString csZhuapai, csBendiheim, csDongtaiHei, csDongtaicun, csTemp;
	csZhuapai.Format(_T("%d"), m_iDrawZhuapailiang);
	csBendiheim.Format(_T("%d"), m_iDrawBendiHei);
	csDongtaiHei.Format(_T("%d"), m_iDrawDongtaiHei);
	csDongtaicun.Format(_T("%d"), m_iDrawDongtaiCun);


	int i = 0;
	USES_CONVERSION;
	for (i = 0; i < csZhuapai.GetLength(); i++)
	{
		csTemp = csZhuapai.Mid(csZhuapai.GetLength() - i - 1, 1);
		switch (atoi(T2A(csTemp)))
		{
		case 0: g->DrawImage(&image0, m_rZhuapailiang[7 - i].left, m_rZhuapailiang[7 - i].top, m_rZhuapailiang[7 - i].Width(), m_rZhuapailiang[7 - i].Height()); break;
		case 1: g->DrawImage(&image1, m_rZhuapailiang[7 - i].left, m_rZhuapailiang[7 - i].top, m_rZhuapailiang[7 - i].Width(), m_rZhuapailiang[7 - i].Height()); break;
		case 2: g->DrawImage(&image2, m_rZhuapailiang[7 - i].left, m_rZhuapailiang[7 - i].top, m_rZhuapailiang[7 - i].Width(), m_rZhuapailiang[7 - i].Height()); break;
		case 3: g->DrawImage(&image3, m_rZhuapailiang[7 - i].left, m_rZhuapailiang[7 - i].top, m_rZhuapailiang[7 - i].Width(), m_rZhuapailiang[7 - i].Height()); break;
		case 4: g->DrawImage(&image4, m_rZhuapailiang[7 - i].left, m_rZhuapailiang[7 - i].top, m_rZhuapailiang[7 - i].Width(), m_rZhuapailiang[7 - i].Height()); break;
		case 5: g->DrawImage(&image5, m_rZhuapailiang[7 - i].left, m_rZhuapailiang[7 - i].top, m_rZhuapailiang[7 - i].Width(), m_rZhuapailiang[7 - i].Height()); break;
		case 6: g->DrawImage(&image6, m_rZhuapailiang[7 - i].left, m_rZhuapailiang[7 - i].top, m_rZhuapailiang[7 - i].Width(), m_rZhuapailiang[7 - i].Height()); break;
		case 7: g->DrawImage(&image7, m_rZhuapailiang[7 - i].left, m_rZhuapailiang[7 - i].top, m_rZhuapailiang[7 - i].Width(), m_rZhuapailiang[7 - i].Height()); break;
		case 8: g->DrawImage(&image8, m_rZhuapailiang[7 - i].left, m_rZhuapailiang[7 - i].top, m_rZhuapailiang[7 - i].Width(), m_rZhuapailiang[7 - i].Height()); break;
		case 9: g->DrawImage(&image9, m_rZhuapailiang[7 - i].left, m_rZhuapailiang[7 - i].top, m_rZhuapailiang[7 - i].Width(), m_rZhuapailiang[7 - i].Height()); break;
		default:
			break;
		}
	}


	for (i = 0; i < csBendiheim.GetLength(); i++)
	{
		csTemp = csBendiheim.Mid(csBendiheim.GetLength() - i - 1, 1);
		switch (atoi(T2A(csTemp)))
		{
		case 0: g->DrawImage(&imageH0, m_rBendiHei[7 - i].left, m_rBendiHei[7 - i].top, m_rBendiHei[7 - i].Width(), m_rBendiHei[7 - i].Height()); break;
		case 1: g->DrawImage(&imageH1, m_rBendiHei[7 - i].left, m_rBendiHei[7 - i].top, m_rBendiHei[7 - i].Width(), m_rBendiHei[7 - i].Height()); break;
		case 2: g->DrawImage(&imageH2, m_rBendiHei[7 - i].left, m_rBendiHei[7 - i].top, m_rBendiHei[7 - i].Width(), m_rBendiHei[7 - i].Height()); break;
		case 3: g->DrawImage(&imageH3, m_rBendiHei[7 - i].left, m_rBendiHei[7 - i].top, m_rBendiHei[7 - i].Width(), m_rBendiHei[7 - i].Height()); break;
		case 4: g->DrawImage(&imageH4, m_rBendiHei[7 - i].left, m_rBendiHei[7 - i].top, m_rBendiHei[7 - i].Width(), m_rBendiHei[7 - i].Height()); break;
		case 5: g->DrawImage(&imageH5, m_rBendiHei[7 - i].left, m_rBendiHei[7 - i].top, m_rBendiHei[7 - i].Width(), m_rBendiHei[7 - i].Height()); break;
		case 6: g->DrawImage(&imageH6, m_rBendiHei[7 - i].left, m_rBendiHei[7 - i].top, m_rBendiHei[7 - i].Width(), m_rBendiHei[7 - i].Height()); break;
		case 7: g->DrawImage(&imageH7, m_rBendiHei[7 - i].left, m_rBendiHei[7 - i].top, m_rBendiHei[7 - i].Width(), m_rBendiHei[7 - i].Height()); break;
		case 8: g->DrawImage(&imageH8, m_rBendiHei[7 - i].left, m_rBendiHei[7 - i].top, m_rBendiHei[7 - i].Width(), m_rBendiHei[7 - i].Height()); break;
		case 9: g->DrawImage(&imageH9, m_rBendiHei[7 - i].left, m_rBendiHei[7 - i].top, m_rBendiHei[7 - i].Width(), m_rBendiHei[7 - i].Height()); break;
		default:
			break;
		}
	}


	for (i = 0; i < csDongtaiHei.GetLength(); i++)
	{
		csTemp = csDongtaiHei.Mid(csDongtaiHei.GetLength() - i - 1, 1);
		switch (atoi(T2A(csTemp)))
		{
		case 0: g->DrawImage(&imageH0, m_rDongtaiHei[7 - i].left, m_rDongtaiHei[7 - i].top, m_rDongtaiHei[7 - i].Width(), m_rDongtaiHei[7 - i].Height()); break;
		case 1: g->DrawImage(&imageH1, m_rDongtaiHei[7 - i].left, m_rDongtaiHei[7 - i].top, m_rDongtaiHei[7 - i].Width(), m_rDongtaiHei[7 - i].Height()); break;
		case 2: g->DrawImage(&imageH2, m_rDongtaiHei[7 - i].left, m_rDongtaiHei[7 - i].top, m_rDongtaiHei[7 - i].Width(), m_rDongtaiHei[7 - i].Height()); break;
		case 3: g->DrawImage(&imageH3, m_rDongtaiHei[7 - i].left, m_rDongtaiHei[7 - i].top, m_rDongtaiHei[7 - i].Width(), m_rDongtaiHei[7 - i].Height()); break;
		case 4: g->DrawImage(&imageH4, m_rDongtaiHei[7 - i].left, m_rDongtaiHei[7 - i].top, m_rDongtaiHei[7 - i].Width(), m_rDongtaiHei[7 - i].Height()); break;
		case 5: g->DrawImage(&imageH5, m_rDongtaiHei[7 - i].left, m_rDongtaiHei[7 - i].top, m_rDongtaiHei[7 - i].Width(), m_rDongtaiHei[7 - i].Height()); break;
		case 6: g->DrawImage(&imageH6, m_rDongtaiHei[7 - i].left, m_rDongtaiHei[7 - i].top, m_rDongtaiHei[7 - i].Width(), m_rDongtaiHei[7 - i].Height()); break;
		case 7: g->DrawImage(&imageH7, m_rDongtaiHei[7 - i].left, m_rDongtaiHei[7 - i].top, m_rDongtaiHei[7 - i].Width(), m_rDongtaiHei[7 - i].Height()); break;
		case 8: g->DrawImage(&imageH8, m_rDongtaiHei[7 - i].left, m_rDongtaiHei[7 - i].top, m_rDongtaiHei[7 - i].Width(), m_rDongtaiHei[7 - i].Height()); break;
		case 9: g->DrawImage(&imageH9, m_rDongtaiHei[7 - i].left, m_rDongtaiHei[7 - i].top, m_rDongtaiHei[7 - i].Width(), m_rDongtaiHei[7 - i].Height()); break;
		default:
			break;
		}
	}


	for (i = 0; i < csDongtaicun.GetLength(); i++)
	{
		csTemp = csDongtaicun.Mid(csDongtaicun.GetLength() - i - 1, 1);
		switch (atoi(T2A(csTemp)))
		{
		case 0: g->DrawImage(&image0, m_rDongtaiCun[7 - i].left, m_rDongtaiCun[7 - i].top, m_rDongtaiCun[7 - i].Width(), m_rDongtaiCun[7 - i].Height()); break;
		case 1: g->DrawImage(&image1, m_rDongtaiCun[7 - i].left, m_rDongtaiCun[7 - i].top, m_rDongtaiCun[7 - i].Width(), m_rDongtaiCun[7 - i].Height()); break;
		case 2: g->DrawImage(&image2, m_rDongtaiCun[7 - i].left, m_rDongtaiCun[7 - i].top, m_rDongtaiCun[7 - i].Width(), m_rDongtaiCun[7 - i].Height()); break;
		case 3: g->DrawImage(&image3, m_rDongtaiCun[7 - i].left, m_rDongtaiCun[7 - i].top, m_rDongtaiCun[7 - i].Width(), m_rDongtaiCun[7 - i].Height()); break;
		case 4: g->DrawImage(&image4, m_rDongtaiCun[7 - i].left, m_rDongtaiCun[7 - i].top, m_rDongtaiCun[7 - i].Width(), m_rDongtaiCun[7 - i].Height()); break;
		case 5: g->DrawImage(&image5, m_rDongtaiCun[7 - i].left, m_rDongtaiCun[7 - i].top, m_rDongtaiCun[7 - i].Width(), m_rDongtaiCun[7 - i].Height()); break;
		case 6: g->DrawImage(&image6, m_rDongtaiCun[7 - i].left, m_rDongtaiCun[7 - i].top, m_rDongtaiCun[7 - i].Width(), m_rDongtaiCun[7 - i].Height()); break;
		case 7: g->DrawImage(&image7, m_rDongtaiCun[7 - i].left, m_rDongtaiCun[7 - i].top, m_rDongtaiCun[7 - i].Width(), m_rDongtaiCun[7 - i].Height()); break;
		case 8: g->DrawImage(&image8, m_rDongtaiCun[7 - i].left, m_rDongtaiCun[7 - i].top, m_rDongtaiCun[7 - i].Width(), m_rDongtaiCun[7 - i].Height()); break;
		case 9: g->DrawImage(&image9, m_rDongtaiCun[7 - i].left, m_rDongtaiCun[7 - i].top, m_rDongtaiCun[7 - i].Width(), m_rDongtaiCun[7 - i].Height()); break;
		default:
			break;
		}
	}

	// 	Image *img_num[10];
	// 	Image *img_numH[10];
	// 	img_num[0] = Image::FromFile(L"img/0.png");
	// 	img_num[1] = Image::FromFile(L"img/1.png");
	// 	img_num[2] = Image::FromFile(L"img/2.png");
	// 	img_num[3] = Image::FromFile(L"img/3.png");
	// 	img_num[4] = Image::FromFile(L"img/4.png");
	// 	img_num[5] = Image::FromFile(L"img/5.png");
	// 	img_num[6] = Image::FromFile(L"img/6.png");
	// 	img_num[7] = Image::FromFile(L"img/7.png");
	// 	img_num[8] = Image::FromFile(L"img/8.png");
	// 	img_num[9] = Image::FromFile(L"img/9.png");
	// 	img_numH[0] = Image::FromFile(L"img/0-H.png");
	// 	img_numH[1] = Image::FromFile(L"img/1-H.png");
	// 	img_numH[2] = Image::FromFile(L"img/2-H.png");
	// 	img_numH[3] = Image::FromFile(L"img/3-H.png");
	// 	img_numH[4] = Image::FromFile(L"img/4-H.png");
	// 	img_numH[5] = Image::FromFile(L"img/5-H.png");
	// 	img_numH[6] = Image::FromFile(L"img/6-H.png");
	// 	img_numH[7] = Image::FromFile(L"img/7-H.png");
	// 	img_numH[8] = Image::FromFile(L"img/8-H.png");
	// 	img_numH[9] = Image::FromFile(L"img/9-H.png");
	// 	int nAll1 = m_iDrawZhuapailiang;
	// 	int nAll2 = m_iDrawBendiHei;
	// 	int nAll3 = m_iDrawDongtaiHei;
	// 	int nAll4 = m_iDrawDongtaiCun;
	// 	int i = 0;
	// 	for (i = 0; i < 8; i++)
	// 	{
	// 		g->DrawImage(img_num[nAll1 % 10], m_rZhuapailiang[7 - i].left, m_rZhuapailiang[7 - i].top, m_rZhuapailiang[7 - i].Width(), m_rZhuapailiang[7 - i].Height());
	// 		nAll1 = nAll1 / 10;
	// 		if (nAll1 == 0)
	// 			break;
	// 	}
	// 	for (i = 0; i < 8; i++)
	// 	{
	// 		g->DrawImage(img_numH[nAll2 % 10], m_rBendiHei[7 - i].left, m_rBendiHei[7 - i].top, m_rBendiHei[7 - i].Width(), m_rBendiHei[7 - i].Height());
	// 		nAll2 = nAll2 / 10;
	// 		if (nAll2 == 0)
	// 			break;
	// 	}
	// 	for (i = 0; i < 8; i++)
	// 	{
	// 		g->DrawImage(img_numH[nAll3 % 10], m_rDongtaiHei[7 - i].left, m_rDongtaiHei[7 - i].top, m_rDongtaiHei[7 - i].Width(), m_rDongtaiHei[7 - i].Height());
	// 		nAll3 = nAll3 / 10;
	// 		if (nAll3 == 0)
	// 			break;
	// 	}
	// 	for (i = 0; i < 8; i++)
	// 	{
	// 		g->DrawImage(img_num[nAll4 % 10], m_rDongtaiCun[7 - i].left, m_rDongtaiCun[7 - i].top, m_rDongtaiCun[7 - i].Width(), m_rDongtaiCun[7 - i].Height());
	// 		nAll4 = nAll4 / 10;
	// 		if (nAll4 == 0)
	// 			break;
	// 	}
}

void CSSJKDlg::DisToday(Graphics *g)
{
	// 	Image *img_num[10];
	// 	Image *img_numH[10];
	// 	img_num[0] = Image::FromFile(L"img/0.png");
	// 	img_num[1] = Image::FromFile(L"img/1.png");
	// 	img_num[2] = Image::FromFile(L"img/2.png");
	// 	img_num[3] = Image::FromFile(L"img/3.png");
	// 	img_num[4] = Image::FromFile(L"img/4.png");
	// 	img_num[5] = Image::FromFile(L"img/5.png");
	// 	img_num[6] = Image::FromFile(L"img/6.png");
	// 	img_num[7] = Image::FromFile(L"img/7.png");
	// 	img_num[8] = Image::FromFile(L"img/8.png");
	// 	img_num[9] = Image::FromFile(L"img/9.png");
	// 
	// 	img_numH[0] = Image::FromFile(L"img/0-H.png");
	// 	img_numH[1] = Image::FromFile(L"img/1-H.png");
	// 	img_numH[2] = Image::FromFile(L"img/2-H.png");
	// 	img_numH[3] = Image::FromFile(L"img/3-H.png");
	// 	img_numH[4] = Image::FromFile(L"img/4-H.png");
	// 	img_numH[5] = Image::FromFile(L"img/5-H.png");
	// 	img_numH[6] = Image::FromFile(L"img/6-H.png");
	// 	img_numH[7] = Image::FromFile(L"img/7-H.png");
	// 	img_numH[8] = Image::FromFile(L"img/8-H.png");
	// 	img_numH[9] = Image::FromFile(L"img/9-H.png");
	//int nAll1 = m_iDrawTodayZhuapai;
	//int nAll2 = m_iDrawTodayAlarm;
	// 	int i = 0;
	// 	for (i = 0; i < 8; i++)
	// 	{
	// 		g->DrawImage(img_num[nAll1 % 10], m_rTodayZhuapai[7 - i].left, m_rTodayZhuapai[7 - i].top, m_rTodayZhuapai[7 - i].Width(), m_rTodayZhuapai[7 - i].Height());
	// 		nAll1 = nAll1 / 10;
	// 		if (nAll1 == 0)
	// 			break;
	// 	}
	// 	for (i = 0; i < 8; i++)
	// 	{
	// 		g->DrawImage(img_numH[nAll2 % 10], m_rTodayAlarn[7 - i].left, m_rTodayAlarn[7 - i].top, m_rTodayAlarn[7 - i].Width(), m_rTodayAlarn[7 - i].Height());
	// 		nAll2 = nAll2 / 10;
	// 		if (nAll2 == 0)
	// 			break;
	// 	}

	CString csZhuapai, csAlarm, csTemp;
	csZhuapai.Format(_T("%d"), m_iDrawTodayZhuapai);
	csAlarm.Format(_T("%d"), m_iDrawTodayAlarm);

	Image image0(L"img/0.png");
	Image image1(L"img/1.png");
	Image image2(L"img/2.png");
	Image image3(L"img/3.png");
	Image image4(L"img/4.png");
	Image image5(L"img/5.png");
	Image image6(L"img/6.png");
	Image image7(L"img/7.png");
	Image image8(L"img/8.png");
	Image image9(L"img/9.png");


	Image imageH0(L"img/0-H.png");
	Image imageH1(L"img/1-H.png");
	Image imageH2(L"img/2-H.png");
	Image imageH3(L"img/3-H.png");
	Image imageH4(L"img/4-H.png");
	Image imageH5(L"img/5-H.png");
	Image imageH6(L"img/6-H.png");
	Image imageH7(L"img/7-H.png");
	Image imageH8(L"img/8-H.png");
	Image imageH9(L"img/9-H.png");
	int i = 0;
	USES_CONVERSION;
	for (i = 0; i < csZhuapai.GetLength(); i++)
	{
		csTemp = csZhuapai.Mid(csZhuapai.GetLength() - i - 1, 1);
		switch (atoi(T2A(csTemp)))
		{
		case 0: g->DrawImage(&image0, m_rTodayZhuapai[7 - i].left, m_rTodayZhuapai[7 - i].top, m_rTodayZhuapai[7 - i].Width(), m_rTodayZhuapai[7 - i].Height()); break;
		case 1: g->DrawImage(&image1, m_rTodayZhuapai[7 - i].left, m_rTodayZhuapai[7 - i].top, m_rTodayZhuapai[7 - i].Width(), m_rTodayZhuapai[7 - i].Height()); break;
		case 2: g->DrawImage(&image2, m_rTodayZhuapai[7 - i].left, m_rTodayZhuapai[7 - i].top, m_rTodayZhuapai[7 - i].Width(), m_rTodayZhuapai[7 - i].Height()); break;
		case 3: g->DrawImage(&image3, m_rTodayZhuapai[7 - i].left, m_rTodayZhuapai[7 - i].top, m_rTodayZhuapai[7 - i].Width(), m_rTodayZhuapai[7 - i].Height()); break;
		case 4: g->DrawImage(&image4, m_rTodayZhuapai[7 - i].left, m_rTodayZhuapai[7 - i].top, m_rTodayZhuapai[7 - i].Width(), m_rTodayZhuapai[7 - i].Height()); break;
		case 5: g->DrawImage(&image5, m_rTodayZhuapai[7 - i].left, m_rTodayZhuapai[7 - i].top, m_rTodayZhuapai[7 - i].Width(), m_rTodayZhuapai[7 - i].Height()); break;
		case 6: g->DrawImage(&image6, m_rTodayZhuapai[7 - i].left, m_rTodayZhuapai[7 - i].top, m_rTodayZhuapai[7 - i].Width(), m_rTodayZhuapai[7 - i].Height()); break;
		case 7: g->DrawImage(&image7, m_rTodayZhuapai[7 - i].left, m_rTodayZhuapai[7 - i].top, m_rTodayZhuapai[7 - i].Width(), m_rTodayZhuapai[7 - i].Height()); break;
		case 8: g->DrawImage(&image8, m_rTodayZhuapai[7 - i].left, m_rTodayZhuapai[7 - i].top, m_rTodayZhuapai[7 - i].Width(), m_rTodayZhuapai[7 - i].Height()); break;
		case 9: g->DrawImage(&image9, m_rTodayZhuapai[7 - i].left, m_rTodayZhuapai[7 - i].top, m_rTodayZhuapai[7 - i].Width(), m_rTodayZhuapai[7 - i].Height()); break;
		default:
			break;
		}
	}


	for (i = 0; i < csAlarm.GetLength(); i++)
	{
		csTemp = csAlarm.Mid(csAlarm.GetLength() - i - 1, 1);
		switch (atoi(T2A(csTemp)))
		{
		case 0: g->DrawImage(&imageH0, m_rTodayAlarn[7 - i].left, m_rTodayAlarn[7 - i].top, m_rTodayAlarn[7 - i].Width(), m_rTodayAlarn[7 - i].Height()); break;
		case 1: g->DrawImage(&imageH1, m_rTodayAlarn[7 - i].left, m_rTodayAlarn[7 - i].top, m_rTodayAlarn[7 - i].Width(), m_rTodayAlarn[7 - i].Height()); break;
		case 2: g->DrawImage(&imageH2, m_rTodayAlarn[7 - i].left, m_rTodayAlarn[7 - i].top, m_rTodayAlarn[7 - i].Width(), m_rTodayAlarn[7 - i].Height()); break;
		case 3: g->DrawImage(&imageH3, m_rTodayAlarn[7 - i].left, m_rTodayAlarn[7 - i].top, m_rTodayAlarn[7 - i].Width(), m_rTodayAlarn[7 - i].Height()); break;
		case 4: g->DrawImage(&imageH4, m_rTodayAlarn[7 - i].left, m_rTodayAlarn[7 - i].top, m_rTodayAlarn[7 - i].Width(), m_rTodayAlarn[7 - i].Height()); break;
		case 5: g->DrawImage(&imageH5, m_rTodayAlarn[7 - i].left, m_rTodayAlarn[7 - i].top, m_rTodayAlarn[7 - i].Width(), m_rTodayAlarn[7 - i].Height()); break;
		case 6: g->DrawImage(&imageH6, m_rTodayAlarn[7 - i].left, m_rTodayAlarn[7 - i].top, m_rTodayAlarn[7 - i].Width(), m_rTodayAlarn[7 - i].Height()); break;
		case 7: g->DrawImage(&imageH7, m_rTodayAlarn[7 - i].left, m_rTodayAlarn[7 - i].top, m_rTodayAlarn[7 - i].Width(), m_rTodayAlarn[7 - i].Height()); break;
		case 8: g->DrawImage(&imageH8, m_rTodayAlarn[7 - i].left, m_rTodayAlarn[7 - i].top, m_rTodayAlarn[7 - i].Width(), m_rTodayAlarn[7 - i].Height()); break;
		case 9: g->DrawImage(&imageH9, m_rTodayAlarn[7 - i].left, m_rTodayAlarn[7 - i].top, m_rTodayAlarn[7 - i].Width(), m_rTodayAlarn[7 - i].Height()); break;
		default:
			break;
		}
	}


	Gdiplus::Font font(L"方正细倩简体", m_rClient.Width() * 9 / 1000, FontStyleRegular, UnitPixel);
	SolidBrush percentBrush(Color(255, 212, 214, 222));
	StringFormat stringformat;
	stringformat.SetAlignment(StringAlignmentCenter);
	stringformat.SetLineAlignment(StringAlignmentCenter);

	g->DrawString(m_csCameraStatus.AllocSysString(), -1, &font, RectF(m_rCameraStatus.left, m_rCameraStatus.top, m_rCameraStatus.Width(), m_rCameraStatus.Height()), &stringformat, &percentBrush);
	g->DrawString(m_csCameraPos.AllocSysString(), -1, &font, RectF(m_rCameraPos.left, m_rCameraPos.top, m_rCameraPos.Width(), m_rCameraPos.Height()), &stringformat, &percentBrush);

}


void CSSJKDlg::DisSnap(Graphics *g)
{
	CFileStatus fs;
	BOOL isexist = CFile::GetStatus(L"data/realtimesnap_10.jpg", fs);
	if (isexist)
	{
		DeleteFile(L"data/realtimesnap_9.jpg");
		MoveFile(L"data/realtimesnap_8.jpg", L"data/realtimesnap_9.jpg");
		MoveFile(L"data/realtimesnap_7.jpg", L"data/realtimesnap_8.jpg");
		MoveFile(L"data/realtimesnap_6.jpg", L"data/realtimesnap_7.jpg");
		MoveFile(L"data/realtimesnap_5.jpg", L"data/realtimesnap_6.jpg");
		MoveFile(L"data/realtimesnap_4.jpg", L"data/realtimesnap_5.jpg");
		MoveFile(L"data/realtimesnap_3.jpg", L"data/realtimesnap_4.jpg");
		MoveFile(L"data/realtimesnap_2.jpg", L"data/realtimesnap_3.jpg");
		MoveFile(L"data/realtimesnap_1.jpg", L"data/realtimesnap_2.jpg");
		MoveFile(L"data/realtimesnap_0.jpg", L"data/realtimesnap_1.jpg");
		MoveFile(L"data/realtimesnap_10.jpg", L"data/realtimesnap_0.jpg");
	}

	Image image0(L"data/realtimesnap_0.jpg");
	Image image1(L"data/realtimesnap_1.jpg");
	Image image2(L"data/realtimesnap_2.jpg");
	Image image3(L"data/realtimesnap_3.jpg");
	Image image4(L"data/realtimesnap_4.jpg");
	Image image5(L"data/realtimesnap_5.jpg");
	Image image6(L"data/realtimesnap_6.jpg");
	Image image7(L"data/realtimesnap_7.jpg");
	Image image8(L"data/realtimesnap_8.jpg");
	Image image9(L"data/realtimesnap_9.jpg");

	g->DrawImage(&image0, m_rSnapImage[0].left, m_rSnapImage[0].top, m_rSnapImage[0].Width(), m_rSnapImage[0].Height());
	g->DrawImage(&image1, m_rSnapImage[1].left, m_rSnapImage[1].top, m_rSnapImage[1].Width(), m_rSnapImage[1].Height());
	g->DrawImage(&image2, m_rSnapImage[2].left, m_rSnapImage[2].top, m_rSnapImage[2].Width(), m_rSnapImage[2].Height());
	g->DrawImage(&image3, m_rSnapImage[3].left, m_rSnapImage[3].top, m_rSnapImage[3].Width(), m_rSnapImage[3].Height());
	g->DrawImage(&image4, m_rSnapImage[4].left, m_rSnapImage[4].top, m_rSnapImage[4].Width(), m_rSnapImage[4].Height());
	g->DrawImage(&image5, m_rSnapImage[5].left, m_rSnapImage[5].top, m_rSnapImage[5].Width(), m_rSnapImage[5].Height());
	g->DrawImage(&image6, m_rSnapImage[6].left, m_rSnapImage[6].top, m_rSnapImage[6].Width(), m_rSnapImage[6].Height());
	g->DrawImage(&image7, m_rSnapImage[7].left, m_rSnapImage[7].top, m_rSnapImage[7].Width(), m_rSnapImage[7].Height());
	g->DrawImage(&image8, m_rSnapImage[8].left, m_rSnapImage[8].top, m_rSnapImage[8].Width(), m_rSnapImage[8].Height());
	g->DrawImage(&image9, m_rSnapImage[9].left, m_rSnapImage[8].top, m_rSnapImage[9].Width(), m_rSnapImage[9].Height());
}

void CSSJKDlg::DisTongji(Graphics *g)
{
	CFileStatus fs;
	BOOL isexist = CFile::GetStatus(L"data/tongjizp_5.jpg", fs);
	if (isexist)
	{
		DeleteFile(L"data/tongjizp_4.jpg");
		MoveFile(L"data/tongjizp_3.jpg", L"data/tongjizp_4.jpg");
		MoveFile(L"data/tongjizp_2.jpg", L"data/tongjizp_3.jpg");
		MoveFile(L"data/tongjizp_1.jpg", L"data/tongjizp_2.jpg");
		MoveFile(L"data/tongjizp_0.jpg", L"data/tongjizp_1.jpg");
		MoveFile(L"data/tongjizp_5.jpg", L"data/tongjizp_0.jpg");

		DeleteFile(L"data/tongjibx_4.jpg");
		MoveFile(L"data/tongjibx_3.jpg", L"data/tongjibx_4.jpg");
		MoveFile(L"data/tongjibx_2.jpg", L"data/tongjibx_3.jpg");
		MoveFile(L"data/tongjibx_1.jpg", L"data/tongjibx_2.jpg");
		MoveFile(L"data/tongjibx_0.jpg", L"data/tongjibx_1.jpg");
		MoveFile(L"data/tongjibx_5.jpg", L"data/tongjibx_0.jpg");
	}

	Image imagezp0(L"data/tongjizp_0.jpg");
	Image imagezp1(L"data/tongjizp_1.jpg");
	Image imagezp2(L"data/tongjizp_2.jpg");
	Image imagezp3(L"data/tongjizp_3.jpg");
	Image imagezp4(L"data/tongjizp_4.jpg");

	Image imagebx0(L"data/tongjibx_0.jpg");
	Image imagebx1(L"data/tongjibx_1.jpg");
	Image imagebx2(L"data/tongjibx_2.jpg");
	Image imagebx3(L"data/tongjibx_3.jpg");
	Image imagebx4(L"data/tongjibx_4.jpg");
	Image imageXQ(L"img/ssjk_xiangqing.png");

	if (m_iDisTongjiCount >= 1)
	{
		g->DrawImage(&imagezp0, m_dongtaifenxi[0].rZP.left, m_dongtaifenxi[0].rZP.top, m_dongtaifenxi[0].rZP.Width(), m_dongtaifenxi[0].rZP.Height());
		g->DrawImage(&imagebx0, m_dongtaifenxi[0].rBX.left, m_dongtaifenxi[0].rBX.top, m_dongtaifenxi[0].rBX.Width(), m_dongtaifenxi[0].rBX.Height());
	}
	if (m_iDisTongjiCount >= 2)
	{
		g->DrawImage(&imagezp1, m_dongtaifenxi[1].rZP.left, m_dongtaifenxi[1].rZP.top, m_dongtaifenxi[1].rZP.Width(), m_dongtaifenxi[1].rZP.Height());
		g->DrawImage(&imagebx1, m_dongtaifenxi[1].rBX.left, m_dongtaifenxi[1].rBX.top, m_dongtaifenxi[1].rBX.Width(), m_dongtaifenxi[1].rBX.Height());
	}
	if (m_iDisTongjiCount >= 3)
	{
		g->DrawImage(&imagezp2, m_dongtaifenxi[2].rZP.left, m_dongtaifenxi[2].rZP.top, m_dongtaifenxi[2].rZP.Width(), m_dongtaifenxi[2].rZP.Height());
		g->DrawImage(&imagebx2, m_dongtaifenxi[2].rBX.left, m_dongtaifenxi[2].rBX.top, m_dongtaifenxi[2].rBX.Width(), m_dongtaifenxi[2].rBX.Height());
	}
	if (m_iDisTongjiCount >= 4)
	{
		g->DrawImage(&imagezp3, m_dongtaifenxi[3].rZP.left, m_dongtaifenxi[3].rZP.top, m_dongtaifenxi[3].rZP.Width(), m_dongtaifenxi[3].rZP.Height());
		g->DrawImage(&imagebx3, m_dongtaifenxi[3].rBX.left, m_dongtaifenxi[3].rBX.top, m_dongtaifenxi[3].rBX.Width(), m_dongtaifenxi[3].rBX.Height());
	}
	if (m_iDisTongjiCount >= 5)
	{
		g->DrawImage(&imagezp4, m_dongtaifenxi[4].rZP.left, m_dongtaifenxi[4].rZP.top, m_dongtaifenxi[4].rZP.Width(), m_dongtaifenxi[4].rZP.Height());
		g->DrawImage(&imagebx4, m_dongtaifenxi[4].rBX.left, m_dongtaifenxi[4].rBX.top, m_dongtaifenxi[4].rBX.Width(), m_dongtaifenxi[4].rBX.Height());
	}

	// 	m_dongtaifenxi[0].dbSim = 97.362;
	// 	m_dongtaifenxi[1].dbSim = 94.023233;
	// 	m_dongtaifenxi[2].dbSim = 91.28743;
	// 	m_dongtaifenxi[3].dbSim = 95.9213;
	// 	m_dongtaifenxi[4].dbSim = 96.2748;
	// 	m_dongtaifenxi[0].strNum = L"ABCDE";
	// 	m_dongtaifenxi[0].strPos = L"紫光智能东侧北边";
	// 	m_dongtaifenxi[0].strTime = L"2017-08-07";
	// 	m_dongtaifenxi[0].iCount = 54;

	Gdiplus::Font font(L"方正细倩简体", m_rClient.Width() * 9 / 1000, FontStyleRegular, UnitPixel);
	SolidBrush percentBrush(Color(255, 212, 214, 222));
	StringFormat stringformat;
	stringformat.SetAlignment(StringAlignmentCenter);
	stringformat.SetLineAlignment(StringAlignmentCenter);

	for (int i = 0; i < m_iDisTongjiCount; i++)
	{
		//if (i == m_iDisTongjiCount)
		//	break;
		CString strSim, strCount;
		strSim.Format(L"%.1f", m_dongtaifenxi[i].dbSim);
		strCount.Format(L"%d", m_dongtaifenxi[i].iCount);
		g->DrawString(strSim.AllocSysString(), -1, &font, RectF(m_dongtaifenxi[i].rSim.left, m_dongtaifenxi[i].rSim.top, m_dongtaifenxi[i].rSim.Width(), m_dongtaifenxi[i].rSim.Height()), &stringformat, &percentBrush);
		g->DrawString(m_dongtaifenxi[i].strNum.AllocSysString(), -1, &font, RectF(m_dongtaifenxi[i].rNum.left, m_dongtaifenxi[i].rNum.top, m_dongtaifenxi[i].rNum.Width(), m_dongtaifenxi[i].rNum.Height()), &stringformat, &percentBrush);
		g->DrawString(strCount.AllocSysString(), -1, &font, RectF(m_dongtaifenxi[i].rCount.left, m_dongtaifenxi[i].rCount.top, m_dongtaifenxi[i].rCount.Width(), m_dongtaifenxi[i].rCount.Height()), &stringformat, &percentBrush);
		g->DrawString(m_dongtaifenxi[i].strPos.AllocSysString(), -1, &font, RectF(m_dongtaifenxi[i].rPos.left, m_dongtaifenxi[i].rPos.top, m_dongtaifenxi[i].rPos.Width(), m_dongtaifenxi[i].rPos.Height()), &stringformat, &percentBrush);
		g->DrawString(m_dongtaifenxi[i].strTime.AllocSysString(), -1, &font, RectF(m_dongtaifenxi[i].rTime.left, m_dongtaifenxi[i].rTime.top, m_dongtaifenxi[i].rTime.Width(), m_dongtaifenxi[i].rTime.Height()), &stringformat, &percentBrush);
		g->DrawImage(&imageXQ, m_dongtaifenxi[i].rOperate.left, m_dongtaifenxi[i].rOperate.top, m_dongtaifenxi[i].rOperate.Width(), m_dongtaifenxi[i].rOperate.Height());
	}
}

void CSSJKDlg::DisHeimingdan(Graphics *g)
{
	CFileStatus fs;
	BOOL isexist = CFile::GetStatus(L"data/heimingdanzp_5.jpg", fs);
	if (isexist)
	{
		DeleteFile(L"data/heimingdanzp_4.jpg");
		MoveFile(L"data/heimingdanzp_3.jpg", L"data/heimingdanzp_4.jpg");
		MoveFile(L"data/heimingdanzp_2.jpg", L"data/heimingdanzp_3.jpg");
		MoveFile(L"data/heimingdanzp_1.jpg", L"data/heimingdanzp_2.jpg");
		MoveFile(L"data/heimingdanzp_0.jpg", L"data/heimingdanzp_1.jpg");
		MoveFile(L"data/heimingdanzp_5.jpg", L"data/heimingdanzp_0.jpg");

		DeleteFile(L"data/heimingdanbx_4.jpg");
		MoveFile(L"data/heimingdanbx_3.jpg", L"data/heimingdanbx_4.jpg");
		MoveFile(L"data/heimingdanbx_2.jpg", L"data/heimingdanbx_3.jpg");
		MoveFile(L"data/heimingdanbx_1.jpg", L"data/heimingdanbx_2.jpg");
		MoveFile(L"data/heimingdanbx_0.jpg", L"data/heimingdanbx_1.jpg");
		MoveFile(L"data/heimingdanbx_5.jpg", L"data/heimingdanbx_0.jpg");
	}

	Image imagezp0(L"data/heimingdanzp_0.jpg");
	Image imagezp1(L"data/heimingdanzp_1.jpg");
	Image imagezp2(L"data/heimingdanzp_2.jpg");
	Image imagezp3(L"data/heimingdanzp_3.jpg");
	Image imagezp4(L"data/heimingdanzp_4.jpg");
	Image imagebx0(L"data/heimingdanbx_0.jpg");
	Image imagebx1(L"data/heimingdanbx_1.jpg");
	Image imagebx2(L"data/heimingdanbx_2.jpg");
	Image imagebx3(L"data/heimingdanbx_3.jpg");
	Image imagebx4(L"data/heimingdanbx_4.jpg");
	Image imageXQ(L"img/ssjk_xiangqing.png");

	if (m_iDisHeimingdanCount >= 1)
	{
		g->DrawImage(&imagezp0, m_heimingdan[0].rZP.left, m_heimingdan[0].rZP.top, m_heimingdan[0].rZP.Width(), m_heimingdan[0].rZP.Height());
		g->DrawImage(&imagebx0, m_heimingdan[0].rBX.left, m_heimingdan[0].rBX.top, m_heimingdan[0].rBX.Width(), m_heimingdan[0].rBX.Height());
	}
	if (m_iDisHeimingdanCount >= 2)
	{
		g->DrawImage(&imagezp1, m_heimingdan[1].rZP.left, m_heimingdan[1].rZP.top, m_heimingdan[1].rZP.Width(), m_heimingdan[1].rZP.Height());
		g->DrawImage(&imagebx1, m_heimingdan[1].rBX.left, m_heimingdan[1].rBX.top, m_heimingdan[1].rBX.Width(), m_heimingdan[1].rBX.Height());
	}
	if (m_iDisHeimingdanCount >= 3)
	{
		g->DrawImage(&imagezp2, m_heimingdan[2].rZP.left, m_heimingdan[2].rZP.top, m_heimingdan[2].rZP.Width(), m_heimingdan[2].rZP.Height());
		g->DrawImage(&imagebx2, m_heimingdan[2].rBX.left, m_heimingdan[2].rBX.top, m_heimingdan[2].rBX.Width(), m_heimingdan[2].rBX.Height());
	}
	if (m_iDisHeimingdanCount >= 4)
	{
		g->DrawImage(&imagezp3, m_heimingdan[3].rZP.left, m_heimingdan[3].rZP.top, m_heimingdan[3].rZP.Width(), m_heimingdan[3].rZP.Height());
		g->DrawImage(&imagebx3, m_heimingdan[3].rBX.left, m_heimingdan[3].rBX.top, m_heimingdan[3].rBX.Width(), m_heimingdan[3].rBX.Height());
	}
	if (m_iDisHeimingdanCount >= 5)
	{
		g->DrawImage(&imagezp4, m_heimingdan[4].rZP.left, m_heimingdan[4].rZP.top, m_heimingdan[4].rZP.Width(), m_heimingdan[4].rZP.Height());
		g->DrawImage(&imagebx4, m_heimingdan[4].rBX.left, m_heimingdan[4].rBX.top, m_heimingdan[4].rBX.Width(), m_heimingdan[4].rBX.Height());
	}
	// 
	// 	m_heimingdan[0].dbSim = 97.362;
	// 	m_heimingdan[1].dbSim = 94.023233;
	// 	m_heimingdan[2].dbSim = 91.28743;
	// 	m_heimingdan[3].dbSim = 95.9213;
	// 	m_heimingdan[4].dbSim = 96.2748;
	// 	m_heimingdan[0].strNum = L"D8chjaFG";
	// 	int i = 0;
	// 	m_heimingdan[0].strPos = L"紫光东边";
	// 	m_heimingdan[0].strTime = L"2017-08-11";
	// 	m_heimingdan[0].iCount = 403;


	Gdiplus::Font font(L"方正细倩简体", m_rClient.Width() * 9 / 1000, FontStyleRegular, UnitPixel);
	SolidBrush percentBrush(Color(255, 212, 214, 222));
	StringFormat stringformat;
	stringformat.SetAlignment(StringAlignmentCenter);
	stringformat.SetLineAlignment(StringAlignmentCenter);
	for (int i = 0; i < m_iDisHeimingdanCount; i++)
	{
		//if (i == m_iDisHeimingdanCount)
		//	break;
		CString strSim, strCount;
		strSim.Format(L"%.1f", m_heimingdan[i].dbSim);
		strCount.Format(L"%d", m_heimingdan[i].iCount);
		g->DrawString(strSim.AllocSysString(), -1, &font, RectF(m_heimingdan[i].rSim.left, m_heimingdan[i].rSim.top, m_heimingdan[i].rSim.Width(), m_heimingdan[i].rSim.Height()), &stringformat, &percentBrush);
		g->DrawString(m_heimingdan[i].strNum.AllocSysString(), -1, &font, RectF(m_heimingdan[i].rNum.left, m_heimingdan[i].rNum.top, m_heimingdan[i].rNum.Width(), m_heimingdan[i].rNum.Height()), &stringformat, &percentBrush);
		g->DrawString(strCount.AllocSysString(), -1, &font, RectF(m_heimingdan[i].rCount.left, m_heimingdan[i].rCount.top, m_heimingdan[i].rCount.Width(), m_heimingdan[i].rCount.Height()), &stringformat, &percentBrush);
		g->DrawString(m_heimingdan[i].strPos.AllocSysString(), -1, &font, RectF(m_heimingdan[i].rPos.left, m_heimingdan[i].rPos.top, m_heimingdan[i].rPos.Width(), m_heimingdan[i].rPos.Height()), &stringformat, &percentBrush);
		g->DrawString(m_heimingdan[i].strTime.AllocSysString(), -1, &font, RectF(m_heimingdan[i].rTime.left, m_heimingdan[i].rTime.top, m_heimingdan[i].rTime.Width(), m_heimingdan[i].rTime.Height()), &stringformat, &percentBrush);
		g->DrawImage(&imageXQ, m_heimingdan[i].rOperate.left, m_heimingdan[i].rOperate.top, m_heimingdan[i].rOperate.Width(), m_heimingdan[i].rOperate.Height());
	}
}

void CSSJKDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// 	CFile file;
	// 	file.Open(L"data/realtimesnap_10.jpg", CFile::modeCreate | CFile::modeWrite);
	// 	char jpgchar[36];
	// 	memset(jpgchar, 0, 36);
	// 	file.Write(jpgchar, 36);
	// 	file.Close();
	// 
	// 	SetSnapArea();

	//m_rCameraVideoClose

	if (m_rCameraVideoClose.PtInRect(point))
	{
		if (m_bIsPlaying)
		{
			StopPlay();
		}
	}

	if (m_rCameraVideo.PtInRect(point))
	{
		if (!m_bIsPlaying)
		{
			StartPlay();
		}
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CSSJKDlg::SetNumArea(int iZP, int iBH, int iDH, int iDC)
{
	m_iDrawZhuapailiang = iZP;
	m_iDrawBendiHei = iBH;
	m_iDrawDongtaiHei = iDH;
	m_iDrawDongtaiCun = iDC;
	InvalidateRect(m_rNum, false);
}

void CSSJKDlg::SetSnapArea()
{
	m_iSnapImageBegin = (m_iSnapImageBegin + 9) % 10;
	InvalidateRect(m_rSnap, false);
}

//////////////////////

CString CSSJKDlg::GetiniPath()
{
	CString strIp;
	TCHAR pFileName[MAX_PATH];
	int nPos = GetCurrentDirectory(MAX_PATH, pFileName);

	CString csFullPath(pFileName);
	csFullPath += DBINI_PATH;

	return csFullPath;
}

BOOL CSSJKDlg::InitParameter()
{
	m_nTongjiIndex = 0;
	m_nComboxIndex = 0;
	m_nHeimingdanIndex = 0;

	m_iDrawTodayZhuapai = 0;
	m_iDrawTodayAlarm = 0;

	m_iDisHeimingdanCount = 0;
	m_iDisTongjiCount = 0;
	m_iSnapImageBegin = 0;

	m_iDrawZhuapailiang = 0;
	m_iDrawBendiHei = 0;
	m_iDrawDongtaiHei = 0;
	m_iDrawDongtaiCun = 0;

	m_nTongjiIndexMax = 0;
	m_nHeimingdanIndexMax = 0;

	CString csFullPath = GetiniPath();
	GetPrivateProfileString(_T("SqlConfiguration"), _T("Img_ServerAdd"), _T(""), m_csImgIP.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);
	return TRUE;
}

BOOL CSSJKDlg::GetMaxCurrent(CString &csMaxCurrnet, CString &csStartTime, CString &csEndTime)
{
	try
	{
		CString csSql;
		_RecordsetPtr pRecordset;
		csSql = (_T("SELECT count(1) as num FROM cycleinfo"));
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		if (!pRecordset->adoEOF)
		{
			USES_CONVERSION;
			CString csNum = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("num")->Value;
			if (atoi(T2A(csNum)) == 0)
			{
				return FALSE;
			}
		}
		csSql = (_T("SELECT MAX(currentcycle) as maxcurr FROM cycleinfo"));
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		if (!pRecordset->adoEOF)
		{
			csMaxCurrnet = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("maxcurr")->Value;

			csSql.Format(_T("SELECT * FROM cycleinfo where currentcycle = '%s'"), csMaxCurrnet);
			pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
			if (!pRecordset->adoEOF)
			{
				csStartTime = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("startdate")->Value;
				csEndTime = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("enddate")->Value;
				csEndTime += " 23:59:59";
			}
			return TRUE;
		}
	}
	catch (CException* e)
	{
	}
	return FALSE;
}

////////////////////////////////抓拍量框

//获得表总量 或者时间内的总数
int CSSJKDlg::GetDbTableMaxSum(CString csTable, CString csStartTime, CString csEndTime)
{
	try
	{
		CString csSql;
		_RecordsetPtr pRecordset;
		csEndTime += " 23:59:59";

		if (csStartTime.IsEmpty())
			csSql.Format(_T("select count(*) from %s"), csTable);
		else
			csSql.Format(_T("select count(*) from %s where alarmtime >=  '%s' and  alarmtime <=  '%s'"), csTable, csStartTime, csEndTime);

		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		if (!pRecordset->adoEOF)
		{
			CString csCameraSum = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("count(*)")->Value;
			USES_CONVERSION;
			return atoi(T2A(csCameraSum));
		}
	}
	catch (CException* e)
	{
	}
	return -1;
}
//设置抓拍报警
void CSSJKDlg::SetWarningNum()
{

	CString  csStartTime, csEndTime, csMaxcurr;
	if (GetMaxCurrent(csMaxcurr, csStartTime, csEndTime))
	{
		int nSnapshot = GetDbTableMaxSum(_T("snapshot"), csStartTime, csEndTime);
		int nBlack = GetDbTableMaxSum(_T("blacklist"));
		int nDynamic = GetDbTableMaxSum(_T("dynamic"));
		int nTemp = GetDbTableMaxSum(_T("temporary"));

		SetNumArea(nSnapshot, nBlack, nDynamic, nTemp);
	}
}

////////////////////////////////相机今日部分框
//获得相机属性
//combox 切换函数
void CSSJKDlg::OnCbnSelchangeComboCamera()
{
	int  nCameraIndex = m_wndCameraCom.GetCurSel();
	if (nCameraIndex != m_nComboxIndex)
	{
		StopPlay();
		DoLogout();
		if (DoLogin(g_CameraArrInfo[nCameraIndex].csCameraIp))
		{
			StartPlay();
		}
		m_nComboxIndex = nCameraIndex;
		SetCameraNum(nCameraIndex);
	}
}
//过去数据库内相机信息
int CSSJKDlg::GetCameraToCombox(CComboBox &CameraCombox)
{
	try
	{
		m_nCameraSum = GetDbTableMaxSum(_T("camera"));
		if (m_nCameraSum == 0)
			return 0;

		CString csSql, csTemp;
		_RecordsetPtr pRecordset;
		int nCameraIndex = 0;
		g_CameraArrInfo = new struct CameraInfo[m_nCameraSum];

		csSql = "select * from camera";
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		while (!pRecordset->adoEOF)
		{
			USES_CONVERSION;
			g_CameraArrInfo[nCameraIndex].csCameraId = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("cameraid")->Value;
			g_CameraArrInfo[nCameraIndex].csCameraIp = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("ip")->Value;
			g_CameraArrInfo[nCameraIndex].csCameraAddr = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("addr")->Value;
			g_CameraArrInfo[nCameraIndex].csCameraRtsp = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("rtsp")->Value;
			g_CameraArrInfo[nCameraIndex].nCameraState = atoi(T2A((TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("camera_state")->Value));
			g_CameraArrInfo[nCameraIndex].nCameraType = atoi(T2A((TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("Camera_type")->Value));

			m_wndCameraCom.AddString(g_CameraArrInfo[nCameraIndex].csCameraAddr);
			nCameraIndex++;

			pRecordset->MoveNext();
		}

		if (nCameraIndex != 0)
		{
			m_wndCameraCom.SetCurSel(0);
			if (DoLogin(g_CameraArrInfo[0].csCameraIp))
				StartPlay();
		}

		return nCameraIndex;
	}
	catch (CException* e)
	{

	}
	return 0;
}

//获取最大周期相机的抓拍总量
BOOL CSSJKDlg::GetSnapshotInfoCameraSum(CString csCameraID, CString &csMaxNum)
{
	try
	{
		CString csSql, csStartTime, csEndTime, csMaxcurr;
		_RecordsetPtr pRecordset;
		if (GetMaxCurrent(csMaxcurr, csStartTime, csEndTime))
		{
			csSql.Format(_T("select  count(*) from  snapshot  where  cameraid = '%s' and alarmtime >=  '%s' and  alarmtime <=  '%s';"), csCameraID, csStartTime, csEndTime);
			pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
			if (!pRecordset->adoEOF)
			{
				USES_CONVERSION;
				csMaxNum = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("count(*)")->Value;
				return TRUE;
			}
		}
	}
	catch (CException* e)
	{
	}

	return FALSE;
}
//获取分库warning个数
BOOL CSSJKDlg::GetCameraWarningSum(CString csTable, CString csCameraID, CString &csMaxNum)
{
	try
	{
		CString csSql, csStartTime, csEndTime, csMaxcurr;
		_RecordsetPtr pRecordset;
		csSql.Format(_T("select  count(*) from  %s  where  cameraid = '%s';"), csTable, csCameraID);
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		if (!pRecordset->adoEOF)
		{
			USES_CONVERSION;
			csMaxNum = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("count(*)")->Value;
			return TRUE;
		}
	}
	catch (CException* e)
	{
	}

	return FALSE;
}

//设置相机相关数据
void CSSJKDlg::SetCameraNum(int nCameraIndex)
{
	CString csMaxSnapNum, csMaxBackNum, csMaxDycNum;
	GetSnapshotInfoCameraSum(g_CameraArrInfo[nCameraIndex].csCameraId, csMaxSnapNum);
	GetCameraWarningSum(_T("blacklist"), g_CameraArrInfo[nCameraIndex].csCameraId, csMaxBackNum);
	GetCameraWarningSum(_T("dynamic"), g_CameraArrInfo[nCameraIndex].csCameraId, csMaxDycNum);
	USES_CONVERSION;
	m_iDrawTodayZhuapai = atoi(T2A(csMaxSnapNum));
	m_iDrawTodayAlarm = atoi(T2A(csMaxBackNum)) + atoi(T2A(csMaxDycNum));
	m_csCameraStatus = g_CameraArrInfo[nCameraIndex].nCameraState == 1 ? _T("正常") : _T("断开");
	m_csCameraPos = g_CameraArrInfo[nCameraIndex].csCameraAddr;

	InvalidateRect(m_rToday, false);
}


////////////////////////////////动态分析部分


BOOL CSSJKDlg::GetTempsimilarity(CString &csSimilarity, CString csIdcard, CString csTime)
{
	try
	{
		CString csSql;
		_RecordsetPtr pRecordset;
		csSql.Format(_T("select * from temporary WHERE id_card = '%s' and  alarmtime = '%s' ;"), csIdcard, csTime);
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		if (!pRecordset->adoEOF)
		{
			csSimilarity = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("similarity")->Value;
			return TRUE;
		}
	}
	catch (CException* e)
	{
	}
	return FALSE;
}
BOOL CSSJKDlg::GetTempinfo(int nIndex)
{
	try
	{
		CString csSql, csStartTime, csEndTime, csMaxcurr, csTemp;
		_RecordsetPtr pRecordset;
		//if (GetMaxCurrent(csMaxcurr, csStartTime, csEndTime))
		{
			USES_CONVERSION;
			//	csSql.Format(_T("select *,MAX(alarmtime),count(*) as num  from temporary WHERE cyclnum = '%s' group by id_card order by num desc limit %d,1;"), csMaxcurr, nIndex);
			//csSql.Format(_T("select *,MAX(alarmtime),count(*) as num  from temporary where similarity != '0' group by id_card order by num desc limit %d,1;"), nIndex);
			csSql.Format(_T("select *,MAX(alarmtime) as Time,count(*) as num  from temporary  group by id_card order by num desc limit %d,1;"), nIndex);
			pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
			if (!pRecordset->adoEOF)
			{
				m_dongtaifenxi[nIndex].strNum = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("id_card")->Value;
				//	m_dongtaifenxi[nIndex].dbSim = atof(T2A((TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("similarity")->Value));
				m_dongtaifenxi[nIndex].strTime = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("Time")->Value;
				m_dongtaifenxi[nIndex].iCount = atoi(T2A((TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("num")->Value));

				if (GetTempsimilarity(csTemp, m_dongtaifenxi[nIndex].strNum, m_dongtaifenxi[nIndex].strTime))
					m_dongtaifenxi[nIndex].dbSim = atof(T2A(csTemp));
				for (int i = 0; i < m_nCameraSum; i++)
				{
					if ((TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("cameraid")->Value == g_CameraArrInfo[i].csCameraId)
					{
						m_dongtaifenxi[nIndex].strPos = g_CameraArrInfo[i].csCameraAddr;
						break;
					}
				}

				GetIdcardImg(TONG_JI,
					(TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("id_card")->Value,
					(TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("snapshotid")->Value, nIndex);
				return TRUE;
			}
		}
	}
	catch (CException* e)
	{
	}

	return FALSE;
}

BOOL CSSJKDlg::GetIdcardImg(CString csImgName, CString csIdcard, CString csSnapshotid, int nIndex)
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
			if (m_cTools.DownloadImageFromFileServer(m_csImgIP, IMG_PORT, csImgPath, csTemp) != 1)
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
			if (m_cTools.DownloadImageFromFileServer(m_csImgIP, IMG_PORT, csImgPath, csTemp) != 1)
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

BOOL CSSJKDlg::SetTongjiInfo(int nIndex)
{

	if (GetTempinfo(nIndex))
	{
		m_iDisTongjiCount = nIndex + 1;

		InvalidateRect(m_rTongji, false);
		Sleep(500);
		return TRUE;
	}
	return FALSE;
}


//////////////////////////////黑名单部分
BOOL CSSJKDlg::GetHeimingdan(int nIndex)
{
	try
	{
		CString csSql, csStartTime, csEndTime, csMaxcurr;
		_RecordsetPtr pRecordset;
		//	if (GetMaxCurrent(csMaxcurr, csStartTime, csEndTime))
		{
			USES_CONVERSION;
			//csSql.Format(_T("SELECT * FROM(select *,  count(*) as num  from blacklist WHERE cyclnum = %s group by id_card union all select *, count(*) as num from dynamic WHERE cyclnum = %s group by id_card) as ALLTables  ORDER BY num desc limit %d, 1;"), csMaxcurr, csMaxcurr, nIndex);
			csSql.Format(_T("SELECT * FROM(select *,  count(*) as num  from blacklist  group by id_card union all select *, count(*) as num from dynamic  group by id_card) as ALLTables  ORDER BY num desc limit %d, 1;"), nIndex);
			pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
			if (!pRecordset->adoEOF)
			{
				CString csIdcard = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("id_card")->Value;
				m_heimingdan[nIndex].dbSim = atof(T2A((TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("similarity")->Value));
				for (int i = 0; i < m_nCameraSum; i++)
				{
					if ((TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("cameraid")->Value == g_CameraArrInfo[i].csCameraId)
					{
						m_heimingdan[nIndex].strPos = g_CameraArrInfo[i].csCameraAddr;
						break;
					}
				}
				m_heimingdan[nIndex].strTime = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("alarmtime")->Value;
				m_heimingdan[nIndex].iCount = atoi(T2A((TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("num")->Value));
				if (IsBlacklist(csIdcard))
					m_heimingdan[nIndex].strNum = _T("黑名单");
				else
					m_heimingdan[nIndex].strNum = _T("动态黑名单");

				GetIdcardImg(HEI_MING_DAN,
					(TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("id_card")->Value,
					(TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("snapshotid")->Value, nIndex);
				return TRUE;
			}
		}
	}
	catch (CException* e)
	{
	}

	return FALSE;
}

BOOL CSSJKDlg::IsBlacklist(CString csIdcard)
{
	try
	{
		CString csSql, csStartTime, csEndTime, csMaxcurr;
		_RecordsetPtr pRecordset;
		csSql.Format(_T("SELECT * FROM blacklist WHERE id_card = '%s'"), csIdcard);
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		if (!pRecordset->adoEOF)
		{
			return TRUE;
		}
	}
	catch (CException* e)
	{
	}

	return FALSE;
}

BOOL CSSJKDlg::SetHeimingdan(int nIndex)
{
	if (GetHeimingdan(nIndex))
	{
		m_iDisHeimingdanCount = nIndex + 1;
		InvalidateRect(m_rAlarm, false);
		Sleep(500);
		return TRUE;
	}
	return FALSE;
}

///////////////////////////////实时抓拍 显示对比成功与失败 的

//BOOL CSSJKDlg::GetSSXS()
//{
//	try
//	{
//		CString csSql, csTemp;
//		_RecordsetPtr pRecordset;
//		//	csSql = (_T("select * FROM `snapshot` WHERE process_status = '3'  OR process_status = '16' order by alarmtime desc LIMIT 1;"));
//		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
//		if(!pRecordset->adoEOF)
//		{
//			CString csImgPath = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("FacePhotoName")->Value;
//			CString csSnapid = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("Snapshotid")->Value;
//			
//			if (csSnapid != m_csSSXSID)
//			{
//				m_csSSXSID = csSnapid;
//				csTemp = _T("data/realtimesnap_10.jpg");
//				int nStart = csImgPath.Find(_T("/Data"));
//				csImgPath = csImgPath.Mid(nStart, csImgPath.GetLength() - nStart);
//
//				if (m_cTools.DownloadImageFromFileServer(m_csImgIP, 10000, csImgPath, csTemp) == 1)
//					return TRUE;
//			}
//		}
//	}
//	catch (CException* e)
//	{
//	}
//	return FALSE;
//}
BOOL CSSJKDlg::GetSSXS()
{
	try
	{
		CString csSql, csTemp;
		_RecordsetPtr pRecordset;
		for (int i = 0; i < g_arImgData.GetSize(); i++)
		{
			csSql.Format(_T("SELECT * FROM `snapshot` WHERE Snapshotid = '%s'"), g_arImgData.GetAt(i));
			pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
			if (!pRecordset->adoEOF)
			{
				CString csImgPath = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("FacePhotoName")->Value;
				csTemp .Format( _T("data/realtimesnap_%d.jpg"),i);
				int nStart = csImgPath.Find(_T("/Data"));
				csImgPath = csImgPath.Mid(nStart, csImgPath.GetLength() - nStart);
				if (m_cTools.DownloadImageFromFileServer(m_csImgIP, 10000, csImgPath, csTemp) != 1)
					SetNullJpg(csTemp);
				else
					SetSnapArea();
			}
		}
	}
	catch (CException* e)
	{
		return FALSE;
	}
	return TRUE;
}
void CSSJKDlg::SetSSXS()
{
	try
	{
		CString csSql, csTemp;
		_RecordsetPtr pRecordset;
		csSql.Format(_T("select * FROM `snapshot` WHERE process_status != '13' and  process_status != '0' order by alarmtime desc LIMIT 0,10"));
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		while (!pRecordset->adoEOF)
		{
			BOOL isBeing = FALSE;
			CString csSnapid = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("Snapshotid")->Value;

			for (int i = 0; i < g_arImgData.GetSize(); i++)
			{
				if (csSnapid == g_arImgData.GetAt(i))
				{
					isBeing = TRUE;
					break;
				}
			}

			if (!isBeing)
			{
				if (g_arImgData.GetSize() > 10)
				{
					g_arImgData.RemoveAt(g_arImgData.GetSize() - 1);
				}
				g_arImgData.InsertAt(0, csSnapid);
			}

			pRecordset->MoveNext();
		}

		if (GetSSXS())
		{
			//SetSnapArea();
		}
	}
	catch (CException* e)
	{
	}
}
////////////////////////////////////////////////
void CSSJKDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)//统计
	{
		KillTimer(1);
		m_nTongjiIndex = 0;
		for (int i = 0; i < 5; i++)
		{
			if (SetTongjiInfo(m_nTongjiIndex))
				m_nTongjiIndex++;
		}
		SetTimer(1, 10000, NULL);
	}
	else if (nIDEvent == 2)//黑名单
	{
		KillTimer(2);
		m_nHeimingdanIndex = 0;
		for (int i = 0; i < 5; i++)
		{
			if (SetHeimingdan(m_nHeimingdanIndex))
				m_nHeimingdanIndex++;
		}
		SetTimer(2, 10000, NULL);
	}
	else if (nIDEvent == 3)//抓拍
	{
		KillTimer(3);
		SetWarningNum();
		SetTimer(3, 1000, NULL);
	}
	else if (nIDEvent == 4)//相机
	{
		KillTimer(4);
		int  nCameraIndex = m_wndCameraCom.GetCurSel();
		SetCameraNum(nCameraIndex);
		SetTimer(4, 1000, NULL);
	}
	else if (nIDEvent == 5)//实时
	{
		KillTimer(5);
		SetSSXS();
		SetTimer(5, 1000, NULL);
	}

	CDialogEx::OnTimer(nIDEvent);
}

BOOL CSSJKDlg::Start()
{
	BOOL bParameter = TRUE;
	BOOL bCamera = TRUE;
	BOOL bWhile = TRUE;
	while (bWhile)
	{

		if (bParameter)
		{
			if (InitParameter())
				bParameter = FALSE;
			else
				continue;
		}

		if (bCamera)
		{
			if (GetCameraToCombox(m_wndCameraCom) != 0)
				bCamera = FALSE;
			else
				continue;
		}
		g_arImgData.RemoveAll();

		SetNullJpgAll();
		SetTongjiInfo(0);
		SetHeimingdan(0);

		SetTimer(1, 10000, NULL);
		SetTimer(2, 10000, NULL);
		SetTimer(3, 1000, NULL);
		SetTimer(4, 1000, NULL);
		SetTimer(5, 1000, NULL);

		bWhile = FALSE;
	}


	return TRUE;

}
void CSSJKDlg::SetNullJpgAll()
{
	CString csTemp;
	for (int i = 0; i < 10; i++)
	{
		csTemp.Format(_T("data/realtimesnap_%d.jpg"), i);
		SetNullJpg(csTemp);
	}
}
void CSSJKDlg::SetNullJpg(CString csPath)
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
BOOL CSSJKDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN&&pMsg->wParam == VK_RETURN)
		return TRUE;
	if (pMsg->message == WM_KEYDOWN&&pMsg->wParam == VK_ESCAPE)
		return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CSSJKDlg::DoLogin(CString csIp)
{

	try
	{
		CString csSql;
		_RecordsetPtr pRecordset;
		NET_DVR_Init();

		GetDlgItem(IDC_STATIC_PLAY)->ShowWindow(SW_HIDE);
		InvalidateRect(m_rCameraVideo, false);

		NET_DVR_DEVICEINFO_V30 DeviceInfoTmp;
		memset(&DeviceInfoTmp, 0, sizeof(NET_DVR_DEVICEINFO_V30));
		USES_CONVERSION;
		char user[] = "admin";
		char pwd[] = "unisib1000";
		LONG lLoginID = NET_DVR_Login_V30(T2A(csIp), 8000, user, pwd, &DeviceInfoTmp);
		if (lLoginID == -1)
		{
			//csSql.Format(_T("UPDATE camera SET camera_state = '0' WHERE ip = '%s'"), csIp);
			//	pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
			MessageBox(L"登录失败");
			return FALSE;
		}

		//	csSql.Format(_T("UPDATE camera SET camera_state = '1' WHERE ip = '%s'"), csIp);
		//	pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);

		m_struDeviceInfo.lLoginID = lLoginID;
		m_struDeviceInfo.iDeviceChanNum = DeviceInfoTmp.byChanNum;
		m_struDeviceInfo.iIPChanNum = DeviceInfoTmp.byIPChanNum;
		m_struDeviceInfo.iStartChan = DeviceInfoTmp.byStartChan;
		m_struDeviceInfo.iIPStartChan = DeviceInfoTmp.byStartDChan;

	}
	catch (CException* e)
	{
		return FALSE;

	}
	return TRUE;
}


BOOL CSSJKDlg::StartPlay()
{
	NET_DVR_CLIENTINFO ClientInfo;
	ClientInfo.hPlayWnd = GetDlgItem(IDC_STATIC_PLAY)->m_hWnd;
	ClientInfo.lChannel = 1;
	ClientInfo.lLinkMode = 0;
	ClientInfo.sMultiCastIP = NULL;
	m_lPlayHandle = NET_DVR_RealPlay_V30(m_struDeviceInfo.lLoginID, &ClientInfo, NULL, NULL, TRUE);
	if (-1 == m_lPlayHandle)
	{
		DWORD err = NET_DVR_GetLastError();
		CString m_csErr;
		m_csErr.Format(_T("播放出错，错误代码%d"), err);
		return FALSE;
	}
	GetDlgItem(IDC_STATIC_PLAY)->ShowWindow(SW_SHOW);
	m_bIsPlaying = TRUE;
	return TRUE;
}

BOOL CSSJKDlg::StopPlay()
{
	if (m_lPlayHandle != -1)
	{
		NET_DVR_StopRealPlay(m_lPlayHandle);
		m_lPlayHandle = -1;
		m_bIsPlaying = FALSE;
		//InvalidateRect(m_rCameraVideo, false);
		return TRUE;
	}
	return FALSE;
}

BOOL CSSJKDlg::DoLogout()
{
	if (m_lPlayHandle != -1)
	{
		NET_DVR_Logout_V30(m_lPlayHandle);
		m_lPlayHandle = -1;
		m_bIsPlaying = FALSE;
		//	InvalidateRect(m_rCameraVideo, false);
		NET_DVR_Cleanup();
		return TRUE;
	}
	return FALSE;
}

HBRUSH CSSJKDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
