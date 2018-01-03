// ChildManagerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PickUpChild.h"
#include "ChildManagerDlg.h"
#include "afxdialogex.h"
#include "AddChildDlg.h"


// CChildManagerDlg 对话框

IMPLEMENT_DYNAMIC(CChildManagerDlg, CDialogEx)

CChildManagerDlg::CChildManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChildManagerDlg::IDD, pParent)
{

}

CChildManagerDlg::~CChildManagerDlg()
{
}

void CChildManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CHOOSECLASS, m_comboChooseClass);
	DDX_Control(pDX, IDC_LIST_CHILD, m_listChild);
	DDX_Control(pDX, IDC_COMBO_EDITCLASS, m_comboEditClass);
	DDX_Control(pDX, IDC_STATIC_PHOTO, m_static_photo);
}


BEGIN_MESSAGE_MAP(CChildManagerDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_CHOOSECLASS, &CChildManagerDlg::OnSelchangeComboChooseclass)
	ON_WM_PAINT()
	ON_NOTIFY(NM_CLICK, IDC_LIST_CHILD, &CChildManagerDlg::OnClickListChild)
	ON_BN_CLICKED(IDC_BUTTON_ADDCHILD, &CChildManagerDlg::OnBnClickedButtonAddchild)
	ON_BN_CLICKED(IDC_BUTTON_CHANGEPHOTO, &CChildManagerDlg::OnBnClickedButtonChangephoto)
	ON_BN_CLICKED(IDOK, &CChildManagerDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CChildManagerDlg 消息处理程序


BOOL CChildManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_listChild.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);// | LVS_EX_TRACKSELECT | LVS_EX_TWOCLICKACTIVATE);
	m_listChild.InsertColumn(0, L"姓名", LVCFMT_CENTER, 80);
	m_listChild.InsertColumn(1, L"班级", LVCFMT_CENTER, 170);

	m_static_photo.ShowWindow(SW_HIDE);
	m_strCurSelPhotoPathName.Empty();
	m_iCurSelID = -1;
	InitComboChooseClass();
	InitComboEditClass();
	RefreshListChild();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CChildManagerDlg::RefreshListChild()
{
	m_listChild.DeleteAllItems();
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
	int cursel = m_comboChooseClass.GetCurSel();
	if (cursel == 0)
		strSQL.Format(L"%s",L"select child.id as childid,child.name as childname,class.name as classname from child left join class on class.id=child.classid");
	else
	{
		CString strChooseClassName;
		m_comboChooseClass.GetLBText(cursel, strChooseClassName);
		strSQL.Format(L"select child.id as childid,child.name as childname,class.name as classname from child left join class on class.id=child.classid where class.name='%s'",strChooseClassName);
	}
	pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	int num = 0;
	CString strChildName, strClassName;
	int iChildID;
	while (!pSet->adoEOF)
	{
		iChildID = getsqldouble("childid");
		strChildName = getsqlstr("childname");
		strClassName = getsqlstr("classname");
		m_listChild.InsertItem(num, strChildName);
		m_listChild.SetItemText(num, 1, strClassName);
		m_listChild.SetItemData(num, iChildID);
		pSet->MoveNext();
		num++;
	}
	pSet->Close();
	conn->Close();
}

void CChildManagerDlg::InitComboChooseClass()
{
	m_comboChooseClass.AddString(L"全部");
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
	strSQL.Format(L"%s", L"select name from class");
	pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	int num = 1;
	CString strClassName;
	while (!pSet->adoEOF)
	{
		strClassName = getsqlstr("name");
		m_comboChooseClass.AddString(strClassName);
		pSet->MoveNext();
		num++;
	}
	pSet->Close();
	conn->Close();
	m_comboChooseClass.SetCurSel(0);
}
void CChildManagerDlg::InitComboEditClass()
{
	m_comboEditClass.AddString(L"无");
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
	strSQL.Format(L"%s", L"select name from class");
	pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	int num = 1;
	CString strClassName;
	while (!pSet->adoEOF)
	{
		strClassName = getsqlstr("name");
		m_comboEditClass.AddString(strClassName);
		pSet->MoveNext();
		num++;
	}
	pSet->Close();
	conn->Close();
	m_comboEditClass.SetCurSel(0);
}
void CChildManagerDlg::OnSelchangeComboChooseclass()
{
	m_iCurSelID = -1;
	RefreshListChild();
	m_comboEditClass.SetCurSel(0);
	Invalidate(true);
}


void CChildManagerDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	if (m_iCurSelID == -1)
		return;
	HDC hDC = GetDC()->m_hDC;
	Graphics g(hDC);
	//画学生头像
	CString strPhotoName;
	strPhotoName.Format(L"child_photo/%d.jpg",m_iCurSelID);
	Image img(strPhotoName);
	CRect rPhoto;
	m_static_photo.GetWindowRect(&rPhoto);
	ScreenToClient(&rPhoto);
	g.DrawImage(&img, rPhoto.left, rPhoto.top, rPhoto.Width(), rPhoto.Height());
	
	g.ReleaseHDC(hDC);
	// 不为绘图消息调用 CDialogEx::OnPaint()
}


void CChildManagerDlg::OnClickListChild(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	int mark = m_listChild.GetSelectionMark();
	if (mark == -1)
		return;
	m_iCurSelID = m_listChild.GetItemData(mark);

	CString strClassName = m_listChild.GetItemText(mark, 1);
	int count = m_comboEditClass.GetCount();
	int i = 0;
	for (i = 0; i < count; i++)
	{
		CString strTemp;
		m_comboEditClass.GetLBText(i, strTemp);
		if (strTemp == strClassName)
		{
			m_comboEditClass.SetCurSel(i);
			break;
		}
	}
	Invalidate(false);
	*pResult = 0;
}


void CChildManagerDlg::OnBnClickedButtonAddchild()
{
	// TODO:  在此添加控件通知处理程序代码
	CAddChildDlg dlg;
	dlg.DoModal();
}


void CChildManagerDlg::OnBnClickedButtonChangephoto()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_iCurSelID == -1)
		return;
	CString filter;
	filter = L"jpg文件(*.jpg)|*.jpg";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter);
	if (dlg.DoModal() == IDOK)
	{
		CString str;
		str = dlg.GetPathName();
		m_strCurSelPhotoPathName = str;
		Invalidate(false);
	}
}


void CChildManagerDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}
