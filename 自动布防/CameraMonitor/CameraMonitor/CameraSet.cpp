// CameraSet.cpp : 实现文件
//

#include "stdafx.h"
#include "CameraMonitor.h"
#include "CameraSet.h"
#include "afxdialogex.h"


// CameraSet 对话框
CameraSet *g_pDlgCameraSet = NULL;

IMPLEMENT_DYNAMIC(CameraSet, CDialog)

CameraSet::CameraSet(CWnd* pParent /*=NULL*/)
: CDialog(CameraSet::IDD, pParent)
{
	m_bChkFilterActive = FALSE;
	m_bDrawPolygon = FALSE;
	m_bChkEnable = FALSE;
	m_byRate = 0;
	m_bSetMiniFilter = FALSE;
	m_bSetMaxFilter = FALSE;
	m_bySnapInterval = 0;
	m_bySnapThreshold = 0;
	m_byReferBright = 0;
	m_byMatchThreshold = 0;
	m_dwFaceExposureMinDuration = 0;
	m_dwTerminalNo = 0;
	m_dwUploadInterval = 0;
	m_dwValidFaceMinDuration = 0;
	//}}AFX_DATA_INIT
	m_lUerID = -1;
	memset(&m_struFaceSnap, 0, sizeof(m_struFaceSnap));
	m_lPlayHandle = -1;
	memset(&m_rcWnd, 0, sizeof(m_rcWnd));
	m_bCloseIn = FALSE;
	m_bMouseMove = FALSE;
	m_bNeedRedraw = FALSE;
	m_dwRuleNum = 0;
}

CameraSet::~CameraSet()
{
}

void CameraSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_CHANNEL, m_cmbChannel);
	DDX_Control(pDX, IDC_COMBO_ROI, m_comROI);
	DDX_Control(pDX, IDC_COMBO_FACE_ENABLE, m_comFaceEnable);
	DDX_Control(pDX, IDC_COMBO_BACK_GROUND, m_comBackgroundPic);
	DDX_Control(pDX, IDC_COMBO_MATCH_ALATM_TYPE, m_comboMatchType);
	DDX_Control(pDX, IDC_COMBO_SENSITIVE, m_comboSensitive);
	DDX_Control(pDX, IDC_COMBO_SNAPTIMES, m_comboSnapTimes);
	DDX_Control(pDX, IDC_COMBO_RULE_NUM, m_comboRuleNum);
	DDX_Control(pDX, IDC_COMBO_PIC_QUALITY, m_comboPicQuality);
	DDX_Control(pDX, IDC_COMBO_RESOLUTION, m_comboPicResolution);
	DDX_Check(pDX, IDC_CHECK_FILTER_ACTIVE, m_bChkFilterActive);
	DDX_Check(pDX, IDC_CHK_DRAW_POLYGON, m_bDrawPolygon);
	DDX_Check(pDX, IDC_CHK_ENABLE, m_bChkEnable);
	DDX_Text(pDX, IDC_EDIT_RATE, m_byRate);
	DDX_Check(pDX, IDC_CHECK_SET_MINI_FILTER, m_bSetMiniFilter);
	DDX_Check(pDX, IDC_CHECK_SET_MAX_FILTER, m_bSetMaxFilter);
	DDX_Text(pDX, IDC_EDIT_SNAP_INTERVAL, m_bySnapInterval);
	DDX_Text(pDX, IDC_EDIT_SNAP_THRESHOLD, m_bySnapThreshold);
	DDX_Text(pDX, IDC_EDIT_REFERENCE_BRIGHT, m_byReferBright);
	DDX_Text(pDX, IDC_EDIT_MATCH_THRESHOLD, m_byMatchThreshold);
	DDX_Text(pDX, IDC_EDIT_FACE_MIN_DURA, m_dwFaceExposureMinDuration);
	DDX_Text(pDX, IDC_EDIT_TERMINAL_NO, m_dwTerminalNo);
	DDX_Text(pDX, IDC_EDIT_UPLOAD_INTERVAL, m_dwUploadInterval);
	DDX_Text(pDX, IDC_EDIT_VALID_FACE_MIN_DURATION, m_dwValidFaceMinDuration);
	DDX_Text(pDX, IDC_EDIT_FACEFILTER_TIME, m_FaceFilteringTime);

}


BEGIN_MESSAGE_MAP(CameraSet, CDialog)
	ON_BN_CLICKED(IDC_BUT_SET, &CameraSet::OnBnClickedButSet)
	ON_BN_CLICKED(IDC_CHECK_FILTER_ACTIVE, &CameraSet::OnBnClickedCheckFilterActive)
END_MESSAGE_MAP()


// CameraSet 消息处理程序




BOOL CameraSet::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	GetDlgItem(IDC_EDIT_TERMINAL_NO)->EnableWindow(FALSE);
	m_cmbChannel.AddString(m_csCamename);
	m_cmbChannel.SetCurSel(0);
	g_pDlgCameraSet = this;


	m_bNeedRedraw = TRUE;//Redraw polygon
	m_bCloseIn = TRUE;//Polygon is closed or not
	m_comboRuleNum.SetCurSel(0);
	m_dwRuleNum = 0;
	m_comboSnapTimes.SetCurSel(0);

	if (!GetFaceSnap())
	{
		AfxMessageBox("Fail to get face snap");
	}
	SetFaceSnapToWnd(m_struFaceSnap);
	StartPlay();
	return TRUE;
}

BOOL CameraSet::GetFaceSnap()
{
	UpdateData(TRUE);

	DWORD dwReturn = 0;
	if (!NET_DVR_GetDVRConfig(m_lUerID, NET_DVR_GET_FACESNAPCFG, 1, &m_struFaceSnap, sizeof(m_struFaceSnap), &dwReturn))
	{
		return FALSE;
	}
	else
	{
		SetFaceSnapToWnd(m_struFaceSnap);
		return TRUE;
	}

	UpdateData(FALSE);
}
BOOL CameraSet::SetFaceSnap()
{
	for (int i = 0; i < MAX_RULE_NUM; i++)
	{
		if (m_struFaceSnap.struRule[i].struVcaPolygon.dwPointNum < 3 && m_struFaceSnap.struRule[i].struVcaPolygon.dwPointNum >0)
		{
			AfxMessageBox("多边形区域不符合要求!");
			return FALSE;
		}
	}

	UpdateData(TRUE);

	m_struFaceSnap.dwSize = sizeof(m_struFaceSnap);
	if (!NET_DVR_SetDVRConfig(m_lUerID, NET_DVR_SET_FACESNAPCFG, 1, &m_struFaceSnap, sizeof(m_struFaceSnap)))
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}
void CameraSet::SetFaceSnapToWnd(NET_VCA_FACESNAPCFG const &struFaceSnap)
{
	m_bChkEnable = struFaceSnap.struRule[m_dwRuleNum].byActive;
	m_bChkFilterActive = struFaceSnap.struRule[m_dwRuleNum].struSizeFilter.byActive;
	m_comboSensitive.SetCurSel(struFaceSnap.bySensitive - 1);
	m_comboSnapTimes.SetCurSel(struFaceSnap.bySnapTime);
	m_byRate = struFaceSnap.byGenerateRate;
	m_bySnapInterval = struFaceSnap.bySnapInterval;
	m_bySnapThreshold = struFaceSnap.bySnapThreshold;
	m_comboPicQuality.SetCurSel(struFaceSnap.struPictureParam.wPicQuality);
	m_comboPicResolution.SetCurSel(struFaceSnap.struPictureParam.wPicSize);
	m_byReferBright = struFaceSnap.byReferenceBright;
	m_comboMatchType.SetCurSel(struFaceSnap.byMatchType);
	m_byMatchThreshold = struFaceSnap.byMatchThreshold;
	//2014-03-21
	m_dwFaceExposureMinDuration = struFaceSnap.wFaceExposureMinDuration;
	m_comFaceEnable.SetCurSel(struFaceSnap.byFaceExposureMode);
	m_comBackgroundPic.SetCurSel(struFaceSnap.byBackgroundPic);
	m_comROI.SetCurSel(struFaceSnap.struRule[m_dwRuleNum].byAutoROIEnable);
	m_dwUploadInterval = struFaceSnap.dwUploadInterval;
	m_dwValidFaceMinDuration = struFaceSnap.dwValidFaceTime;
	m_FaceFilteringTime = struFaceSnap.dwFaceFilteringTime;
	UpdateData(FALSE);
}

void CameraSet::GetFaceSnapFromWnd(NET_VCA_FACESNAPCFG& struFaceSnap)
{
	UpdateData(TRUE);
	m_dwRuleNum = m_comboRuleNum.GetCurSel();
	struFaceSnap.byGenerateRate = m_byRate;
	struFaceSnap.struRule[m_dwRuleNum].byActive = m_bChkEnable;
	struFaceSnap.bySnapTime = m_comboSnapTimes.GetCurSel();
	struFaceSnap.bySensitive = m_comboSensitive.GetCurSel() + 1;
	struFaceSnap.struRule[m_dwRuleNum].struSizeFilter.byActive = m_bChkFilterActive;
	struFaceSnap.bySnapInterval = m_bySnapInterval;
	struFaceSnap.bySnapThreshold = m_bySnapThreshold;
	struFaceSnap.struPictureParam.wPicQuality = m_comboPicQuality.GetCurSel();
	struFaceSnap.struPictureParam.wPicSize = m_comboPicResolution.GetCurSel();
	struFaceSnap.byReferenceBright = m_byReferBright;
	struFaceSnap.byMatchThreshold = m_byMatchThreshold;
	struFaceSnap.byMatchType = m_comboMatchType.GetCurSel();
	//2014-03-21
	struFaceSnap.wFaceExposureMinDuration = m_dwFaceExposureMinDuration;
	struFaceSnap.byFaceExposureMode = m_comFaceEnable.GetCurSel();
	struFaceSnap.byBackgroundPic = m_comBackgroundPic.GetCurSel();
	struFaceSnap.struRule[m_dwRuleNum].byAutoROIEnable = m_comROI.GetCurSel();
	struFaceSnap.dwUploadInterval = m_dwUploadInterval;
	struFaceSnap.dwValidFaceTime = m_dwValidFaceMinDuration;
	struFaceSnap.dwFaceFilteringTime = m_FaceFilteringTime;
}

void CameraSet::OnBnClickedButSet()
{
	GetFaceSnapFromWnd(m_struFaceSnap);
	if (SetFaceSnap())
		MessageBox("设置成功", "提示");
	else
		MessageBox("设置失败", "提示");
}

BOOL CameraSet::StartPlay()
{
	try
	{
		NET_DVR_CLIENTINFO ClientInfo;
		ClientInfo.hPlayWnd = GetDlgItem(IDC_STATIC_VIDEO)->m_hWnd;
		ClientInfo.lChannel = 1;
		ClientInfo.lLinkMode = 0;
		ClientInfo.sMultiCastIP = NULL;
		LONG lPlayHandle = NET_DVR_RealPlay_V30(m_lUerID, &ClientInfo, NULL, NULL, TRUE);
		if (-1 == lPlayHandle)
		{
			DWORD err = NET_DVR_GetLastError();
			CString m_csErr;
			m_csErr.Format(_T("播放出错，错误代码%d"), err);
			MessageBox(m_csErr, "提示");
			return FALSE;
		}
	}
	catch (...)
	{
		return FALSE;
	}
	return TRUE;
}

void CameraSet::OnBnClickedCheckFilterActive()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_CHECK_SET_MAX_FILTER)->EnableWindow(m_bChkFilterActive);
	GetDlgItem(IDC_CHECK_SET_MINI_FILTER)->EnableWindow(m_bChkFilterActive);
}
