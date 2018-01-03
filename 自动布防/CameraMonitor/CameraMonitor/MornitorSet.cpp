// MornitorSet.cpp : 实现文件
//

#include "stdafx.h"
#include "CameraMonitor.h"
#include "MornitorSet.h"
#include "afxdialogex.h"


// MornitorSet 对话框

IMPLEMENT_DYNAMIC(MornitorSet, CDialog)

MornitorSet::MornitorSet(CWnd* pParent /*=NULL*/)
	: CDialog(MornitorSet::IDD, pParent)
{

}

MornitorSet::~MornitorSet()
{
}

void MornitorSet::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_COMBO_FACE_ALARM_TYPE, m_comFaceAlarmType);
	DDX_Control(pDX, IDC_COMBO_DEPLOY_TYPE, m_comDeployType);
	DDX_Text(pDX, IDC_EDIT_TASK_NO, m_iTaskNo);
	DDX_Check(pDX, IDC_CHK_BKN, m_bChkBKN);
	DDX_Check(pDX, IDC_CHK_PDC, m_bChkPDC);
	DDX_Check(pDX, IDC_CHK_HEATMAP, m_bChkHeatMap);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHK_CONFIRM, m_bConfirmMechanism);

	DDX_Check(pDX, IDC_CHK_FACESNAP, m_bChkFaceSnap);
	DDX_Check(pDX, IDC_CHK_JSONURL, m_bChkJsonURL);
	DDX_Check(pDX, IDC_CHK_FACEMATCH, m_bFaceMatch);
	DDX_Control(pDX, IDC_COMBO_ALARM_LEVEL, m_comAlarmLevel);
	DDX_Check(pDX, IDC_CHK_COPILOT_FACE, m_bCopilotFace);
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MornitorSet, CDialog)
	
	ON_BN_CLICKED(IDC_BUT_SAVE, &MornitorSet::OnBnClickedButSave)
END_MESSAGE_MAP()


// MornitorSet 消息处理程序


void MornitorSet::OnBnClickedButSave()
{
	// TODO:  在此添加控件通知处理程序代码
}
