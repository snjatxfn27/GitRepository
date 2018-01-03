// BottomDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WinGUIClient.h"
#include "BottomDlg.h"
#include "afxdialogex.h"


// CBottomDlg 对话框

IMPLEMENT_DYNAMIC(CBottomDlg, CDialogEx)

CBottomDlg::CBottomDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBottomDlg::IDD, pParent)
{

}

CBottomDlg::~CBottomDlg()
{
}

void CBottomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBottomDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CBottomDlg 消息处理程序


void CBottomDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	HDC hDC = GetDC()->m_hDC;
	Graphics g(hDC);
	Image img_bk(L"img/bottom_bk.jpg");
	g.DrawImage(&img_bk, 0, 0, m_rClient.Width(), m_rClient.Height());
	g.ReleaseHDC(hDC);
	// 不为绘图消息调用 CDialogEx::OnPaint()
}
