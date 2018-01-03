// RLKGLDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CrowdAnalyz.h"
#include "RLKGLDlg.h"
#include "afxdialogex.h"


// CRLKGLDlg 对话框

IMPLEMENT_DYNAMIC(CRLKGLDlg, CDialogEx)

CRLKGLDlg::CRLKGLDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CRLKGLDlg::IDD, pParent)
{

}

CRLKGLDlg::~CRLKGLDlg()
{
}

void CRLKGLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_EDIT_BIRTH, m_editBirth);
	DDX_Control(pDX, IDC_EDIT_TYPE, m_editType);
	DDX_Control(pDX, IDC_EDIT_PHONE, m_editPhone);
	DDX_Control(pDX, IDC_EDIT_NATIVEPLACE, m_editNativeplace);
	DDX_Control(pDX, IDC_EDIT_ADDRESS, m_editAddress);
	DDX_Control(pDX, IDC_COMBO_SEX, m_comboSex);
	DDX_Control(pDX, IDC_COMBO_FACEDB, m_comboFacedb);
	DDX_Control(pDX, IDC_EDIT_SEARCHNAME, m_editSearchName);
	DDX_Control(pDX, IDC_EDIT_SEARCHTYPE, m_editSearchType);
	DDX_Control(pDX, IDC_COMBO_SEARCHSEX, m_comboSearchSex);
}


BEGIN_MESSAGE_MAP(CRLKGLDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CRLKGLDlg 消息处理程序


void CRLKGLDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO:  在此处添加消息处理程序代码
	//界面
	delete pMemDC;
	delete pBitmap;
}


BOOL CRLKGLDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	int i = 0;
	m_colorEditText = RGB(212, 214, 222);
	m_colorEditBK = RGB(17, 29, 67);
	m_pEditBkBrush = new CBrush(RGB(21, 25, 60));

	m_iCurMingdan = 0;
	m_rClient.SetRect(0, 0, 1920 * WIDTH / 1920, 830 * HEIGHT / 1080);
	m_rJibenxinxi.SetRect(781 * m_rClient.Width() / 1920, 12 * m_rClient.Height() / 830, 1440 * m_rClient.Width() / 1920, 525 * m_rClient.Height() / 830);
	m_rBianji.SetRect(781 * m_rClient.Width() / 1920, 10 * m_rClient.Height() / 830, 1442 * m_rClient.Width() / 1920, 356 * m_rClient.Height() / 830);
	m_rAdd.SetRect(781 * m_rClient.Width() / 1920, 12 * m_rClient.Height() / 830, 1440 * m_rClient.Width() / 1920, 354 * m_rClient.Height() / 830);
	m_rBDBlack.SetRect(169 * m_rClient.Width() / 1920, 32 * m_rClient.Height() / 830, 328 * m_rClient.Width() / 1920, 55 * m_rClient.Height() / 830);
	m_rDTBlack.SetRect(333 * m_rClient.Width() / 1920, 32 * m_rClient.Height() / 830, 492 * m_rClient.Width() / 1920, 55 * m_rClient.Height() / 830);
	m_rWhite.SetRect(498 * m_rClient.Width() / 1920, 32 * m_rClient.Height() / 830, 655 * m_rClient.Width() / 1920, 55 * m_rClient.Height() / 830);
	m_rLeftBeginPage.SetRect(270 * m_rClient.Width() / 1920, 775 * m_rClient.Height() / 830, 316 * m_rClient.Width() / 1920, 797 * m_rClient.Height() / 830);
	m_rLeftPrePage.SetRect(330 * m_rClient.Width() / 1920, 775 * m_rClient.Height() / 830, 389 * m_rClient.Width() / 1920, 797 * m_rClient.Height() / 830);
	m_rLeftNextPage.SetRect(493 * m_rClient.Width() / 1920, 775 * m_rClient.Height() / 830, 554 * m_rClient.Width() / 1920, 797 * m_rClient.Height() / 830);
	m_rLeftEndPage.SetRect(570 * m_rClient.Width() / 1920, 775 * m_rClient.Height() / 830, 614 * m_rClient.Width() / 1920, 797 * m_rClient.Height() / 830);
	m_rLeftCurPage.SetRect(423 * m_rClient.Width() / 1920, 779 * m_rClient.Height() / 830, 459 * m_rClient.Width() / 1920, 801 * m_rClient.Height() / 830);
	m_rLeftTotalPage.SetRect(710 * m_rClient.Width() / 1920, 779 * m_rClient.Height() / 830, 749 * m_rClient.Width() / 1920, 801 * m_rClient.Height() / 830);
	m_rJbxxxq[0].SetRect(970 * m_rClient.Width() / 1920, 76 * m_rClient.Height() / 830, 1400 * m_rClient.Width() / 1920, 106 * m_rClient.Height() / 830);
	m_rJbxxxq[1].SetRect(970 * m_rClient.Width() / 1920, 107 * m_rClient.Height() / 830, 1400 * m_rClient.Width() / 1920, 137 * m_rClient.Height() / 830);
	m_rJbxxxq[2].SetRect(970 * m_rClient.Width() / 1920, 138 * m_rClient.Height() / 830, 1400 * m_rClient.Width() / 1920, 168 * m_rClient.Height() / 830);
	m_rJbbx.SetRect(840 * m_rClient.Width() / 1920, 60 * m_rClient.Height() / 830, 950 * m_rClient.Width() / 1920, 178 * m_rClient.Height() / 830);
	m_rBjbx.SetRect(820 * m_rClient.Width() / 1920, 85 * m_rClient.Height() / 830, 940 * m_rClient.Width() / 1920, 223 * m_rClient.Height() / 830);
	m_rTjbx.SetRect(820 * m_rClient.Width() / 1920, 85 * m_rClient.Height() / 830, 940 * m_rClient.Width() / 1920, 223 * m_rClient.Height() / 830);
	m_rBjbc.SetRect(1272 * m_rClient.Width() / 1920, 272 * m_rClient.Height() / 830, 1332 * m_rClient.Width() / 1920, 307 * m_rClient.Height() / 830);
	m_rTjbc.SetRect(1272 * m_rClient.Width() / 1920, 272 * m_rClient.Height() / 830, 1332 * m_rClient.Width() / 1920, 307 * m_rClient.Height() / 830);
	m_rButtonAdd.SetRect(621 * m_rClient.Width() / 1920, 72 * m_rClient.Height() / 830, 671 * m_rClient.Width() / 1920, 99 * m_rClient.Height() / 830);
	m_rAlarmBeginPage.SetRect(945 * m_rClient.Width() / 1920, 490 * m_rClient.Height() / 830, 996 * m_rClient.Width() / 1920, 512 * m_rClient.Height() / 830);
	m_rAlarmPrePage.SetRect(1000 * m_rClient.Width() / 1920, 490 * m_rClient.Height() / 830, 1070 * m_rClient.Width() / 1920, 512 * m_rClient.Height() / 830);
	m_rAlarmCurPage.SetRect(1090 * m_rClient.Width() / 1920, 490 * m_rClient.Height() / 830, 1130 * m_rClient.Width() / 1920, 512 * m_rClient.Height() / 830);
	m_rAlarmNextPage.SetRect(1155 * m_rClient.Width() / 1920, 490 * m_rClient.Height() / 830, 1218 * m_rClient.Width() / 1920, 512 * m_rClient.Height() / 830);
	m_rAlarmEndPage.SetRect(1227 * m_rClient.Width() / 1920, 490 * m_rClient.Height() / 830, 1276 * m_rClient.Width() / 1920, 512 * m_rClient.Height() / 830);
	m_rSearchName.SetRect(113 * m_rClient.Width() / 1920, 73 * m_rClient.Height() / 830, 228 * m_rClient.Width() / 1920, 96 * m_rClient.Height() / 830);
	m_rSearchSex.SetRect(276 * m_rClient.Width() / 1920, 73 * m_rClient.Height() / 830, 392 * m_rClient.Width() / 1920, 96 * m_rClient.Height() / 830);
	m_rSearchType.SetRect(439 * m_rClient.Width() / 1920, 73 * m_rClient.Height() / 830, 554 * m_rClient.Width() / 1920, 96 * m_rClient.Height() / 830);
	m_rSearchButton.SetRect(563 * m_rClient.Width() / 1920, 71 * m_rClient.Height() / 830, 613 * m_rClient.Width() / 1920, 98 * m_rClient.Height() / 830);

	for (i = 0; i < 21; i++)
	{
		m_mdList[i].rName.SetRect((62 * m_rClient.Width() / 1920), (143 + (30 * i))* m_rClient.Height() / 830, (174 * m_rClient.Width() / 1920), (170 + (30 * i))* m_rClient.Height() / 830);
		m_mdList[i].rSex.SetRect((222 * m_rClient.Width() / 1920), (143 + (30 * i))* m_rClient.Height() / 830, (288 * m_rClient.Width() / 1920), (170 + (30 * i))* m_rClient.Height() / 830);
		m_mdList[i].rType.SetRect((338 * m_rClient.Width() / 1920), (143 + (30 * i))* m_rClient.Height() / 830, (493 * m_rClient.Width() / 1920), (170 + (30 * i))* m_rClient.Height() / 830);
		m_mdList[i].rXQ.SetRect((530 * m_rClient.Width() / 1920), (143 + (30 * i))* m_rClient.Height() / 830, (590 * m_rClient.Width() / 1920), (170 + (30 * i))* m_rClient.Height() / 830);
		m_mdList[i].rDEL.SetRect((600 * m_rClient.Width() / 1920), (143 + (30 * i))* m_rClient.Height() / 830, (660 * m_rClient.Width() / 1920), (170 + (30 * i))* m_rClient.Height() / 830);
		m_mdList[i].rEDIT.SetRect((670 * m_rClient.Width() / 1920), (143 + (30 * i))* m_rClient.Height() / 830, (730 * m_rClient.Width() / 1920), (170 + (30 * i))* m_rClient.Height() / 830);
	}
	for (i = 0; i < 4; i++)
	{
		m_alarmList[i].rZP.SetRect((810 * m_rClient.Width() / 1920), (240 + (63 * i))* m_rClient.Height() / 830, (860 * m_rClient.Width() / 1920), (293 + (63 * i))* m_rClient.Height() / 830);
		m_alarmList[i].rSim.SetRect((861 * m_rClient.Width() / 1920), (240 + (63 * i))* m_rClient.Height() / 830, (970 * m_rClient.Width() / 1920), (293 + (63 * i))* m_rClient.Height() / 830);
		m_alarmList[i].rBX.SetRect((985 * m_rClient.Width() / 1920), (240 + (63 * i))* m_rClient.Height() / 830, (1035 * m_rClient.Width() / 1920), (293 + (63 * i))* m_rClient.Height() / 830);
		m_alarmList[i].rAddr.SetRect((1036 * m_rClient.Width() / 1920), (240 + (63 * i))* m_rClient.Height() / 830, (1220 * m_rClient.Width() / 1920), (293 + (63 * i))* m_rClient.Height() / 830);
		m_alarmList[i].rTime.SetRect((1221 * m_rClient.Width() / 1920), (240 + (63 * i))* m_rClient.Height() / 830, (1390 * m_rClient.Width() / 1920), (293 + (63 * i))* m_rClient.Height() / 830);
	}
	//初始化
	m_bIsHaveTJBX = m_bIsChangeBJBX = false;
	m_iLeftCurPage = m_iLeftTotalPage = 0;
	m_iJBXXXZIndex = -1;
	m_iDisWhat = 0;
	m_bIsHaveTJBX = false;
	m_editSearchName.MoveWindow(m_rSearchName, false);
	m_editSearchType.MoveWindow(m_rSearchType, false);
	m_comboSearchSex.MoveWindow(m_rSearchSex, false);
	m_comboSearchSex.SetCurSel(0);
	HideEditControls();
	HideAddControls();
	for (i = 0; i < 21; i++)
	{
		m_mdList[i].bIsUse = false;
	}
	SetDataLeftList();
	//双缓存
	CClientDC dc(this);
	pMemDC = new CDC;
	pBitmap = new CBitmap;
	pMemDC->CreateCompatibleDC(&dc);
	pBitmap->CreateCompatibleBitmap(&dc, m_rClient.Width(), m_rClient.Height());

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CRLKGLDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (m_rBDBlack.PtInRect(point))
	{
		if (m_iCurMingdan == 0)
			return;
		m_iCurMingdan = 0;
		m_iDisWhat = 0;
		m_iJBXXXZIndex = -1;
		HideAddControls();
		HideEditControls();
		m_iLeftCurPage = 0;
		SetDataLeftList();
		Invalidate(false);
	}
	else if (m_rDTBlack.PtInRect(point))
	{
		if (m_iCurMingdan == 1)
			return;
		m_iCurMingdan = 1;
		m_iDisWhat = 0;
		m_iJBXXXZIndex = -1;
		HideAddControls();
		HideEditControls();
		m_iLeftCurPage = 0;
		SetDataLeftList();
		Invalidate(false);
	}
	else if (m_rWhite.PtInRect(point))
	{
		if (m_iCurMingdan == 2)
			return;
		m_iCurMingdan = 2;
		m_iDisWhat = 0;
		m_iJBXXXZIndex = -1;
		HideAddControls();
		HideEditControls();
		m_iLeftCurPage = 0;
		SetDataLeftList();
		Invalidate(false);
	}
	//判断鼠标是否点击到列表每条数据的详情按钮了
	int i = 0;
	for (i = 0; i < 21; i++)
	{
		if (!m_mdList[i].bIsUse)
			break;
		if (m_mdList[i].rXQ.PtInRect(point))
		{
			m_iDisWhat = 1;
			m_iJBXXXZIndex = i;
			SetDataAlarmList();
			HideAddControls();
			HideEditControls();
			Invalidate(false);
			break;
		}
	}
	//判断鼠标是否点击到列表每条数据的编辑按钮了
	for (i = 0; i < 21; i++)
	{
		if (!m_mdList[i].bIsUse)
			break;
		if (m_mdList[i].rEDIT.PtInRect(point))
		{
			m_iDisWhat = 2;
			m_iJBXXXZIndex = i;
			m_bIsChangeBJBX = false;
			HideAddControls();
			ShowEditControls();
			Invalidate(false);
			break;
		}
	}
	//判断鼠标是否点击到列表每条数据的删除按钮了
	for (i = 0; i < 21; i++)
	{
		if (!m_mdList[i].bIsUse)
			break;
		if (m_mdList[i].rDEL.PtInRect(point))
		{
			m_iDisWhat = 0;
			m_iJBXXXZIndex = i;
			ShanchuButton();
			HideAddControls();
			HideEditControls();
			Invalidate(false);
			break;
		}
	}

	//判断鼠标是否点击到编辑的保存按钮中了
	if (m_rBjbc.PtInRect(point))
	{
		if (m_iDisWhat == 2)
		{
			BianjiBaocun();
		}
	}
	//判断鼠标是否点击到添加的保存按钮中了
	if (m_rTjbc.PtInRect(point))
	{
		if (m_iDisWhat == 3)
		{
			TianjiaBaocun();
		}
	}

	//判断鼠标是否点击到“添加”按钮上了
	if (m_rButtonAdd.PtInRect(point))
	{
		if (m_iCurMingdan == 1)
		{
			MessageBox(L"动态黑名单不允许手动添加", L"提示", MB_OK | MB_ICONINFORMATION);
			return;
		}
		if (m_iDisWhat != 3)
		{
			m_iDisWhat = 3;
			m_iJBXXXZIndex = -1;
			m_bIsHaveTJBX = false;
			HideEditControls();
			ShowAddControls();
			Invalidate(false);
		}
	}
	//判断鼠标是否点击到“添加”功能的照片位置
	if (m_rTjbx.PtInRect(point))
	{
		if (m_iDisWhat == 3)
		{
			if (SelectPic())
			{
				m_bIsHaveTJBX = true;
				Invalidate(false);
			}
			else
				MessageBox(L"照片载入失败", L"提示", MB_OK | MB_ICONINFORMATION);
		}
	}
	//判断鼠标是否点击到“编辑”功能的照片位置
	if (m_rTjbx.PtInRect(point))
	{
		if (m_iDisWhat == 2)
		{
			if (SelectPic())
			{
				m_bIsChangeBJBX = true;
				Invalidate(false);
			}
			else
				MessageBox(L"照片载入失败", L"提示", MB_OK | MB_ICONINFORMATION);
		}
	}

	//判断是否点击到“详情”的页码了
	//首页
	if (m_rAlarmBeginPage.PtInRect(point))
	{
		if (m_iDisWhat == 1)
		{
			m_iCurAlarmPage = 0;
			Invalidate(false);
		}
	}
	//下一页
	if (m_rAlarmNextPage.PtInRect(point))
	{
		if (m_iDisWhat == 1)
		{
			if ((m_iCurAlarmPage + 1) * 4 < m_iTotalAlarm)
				m_iCurAlarmPage++;
			Invalidate(false);
		}
	}
	//上一页
	if (m_rAlarmPrePage.PtInRect(point))
	{
		if (m_iDisWhat == 1)
		{
			if (m_iCurAlarmPage > 0)
				m_iCurAlarmPage--;
			Invalidate(false);
		}
	}
	//末页
	if (m_rAlarmEndPage.PtInRect(point))
	{
		if (m_iDisWhat == 1)
		{
			int iCurPage = m_iTotalAlarm / 4;
			int iYu = m_iTotalAlarm % 4;
			if (iYu != 0)
				m_iCurAlarmPage = iCurPage;
			Invalidate(false);
		}
	}
	//判断是否点击到人脸列表的页码了
	//首页
	if (m_rLeftBeginPage.PtInRect(point))
	{
		if (m_iLeftCurPage != 0)
			m_iLeftCurPage = 0;
		SetDataLeftList();
		Invalidate(false);
	}
	//上一页
	if (m_rLeftPrePage.PtInRect(point))
	{
		if (m_iLeftCurPage > 0)
			m_iLeftCurPage--;
		SetDataLeftList();
		Invalidate(false);
	}
	//下一页
	if (m_rLeftNextPage.PtInRect(point))
	{
		if (m_iLeftCurPage < m_iLeftTotalPage)
			m_iLeftCurPage++;
		SetDataLeftList();
		Invalidate(false);
	}
	//末页
	if (m_rLeftEndPage.PtInRect(point))
	{
		if (m_iLeftCurPage != m_iLeftTotalPage)
			m_iLeftCurPage = m_iLeftTotalPage;
		SetDataLeftList();
		Invalidate(false);
	}

	//判断是否点击“搜索”按钮
	if (m_rSearchButton.PtInRect(point))
	{
		m_iLeftCurPage = 0;
		SetDataLeftList();
		Invalidate(false);
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}
void CRLKGLDlg::ShanchuButton()
{
	if (!ServerDeletePerson(m_mdList[m_iJBXXXZIndex].strPersonID, m_mdList[m_iJBXXXZIndex].strIDCard))
	{
		MessageBox(L"删除人员失败", L"提示", MB_OK | MB_ICONINFORMATION);
	}
	MessageBox(L"删除人员成功", L"提示", MB_OK | MB_ICONINFORMATION);
	SetDataLeftList();
}
BOOL CRLKGLDlg::SelectPic()
{
	USES_CONVERSION;
	CFileDialog getfilepathdlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"All Files(*.jpg||", AfxGetMainWnd());
	if (getfilepathdlg.DoModal() == IDOK)
	{
		CString strAllPath = getfilepathdlg.GetPathName();
		CFile picfile;
		picfile.Open(strAllPath, CFile::modeRead);
		int filelen = picfile.GetLength();
		if (filelen > 800 * 1024)
		{
			MessageBox(L"照片文件不能超过800K", L"提示", MB_OK | MB_ICONINFORMATION);
			picfile.Close();
			return false;
		}
		picfile.Close();
		char chSession[40];
		memset(chSession, 0, 40);
		if (DeviceLogin(T2A(theApp.m_strServerIP), chSession) != 0)
			return false;
		char chJson[1024 * 1024];
		memset(chJson, 0, 1024 * 1024);
		if (FASFeatureDetectforImageFile(T2A(theApp.m_strServerIP), chSession, T2A(strAllPath), chJson, 1024 * 1024) != 0)
			return false;
		DeviceLogout(T2A(theApp.m_strServerIP), chSession);
		Json::Reader reader;
		Json::Value root;
		Json::Value result;
		Json::Value content;
		BOOL readre = reader.parse(chJson, root);
		if (!readre)
			return false;
		int iZero = 0;
		result = root["data"]["result"][iZero];
		if (result.isNull())
			return false;
		CString strFeature, strFaceImage;
		strFeature = result["feature"].asCString();
		strFaceImage = result["face_image"].asCString();
		int iSrcLen = strFaceImage.GetLength();
		char * dst = new char[iSrcLen];
		memset(dst, 0, iSrcLen);
		CTools tl;
		int iDstLen = tl.DecodeBase64(T2A(strFaceImage), (unsigned char*)dst, iSrcLen);
		CFile file;
		if (m_iDisWhat == 2)
			file.Open(L"data/rlkgl_bjbx.jpg", CFile::modeCreate | CFile::modeWrite);
		else if (m_iDisWhat == 3)
			file.Open(L"data/rlkgl_tjbx.jpg", CFile::modeCreate | CFile::modeWrite);
		file.Write(dst, iDstLen);
		file.Close();
		if (m_iDisWhat == 2)
			file.Open(L"data/rlkgl_bjfeature.txt", CFile::modeCreate | CFile::modeWrite);
		else if (m_iDisWhat == 3)
			file.Open(L"data/rlkgl_tjfeature.txt", CFile::modeCreate | CFile::modeWrite);
		file.Write(T2A(strFeature), strFeature.GetLength());
		file.Close();
		delete[] dst;
		return true;
	}
	return false;
}
void CRLKGLDlg::BianjiBaocun()
{
	CString strName, strSex, strBirth, strLibraryid, strNativeplace, strPhone, strType, strAddress;
	m_editName.GetWindowTextW(strName);
	if (strName.IsEmpty())
	{
		MessageBox(L"姓名不能为空", L"提示", MB_OK | MB_ICONINFORMATION);
		return;
	}
	m_comboSex.GetLBText(m_comboSex.GetCurSel(), strSex);
	m_editBirth.GetWindowTextW(strBirth);
	int iLID = m_comboFacedb.GetCurSel();
	if ((m_iCurMingdan == 0 || m_iCurMingdan == 2) && iLID == 1)
	{
		MessageBox(L"不允许将本地黑名单和本地白名单的人员移动至动态黑名单", L"提示", MB_OK | MB_ICONINFORMATION);
		return;
	}
	if (iLID == 0)
		strLibraryid = L"1";
	else if (iLID == 1)
		strLibraryid = L"3";
	else if (iLID == 2)
		strLibraryid = L"2";
	m_editNativeplace.GetWindowTextW(strNativeplace);
	m_editPhone.GetWindowTextW(strPhone);
	m_editType.GetWindowTextW(strType);
	m_editAddress.GetWindowTextW(strAddress);
	USES_CONVERSION;
	if (!m_bIsChangeBJBX)//没换脸
	{
		if (iLID != m_iCurMingdan)//换了库
		{
			//提取fea
			char chSession[40];
			memset(chSession, 0, 40);
			if (DeviceLogin(T2A(theApp.m_strServerIP), chSession) != 0)
			{
				MessageBox(L"保存失败", L"提示", MB_OK | MB_ICONINFORMATION);
				return;
			}
			char chJson[1024 * 1024];
			memset(chJson, 0, 1024 * 1024);
			CString strAllPath = L"data/rlkgl_bjbx.jpg";
			if (FASFeatureDetectforImageFile(T2A(theApp.m_strServerIP), chSession, T2A(strAllPath), chJson, 1024 * 1024) != 0)
			{
				DeviceLogout(T2A(theApp.m_strServerIP), chSession);
				MessageBox(L"保存失败", L"提示", MB_OK | MB_ICONINFORMATION);
				return;
			}
			DeviceLogout(T2A(theApp.m_strServerIP), chSession);
			Json::Reader reader;
			Json::Value root;
			Json::Value result;
			Json::Value content;
			BOOL readre = reader.parse(chJson, root);
			if (!readre)
			{
				MessageBox(L"保存失败", L"提示", MB_OK | MB_ICONINFORMATION);
				return;
			}
			int iZero = 0;
			result = root["data"]["result"][iZero];
			if (result.isNull())
			{
				MessageBox(L"保存失败", L"提示", MB_OK | MB_ICONINFORMATION);
				return;
			}
			CString strFeature;
			strFeature = result["feature"].asCString();
			CFile file;
			file.Open(L"data/rlkgl_bjfeature.txt", CFile::modeCreate | CFile::modeWrite);
			file.Write(T2A(strFeature), strFeature.GetLength());
			file.Close();

			//删除
			if (!ServerDeletePerson(m_mdList[m_iJBXXXZIndex].strPersonID, m_mdList[m_iJBXXXZIndex].strIDCard))
			{
				MessageBox(L"保存失败", L"提示", MB_OK | MB_ICONINFORMATION);
				return;
			}
			//添加
			if (!ServerAddPerson(strName, strSex, strBirth, strLibraryid, strNativeplace, strPhone, strAddress, strType))
			{
				MessageBox(L"保存失败", L"提示", MB_OK | MB_ICONINFORMATION);
				return;
			}
		}
		else//没换库
		{
			//更新数据库
			_ConnectionPtr conn;
			conn.CreateInstance(__uuidof(Connection));
			_RecordsetPtr pSet;
			pSet.CreateInstance(__uuidof(Recordset));
			CString strConn;
			strConn.Format(_T("Driver={MySQL ODBC 5.1 Driver}; Server=%s;Database=%s; uid=%s; pwd=%s;"), theApp.m_strDBIP, theApp.m_strDBName, theApp.m_strDBUsername, theApp.m_strDBPassword);
			try
			{
				conn->Open(_bstr_t(strConn), "", "", adModeUnknown);
			}
			catch (_com_error e)
			{
				MessageBox(L"数据库访问失败", L"提示", MB_OK | MB_ICONINFORMATION);
				return;
			}
			CString strSQL;
			strSQL.Format(_T("select * from employeeinfo where id_card='%s'"), m_mdList[m_iJBXXXZIndex].strIDCard);
			pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
			pSet->put_Collect(_variant_t("name"), _variant_t(strName));
			CString strRealSex;
			if (strSex == L"男")
				strRealSex = L"male";
			else
				strRealSex = L"female";
			pSet->put_Collect(_variant_t("sex"), _variant_t(strRealSex));
			pSet->put_Collect(_variant_t("birth"), _variant_t(strBirth));
			pSet->put_Collect(_variant_t("nativeplace"), _variant_t(strNativeplace));
			pSet->put_Collect(_variant_t("phone"), _variant_t(strPhone));
			pSet->put_Collect(_variant_t("address"), _variant_t(strAddress));
			pSet->put_Collect(_variant_t("type"), _variant_t(strType));
			pSet->Update();
			pSet->Close();
			conn->Close();
		}
	}
	else//换了脸
	{
		//删除
		if (!ServerDeletePerson(m_mdList[m_iJBXXXZIndex].strPersonID, m_mdList[m_iJBXXXZIndex].strIDCard))
		{
			MessageBox(L"保存失败", L"提示", MB_OK | MB_ICONINFORMATION);
			return;
		}
		//添加
		if (!ServerAddPerson(strName, strSex, strBirth, strLibraryid, strNativeplace, strPhone, strAddress, strType))
		{
			MessageBox(L"保存失败", L"提示", MB_OK | MB_ICONINFORMATION);
			return;
		}
	}
	MessageBox(L"编辑人员成功", L"提示", MB_OK | MB_ICONINFORMATION);
	m_iDisWhat = 0;
	SetDataLeftList();
	HideEditControls();
	Invalidate(false);
	return;
}
void CRLKGLDlg::TianjiaBaocun()
{
	if (!m_bIsHaveTJBX)
	{
		MessageBox(L"请选择人员照片", L"提示", MB_OK | MB_ICONINFORMATION);
		return;
	}
	CString strName, strSex, strBirth, strLibraryid, strNativeplace, strPhone, strType, strAddress;
	m_editName.GetWindowTextW(strName);
	if (strName.IsEmpty())
	{
		MessageBox(L"姓名不能为空", L"提示", MB_OK | MB_ICONINFORMATION);
		return;
	}
	m_comboSex.GetLBText(m_comboSex.GetCurSel(), strSex);
	m_editBirth.GetWindowTextW(strBirth);
	if (m_iCurMingdan == 0)
		strLibraryid = L"1";
	else if (m_iCurMingdan == 2)
		strLibraryid = L"2";
	m_editNativeplace.GetWindowTextW(strNativeplace);
	m_editPhone.GetWindowTextW(strPhone);
	m_editType.GetWindowTextW(strType);
	m_editAddress.GetWindowTextW(strAddress);

	if (ServerAddPerson(strName, strSex, strBirth, strLibraryid, strNativeplace, strPhone, strAddress, strType))
	{
		MessageBox(L"人员添加成功", L"提示", MB_OK | MB_ICONINFORMATION);
		m_iDisWhat = 0;
		SetDataLeftList();
		HideAddControls();
		Invalidate(false);
	}
	else
	{
		MessageBox(L"人员添加失败", L"提示", MB_OK | MB_ICONINFORMATION);
	}
}
BOOL CRLKGLDlg::ServerAddPerson(CString strName, CString strSex, CString strBirth, CString strLibraryID, CString strNativeplace, CString strPhone, CString strAddress, CString strType)
{
	USES_CONVERSION;
	CString strIDCard;
	_ConnectionPtr conn;
	conn.CreateInstance(__uuidof(Connection));
	_RecordsetPtr pSet;
	pSet.CreateInstance(__uuidof(Recordset));
	CString strConn;
	strConn.Format(_T("Driver={MySQL ODBC 5.1 Driver}; Server=%s;Database=%s; uid=%s; pwd=%s;"), theApp.m_strDBIP, theApp.m_strDBName, theApp.m_strDBUsername, theApp.m_strDBPassword);
	try
	{
		conn->Open(_bstr_t(strConn), "", "", adModeUnknown);
	}
	catch (_com_error e)
	{
		MessageBox(L"数据库访问失败", L"提示", MB_OK | MB_ICONINFORMATION);
		return false;
	}
	CString strSQL;
	strSQL.Format(_T("select max(id_card) as max from employeeinfo where libraryid in (1,2,3,4)"));
	pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	_variant_t vtMax = pSet->GetCollect(_variant_t("max"));
	if (vtMax.vt == VT_NULL)
		strIDCard = L"0000000000";
	else
	{
		CString strCurMaxIDCard;
		strCurMaxIDCard = (char*)_bstr_t(vtMax);
		int iCurMaxIDCard = atoi(T2A(strCurMaxIDCard));
		strIDCard.Format(L"%010d", iCurMaxIDCard + 1);
	}
	pSet->Close();

	char chSession[40];
	memset(chSession, 0, 40);
	if (DeviceLogin(T2A(theApp.m_strServerIP), chSession) != 0)
	{
		MessageBox(L"入库失败", L"提示", MB_OK | MB_ICONINFORMATION);
		return false;
	}
	/*	char chJson[1024];
		memset(chJson, 0, 1024);
		strIDCard = L"8";
		FASGetFaceListPageInDb(T2A(theApp.m_strServerIP), chSession, T2A(theApp.m_strBID), T2A(strIDCard), chJson, 1024);
		DeviceLogout(T2A(theApp.m_strServerIP), chSession);**/
	long lFacedbID;
	if (strLibraryID == L"1")
		lFacedbID = atoi(T2A(theApp.m_strBID));
	else if (strLibraryID == L"2")
		lFacedbID = atoi(T2A(theApp.m_strWID));
	else if (strLibraryID == L"3")
		lFacedbID = atoi(T2A(theApp.m_strTBID));
	CFile file;
	if (m_iDisWhat == 3)
		file.Open(L"data/rlkgl_tjfeature.txt", CFile::modeRead);
	else if (m_iDisWhat == 2)
		file.Open(L"data/rlkgl_bjfeature.txt", CFile::modeRead);
	int filelen = file.GetLength();
	char *chFile = new char[filelen + 1];
	memset(chFile, 0, filelen + 1);
	file.Read(chFile, filelen);
	file.Close();
	CString strFea;
	strFea = chFile;
	char chImage[2] = { '1', '\0' };
	CString strValid = L"2099-12-31";
	CString strBirthday = L"1980-01-01";
	char chPersonID[20];
	memset(chPersonID, 0, 20);
	if (FASAddPersoninfo(T2A(theApp.m_strServerIP), chSession, T2A(strIDCard), T2A(strIDCard), 0, lFacedbID, T2A(strFea), chImage, T2A(strBirthday), T2A(strValid), chPersonID, 20) != 0)
	{
		delete[] chFile;
		DeviceLogout(T2A(theApp.m_strServerIP), chSession);
		MessageBox(L"入库失败", L"提示", MB_OK | MB_ICONINFORMATION);
		return false;
	}
	delete[] chFile;
	DeviceLogout(T2A(theApp.m_strServerIP), chSession);
	CString strPersonID;
	strPersonID = chPersonID;
	strSQL.Format(_T("select * from employeeinfo limit 0,1"));
	pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	pSet->AddNew();
	pSet->put_Collect(_variant_t("name"), _variant_t(strName));
	CString strRealSex;
	if (strSex == L"男")
		strRealSex = L"male";
	else
		strRealSex = L"female";
	pSet->put_Collect(_variant_t("sex"), _variant_t(strRealSex));
	pSet->put_Collect(_variant_t("birth"), _variant_t(strBirth));
	pSet->put_Collect(_variant_t("id_card"), _variant_t(strIDCard));
	pSet->put_Collect(_variant_t("libraryid"), _variant_t(strLibraryID));
	pSet->put_Collect(_variant_t("nativeplace"), _variant_t(strNativeplace));
	pSet->put_Collect(_variant_t("phone"), _variant_t(strPhone));
	pSet->put_Collect(_variant_t("address"), _variant_t(strAddress));
	pSet->put_Collect(_variant_t("type"), _variant_t(strType));
	pSet->put_Collect(_variant_t("personid"), _variant_t(strPersonID));
	pSet->Update();
	pSet->Close();

	strSQL.Format(_T("select * from imginfo limit 0,1"));
	pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	pSet->AddNew();
	pSet->put_Collect(_variant_t("id_card"), _variant_t(strIDCard));
	CString strImgpath = L"/Data/bx/" + strIDCard + L".jpg";
	pSet->put_Collect(_variant_t("imgpath"), _variant_t(strImgpath));
	pSet->Update();
	pSet->Close();
	conn->Close();

	SOCKET sockClient;
	sockClient = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(T2A(theApp.m_strDBIP));
	//	addrSrv.sin_addr.S_un.S_addr = inet_addr(T2A(L"192.168.2.153"));
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(29001);

	int re = 0;
	re = connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	CString strSend;
	strSend.Format(L"add%s", strIDCard);
	if (m_iDisWhat == 3)
		file.Open(L"data/rlkgl_tjbx.jpg", CFile::modeRead);
	else if (m_iDisWhat == 2)
		file.Open(L"data/rlkgl_bjbx.jpg", CFile::modeRead);
	int imagefilelen = file.GetLength();
	char * sendImage = new char[imagefilelen + 1];
	memset(sendImage, 0, imagefilelen + 1);
	file.Read(sendImage, imagefilelen);
	file.Close();
	if (re == 0)
	{
		send(sockClient, T2A(strSend), 13, 0);
		CString strFL;
		strFL.Format(L"%010d", imagefilelen);
		send(sockClient, T2A(strFL), 10, 0);
		int allsendlen = 0;
		while (true)
		{
			int curSendlen = imagefilelen - allsendlen;
			if (curSendlen > 50 * 1024)
				curSendlen = 50 * 1024;
			int sendre = send(sockClient, sendImage + allsendlen, curSendlen, 0);
			TRACE(L"sendre = %d\n", sendre);
			allsendlen += sendre;
			if (allsendlen == imagefilelen)
				break;
		}
		TRACE(L"allsendlen = %d\n", allsendlen);
	}
	closesocket(sockClient);

	return true;
}

BOOL CRLKGLDlg::ServerDeletePerson(CString strPersonID, CString strIDCard)
{
	USES_CONVERSION;
	char chSession[40];
	memset(chSession, 0, 40);
	if (DeviceLogin(T2A(theApp.m_strServerIP), chSession) != 0)
	{
		MessageBox(L"删除失败", L"提示", MB_OK | MB_ICONINFORMATION);
		return false;
	}
	if (FASDELETEPersoninfo(T2A(theApp.m_strServerIP), chSession, T2A(strPersonID)) != 0)
	{
		DeviceLogout(T2A(theApp.m_strServerIP), chSession);
		MessageBox(L"删除失败", L"提示", MB_OK | MB_ICONINFORMATION);
		return false;
	}
	DeviceLogout(T2A(theApp.m_strServerIP), chSession);
	_ConnectionPtr conn;
	conn.CreateInstance(__uuidof(Connection));
	_RecordsetPtr pSet;
	pSet.CreateInstance(__uuidof(Recordset));
	CString strConn;
	strConn.Format(_T("Driver={MySQL ODBC 5.1 Driver}; Server=%s;Database=%s; uid=%s; pwd=%s;"), theApp.m_strDBIP, theApp.m_strDBName, theApp.m_strDBUsername, theApp.m_strDBPassword);
	try
	{
		conn->Open(_bstr_t(strConn), "", "", adModeUnknown);
	}
	catch (_com_error e)
	{
		MessageBox(L"数据库访问失败", L"提示", MB_OK | MB_ICONINFORMATION);
		return false;
	}
	CString strSQL;
	strSQL.Format(_T("select * from employeeinfo where id_card='%s'"), strIDCard);
	pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	pSet->Delete(adAffectCurrent);
	pSet->Update();
	pSet->Close();
	strSQL.Format(_T("select * from imginfo where id_card='%s'"), strIDCard);
	pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	pSet->Delete(adAffectCurrent);
	pSet->Update();
	pSet->Close();
	conn->Close();

	SOCKET sockClient;
	sockClient = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(T2A(theApp.m_strDBIP));
	//	addrSrv.sin_addr.S_un.S_addr = inet_addr(T2A(L"192.168.2.153"));
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(29001);

	int re = 0;
	re = connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	CString strSend;
	strSend.Format(L"del%s", strIDCard);
	char chSend[14];
	memset(chSend, 0, 14);
	strcpy_s(chSend, T2A(strSend));
	if (re == 0)
		send(sockClient, chSend, 13, 0);
	closesocket(sockClient);

	return true;
}
void CRLKGLDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	CBitmap * pOldBitmap = pMemDC->SelectObject(pBitmap);
	pMemDC->PatBlt(0, 0, m_rClient.Width(), m_rClient.Height(), PATCOPY);
	Graphics g(pMemDC->m_hDC);
	//画左侧列表及右侧背景
	if (m_iCurMingdan == 0)
	{
		Image img_bk(L"img/rlkgl_bdhmd.jpg");
		g.DrawImage(&img_bk, 0, 0, m_rClient.Width(), m_rClient.Height());
	}
	else if (m_iCurMingdan == 1)
	{
		Image img_bk(L"img/rlkgl_dthmd.jpg");
		g.DrawImage(&img_bk, 0, 0, m_rClient.Width(), m_rClient.Height());
	}
	else if (m_iCurMingdan == 2)
	{
		Image img_bk(L"img/rlkgl_bmd.jpg");
		g.DrawImage(&img_bk, 0, 0, m_rClient.Width(), m_rClient.Height());
	}
	if (m_iDisWhat == 1)
	{
		//画基本信息详情框
		Image img_jbxxxq(L"img/rlkgl_jibenxinxixiangqing.jpg");
		g.DrawImage(&img_jbxxxq, m_rJibenxinxi.left, m_rJibenxinxi.top, m_rJibenxinxi.Width(), m_rJibenxinxi.Height());
		if (m_iJBXXXZIndex != -1)
			DisJBXXXQ(&g, m_iJBXXXZIndex);
	}
	else if (m_iDisWhat == 2)
	{
		//画基本信息编辑框
		Image img_jbxxbj(L"img/rlkgl_jibenxinxibianji.jpg");
		g.DrawImage(&img_jbxxbj, m_rBianji.left, m_rBianji.top, m_rBianji.Width(), m_rBianji.Height());
		if (m_iJBXXXZIndex != -1)
			DisJBXXBJ(&g, m_iJBXXXZIndex);
	}
	else if (m_iDisWhat == 3)
	{
		//画基本信息添加框
		Image img_jbxxtj(L"img/rlkgl_jibenxinxitianjia.jpg");
		g.DrawImage(&img_jbxxtj, m_rAdd.left, m_rAdd.top, m_rAdd.Width(), m_rAdd.Height());
		if (m_bIsHaveTJBX)
		{
			Image img_tjbx(L"data/rlkgl_tjbx.jpg");
			g.DrawImage(&img_tjbx, m_rTjbx.left, m_rTjbx.top, m_rTjbx.Width(), m_rTjbx.Height());
		}
	}
	//测试位置
	int i = 0;
	Pen redPen(Color(255, 0, 0), 1);
	//	g.DrawRectangle(&redPen, m_rBjbx.left, m_rBjbx.top, m_rBjbx.Width(), m_rBjbx.Height());
	/*	g.DrawRectangle(&redPen, m_rAlarmBeginPage.left, m_rAlarmBeginPage.top, m_rAlarmBeginPage.Width(), m_rAlarmBeginPage.Height());
		g.DrawRectangle(&redPen, m_rSearchName.left, m_rSearchName.top, m_rSearchName.Width(), m_rSearchName.Height());
		g.DrawRectangle(&redPen, m_rSearchSex.left, m_rSearchSex.top, m_rSearchSex.Width(), m_rSearchSex.Height());
		g.DrawRectangle(&redPen, m_rSearchType.left, m_rSearchType.top, m_rSearchType.Width(), m_rSearchType.Height());
		g.DrawRectangle(&redPen, m_rSearchButton.left, m_rSearchButton.top, m_rSearchButton.Width(), m_rSearchButton.Height());*/
	for (i = 0; i < 4; i++)
	{
		//		g.DrawRectangle(&redPen, m_rJbxxxq[i].left, m_rJbxxxq[i].top, m_rJbxxxq[i].Width(), m_rJbxxxq[i].Height());
		/*		g.DrawRectangle(&redPen, m_alarmList[i].rZP.left, m_alarmList[i].rZP.top, m_alarmList[i].rZP.Width(), m_alarmList[i].rZP.Height());
				g.DrawRectangle(&redPen, m_alarmList[i].rSim.left, m_alarmList[i].rSim.top, m_alarmList[i].rSim.Width(), m_alarmList[i].rSim.Height());
				g.DrawRectangle(&redPen, m_alarmList[i].rBX.left, m_alarmList[i].rBX.top, m_alarmList[i].rBX.Width(), m_alarmList[i].rBX.Height());
				g.DrawRectangle(&redPen, m_alarmList[i].rAddr.left, m_alarmList[i].rAddr.top, m_alarmList[i].rAddr.Width(), m_alarmList[i].rAddr.Height());
				g.DrawRectangle(&redPen, m_alarmList[i].rTime.left, m_alarmList[i].rTime.top, m_alarmList[i].rTime.Width(), m_alarmList[i].rTime.Height());*/
	}

	DisLeftList(&g);
	/*	DisNum(&g);
		DisToday(&g);
		DisSnap(&g);
		DisTongji(&g);
		DisHeimingdan(&g);*/
	//显示缓存，清除HDC
	dc.BitBlt(0, 0, m_rClient.Width(), m_rClient.Height(), pMemDC, 0, 0, SRCCOPY);
	pMemDC->SelectObject(pOldBitmap);
	g.ReleaseHDC(pMemDC->m_hDC);
	// 不为绘图消息调用 CDialogEx::OnPaint()
}

void CRLKGLDlg::DisLeftList(Graphics *g)
{
	int i = 0;
	Image imageXQ(L"img/rlkgl_xiangqing.jpg");
	Image imageDEL(L"img/rlkgl_shanchu.jpg");
	Image imageEDIT(L"img/rlkgl_bianji.jpg");
	Gdiplus::Font font(L"方正细倩简体", m_rClient.Width() * 9 / 1000, FontStyleRegular, UnitPixel);
	Gdiplus::Font font1(L"方正细倩简体", m_rClient.Width() * 6 / 1000, FontStyleRegular, UnitPixel);
	SolidBrush percentBrush(Color(255, 212, 214, 222));
	StringFormat stringformat;
	stringformat.SetAlignment(StringAlignmentCenter);
	stringformat.SetLineAlignment(StringAlignmentCenter);
	for (i = 0; i < 21; i++)
	{
		if (!m_mdList[i].bIsUse)
			break;
		g->DrawString(m_mdList[i].strName.AllocSysString(), -1, &font, RectF(m_mdList[i].rName.left, m_mdList[i].rName.top, m_mdList[i].rName.Width(), m_mdList[i].rName.Height()), &stringformat, &percentBrush);
		g->DrawString(m_mdList[i].strSex.AllocSysString(), -1, &font, RectF(m_mdList[i].rSex.left, m_mdList[i].rSex.top, m_mdList[i].rSex.Width(), m_mdList[i].rSex.Height()), &stringformat, &percentBrush);
		g->DrawString(m_mdList[i].strType.AllocSysString(), -1, &font, RectF(m_mdList[i].rType.left, m_mdList[i].rType.top, m_mdList[i].rType.Width(), m_mdList[i].rType.Height()), &stringformat, &percentBrush);
		g->DrawImage(&imageXQ, m_mdList[i].rXQ.left, m_mdList[i].rXQ.top, m_mdList[i].rXQ.Width(), m_mdList[i].rXQ.Height());
		g->DrawImage(&imageDEL, m_mdList[i].rDEL.left, m_mdList[i].rDEL.top, m_mdList[i].rDEL.Width(), m_mdList[i].rDEL.Height());
		g->DrawImage(&imageEDIT, m_mdList[i].rEDIT.left, m_mdList[i].rEDIT.top, m_mdList[i].rEDIT.Width(), m_mdList[i].rEDIT.Height());
	}
	CString strCurPage, strTotalPage;
	strCurPage.Format(L"%d", m_iLeftCurPage + 1);
	strTotalPage.Format(L"%d", m_iLeftTotalPage + 1);
	g->DrawString(strCurPage.AllocSysString(), -1, &font1, RectF(m_rLeftCurPage.left, m_rLeftCurPage.top, m_rLeftCurPage.Width(), m_rLeftCurPage.Height()), &stringformat, &percentBrush);
	g->DrawString(strTotalPage.AllocSysString(), -1, &font1, RectF(m_rLeftTotalPage.left, m_rLeftTotalPage.top, m_rLeftTotalPage.Width(), m_rLeftTotalPage.Height()), &stringformat, &percentBrush);
}

void CRLKGLDlg::DisJBXXXQ(Graphics *g, int iIndex)
{
	CString strDis[3];
	strDis[0].Format(L"姓名  %s    性别  %s    编号  %s", m_mdList[iIndex].strName, m_mdList[iIndex].strSex, m_mdList[iIndex].strIDCard);
	strDis[1].Format(L"出生日期  %s    最后更新  %s", m_mdList[iIndex].strBirth, m_mdList[iIndex].strInsertTime);
	strDis[2].Format(L"住址  %s", m_mdList[iIndex].strAddress);
	Gdiplus::Font font(L"方正细倩简体", m_rClient.Width() * 9 / 1000, FontStyleRegular, UnitPixel);
	Gdiplus::Font font1(L"方正细倩简体", m_rClient.Width() * 9 / 1000, FontStyleRegular, UnitPixel);
	SolidBrush percentBrush(Color(255, 212, 214, 222));
	StringFormat stringformat;
	stringformat.SetAlignment(StringAlignmentNear);
	stringformat.SetLineAlignment(StringAlignmentCenter);
	StringFormat stringformat1;
	stringformat1.SetAlignment(StringAlignmentCenter);
	stringformat1.SetLineAlignment(StringAlignmentCenter);
	int i = 0;
	for (i = 0; i < 3; i++)
		g->DrawString(strDis[i].AllocSysString(), -1, &font, RectF(m_rJbxxxq[i].left, m_rJbxxxq[i].top, m_rJbxxxq[i].Width(), m_rJbxxxq[i].Height()), &stringformat, &percentBrush);
	CTools tl;
	CString strSrc, strDst;
	//	strSrc.Format(L"/Data/bx/%s.jpg", m_mdList[iIndex].strIDCard);
	strSrc = m_mdList[iIndex].strBXPath;
	strDst.Format(L"Data/rlkgl_jbbx.jpg");
	tl.DownloadImageFromFileServer(theApp.m_strDBIP, 10000, strSrc, strDst);
	Image imgBX(L"Data/rlkgl_jbbx.jpg");
	g->DrawImage(&imgBX, m_rJbbx.left, m_rJbbx.top, m_rJbbx.Width(), m_rJbbx.Height());
	for (i = 0; i < 4; i++)
	{
		if (m_iCurAlarmPage * 4 + i >= m_iTotalAlarm)
			break;
		CString strDst;
		strDst.Format(L"Data/rlkgl_alarmzp%d.jpg", i);
		tl.DownloadImageFromFileServer(theApp.m_strDBIP, 10000, m_alarmdata[m_iCurAlarmPage * 4 + i].strSrc, strDst);
		Image imgAlarmZP(strDst);
		g->DrawImage(&imgBX, m_alarmList[i].rBX.left, m_alarmList[i].rBX.top, m_alarmList[i].rBX.Width(), m_alarmList[i].rBX.Height());
		g->DrawImage(&imgAlarmZP, m_alarmList[i].rZP.left, m_alarmList[i].rZP.top, m_alarmList[i].rZP.Width(), m_alarmList[i].rZP.Height());
		g->DrawString(m_alarmdata[m_iCurAlarmPage * 4 + i].strSim.AllocSysString(), -1, &font1, RectF(m_alarmList[i].rSim.left, m_alarmList[i].rSim.top, m_alarmList[i].rSim.Width(), m_alarmList[i].rSim.Height()), &stringformat1, &percentBrush);
		g->DrawString(m_alarmdata[m_iCurAlarmPage * 4 + i].strAddr.AllocSysString(), -1, &font1, RectF(m_alarmList[i].rAddr.left, m_alarmList[i].rAddr.top, m_alarmList[i].rAddr.Width(), m_alarmList[i].rAddr.Height()), &stringformat1, &percentBrush);
		g->DrawString(m_alarmdata[m_iCurAlarmPage * 4 + i].strAlarmTime.AllocSysString(), -1, &font1, RectF(m_alarmList[i].rTime.left, m_alarmList[i].rTime.top, m_alarmList[i].rTime.Width(), m_alarmList[i].rTime.Height()), &stringformat1, &percentBrush);
	}
	CString strCurPage;
	strCurPage.Format(L"%d", m_iCurAlarmPage + 1);
	g->DrawString(strCurPage.AllocSysString(), -1, &font1, RectF(m_rAlarmCurPage.left, m_rAlarmCurPage.top, m_rAlarmCurPage.Width(), m_rAlarmCurPage.Height()), &stringformat1, &percentBrush);
}

void CRLKGLDlg::DisJBXXBJ(Graphics *g, int iIndex)
{
	m_editName.SetWindowTextW(m_mdList[iIndex].strName);
	m_editBirth.SetWindowTextW(m_mdList[iIndex].strBirth);
	m_editPhone.SetWindowTextW(m_mdList[iIndex].strPhone);
	m_editType.SetWindowTextW(m_mdList[iIndex].strType);
	m_editNativeplace.SetWindowTextW(m_mdList[iIndex].strNativeplace);
	m_editAddress.SetWindowTextW(m_mdList[iIndex].strAddress);
	if (m_mdList[iIndex].strSex == L"男")
		m_comboSex.SetCurSel(0);
	else
		m_comboSex.SetCurSel(1);
	m_comboFacedb.SetCurSel(m_iCurMingdan);
	if (!m_bIsChangeBJBX)
	{
		CTools tl;
		CString strSrc, strDst;
		//		strSrc.Format(L"/Data/bx/%s.jpg", m_mdList[iIndex].strIDCard);
		strSrc = m_mdList[iIndex].strBXPath;
		strDst.Format(L"Data/rlkgl_bjbx.jpg");
		tl.DownloadImageFromFileServer(theApp.m_strDBIP, 10000, strSrc, strDst);
	}
	Image imgBX(L"Data/rlkgl_bjbx.jpg");
	g->DrawImage(&imgBX, m_rBjbx.left, m_rBjbx.top, m_rBjbx.Width(), m_rBjbx.Height());
}

void CRLKGLDlg::SetDataLeftList()
{
	CString strSearchName, strSearchSex, strSearchType, strAnd;
	m_editSearchName.GetWindowTextW(strSearchName);
	m_editSearchType.GetWindowTextW(strSearchType);
	m_comboSearchSex.GetLBText(m_comboSearchSex.GetCurSel(), strSearchSex);
	strSearchName.Trim();
	strSearchType.Trim();
	if (strSearchSex == L"全部")
		strAnd = L"and sex in ('male','female')";
	else if (strSearchSex == L"男")
		strAnd = L"and sex in ('male')";
	else if (strSearchSex == L"女")
		strAnd = L"and sex in ('female')";
	if (!strSearchName.IsEmpty())
		strAnd += L" and name like '%" + strSearchName + L"%'";
	if (!strSearchType.IsEmpty())
		strAnd += L" and type like '%" + strSearchType + L"%'";

	int i = 0;
	for (i = 0; i < 21; i++)
		m_mdList[i].bIsUse = false;

	// 	_ConnectionPtr conn;
	// 	conn.CreateInstance(__uuidof(Connection));
	_RecordsetPtr pSet;
	// 	pSet.CreateInstance(__uuidof(Recordset));
	// 	CString strConn;
	// 	strConn.Format(_T("Driver={MySQL ODBC 5.1 Driver}; Server=%s;Database=%s; uid=%s; pwd=%s;"), theApp.m_strDBIP, theApp.m_strDBName, theApp.m_strDBUsername, theApp.m_strDBPassword);
	// 	try
	// 	{
	// 		conn->Open(_bstr_t(strConn), "", "", adModeUnknown);
	// 	}
	// 	catch (_com_error e)
	// 	{
	// 		return;
	// 	}
	USES_CONVERSION;

	CString strSQL;
	int iMingdan;
	if (m_iCurMingdan == 0)
		iMingdan = 1;
	else if (m_iCurMingdan == 1)
		iMingdan = 3;
	else if (m_iCurMingdan == 2)
		iMingdan = 2;
	strSQL.Format(_T("select count(*) as num from employeeinfo where libraryid='%d' %s"), iMingdan, strAnd);
	pSet = g_pConnect->Execute(_bstr_t(strSQL), NULL, adCmdText);
	CString csTemp = (TCHAR *)(_bstr_t)pSet->GetFields()->GetItem("num")->Value;
	int iNum = atoi(T2A(csTemp));
	//pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	//	int iNum = pSet->GetCollect(_variant_t("num")).iVal;
	m_iLeftTotalPage = iNum / 21;
	pSet->Close();

	strSQL.Format(_T("select employeeinfo.id_card,name,sex,inserttime,birth,address,type,phone,nativeplace,personid,imgpath from employeeinfo left join imginfo on imginfo.id_card=employeeinfo.id_card where libraryid=%d %s limit %d,21"), iMingdan, strAnd, m_iLeftCurPage * 21);
	CString strIDCard, strName, strSex, strType;
	pSet = g_pConnect->Execute(_bstr_t(strSQL), NULL, adCmdText);
	//	pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	int iCount = 0;
	while (!pSet->adoEOF)
	{
		strIDCard = (TCHAR *)(_bstr_t)pSet->GetFields()->GetItem("id_card")->Value;
		strName = (TCHAR *)(_bstr_t)pSet->GetFields()->GetItem("name")->Value;
		strSex = (TCHAR *)(_bstr_t)pSet->GetFields()->GetItem("sex")->Value;
		strType = (TCHAR *)(_bstr_t)pSet->GetFields()->GetItem("type")->Value;
		m_mdList[iCount].bIsUse = true;
		m_mdList[iCount].strIDCard = strIDCard;
		m_mdList[iCount].strName = strName;
		m_mdList[iCount].strType = strType;
		m_mdList[iCount].strPersonID = (TCHAR *)(_bstr_t)pSet->GetFields()->GetItem("personid")->Value;
		m_mdList[iCount].strPhone = (TCHAR *)(_bstr_t)pSet->GetFields()->GetItem("phone")->Value;
		m_mdList[iCount].strNativeplace = (TCHAR *)(_bstr_t)pSet->GetFields()->GetItem("nativeplace")->Value;
		m_mdList[iCount].strSex = (strSex == "male") ? "男" : "女";
		m_mdList[iCount].strInsertTime = m_mdList[iCount].strInsertTime.Mid(0, m_mdList[iCount].strInsertTime.Find(L" "));
		m_mdList[iCount].strBirth = (TCHAR *)(_bstr_t)pSet->GetFields()->GetItem("birth")->Value;
		m_mdList[iCount].strAddress = (TCHAR *)(_bstr_t)pSet->GetFields()->GetItem("address")->Value;
		m_mdList[iCount].strBXPath = (TCHAR *)(_bstr_t)pSet->GetFields()->GetItem("imgpath")->Value;
		int nStart = m_mdList[iCount].strBXPath.Find(_T("/Data"));
		m_mdList[iCount].strBXPath = m_mdList[iCount].strBXPath.Mid(nStart, m_mdList[iCount].strBXPath.GetLength() - nStart);


		// 		strIDCard = (char*)_bstr_t(pSet->GetCollect(_variant_t("id_card")));
		// 		strName = (char*)_bstr_t(pSet->GetCollect(_variant_t("name")));
		// 		strSex = (char*)_bstr_t(pSet->GetCollect(_variant_t("sex")));
		// 		strType = (char*)_bstr_t(pSet->GetCollect(_variant_t("type")));
		// 		m_mdList[iCount].bIsUse = true;
		// 		m_mdList[iCount].strIDCard = strIDCard;
		// 		m_mdList[iCount].strName = strName;
		// 		m_mdList[iCount].strType = strType;
		// 		m_mdList[iCount].strPersonID = (char*)_bstr_t(pSet->GetCollect(_variant_t("personid")));
		// 		m_mdList[iCount].strPhone = (char*)_bstr_t(pSet->GetCollect(_variant_t("phone")));
		// 		m_mdList[iCount].strNativeplace = (char*)_bstr_t(pSet->GetCollect(_variant_t("nativeplace")));
		// 		m_mdList[iCount].strSex = (strSex == "male") ? "男" : "女";
		// 		m_mdList[iCount].strInsertTime = (char*)_bstr_t(pSet->GetCollect(_variant_t("inserttime")));
		// 		m_mdList[iCount].strInsertTime = m_mdList[iCount].strInsertTime.Mid(0, m_mdList[iCount].strInsertTime.Find(L" "));
		// 		m_mdList[iCount].strBirth = (char*)_bstr_t(pSet->GetCollect(_variant_t("birth")));
		// 		m_mdList[iCount].strAddress = (char*)_bstr_t(pSet->GetCollect(_variant_t("address")));
		// 		m_mdList[iCount].strBXPath = (char*)_bstr_t(pSet->GetCollect(_variant_t("imgpath")));
		// 		int nStart = m_mdList[iCount].strBXPath.Find(_T("/Data"));
		// 		m_mdList[iCount].strBXPath = m_mdList[iCount].strBXPath.Mid(nStart, m_mdList[iCount].strBXPath.GetLength() - nStart);

		iCount++;
		pSet->MoveNext();
	}
	//pSet->Close();
	//conn->Close();
}

void CRLKGLDlg::SetDataAlarmList()
{
	m_iTotalAlarm = m_iCurAlarmPage = 0;
	_ConnectionPtr conn;
	conn.CreateInstance(__uuidof(Connection));
	_RecordsetPtr pSet;
	pSet.CreateInstance(__uuidof(Recordset));
	CString strConn;
	strConn.Format(_T("Driver={MySQL ODBC 5.1 Driver}; Server=%s;Database=%s; uid=%s; pwd=%s;"), theApp.m_strDBIP, theApp.m_strDBName, theApp.m_strDBUsername, theApp.m_strDBPassword);
	try
	{
		conn->Open(_bstr_t(strConn), "", "", adModeUnknown);
	}
	catch (_com_error e)
	{
		return;
	}
	CString strSQL;
	int iMingdan;
	if (m_iCurMingdan == 0)
		iMingdan = 1;
	else if (m_iCurMingdan == 1)
		iMingdan = 3;
	else if (m_iCurMingdan == 2)
		iMingdan = 2;
	strSQL.Format(_T("select snapshot.imagepath,blacklist.snapshotid,blacklist.similarity,blacklist.alarmtime,camera.addr from blacklist left join snapshot on snapshot.snapshotid=blacklist.snapshotid left join camera on blacklist.cameraid=camera.cameraid where id_card='%s' order by alarmtime desc limit 0,40;"), m_mdList[m_iJBXXXZIndex].strIDCard);
	CString strIDCard, strName, strSex, strType;
	pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	int iCount = 0;
	while (!pSet->adoEOF)
	{
		m_alarmdata[iCount].strSnapshotID = (char*)_bstr_t(pSet->GetCollect(_variant_t("snapshotid")));
		m_alarmdata[iCount].strSim = (char*)_bstr_t(pSet->GetCollect(_variant_t("similarity")));
		m_alarmdata[iCount].strAlarmTime = (char*)_bstr_t(pSet->GetCollect(_variant_t("alarmtime")));
		m_alarmdata[iCount].strAddr = (char*)_bstr_t(pSet->GetCollect(_variant_t("addr")));
		CString strImagePath;
		strImagePath = (char*)_bstr_t(pSet->GetCollect(_variant_t("imagepath")));
		m_alarmdata[iCount].strSrc = L"/Data/" + strImagePath + L"/imgCurFaceJpg/" + m_alarmdata[iCount].strSnapshotID + L".jpg";
		iCount++;
		pSet->MoveNext();
	}
	pSet->Close();
	conn->Close();
	m_iTotalAlarm = iCount;
}

HBRUSH CRLKGLDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	switch (nCtlColor)
	{
	case CTLCOLOR_EDIT:
		pDC->SetTextColor(m_colorEditText);
		pDC->SetBkColor(m_colorEditBK);
		return (HBRUSH)(m_pEditBkBrush->GetSafeHandle());
	default:
		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CRLKGLDlg::ShowEditControls()
{
	m_editName.MoveWindow(1055 * m_rClient.Width() / 1920, 77 * m_rClient.Height() / 830, 120 * m_rClient.Width() / 1920, 25 * m_rClient.Height() / 830);
	m_editBirth.MoveWindow(1055 * m_rClient.Width() / 1920, 113 * m_rClient.Height() / 830, 120 * m_rClient.Width() / 1920, 25 * m_rClient.Height() / 830);
	m_editType.MoveWindow(1237 * m_rClient.Width() / 1920, 113 * m_rClient.Height() / 830, 120 * m_rClient.Width() / 1920, 25 * m_rClient.Height() / 830);
	m_editPhone.MoveWindow(1237 * m_rClient.Width() / 1920, 149 * m_rClient.Height() / 830, 120 * m_rClient.Width() / 1920, 25 * m_rClient.Height() / 830);
	m_editNativeplace.MoveWindow(1055 * m_rClient.Width() / 1920, 191 * m_rClient.Height() / 830, 302 * m_rClient.Width() / 1920, 25 * m_rClient.Height() / 830);
	m_editAddress.MoveWindow(1055 * m_rClient.Width() / 1920, 227 * m_rClient.Height() / 830, 302 * m_rClient.Width() / 1920, 25 * m_rClient.Height() / 830);
	m_comboSex.MoveWindow(1237 * m_rClient.Width() / 1920, 77 * m_rClient.Height() / 830, 120 * m_rClient.Width() / 1920, 25 * m_rClient.Height() / 830);
	m_comboFacedb.MoveWindow(1055 * m_rClient.Width() / 1920, 149 * m_rClient.Height() / 830, 120 * m_rClient.Width() / 1920, 25 * m_rClient.Height() / 830);
	m_editName.ShowWindow(SW_SHOW);
	m_editBirth.ShowWindow(SW_SHOW);
	m_editType.ShowWindow(SW_SHOW);
	m_editPhone.ShowWindow(SW_SHOW);
	m_editNativeplace.ShowWindow(SW_SHOW);
	m_editAddress.ShowWindow(SW_SHOW);
	m_comboFacedb.ShowWindow(SW_SHOW);
	m_comboSex.ShowWindow(SW_SHOW);
}
void CRLKGLDlg::HideEditControls()
{
	m_editName.ShowWindow(SW_HIDE);
	m_editBirth.ShowWindow(SW_HIDE);
	m_editType.ShowWindow(SW_HIDE);
	m_editPhone.ShowWindow(SW_HIDE);
	m_editNativeplace.ShowWindow(SW_HIDE);
	m_editAddress.ShowWindow(SW_HIDE);
	m_comboSex.ShowWindow(SW_HIDE);
	m_comboFacedb.ShowWindow(SW_HIDE);
	m_editName.SetWindowTextW(L"");
	m_editBirth.SetWindowTextW(L"");
	m_editType.SetWindowTextW(L"");
	m_editPhone.SetWindowTextW(L"");
	m_editNativeplace.SetWindowTextW(L"");
	m_editAddress.SetWindowTextW(L"");
	m_comboSex.SetCurSel(0);
	m_comboFacedb.SetCurSel(0);
}
void CRLKGLDlg::ShowAddControls()
{
	m_editName.MoveWindow(1055 * m_rClient.Width() / 1920, 77 * m_rClient.Height() / 830, 120 * m_rClient.Width() / 1920, 25 * m_rClient.Height() / 830);
	m_editBirth.MoveWindow(1055 * m_rClient.Width() / 1920, 112 * m_rClient.Height() / 830, 120 * m_rClient.Width() / 1920, 25 * m_rClient.Height() / 830);
	m_editType.MoveWindow(1237 * m_rClient.Width() / 1920, 112 * m_rClient.Height() / 830, 120 * m_rClient.Width() / 1920, 25 * m_rClient.Height() / 830);
	m_editPhone.MoveWindow(1055 * m_rClient.Width() / 1920, 149 * m_rClient.Height() / 830, 120 * m_rClient.Width() / 1920, 25 * m_rClient.Height() / 830);
	m_editNativeplace.MoveWindow(1055 * m_rClient.Width() / 1920, 190 * m_rClient.Height() / 830, 302 * m_rClient.Width() / 1920, 25 * m_rClient.Height() / 830);
	m_editAddress.MoveWindow(1055 * m_rClient.Width() / 1920, 229 * m_rClient.Height() / 830, 302 * m_rClient.Width() / 1920, 25 * m_rClient.Height() / 830);
	m_comboSex.MoveWindow(1237 * m_rClient.Width() / 1920, 77 * m_rClient.Height() / 830, 120 * m_rClient.Width() / 1920, 25 * m_rClient.Height() / 830);
	m_editName.ShowWindow(SW_SHOW);
	m_editBirth.ShowWindow(SW_SHOW);
	m_editType.ShowWindow(SW_SHOW);
	m_editPhone.ShowWindow(SW_SHOW);
	m_editNativeplace.ShowWindow(SW_SHOW);
	m_editAddress.ShowWindow(SW_SHOW);
	m_comboSex.ShowWindow(SW_SHOW);
}
void CRLKGLDlg::HideAddControls()
{
	m_editName.ShowWindow(SW_HIDE);
	m_editBirth.ShowWindow(SW_HIDE);
	m_editType.ShowWindow(SW_HIDE);
	m_editPhone.ShowWindow(SW_HIDE);
	m_editNativeplace.ShowWindow(SW_HIDE);
	m_editAddress.ShowWindow(SW_HIDE);
	m_comboSex.ShowWindow(SW_HIDE);
	m_editName.SetWindowTextW(L"");
	m_editBirth.SetWindowTextW(L"");
	m_editType.SetWindowTextW(L"");
	m_editPhone.SetWindowTextW(L"");
	m_editNativeplace.SetWindowTextW(L"");
	m_editAddress.SetWindowTextW(L"");
	m_comboSex.SetCurSel(0);
}


BOOL CRLKGLDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN&&pMsg->wParam == VK_RETURN)
		return TRUE;
	if (pMsg->message == WM_KEYDOWN&&pMsg->wParam == VK_ESCAPE)
		return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}
