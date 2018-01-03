// RTAlarmInfoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WinGUIClient.h"
#include "RTAlarmInfoDlg.h"
#include "afxdialogex.h"


// CRTAlarmInfoDlg �Ի���

IMPLEMENT_DYNAMIC(CRTAlarmInfoDlg, CDialogEx)

CRTAlarmInfoDlg::CRTAlarmInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRTAlarmInfoDlg::IDD, pParent)
{

}

CRTAlarmInfoDlg::~CRTAlarmInfoDlg()
{
}

void CRTAlarmInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRTAlarmInfoDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CRTAlarmInfoDlg ��Ϣ�������


void CRTAlarmInfoDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
	HDC hDC = GetDC()->m_hDC;
	Graphics g(hDC);
	Image img_bk(L"img/rtalarminfo_bk.jpg");
	g.DrawImage(&img_bk, 0, 0, m_rClient.Width(), m_rClient.Height());
	g.ReleaseHDC(hDC);
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}
