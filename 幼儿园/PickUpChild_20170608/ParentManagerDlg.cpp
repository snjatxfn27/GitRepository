// ParentManagerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PickUpChild.h"
#include "ParentManagerDlg.h"
#include "afxdialogex.h"
#include "AddParentDlg.h"


// CParentManagerDlg 对话框

IMPLEMENT_DYNAMIC(CParentManagerDlg, CDialogEx)

CParentManagerDlg::CParentManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CParentManagerDlg::IDD, pParent)
{

}

CParentManagerDlg::~CParentManagerDlg()
{
}

void CParentManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CHOOSECLASS, m_comboChooseClass);
	DDX_Control(pDX, IDC_LIST_CHILD, m_listChild);
	DDX_Control(pDX, IDC_STATIC_PHOTO, m_static_childphoto);
	DDX_Control(pDX, IDC_STATIC_PARENTPHOTO, m_static_parentphoto);
	DDX_Control(pDX, IDC_LIST_PARENT, m_listParent);
}


BEGIN_MESSAGE_MAP(CParentManagerDlg, CDialogEx)
//	ON_NOTIFY(NM_CLICK, IDC_LIST_CHILD, &CParentManagerDlg::OnNMClickListChild)
	ON_NOTIFY(NM_CLICK, IDC_LIST_CHILD, &CParentManagerDlg::OnClickListChild)
	ON_WM_PAINT()
	ON_NOTIFY(NM_CLICK, IDC_LIST_PARENT, &CParentManagerDlg::OnClickListParent)
	ON_BN_CLICKED(IDC_BUTTON_DELPARENT, &CParentManagerDlg::OnBnClickedButtonDelparent)
	ON_BN_CLICKED(IDC_BUTTON_ADDPARENT, &CParentManagerDlg::OnBnClickedButtonAddparent)
	ON_CBN_SELCHANGE(IDC_COMBO_CHOOSECLASS, &CParentManagerDlg::OnSelchangeComboChooseclass)
END_MESSAGE_MAP()


// CParentManagerDlg 消息处理程序


BOOL CParentManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_listChild.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);// | LVS_EX_TRACKSELECT | LVS_EX_TWOCLICKACTIVATE);
	m_listChild.InsertColumn(0, L"姓名", LVCFMT_CENTER, 80);
	m_listChild.InsertColumn(1, L"班级", LVCFMT_CENTER, 170);
	m_listParent.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);// | LVS_EX_TRACKSELECT | LVS_EX_TWOCLICKACTIVATE);
	m_listParent.InsertColumn(0, L"姓名", LVCFMT_CENTER, 80);
	m_listParent.InsertColumn(1, L"关系", LVCFMT_CENTER, 170);

	m_static_childphoto.ShowWindow(SW_HIDE);
	m_static_parentphoto.ShowWindow(SW_HIDE);
	m_iCurSelID = -1;
	m_iCurSelParentID = -1;
	InitComboChooseClass();
	RefreshListChild();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CParentManagerDlg::InitComboChooseClass()
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

void CParentManagerDlg::RefreshListChild()
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
		strSQL.Format(L"%s", L"select child.id as childid,child.name as childname,class.name as classname from child left join class on class.id=child.classid");
	else
	{
		CString strChooseClassName;
		m_comboChooseClass.GetLBText(cursel, strChooseClassName);
		strSQL.Format(L"select child.id as childid,child.name as childname,class.name as classname from child left join class on class.id=child.classid where class.name='%s'", strChooseClassName);
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

void CParentManagerDlg::OnClickListChild(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	int mark = m_listChild.GetSelectionMark();
	if (mark == -1)
		return;
	m_iCurSelID = m_listChild.GetItemData(mark);
	RefreshListParent(m_iCurSelID);
	m_iCurSelParentID = -1;

	Invalidate(true);
	*pResult = 0;
}


void CParentManagerDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	if (m_iCurSelID == -1)
		return;
	HDC hDC = GetDC()->m_hDC;
	Graphics g(hDC);
	//画学生头像
	CString strPhotoName;
	strPhotoName.Format(L"child_photo/%d.jpg", m_iCurSelID);
	Image img(strPhotoName);
	CRect rPhoto;
	m_static_childphoto.GetWindowRect(&rPhoto);
	ScreenToClient(&rPhoto);
	g.DrawImage(&img, rPhoto.left, rPhoto.top, rPhoto.Width(), rPhoto.Height());

	//画家长头像
	if (m_iCurSelParentID != -1)
	{
		CString strParentPhotoName;
		strParentPhotoName.Format(L"parent_photo/%d.jpg", m_iCurSelParentID);
		Image imgParent(strParentPhotoName);
		CRect rPhotoParent;
		m_static_parentphoto.GetWindowRect(&rPhotoParent);
		ScreenToClient(&rPhotoParent);
		g.DrawImage(&imgParent, rPhotoParent.left, rPhotoParent.top, rPhotoParent.Width(), rPhotoParent.Height());
	}

	g.ReleaseHDC(hDC);
	// 不为绘图消息调用 CDialogEx::OnPaint()
}

void CParentManagerDlg::RefreshListParent(int iChildID)
{
	m_listParent.DeleteAllItems();

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
	strSQL.Format(L"%s%d", L"select * from parent where childid=",iChildID);
	pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	int num = 0;
	CString strParentName, strRelation;
	int iParentID;
	while (!pSet->adoEOF)
	{
		iParentID = getsqldouble("id");
		strParentName = getsqlstr("name");
		strRelation = getsqlstr("relation");
		m_listParent.InsertItem(num, strParentName);
		m_listParent.SetItemText(num, 1, strRelation);
		m_listParent.SetItemData(num, iParentID);
		pSet->MoveNext();
		num++;
	}
	pSet->Close();
	conn->Close();
}

void CParentManagerDlg::OnClickListParent(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	int mark = m_listParent.GetSelectionMark();
	if (mark == -1)
		return;
	m_iCurSelParentID = m_listParent.GetItemData(mark);
	Invalidate(false);
	*pResult = 0;
}


void CParentManagerDlg::OnBnClickedButtonDelparent()
{
	// TODO:  在此添加控件通知处理程序代码
	int mark = m_listParent.GetSelectionMark();
	if (mark == -1)
	{
		MessageBox(L"请选择要删除的家长", L"提示", MB_ICONINFORMATION | MB_OK);
		return;
	}
	//删除数据库信息
	m_iCurSelParentID = m_listParent.GetItemData(mark);
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
	strSQL.Format(L"%s%d", L"delete from parent where id=", m_iCurSelParentID);
	pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	conn->Close();
	//删除图片文件
	CString strDelFilePathName;
	strDelFilePathName.Format(L"parent_photo/%d.jpg", m_iCurSelParentID);
	DeleteFile(strDelFilePathName);
	//刷新列表
	RefreshListParent(m_iCurSelID);
	//刷新显示
	m_iCurSelParentID = -1;
	Invalidate(false);
}


void CParentManagerDlg::OnBnClickedButtonAddparent()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_iCurSelID == -1)
		return;
	if (m_listParent.GetItemCount() >= 3)
	{
		MessageBox(L"演示版每个学生最多添加3个家长", L"提示", MB_ICONINFORMATION | MB_OK);
		return;
	}
	CAddParentDlg dlg;
	dlg.m_iChildID = m_iCurSelID;
	if (dlg.DoModal() == IDOK)
	{
		RefreshListParent(m_iCurSelID);
	}
}


void CParentManagerDlg::OnSelchangeComboChooseclass()
{
	// TODO:  在此添加控件通知处理程序代码
	m_iCurSelID = -1;
	RefreshListChild();
	Invalidate(true);

}
