// AddChildDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PickUpChild.h"
#include "AddChildDlg.h"
#include "afxdialogex.h"


// CAddChildDlg �Ի���

IMPLEMENT_DYNAMIC(CAddChildDlg, CDialogEx)

CAddChildDlg::CAddChildDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddChildDlg::IDD, pParent)
	, m_strChildName(_T(""))
{

}

CAddChildDlg::~CAddChildDlg()
{
}

void CAddChildDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strChildName);
	DDX_Control(pDX, IDC_COMBO_CLASS, m_comboChildClass);
	DDX_Control(pDX, IDC_STATIC_PHOTO, m_static_photo);
}


BEGIN_MESSAGE_MAP(CAddChildDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CAddChildDlg::OnBnClickedButton1)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, &CAddChildDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAddChildDlg ��Ϣ�������


BOOL CAddChildDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_strCurSelPhotoPathName.Empty();
	m_static_photo.ShowWindow(SW_HIDE);
	InitComboClass();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

void CAddChildDlg::InitComboClass()
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
	strSQL.Format(L"%s", L"select id,name from class");
	pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	int num = 0;
	CString strClassName;
	int iClassID;
	while (!pSet->adoEOF)
	{
		iClassID = getsqldouble("id");
		strClassName = getsqlstr("name");
		m_comboChildClass.AddString(strClassName);
		m_comboChildClass.SetItemData(num,iClassID);
		pSet->MoveNext();
		num++;
	}
	pSet->Close();
	conn->Close();
	m_comboChildClass.SetCurSel(0);
}

void CAddChildDlg::OnBnClickedButton1()
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


void CAddChildDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
	if (m_strCurSelPhotoPathName.IsEmpty())
		return;
	HDC hDC = GetDC()->m_hDC;
	Graphics g(hDC);
	//��ѧ��ͷ��
	Image img(m_strCurSelPhotoPathName);
	CRect rPhoto;
	m_static_photo.GetWindowRect(&rPhoto);
	ScreenToClient(&rPhoto);
	g.DrawImage(&img, rPhoto.left, rPhoto.top, rPhoto.Width(), rPhoto.Height());

	g.ReleaseHDC(hDC);
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}


void CAddChildDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	m_strChildName.Trim();
	if (m_strChildName.IsEmpty())
	{
		MessageBox(L"����дѧ������", L"��ʾ", MB_ICONINFORMATION|MB_OK);
		return;
	}
	if (m_strCurSelPhotoPathName.IsEmpty())
	{
		MessageBox(L"��ѡ��ѧ����Ƭ", L"��ʾ", MB_ICONINFORMATION | MB_OK);
		return;
	}
	int id = m_comboChildClass.GetItemData(m_comboChildClass.GetCurSel());

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
	strSQL.Format(L"%s", L"select * from child");
	pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	CString strClassid;
	strClassid.Format(L"%d", id);
	pSet->AddNew();
	pSet->put_Collect(_variant_t("name"), _variant_t(m_strChildName));
	pSet->put_Collect(_variant_t("classid"), _variant_t(strClassid));
	pSet->put_Collect(_variant_t("ishere"), _variant_t("1"));
	pSet->put_Collect(_variant_t("todayishere"), _variant_t("1"));
	pSet->put_Collect(_variant_t("isend"), _variant_t("0"));
	pSet->Update();
	pSet->Close();
	strSQL.Format(L"%s", L"select max(id) as maxid from child");
	pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	int iCurID = getsqldouble("maxid");
	pSet->Close();
	conn->Close();

	CString strDesPathName;
	strDesPathName.Format(L"child_photo/%d.jpg", iCurID);
	MoveFile(m_strCurSelPhotoPathName, strDesPathName);
	CDialogEx::OnOK();
}
