// AddParentDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PickUpChild.h"
#include "AddParentDlg.h"
#include "afxdialogex.h"


// CAddParentDlg �Ի���

IMPLEMENT_DYNAMIC(CAddParentDlg, CDialogEx)

CAddParentDlg::CAddParentDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddParentDlg::IDD, pParent)
	, m_strChildName(_T(""))
	, m_strClassName(_T(""))
	, m_strParentName(_T(""))
	, m_strParentRelation(_T(""))
{

}

CAddParentDlg::~CAddParentDlg()
{
}

void CAddParentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CHILDNAME, m_strChildName);
	DDX_Text(pDX, IDC_EDIT_CLASSNAME, m_strClassName);
	DDX_Control(pDX, IDC_STATIC_PHOTO, m_static_photo);
	DDX_Text(pDX, IDC_EDIT_PARENTNAME, m_strParentName);
	DDX_Text(pDX, IDC_EDIT_PARENTRELATION, m_strParentRelation);
}


BEGIN_MESSAGE_MAP(CAddParentDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CHOOSEPHOTO, &CAddParentDlg::OnBnClickedButtonChoosephoto)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, &CAddParentDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAddParentDlg ��Ϣ�������


BOOL CAddParentDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_static_photo.ShowWindow(SW_HIDE);
	m_strCurSelPhotoPathName.Empty();
	InitChildData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

void CAddParentDlg::InitChildData()
{
	_ConnectionPtr conn;
	conn.CreateInstance(__uuidof(Connection));
	CString strConn;
	strConn.Format(_T("Driver={MySQL ODBC 5.1 Driver}; Server = %s; Database = %s;uid=%s;pwd=%s;"), theApp.m_strDBIP, theApp.m_strDBName, theApp.m_strDBUsername, theApp.m_strDBPassword);
	try
	{
		conn->Open(_bstr_t(strConn), "", "", adModeUnknown);
	}
	catch (_com_error e)
	{
		return;
	}
	_RecordsetPtr pSet;
	pSet.CreateInstance(__uuidof(Recordset));
	CString strSQL;
	strSQL.Format(L"%s%d", L"select child.name as childname,class.name as classname from child left join class on class.id=child.classid where child.id=",m_iChildID);
	pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	CString strChildName, strClassName;
	strChildName = getsqlstr("childname");
	strClassName = getsqlstr("classname");
	pSet->Close();
	conn->Close();
	m_strChildName = strChildName;
	m_strClassName = strClassName;
	UpdateData(false);
}

void CAddParentDlg::OnBnClickedButtonChoosephoto()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString filter;
	filter = L"jpg�ļ�(*.jpg)|*.jpg";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter);
	if (dlg.DoModal() == IDOK)
	{
		CString str;
		str = dlg.GetPathName();
		//		MessageBox(str);
		m_strCurSelPhotoPathName = str;
		Invalidate(false);
	}
}


void CAddParentDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
	if (m_strCurSelPhotoPathName.IsEmpty())
		return;
	HDC hDC = GetDC()->m_hDC;
	Graphics g(hDC);
	//���ҳ�ͷ��
	Image img(m_strCurSelPhotoPathName);
	CRect rPhoto;
	m_static_photo.GetWindowRect(&rPhoto);
	ScreenToClient(&rPhoto);
	g.DrawImage(&img, rPhoto.left, rPhoto.top, rPhoto.Width(), rPhoto.Height());

	g.ReleaseHDC(hDC);
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}


void CAddParentDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	m_strParentName.Trim();
	m_strParentRelation.Trim();
	if (m_strParentName.IsEmpty())
	{
		MessageBox(L"����д�ҳ�����", L"��ʾ", MB_ICONINFORMATION | MB_OK);
		return;
	}
	if (m_strParentRelation.IsEmpty())
	{
		MessageBox(L"����д�ҳ���ѧ����ϵ", L"��ʾ", MB_ICONINFORMATION | MB_OK);
		return;
	}
	if (m_strCurSelPhotoPathName.IsEmpty())
	{
		MessageBox(L"��ѡ��ҳ���Ƭ", L"��ʾ", MB_ICONINFORMATION | MB_OK);
		return;
	}
	_ConnectionPtr conn;
	conn.CreateInstance(__uuidof(Connection));
	CString strConn;
	strConn.Format(_T("Driver={MySQL ODBC 5.1 Driver}; Server = %s; Database = %s;uid=%s;pwd=%s;"), theApp.m_strDBIP, theApp.m_strDBName, theApp.m_strDBUsername, theApp.m_strDBPassword);
	try
	{
		conn->Open(_bstr_t(strConn), "", "", adModeUnknown);
	}
	catch (_com_error e)
	{
		return;
	}
	_RecordsetPtr pSet;
	pSet.CreateInstance(__uuidof(Recordset));
	CString strSQL;
	strSQL.Format(L"%s", L"select * from parent");
	pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	pSet->AddNew();
	pSet->put_Collect(_variant_t("name"), _variant_t(m_strParentName));
	pSet->put_Collect(_variant_t("relation"), _variant_t(m_strParentRelation));
	CString strChildID;
	strChildID.Format(L"%d", m_iChildID);
	pSet->put_Collect(_variant_t("childid"), _variant_t(strChildID));
	pSet->Update();
	pSet->Close();
	strSQL.Format(L"%s", L"select max(id) as maxid from parent");
	pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	int iCurID = getsqldouble("maxid");
	pSet->Close();
	conn->Close();
	CString strDesPathName;
	strDesPathName.Format(L"parent_photo/%d.jpg", iCurID);
	MoveFile(m_strCurSelPhotoPathName, strDesPathName);

	CDialogEx::OnOK();
}
