// HistoryDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WinGUIClient.h"
#include "HistoryDlg.h"
#include "afxdialogex.h"


// CHistoryDlg 对话框

IMPLEMENT_DYNAMIC(CHistoryDlg, CDialogEx)

CHistoryDlg::CHistoryDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHistoryDlg::IDD, pParent)
{

}

CHistoryDlg::~CHistoryDlg()
{
}

void CHistoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHistoryDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CHistoryDlg 消息处理程序


void CHistoryDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	HDC hDC = GetDC()->m_hDC;
	Graphics g(hDC);
	Image img_bk(L"img/history_bk.jpg");
	g.DrawImage(&img_bk, 0, 0, m_rClient.Width(), m_rClient.Height());
	g.ReleaseHDC(hDC);
	// 不为绘图消息调用 CDialogEx::OnPaint()
}
