// BottomDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WinGUIClient.h"
#include "BottomDlg.h"
#include "afxdialogex.h"


// CBottomDlg �Ի���

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


// CBottomDlg ��Ϣ�������


void CBottomDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
	HDC hDC = GetDC()->m_hDC;
	Graphics g(hDC);
	Image img_bk(L"img/bottom_bk.jpg");
	g.DrawImage(&img_bk, 0, 0, m_rClient.Width(), m_rClient.Height());
	g.ReleaseHDC(hDC);
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}
