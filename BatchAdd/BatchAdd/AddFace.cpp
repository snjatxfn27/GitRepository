// AddFace.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BatchAdd.h"
#include "AddFace.h"
#include "afxdialogex.h"


// AddFace �Ի���

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


// AddFace ��Ϣ�������
