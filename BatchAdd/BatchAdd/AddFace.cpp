// AddFace.cpp : 实现文件
//

#include "stdafx.h"
#include "BatchAdd.h"
#include "AddFace.h"
#include "afxdialogex.h"


// AddFace 对话框

IMPLEMENT_DYNAMIC(AddFace, CDialog)

AddFace::AddFace(CWnd* pParent /*=NULL*/)
	: CDialog(AddFace::IDD, pParent)
	, m_csFaceName(_T(""))
{

}

AddFace::~AddFace()
{
}

void AddFace::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_csFaceName);
}


BEGIN_MESSAGE_MAP(AddFace, CDialog)
END_MESSAGE_MAP()


// AddFace 消息处理程序
