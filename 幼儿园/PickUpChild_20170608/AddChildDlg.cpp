// AddChildDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PickUpChild.h"
#include "AddChildDlg.h"
#include "afxdialogex.h"


// CAddChildDlg 对话框

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


// CAddChildDlg 消息处理程序


BOOL CAddChildDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_strCurSelPhotoPathName.Empty();
	m_static_photo.ShowWindow(SW_HIDE);
	InitComboClass();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
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
	// TODO:  在此添加控件通知处理程序代码
	CString filter;
	filter = L"jpg文件(*.jpg)|*.jpg";
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
	// TODO:  在此处添加消息处理程序代码
	if (m_strCurSelPhotoPathName.IsEmpty())
		return;
	HDC hDC = GetDC()->m_hDC;
	Graphics g(hDC);
	//画学生头像
	Image img(m_strCurSelPhotoPathName);
	CRect rPhoto;
	m_static_photo.GetWindowRect(&rPhoto);
	ScreenToClient(&rPhoto);
	g.DrawImage(&img, rPhoto.left, rPhoto.top, rPhoto.Width(), rPhoto.Height());

	g.ReleaseHDC(hDC);
	// 不为绘图消息调用 CDialogEx::OnPaint()
}


void CAddChildDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	m_strChildName.Trim();
	if (m_strChildName.IsEmpty())
	{
		MessageBox(L"请填写学生姓名", L"提示", MB_ICONINFORMATION|MB_OK);
		return;
	}
	if (m_strCurSelPhotoPathName.IsEmpty())
	{
		MessageBox(L"请选择学生照片", L"提示", MB_ICONINFORMATION | MB_OK);
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
