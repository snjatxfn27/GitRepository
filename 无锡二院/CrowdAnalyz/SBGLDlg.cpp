// SBGLDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CrowdAnalyz.h"
#include "SBGLDlg.h"
#include "afxdialogex.h"


// CSBGLDlg 对话框

IMPLEMENT_DYNAMIC(CSBGLDlg, CDialogEx)

CSBGLDlg::CSBGLDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CSBGLDlg::IDD, pParent)
, m_csIp(_T(""))
, m_csPos(_T(""))
, m_csFaceDbs(_T(""))
, m_csPage(_T(""))
{
	m_nSelectIndex = 0;
	m_nRedact = 0;
}

CSBGLDlg::~CSBGLDlg()
{
}

void CSBGLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_IP, m_csIp);
	DDX_Text(pDX, IDC_EDIT_POST, m_csPos);
	DDX_Text(pDX, IDC_EDIT_POST2, m_csFaceDbs);
	DDX_Text(pDX, IDC_EDIT_PAGE, m_csPage);
}


BEGIN_MESSAGE_MAP(CSBGLDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CSBGLDlg 消息处理程序


void CSBGLDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	delete pMemDC;
	delete pBitmap;
	// TODO:  在此处添加消息处理程序代码
}




BOOL CSBGLDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitRet();

	m_colorEditText = RGB(212, 214, 222);
	m_colorEditBK = RGB(21, 25, 60);
	m_pEditBkBrush = new CBrush(RGB(21, 25, 60));

	GetDlgItem(IDC_EDIT_IP)->MoveWindow(m_rShebei.left, m_rShebei.top, m_rShebei.Width(), m_rShebei.Height());
	GetDlgItem(IDC_EDIT_POST)->MoveWindow(m_rIp.left, m_rIp.top, m_rIp.Width(), m_rIp.Height());
	GetDlgItem(IDC_EDIT_POST2)->MoveWindow(m_rPos.left, m_rPos.top, m_rPos.Width(), m_rPos.Height());
	GetDlgItem(IDC_EDIT_PAGE)->MoveWindow(m_rPaga.left, m_rPaga.top, m_rPaga.Width(), m_rPaga.Height());

	m_nPagasMax = 1;
	m_nPagaNum = 1;
	m_csPage = _T("1");
	GetMaxPaga();
	SetDeviceInfo(m_nPagaNum);
	UpdateData(FALSE);
	//双缓存
	CClientDC dc(this);
	pMemDC = new CDC;
	pBitmap = new CBitmap;
	pMemDC->CreateCompatibleDC(&dc);
	pBitmap->CreateCompatibleBitmap(&dc, WIDTH, HEIGHT);

	return TRUE;
}

void CSBGLDlg::OnPaint()
{
	CPaintDC dc(this);
	CBitmap * pOldBitmap = pMemDC->SelectObject(pBitmap);
	pMemDC->PatBlt(0, 0, m_rClient.Width(), m_rClient.Height(), PATCOPY);
	Graphics g(pMemDC->m_hDC);
	//画总背景
	if (m_nSelectIndex == 0)
	{
		Image img_bk(L"img/fuwuqi_BK.jpg");
		g.DrawImage(&img_bk, 0, 0, m_rClient.Width(), m_rClient.Height());
	}

	else if (m_nSelectIndex == 1)
	{
		Image img_bk(L"img/shexiangji_Bk.jpg");
		g.DrawImage(&img_bk, 0, 0, m_rClient.Width(), m_rClient.Height());
	}


	if (m_nRedact == 0)
	{
		Image img_quxiao(L"img/quxiao-an.jpg"); 
			g.DrawImage(&img_quxiao, m_rQuxiao.left, m_rQuxiao.top, m_rQuxiao.Width(), m_rQuxiao.Height());

		Image img_baocun(L"img/baocun-an.jpg");
		g.DrawImage(&img_baocun, m_rBaocun.left, m_rBaocun.top, m_rBaocun.Width(), m_rBaocun.Height());

		Image img_tianjia(L"img/tianjia-liang.jpg");
		g.DrawImage(&img_tianjia, m_rTianjia.left, m_rTianjia.top, m_rTianjia.Width(), m_rTianjia.Height());
	}
	else if (m_nRedact == 1)
	{
		Image img_quxiao(L"img/quxiao-liang.jpg"); 
			g.DrawImage(&img_quxiao, m_rQuxiao.left, m_rQuxiao.top, m_rQuxiao.Width(), m_rQuxiao.Height());

		Image img_baocun(L"img/baocun-liang.jpg");
		g.DrawImage(&img_baocun, m_rBaocun.left, m_rBaocun.top, m_rBaocun.Width(), m_rBaocun.Height());

		Image img_tianjia(L"img/tianjia-an.jpg");
		g.DrawImage(&img_tianjia, m_rTianjia.left, m_rTianjia.top, m_rTianjia.Width(), m_rTianjia.Height());
	}

	Gdiplus::Font font(L"方正细倩简体", m_rClient.Width() * 9 / 1000, FontStyleRegular, UnitPixel);
	SolidBrush percentBrush(Color(255, 212, 214, 222));
	StringFormat stringformat;
	stringformat.SetAlignment(StringAlignmentCenter);
	stringformat.SetLineAlignment(StringAlignmentCenter);

	for (int i = 0; i < m_iDisTongjiCount; i++)
	{
		g.DrawString(m_DataList[i].csIP.AllocSysString(), -1, &font, RectF(m_DataList[i].rIP.left, m_DataList[i].rIP.top, m_DataList[i].rIP.Width(), m_DataList[i].rIP.Height()), &stringformat, &percentBrush);
		g.DrawString(m_DataList[i].strPos.AllocSysString(), -1, &font, RectF(m_DataList[i].rPos.left, m_DataList[i].rPos.top, m_DataList[i].rPos.Width(), m_DataList[i].rPos.Height()), &stringformat, &percentBrush);
		g.DrawString(m_DataList[i].csState.AllocSysString(), -1, &font, RectF(m_DataList[i].rState.left, m_DataList[i].rState.top, m_DataList[i].rState.Width(), m_DataList[i].rState.Height()), &stringformat, &percentBrush);


		Image img_shanchu(L"img/shanchu-liang.jpg");
		g.DrawImage(&img_shanchu, m_DataList[i].rSC.left, m_DataList[i].rSC.top, m_DataList[i].rSC.Width(), m_DataList[i].rSC.Height());

		Image img_bianji(L"img/bianji-liang.jpg");
		g.DrawImage(&img_bianji, m_DataList[i].rBJ.left, m_DataList[i].rBJ.top, m_DataList[i].rBJ.Width(), m_DataList[i].rBJ.Height());
	}

	/////ceshi
	Pen redPen(Color(255, 0, 0), 1);
	// 	g.DrawRectangle(&redPen, m_rFuwuqi.left, m_rFuwuqi.top, m_rFuwuqi.Width(), m_rFuwuqi.Height());
	// 	g.DrawRectangle(&redPen, m_rXiangji.left, m_rXiangji.top, m_rXiangji.Width(), m_rXiangji.Height());
	// 	g.DrawRectangle(&redPen, m_rBaocun.left, m_rBaocun.top, m_rBaocun.Width(), m_rBaocun.Height());
	// 	g.DrawRectangle(&redPen, m_rQuxiao.left, m_rQuxiao.top, m_rQuxiao.Width(), m_rQuxiao.Height());
	// 	g.DrawRectangle(&redPen, m_rTianjia.left, m_rTianjia.top, m_rTianjia.Width(), m_rTianjia.Height());
	 	//g.DrawRectangle(&redPen, m_rShebei.left, m_rShebei.top, m_rShebei.Width(), m_rShebei.Height());
	 	//g.DrawRectangle(&redPen, m_rIp.left, m_rIp.top, m_rIp.Width(), m_rIp.Height());
	// 	g.DrawRectangle(&redPen, m_rPos.left, m_rPos.top, m_rPos.Width(), m_rPos.Height());
	// 	g.DrawRectangle(&redPen, m_rUp.left, m_rUp.top, m_rUp.Width(), m_rUp.Height());
	// 	g.DrawRectangle(&redPen, m_rNext.left, m_rNext.top, m_rNext.Width(), m_rNext.Height());
	//g.DrawRectangle(&redPen, m_rTable.left, m_rTable.top, m_rTable.Width(), m_rTable.Height());
	//g.DrawRectangle(&redPen, m_rData.left, m_rData.top, m_rData.Width(), m_rData.Height());

	// 	for (int i = 0; i < 21; i++)
	// 	{
	// 		g.DrawRectangle(&redPen, m_DataList[i].rIP.left, m_DataList[i].rIP.top, m_DataList[i].rIP.Width(), m_DataList[i].rIP.Height());
	// 		g.DrawRectangle(&redPen, m_DataList[i].rPos.left, m_DataList[i].rPos.top, m_DataList[i].rPos.Width(), m_DataList[i].rPos.Height());
	// 		g.DrawRectangle(&redPen, m_DataList[i].rState.left, m_DataList[i].rState.top, m_DataList[i].rState.Width(), m_DataList[i].rState.Height());
	// 		g.DrawRectangle(&redPen, m_DataList[i].rXQ.left, m_DataList[i].rXQ.top, m_DataList[i].rXQ.Width(), m_DataList[i].rXQ.Height());
	// 		g.DrawRectangle(&redPen, m_DataList[i].rQC.left, m_DataList[i].rQC.top, m_DataList[i].rQC.Width(), m_DataList[i].rQC.Height());
	// 		g.DrawRectangle(&redPen, m_DataList[i].rBJ.left, m_DataList[i].rBJ.top, m_DataList[i].rBJ.Width(), m_DataList[i].rBJ.Height());
	// 	}




	//显示缓存，清除HDC
	dc.BitBlt(0, 0, m_rClient.Width(), m_rClient.Height(), pMemDC, 0, 0, SRCCOPY);
	pMemDC->SelectObject(pOldBitmap);
	g.ReleaseHDC(pMemDC->m_hDC);

}

void CSBGLDlg::InitRet()
{
	m_rClient.SetRect(0, 0, 1920 * WIDTH / 1920, 830 * HEIGHT / 1080);

	m_rTable.SetRect(62 * m_rClient.Width() / 1920, 66 * m_rClient.Height() / 830, 756 * m_rClient.Width() / 1920, 106 * m_rClient.Height() / 830);
	m_rData.SetRect(57 * m_rClient.Width() / 1920, 146 * m_rClient.Height() / 830, 762 * m_rClient.Width() / 1920, 769 * m_rClient.Height() / 830);



	m_rFuwuqi.SetRect(247 * m_rClient.Width() / 1920, 29 * m_rClient.Height() / 830, 408 * m_rClient.Width() / 1920, 55 * m_rClient.Height() / 830);
	m_rXiangji.SetRect(415 * m_rClient.Width() / 1920, 29 * m_rClient.Height() / 830, 570 * m_rClient.Width() / 1920, 55 * m_rClient.Height() / 830);

	m_rBaocun.SetRect( 583 * m_rClient.Width() / 1920, 71 * m_rClient.Height() / 830,  632 * m_rClient.Width() / 1920, 99 * m_rClient.Height() / 830);
	m_rQuxiao.SetRect( 640 * m_rClient.Width() / 1920, 71 * m_rClient.Height() / 830,  689 * m_rClient.Width() / 1920, 99 * m_rClient.Height() / 830);
	m_rTianjia.SetRect(697 * m_rClient.Width() / 1920, 71 * m_rClient.Height() / 830,  750 * m_rClient.Width() / 1920, 99 * m_rClient.Height() / 830);

	m_rShebei.SetRect(98 * m_rClient.Width() / 1920, 76 * m_rClient.Height() / 830, 207 * m_rClient.Width() / 1920, 94 * m_rClient.Height() / 830);
	m_rIp.SetRect(261 * m_rClient.Width() / 1920, 76 * m_rClient.Height() / 830, 372 * m_rClient.Width() / 1920, 94 * m_rClient.Height() / 830);
	m_rPos.SetRect(455 * m_rClient.Width() / 1920, 76 * m_rClient.Height() / 830, 566 * m_rClient.Width() / 1920, 94 * m_rClient.Height() / 830);

	m_rUp.SetRect(325 * m_rClient.Width() / 1920, 777 * m_rClient.Height() / 830, 395 * m_rClient.Width() / 1920, 795 * m_rClient.Height() / 830);
	m_rNext.SetRect(491 * m_rClient.Width() / 1920, 777 * m_rClient.Height() / 830, 560 * m_rClient.Width() / 1920, 795 * m_rClient.Height() / 830);
	m_rPaga.SetRect(425 * m_rClient.Width() / 1920, 780 * m_rClient.Height() / 830, 457 * m_rClient.Width() / 1920, 795 * m_rClient.Height() / 830);


	//71,144   748,168
	for (int i = 0; i < 21; i++)
	{
		m_DataList[i].rIP.SetRect((71 * m_rClient.Width() / 1920), (146 + (30 * i))* m_rClient.Height() / 830, (240 * m_rClient.Width() / 1920), (168 + (30 * i))* m_rClient.Height() / 830);
		m_DataList[i].rPos.SetRect((250 * m_rClient.Width() / 1920), (146 + (30 * i))* m_rClient.Height() / 830, (460 * m_rClient.Width() / 1920), (168 + (30 * i))* m_rClient.Height() / 830);
		m_DataList[i].rState.SetRect((470 * m_rClient.Width() / 1920), (146 + (30 * i))* m_rClient.Height() / 830, (540 * m_rClient.Width() / 1920), (168 + (30 * i))* m_rClient.Height() / 830);
		m_DataList[i].rXQ.SetRect((550 * m_rClient.Width() / 1920), (146 + (30 * i))* m_rClient.Height() / 830, (610 * m_rClient.Width() / 1920), (168 + (30 * i))* m_rClient.Height() / 830);
		m_DataList[i].rSC.SetRect((620 * m_rClient.Width() / 1920), (146 + (30 * i))* m_rClient.Height() / 830, (680 * m_rClient.Width() / 1920), (168 + (30 * i))* m_rClient.Height() / 830);
		m_DataList[i].rBJ.SetRect((690 * m_rClient.Width() / 1920), (146 + (30 * i))* m_rClient.Height() / 830, (750 * m_rClient.Width() / 1920), (168 + (30 * i))* m_rClient.Height() / 830);
	}

}

BOOL CSBGLDlg::GetDeviceInfo(int nIndex)
{
	try
	{
		m_iDisTongjiCount = 0;
		CString csSql, csTemp;
		_RecordsetPtr pRecordset;
		int nStart;
		nStart = (nIndex - 1) * 21;
		csSql.Format(_T("SELECT * FROM device limit %d,21;"), nStart);

		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		while (!pRecordset->adoEOF)
		{
			m_DataList[m_iDisTongjiCount].csIP = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("ip")->Value;
			m_DataList[m_iDisTongjiCount].strPos = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("description")->Value;
			m_DataList[m_iDisTongjiCount].csFaceDbs = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("facedbs")->Value;
			m_DataList[m_iDisTongjiCount].csState = _T("正常");
			m_iDisTongjiCount++;
			pRecordset->MoveNext();
		}
		return TRUE;

	}
	catch (CException* e)
	{

	}
	return FALSE;
}

BOOL CSBGLDlg::SetDeviceInfo(int nIndex)
{
	if (GetDeviceInfo(nIndex))
	{
		InvalidateRect(m_rClient, false);
		return TRUE;
	}
	return FALSE;
}
BOOL CSBGLDlg::GetCameraInfo(int nIndex)
{
	try
	{
		m_iDisTongjiCount = 0;
		CString csSql, csTemp;
		_RecordsetPtr pRecordset;
		int nStart;
		nStart = (nIndex - 1) * 21;
		csSql.Format(_T("SELECT * FROM camera limit %d,21;"), nStart);

		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		while (!pRecordset->adoEOF)
		{
			m_DataList[m_iDisTongjiCount].csIP = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("ip")->Value;
			m_DataList[m_iDisTongjiCount].strPos = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("addr")->Value;
			m_DataList[m_iDisTongjiCount].csState = _T("正常");
			m_iDisTongjiCount++;
			pRecordset->MoveNext();
		}
		return TRUE;

	}
	catch (CException* e)
	{

	}
	return FALSE;
}

BOOL CSBGLDlg::SetCameraInfo(int nIndex)
{
	if (GetCameraInfo(nIndex))
	{
		InvalidateRect(m_rClient, false);
		return TRUE;
	}
	return FALSE;
}

BOOL CSBGLDlg::SetShanchu(int nIndex)
{
	try
	{
		CString csSql;
		if (m_nSelectIndex == 0)
			csSql.Format(_T("DELETE FROM device WHERE ip = '%s'"), m_DataList[nIndex].csIP);
		else
			csSql.Format(_T("DELETE FROM camera WHERE ip = '%s'"), m_DataList[nIndex].csIP);

		g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		
		m_nRedact = 0;
		if (m_nSelectIndex == 0)
			SetDeviceInfo(m_nPagaNum);
		else
			SetCameraInfo(m_nPagaNum);
		return TRUE;
	}
	catch (CException* e)
	{

	}
	return FALSE;
}

BOOL CSBGLDlg::SetBianji(int nIndex)
{
	m_nRedact = 1;
	m_csIpOld = m_DataList[nIndex].csIP;
	m_csIp = m_DataList[nIndex].csIP;
	m_csPos = m_DataList[nIndex].strPos;
	m_csFaceDbs = m_DataList[nIndex].csFaceDbs;

	UpdateData(FALSE);
	InvalidateRect(m_rTable, false);

	return TRUE;
}

BOOL CSBGLDlg::SetBaocun()
{
	try
	{
		UpdateData();
		CString csSql;

		if (m_nSelectIndex == 0)
		{
			csSql.Format(_T("UPDATE device set description = '%s',ip = '%s' WHERE ip = '%s'"), m_csPos, m_csIp, m_csIpOld);
			g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		}
		else
		{
			_RecordsetPtr pRecordset;

			csSql.Format(_T("SELECT * FROM camera WHERE ip = '%s'"), m_csIpOld);
			pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
			if (!pRecordset->adoEOF)
			{
				CString csCameraRts = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("rtsp")->Value;
				csCameraRts.Replace(m_csIpOld, m_csIp);
				csSql.Format(_T("UPDATE camera set addr = '%s',ip = '%s' ,rtsp = '%s'WHERE ip = '%s'"), m_csPos, m_csIp, csCameraRts, m_csIpOld);
				g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
			}
			
		}

		m_nRedact = 0;
		if (m_nSelectIndex == 0)
			SetDeviceInfo(m_nPagaNum);
		else
			SetCameraInfo(m_nPagaNum);

		return TRUE;
	}
	catch (CException* e)
	{

	}
	return FALSE;
}

BOOL CSBGLDlg::SetQuxiao()
{
	try
	{
		m_nRedact = 0;
		
		 m_csIp = _T("");
		 m_csIpOld = _T("");
		 m_csPos = _T("");
		 m_csFaceDbs = _T("");
		 UpdateData(FALSE);
		InvalidateRect(m_rTable, false);
		return TRUE;
	}
	catch (CException* e)
	{

	}
	return FALSE;
}

BOOL CSBGLDlg::SetTianjia()
{
	try
	{
		UpdateData();
		CString csSql;
		COleDateTime cTime = COleDateTime::GetCurrentTime();
		CString csTime = cTime.Format(_T("%Y-%m-%d %H:%M:%S"));
		CString csRtst; csRtst.Format(_T("rtsp://admin:unisib1000@%s/h264/ch1/main/av_stream"), m_csIp);
		if (m_nSelectIndex == 0)
			csSql.Format(_T("insert into device(onlyid, ip, description,isonline,lasttime, type,facedbs)values('%s','%s','%s','%s','%s','%s','%s')"),
			_T("1"), m_csIp, m_csPos, _T("1"), csTime, _T("1"), _T("1"), m_csFaceDbs);
		else
			csSql.Format(_T("insert into camera(ip, addr, rtsp,camera_state, Camera_type)values('%s','%s','%s','%s','%s')"),
			m_csIp, m_csPos, csRtst, _T("1"), _T("1"));

		g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);

		if (m_nSelectIndex == 0)
			SetDeviceInfo(m_nPagaNum);
		else
			SetCameraInfo(m_nPagaNum);
		return TRUE;
	}
	catch (CException* e)
	{

	}
	return FALSE;
}


BOOL CSBGLDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN&&pMsg->wParam == VK_RETURN)
		return TRUE;
	if (pMsg->message == WM_KEYDOWN&&pMsg->wParam == VK_ESCAPE)
		return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}

void CSBGLDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (m_rFuwuqi.PtInRect(point))
	{
		if (m_nSelectIndex != 0)
		{
			m_csIp = _T("");
			m_csPos = _T("");
			m_csIpOld = _T("");
			m_csFaceDbs = _T("");
			GetDlgItem(IDC_EDIT_POST2)->ShowWindow(SW_SHOW);
			UpdateData(FALSE);
			m_nSelectIndex = 0;
			GetMaxPaga();
			m_nPagaNum = 1;

			SetDeviceInfo(m_nPagaNum);
		}
	}
	else if (m_rXiangji.PtInRect(point))
	{
		if (m_nSelectIndex != 1)
		{
			m_csIp = _T("");
			m_csPos = _T("");
			m_csIpOld = _T("");
			m_csFaceDbs = _T("");
			GetDlgItem(IDC_EDIT_POST2)->ShowWindow(SW_HIDE);

			UpdateData(FALSE);
			m_nSelectIndex = 1;
			GetMaxPaga();
			m_nPagaNum = 1;
			SetCameraInfo(m_nPagaNum);
		}
	}
	else if (m_rBaocun.PtInRect(point))
	{
		SetBaocun();
	}
	else if (m_rQuxiao.PtInRect(point))
	{
		SetQuxiao();
	}
	else if (m_rTianjia.PtInRect(point))
	{
		SetTianjia();
	}
	else if (m_rUp.PtInRect(point))
	{
		SetUp();
	}
	else if (m_rNext.PtInRect(point))
	{
		SetNext();
	}
	else
	{
		for (int nIndex = 0; nIndex < m_iDisTongjiCount; nIndex++)
		{
			if (m_DataList[nIndex].rBJ.PtInRect(point))
			{
				SetBianji(nIndex);
				break;
			}
			else if (m_DataList[nIndex].rSC.PtInRect(point))
			{
				SetShanchu(nIndex);
				break;
			}
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


HBRUSH CSBGLDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor)
	{
	case CTLCOLOR_EDIT:
		pDC->SetTextColor(m_colorEditText);
		pDC->SetBkColor(m_colorEditBK);
		return (HBRUSH)(m_pEditBkBrush->GetSafeHandle());

	default:
		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	}
}
BOOL CSBGLDlg::GetMaxPaga()
{

	CString csSql;

	if (m_nSelectIndex == 0)

		csSql = _T("select count(1) from device");
	else
		csSql = _T("select count(1) from camera ");
	try
	{
		_RecordsetPtr pRecordset;
		USES_CONVERSION;

		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);

		if (!pRecordset->adoEOF)
		{
			CString csEmpInfoName = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("count(1)")->Value;
			int nSum = atoi(T2A(csEmpInfoName));

			if (nSum > 21)
			{
				m_nPagasMax = nSum /21;
				if (nSum % 21 != 0)
					m_nPagasMax++;
			}
		}
	}
	catch (...)
	{
		return FALSE;
	}

	return  TRUE;
}
void CSBGLDlg::SetUp()
{
	if (1 < m_nPagaNum)
	{
		m_nPagaNum--;
		m_nRedact = 0;
		m_csPage.Format(_T("%d"), m_nPagaNum);
		UpdateData(FALSE);
		if (m_nSelectIndex == 0)
			SetDeviceInfo(m_nPagaNum);
		else
			SetCameraInfo(m_nPagaNum);
	}
}
void CSBGLDlg::SetNext()
{
	
	if (m_nPagaNum < m_nPagasMax)
	{
		m_nPagaNum++;
		m_nRedact = 0;
		if (m_nSelectIndex == 0)
			SetDeviceInfo(m_nPagaNum);
		else
			SetCameraInfo(m_nPagaNum);

		m_csPage.Format(_T("%d"), m_nPagaNum);
		UpdateData(FALSE);
	}
}