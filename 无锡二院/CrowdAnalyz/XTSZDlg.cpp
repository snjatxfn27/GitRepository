// XTSZDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CrowdAnalyz.h"
#include "XTSZDlg.h"
#include "afxdialogex.h"


// CXTSZDlg 对话框

IMPLEMENT_DYNAMIC(CXTSZDlg, CDialogEx)

CXTSZDlg::CXTSZDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CXTSZDlg::IDD, pParent)
, m_csJiankong(_T(""))
, m_csShijian(_T(""))
, m_csXiangsidu(_T(""))
{

}

CXTSZDlg::~CXTSZDlg()
{
}

void CXTSZDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_JIANKONG, m_csJiankong);
	DDX_Text(pDX, IDC_EDIT_SHIJIAN, m_csShijian);
	DDX_Text(pDX, IDC_EDIT_FAZHI, m_csXiangsidu);
}


BEGIN_MESSAGE_MAP(CXTSZDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CXTSZDlg 消息处理程序


void CXTSZDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	delete pMemDC;
	delete pBitmap;
}


BOOL CXTSZDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_rClient.SetRect(0, 0, 1920 * WIDTH / 1920, 830 * HEIGHT / 1080);


	m_rJkzq.SetRect(214 * m_rClient.Width() / 1920, 69 * m_rClient.Height() / 830, 326 * m_rClient.Width() / 1920, 88 * m_rClient.Height() / 830);
	m_rBbsj.SetRect(214 * m_rClient.Width() / 1920, 108 * m_rClient.Height() / 830, 326 * m_rClient.Width() / 1920, 127 * m_rClient.Height() / 830);
	m_rXsd.SetRect(214 * m_rClient.Width() / 1920, 147 * m_rClient.Height() / 830, 326 * m_rClient.Width() / 1920, 168 * m_rClient.Height() / 830);
	m_rBC.SetRect(371 * m_rClient.Width() / 1920, 66 * m_rClient.Height() / 830, 430 * m_rClient.Width() / 1920, 96 * m_rClient.Height() / 830);

	GetDlgItem(IDC_EDIT_JIANKONG)->MoveWindow(m_rJkzq.left, m_rJkzq.top, m_rJkzq.Width(), m_rJkzq.Height());
	GetDlgItem(IDC_EDIT_SHIJIAN)->MoveWindow(m_rBbsj.left, m_rBbsj.top, m_rBbsj.Width(), m_rBbsj.Height());
	GetDlgItem(IDC_EDIT_FAZHI)->MoveWindow(m_rXsd.left, m_rXsd.top, m_rXsd.Width(), m_rXsd.Height());
	GetDlgItem(IDC_BUTTON1)->MoveWindow(-1, -1, 0, 0);



	m_colorEditText = RGB(212, 214, 222);
	m_colorEditBK = RGB(21, 25, 60);
	m_pEditBkBrush = new CBrush(RGB(21, 25, 60));

	//双缓存
	CClientDC dc(this);
	pMemDC = new CDC;
	pBitmap = new CBitmap;
	pMemDC->CreateCompatibleDC(&dc);
	pBitmap->CreateCompatibleBitmap(&dc, m_rClient.Width(), m_rClient.Height());


	//////////////
	GetConfig();

	return TRUE;  // return TRUE unless you set the focus to a control
}


void CXTSZDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CBitmap * pOldBitmap = pMemDC->SelectObject(pBitmap);
	pMemDC->PatBlt(0, 0, m_rClient.Width(), m_rClient.Height(), PATCOPY);
	Graphics g(pMemDC->m_hDC);
	//画总背景
	Image img_bk(L"img/xtsz_bk.jpg");
	g.DrawImage(&img_bk, 0, 0, m_rClient.Width(), m_rClient.Height());

	Image img_bt(L"img/baocun-liang.jpg");
	g.DrawImage(&img_bt, m_rBC.left, m_rBC.top, m_rBC.Width(), m_rBC.Height());


	//显示缓存，清除HDC
	dc.BitBlt(0, 0, m_rClient.Width(), m_rClient.Height(), pMemDC, 0, 0, SRCCOPY);
	pMemDC->SelectObject(pOldBitmap);
	g.ReleaseHDC(pMemDC->m_hDC);
}

BOOL CXTSZDlg::GetConfig()
{
	try
	{
		CString csSql, csTemp;
		_RecordsetPtr pRecordset;
		m_csJiankong = _T("1");
		csSql = _T("SELECT * FROM parameterini WHERE Parameter = 'computer';");
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		if (!pRecordset->adoEOF)
		{
			m_csXiangsidu = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("CameraSimilarity")->Value;
			UpdateData(FALSE);
			return TRUE;
		}
	}
	catch (CException* e)
	{

	}
	return FALSE;
}


BOOL CXTSZDlg::SetConfig()
{
	try
	{
		UpdateData();
		CString csSql, csTemp;
		_RecordsetPtr pRecordset;
		csSql.Format(_T("UPDATE parameterini SET CameraSimilarity = '%s' WHERE  Parameter = 'computer'"), m_csXiangsidu);
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		return TRUE;
	}
	catch (CException* e)
	{

	}
	return FALSE;
}

HBRUSH CXTSZDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
BOOL CXTSZDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN&&pMsg->wParam == VK_RETURN)
		return TRUE;
	if (pMsg->message == WM_KEYDOWN&&pMsg->wParam == VK_ESCAPE)
		return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}

void CXTSZDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (m_rBC.PtInRect(point))
	{
		if (SetConfig())
			MessageBox(_T("修改成功"), _T("提示"));
		else
			MessageBox(_T("修改失败"), _T("提示"));
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}
