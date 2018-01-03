// RTAlarmDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WinGUIClient.h"
#include "RTAlarmDlg.h"
#include "afxdialogex.h"


// CRTAlarmDlg �Ի���

IMPLEMENT_DYNAMIC(CRTAlarmDlg, CDialogEx)

CRTAlarmDlg::CRTAlarmDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRTAlarmDlg::IDD, pParent)
{

}

CRTAlarmDlg::~CRTAlarmDlg()
{
}

void CRTAlarmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRTAlarmDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CRTAlarmDlg ��Ϣ�������


void CRTAlarmDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
	HDC hDC = GetDC()->m_hDC;
	Graphics g(hDC);
	Image img_bk(L"img/rtalarm_bk.jpg");
	g.DrawImage(&img_bk, 0, 0, m_rClient.Width(), m_rClient.Height());
	g.ReleaseHDC(hDC);
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}
