// AddServer.cpp : 实现文件
//

#include "stdafx.h"
#include "BatchAdd.h"
#include "AddServer.h"
#include "afxdialogex.h"


// CAddServer 对话框

IMPLEMENT_DYNAMIC(CAddServer, CDialog)

CAddServer::CAddServer(CWnd* pParent /*=NULL*/, int nType)
: CDialog(CAddServer::IDD, pParent)
, m_csIp(_T(""))
, m_csName(_T(""))
, m_csDescription(_T(""))
, m_csAddr(_T(""))
, m_csMemark(_T(""))
, m_csOnlyId(_T(""))
{
	m_nType = nType;
}

CAddServer::~CAddServer()
{
}

void CAddServer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_IP, m_csIp);
	DDX_Text(pDX, IDC_EDIT_NAME, m_csName);
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_csDescription);
	DDX_Text(pDX, IDC_EDIT_ADDR, m_csAddr);
	DDX_Text(pDX, IDC_EDIT_REMARK, m_csMemark);
	DDX_Text(pDX, IDC_EDIT_ONLYID, m_csOnlyId);
}


BEGIN_MESSAGE_MAP(CAddServer, CDialog)
	ON_BN_CLICKED(IDOK, &CAddServer::OnBnClickedOk)
END_MESSAGE_MAP()


// CAddServer 消息处理程序


void CAddServer::OnBnClickedOk()
{
	if (((CButton*)GetDlgItem(IDC_RADIO_IN))->GetCheck())
		m_nInOrOut = 0;
	else
		m_nInOrOut = 1;


	if (((CButton*)GetDlgItem(IDC_RADIO_STATIC))->GetCheck())
		m_nStatic = 0;
	else
		m_nStatic = 1;

	CDialog::OnOK();
}


BOOL CAddServer::OnInitDialog()
{
	CDialog::OnInitDialog();
	if (m_nType == 0)
	{
		m_bStatic = TRUE;
		m_bInorOut = TRUE;
		((CButton*)GetDlgItem(IDC_RADIO_IN))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO_STATIC))->SetCheck(TRUE);
	}
	else
	{
		if (m_bStatic)
			((CButton*)GetDlgItem(IDC_RADIO_STATIC))->SetCheck(TRUE);
		else
			((CButton*)GetDlgItem(IDC_RADIO_DYNAMIC))->SetCheck(TRUE);

		if (m_bInorOut)
			((CButton*)GetDlgItem(IDC_RADIO_IN))->SetCheck(TRUE);
		else
			((CButton*)GetDlgItem(IDC_RADIO_OUT))->SetCheck(TRUE);


	}


	UpdateData(FALSE);

	return TRUE;
}
