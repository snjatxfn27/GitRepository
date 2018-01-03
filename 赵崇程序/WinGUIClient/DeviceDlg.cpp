// DeviceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WinGUIClient.h"
#include "DeviceDlg.h"
#include "afxdialogex.h"
#include "WinGUIClientDlg.h"

// CDeviceDlg 对话框

IMPLEMENT_DYNAMIC(CDeviceDlg, CDialogEx)

CDeviceDlg::CDeviceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDeviceDlg::IDD, pParent)
{

}

CDeviceDlg::~CDeviceDlg()
{
}

void CDeviceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDeviceDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDeviceDlg 消息处理程序


void CDeviceDlg::OnPaint()
{
	if (!m_bIsInitRect)
	{
		m_iLineHeight = m_rClient.Height() * 620 / 642 / 13;
		for (int i = 0; i < 13; i++)
		for (int j = 0; j < 2; j++)
			m_devicestat[i].rCameraRect[j].SetRect(
			m_rClient.Width() * 8 / 100 + j * m_rClient.Width() * 41 / 100,
			m_rClient.Height() * 8 / 100 + m_iLineHeight*i,
			m_rClient.Width() * 8 / 100 + j * m_rClient.Width() * 41 / 100 + 16,
			m_rClient.Height() * 8 / 100 + m_iLineHeight*i + 16
			);
		m_bIsInitRect = true;
	}

	CPaintDC dc(this); // device context for painting
	HDC hDC = GetDC()->m_hDC;
	Graphics g(hDC);
	Image img_bk(L"img/device_bk.jpg");
	Image imageonline(L"img/device_deviceonline.png");
	Image imageoffline(L"img/device_deviceoffline.png");
	Image imagecameraon(L"img/device_camera_on.png");
	Image imagecameraoff(L"img/device_camera_off.png");
	g.DrawImage(&img_bk, 0, 0, m_rClient.Width(), m_rClient.Height());
	SolidBrush onLineBrush(Color(255, 78, 255, 0));
	SolidBrush offLineBrush(Color(255, 184, 186, 197));
	Gdiplus::Font font(L"新宋体", 13, FontStyleRegular, UnitPixel);
	g.SetTextRenderingHint(TextRenderingHintClearTypeGridFit);
	PointF pointF(m_rClient.Width() * 10 / 100, m_rClient.Height() * 27 / 642);
	int iLineY = m_rClient.Height() * 27 / 642;
	for (int i = 0; i<m_iDeviceCount; i++)
	{
		pointF.Y = m_rClient.Height() * 27 / 642 + m_rClient.Height()*i * 620 / 642 / 13;
		iLineY = m_rClient.Height() * 27 / 642 + m_rClient.Height()*i * 620 / 642 / 13;

		if (m_devicestat[i].bIsOnline)
		{
			g.DrawString(m_devicestat[i].strDeviceName.AllocSysString(), -1, &font, pointF, &onLineBrush);
			g.DrawImage(&imageonline, 10, iLineY, 16, 16);
		}
		else
		{
			g.DrawString(m_devicestat[i].strDeviceName.AllocSysString(), -1, &font, pointF, &offLineBrush);
			g.DrawImage(&imageoffline, 10, iLineY, 16, 16);
		}
		for (int j = 0; j<m_devicestat[i].iCameraCount; j++)
		{
			PointF pointFF;
			pointFF = pointF;
			pointFF.Y += 25;
			pointFF.X += 10 + j * 165;
			g.DrawString(m_devicestat[i].strCameraAddr[j].AllocSysString(), -1, &font, pointFF, &offLineBrush);
			if (m_devicestat[i].iCameraState[j] == 4)
				g.DrawImage(&imagecameraoff, m_devicestat[i].rCameraRect[j].left, m_devicestat[i].rCameraRect[j].top, 16, 16); 
			else
				g.DrawImage(&imagecameraon, m_devicestat[i].rCameraRect[j].left, m_devicestat[i].rCameraRect[j].top, 16, 16);
		}
	}

	g.ReleaseHDC(hDC);
	// 不为绘图消息调用 CDialogEx::OnPaint()
}


BOOL CDeviceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_iDeviceCount = 0;
	m_strCurRTSP.Empty();
	for (int i = 0; i<13; i++)
	{
		m_devicestat[i].bIsOnline = FALSE;
		m_devicestat[i].strDeviceName.Empty();
		m_devicestat[i].iCameraCount = 0;
		for (int j = 0; j<2; j++)
		{
			m_devicestat[i].strCameraAddr[j].Empty();
			m_devicestat[i].strCameraRTSP[j].Empty();
			m_devicestat[i].iCameraState[j] = 4;
		}
	}
	m_bIsInitRect = false;
	UpdateStatData();
	SetTimer(1, 10000, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CDeviceDlg::UpdateStatData()
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
	strSQL.Format(_T("select * from device"));
	pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	m_iDeviceCount = 0;
	BOOL bRefresh = FALSE;
	while (!pSet->adoEOF)
	{
		CString strIP, strDescription,strOnline;
		strIP = getsqlstr("ip");
		strDescription = getsqlstr("description");
		m_devicestat[m_iDeviceCount].strDeviceName = strIP + "：" + strDescription;
		strOnline = getsqlstr("isonline");
		BOOL bOnlineStat = FALSE;
		if (strOnline == "0")
			bOnlineStat = FALSE;
		else
			bOnlineStat = TRUE;
		if (m_devicestat[m_iDeviceCount].bIsOnline != bOnlineStat)
		{
			m_devicestat[m_iDeviceCount].bIsOnline = bOnlineStat;
			bRefresh = TRUE;
		}

		CString strDeviceID;
		strDeviceID= getsqlstr("id");
		_RecordsetPtr pSet1;
		pSet1.CreateInstance(__uuidof(Recordset));
		strSQL.Format(_T("select addr,rtsp,camera_state from camera where deviceid='%s'"),strDeviceID);
		pSet1->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
		m_devicestat[m_iDeviceCount].iCameraCount = 0;
		while (!pSet1->adoEOF)
		{
			m_devicestat[m_iDeviceCount].strCameraAddr[m_devicestat[m_iDeviceCount].iCameraCount] = (char*)_bstr_t(pSet1->GetCollect(_variant_t("addr")));
			m_devicestat[m_iDeviceCount].strCameraRTSP[m_devicestat[m_iDeviceCount].iCameraCount] = (char*)_bstr_t(pSet1->GetCollect(_variant_t("rtsp")));
			int iCameraState = pSet1->GetCollect(_variant_t("camera_state")).iVal;
			if (m_devicestat[m_iDeviceCount].iCameraState[m_devicestat[m_iDeviceCount].iCameraCount] != iCameraState)
			{
				m_devicestat[m_iDeviceCount].iCameraState[m_devicestat[m_iDeviceCount].iCameraCount] = iCameraState;
				bRefresh = TRUE;
			}
			m_devicestat[m_iDeviceCount].iCameraCount++;
			pSet1->MoveNext();
		}
		pSet1->Close();

		m_iDeviceCount++;
		pSet->MoveNext();
	}
	pSet->Close();
	conn->Close();
	if (bRefresh)
		Invalidate(FALSE);
}

void CDeviceDlg::PlayVideo(CString strRTSP)
{
	const size_t rtspsize = (strRTSP.GetLength() + 1) * 2; // 宽字符的长度;
	char * rtspchar = new char[rtspsize]; //分配空间;
	size_t sz = 0;
	wcstombs_s(&sz, rtspchar, rtspsize, strRTSP, _TRUNCATE);

	CWinGUIClientDlg *parentDlg;
	parentDlg = (CWinGUIClientDlg *)GetParent();
//	parentDlg->m_dlgVideo->VideoStop();
//	parentDlg->m_dlgVideo->VideoInit();
//	parentDlg->m_dlgVideo->VideoSetup(strRTSP);

	parentDlg->m_dlgVideo->Play(rtspchar);
	m_strCurRTSP = strRTSP;
	delete[] rtspchar;

}

void CDeviceDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	for (int i = 0; i<13; i++)
	{
		for (int j = 0; j<2; j++)
		{
			if (m_devicestat[i].iCameraCount>j && m_devicestat[i].rCameraRect[j].PtInRect(point))
			{
				if (m_devicestat[i].iCameraState[j] != 4)
				{
					PlayVideo(m_devicestat[i].strCameraRTSP[j]);
					break;
				}
			}
		}
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CDeviceDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	UpdateStatData();

	CDialogEx::OnTimer(nIDEvent);
}
