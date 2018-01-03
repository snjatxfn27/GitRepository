// TopDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WinGUIClient.h"
#include "TopDlg.h"
#include "afxdialogex.h"
#include "WinGUIClientDlg.h"


// CTopDlg 对话框

IMPLEMENT_DYNAMIC(CTopDlg, CDialogEx)

CTopDlg::CTopDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTopDlg::IDD, pParent)
{

}

CTopDlg::~CTopDlg()
{
}

void CTopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTopDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CTopDlg 消息处理程序


BOOL CTopDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CTopDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	HDC hDC = GetDC()->m_hDC;
	Graphics g(hDC);
	Image img_bk(L"img/title_bk.jpg");
	g.DrawImage(&img_bk, 0, 0, m_rClient.Width(), m_rClient.Height());
	g.ReleaseHDC(hDC);
	// 不为绘图消息调用 CDialogEx::OnPaint()
}


void CTopDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rMin(
		m_rClient.Width() * 1567 / 1680,
		0,
		m_rClient.Width() * 1619 / 1680,
		m_rClient.Height() * 32 / 131
		);
	CRect rClose(
		m_rClient.Width() * 1620 / 1680,
		0,
		m_rClient.Width() * 1666 / 1680,
		m_rClient.Height() * 32 / 131
		);
	if (rMin.PtInRect(point))
	{
		CWinGUIClientDlg *parentDlg;
		parentDlg = (CWinGUIClientDlg *)GetParent();
		parentDlg->PostMessage(WM_SYSCOMMAND, SC_MINIMIZE);
	}
	else if (rClose.PtInRect(point))
	{
		CWinGUIClientDlg *parentDlg;
		parentDlg = (CWinGUIClientDlg *)GetParent();
		parentDlg->PostMessage(WM_CLOSE);
	}
	
	CDialogEx::OnLButtonDown(nFlags, point);
}
