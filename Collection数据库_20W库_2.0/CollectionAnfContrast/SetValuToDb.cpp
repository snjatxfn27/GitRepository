// SetValuToDb.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CollectionAnfContrast.h"
#include "SetValuToDb.h"
#include "afxdialogex.h"


// CSetValuToDb �Ի���

IMPLEMENT_DYNAMIC(CSetValuToDb, CDialog)

CSetValuToDb::CSetValuToDb(CWnd* pParent /*=NULL*/, int csSimilarity, int csSleeptime, int csBideTime, int csOnlineTime)
: CDialog(CSetValuToDb::IDD, pParent)
, m_CSSimilarity(_T(""))
, m_CSSleep(_T(""))
, m_CSBide(_T(""))
, m_CSOnline(_T(""))
{
	m_CSSimilarity.Format("%d", csSimilarity);
	m_CSSleep.Format("%d", csSleeptime);
	m_CSBide.Format("%d", csBideTime);
	m_CSOnline.Format("%d", csOnlineTime);
}

CSetValuToDb::~CSetValuToDb()
{
}

void CSetValuToDb::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SIMILARITY, m_CSSimilarity);
	DDX_Text(pDX, IDC_EDIT_SLEEP, m_CSSleep);
	DDX_Text(pDX, IDC_EDIT_BIDE, m_CSBide);
	DDX_Text(pDX, IDC_EDIT_ONLINE, m_CSOnline);
}


BEGIN_MESSAGE_MAP(CSetValuToDb, CDialog)
	ON_BN_CLICKED(IDOK, &CSetValuToDb::OnBnClickedOk)
END_MESSAGE_MAP()


// CSetValuToDb ��Ϣ�������


void CSetValuToDb::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	_ConnectionPtr  pConnect;

	CString strIp, csSql;
	TCHAR pFileName[MAX_PATH];
	int nPos = GetCurrentDirectory(MAX_PATH, pFileName);

	CString csFullPath(pFileName);
	csFullPath += "\\Parameter.ini";
	CString csSqlIP, csSqlDatabase;
	GetPrivateProfileString(("SqlConfiguration"), "Sql_ServerAdd", (""), csSqlIP.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);
	GetPrivateProfileString(("SqlConfiguration"), "Sql_Database", (""), csSqlDatabase.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);
	CString csSqlConnect;
	csSqlConnect.Format("Driver={MySQL ODBC 5.1 Driver};Server=%s;Database=%s;uid=root;pwd=cr;", csSqlIP, csSqlDatabase);

	try
	{
		pConnect.CreateInstance(__uuidof(Connection));

		_bstr_t strConnect = csSqlConnect;
		pConnect->Open(strConnect, "", "", adModeUnknown);
		csSql.Format("UPDATE ParameterIni SET CameraSimilarity = '%s' , SleepTime = '%s', BideTime = '%s',IsOnlineTime = '%s' where Parameter = 'computer' ",
			m_CSSimilarity, m_CSSleep, m_CSBide, m_CSOnline);
		pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);

		MessageBox("�޸ĳɹ�");
	}
	catch (_com_error e)
	{
		MessageBox("�޸�ʧ��");
	}

	CDialog::OnOK();
}


BOOL CSetValuToDb::OnInitDialog()
{
	CDialog::OnInitDialog();



	return TRUE;  // return TRUE unless you set the focus to a control
}
