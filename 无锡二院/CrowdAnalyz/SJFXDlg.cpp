// SJFXDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CrowdAnalyz.h"
#include "SJFXDlg.h"
#include "afxdialogex.h"
#include "CrowdAnalyzDlg.h"

static _ConnectionPtr  g_pConnectMigration;

// CSJFXDlg 对话框

IMPLEMENT_DYNAMIC(CSJFXDlg, CDialogEx)

CSJFXDlg::CSJFXDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CSJFXDlg::IDD, pParent)
, m_csCurrect(_T(""))
{

}

CSJFXDlg::~CSJFXDlg()
{
}

void CSJFXDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COM_CURROR, m_comCurrect);
	DDX_CBString(pDX, IDC_COM_CURROR, m_csCurrect);
}


BEGIN_MESSAGE_MAP(CSJFXDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CSJFXDlg 消息处理程序


void CSJFXDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO:  在此处添加消息处理程序代码
}


void CSJFXDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	CBitmap * pOldBitmap = pMemDC->SelectObject(pBitmap);
	pMemDC->PatBlt(0, 0, m_rClient.Width(), m_rClient.Height(), PATCOPY);
	Graphics g(pMemDC->m_hDC);
	//画总背景
	Image img_bk(L"img/zqzl.jpg");
	g.DrawImage(&img_bk, 0, 0, m_rClient.Width(), m_rClient.Height());

	Image img_dc(L"img/dc-1.jpg");
	g.DrawImage(&img_dc, m_rOut.left, m_rOut.top, m_rOut.Width(), m_rOut.Height());

	// 	Pen redPen(Color(255, 0, 0), 1);
	// 	for (int i = 0; i < 9; i++)
	// 	{
	// 		g.DrawRectangle(&redPen, m_zhouqizhonglan[i].rNum.left, m_zhouqizhonglan[i].rNum.top, m_zhouqizhonglan[i].rNum.Width(), m_zhouqizhonglan[i].rNum.Height());
	// 		g.DrawRectangle(&redPen, m_zhouqizhonglan[i].rBX.left, m_zhouqizhonglan[i].rBX.top, m_zhouqizhonglan[i].rBX.Width(), m_zhouqizhonglan[i].rBX.Height());
	// 		g.DrawRectangle(&redPen, m_zhouqizhonglan[i].rCount.left, m_zhouqizhonglan[i].rCount.top, m_zhouqizhonglan[i].rCount.Width(), m_zhouqizhonglan[i].rCount.Height());
	// 		g.DrawRectangle(&redPen, m_zhouqizhonglan[i].rOperate.left, m_zhouqizhonglan[i].rOperate.top, m_zhouqizhonglan[i].rOperate.Width(), m_zhouqizhonglan[i].rOperate.Height());
	// 	}
	// 
	// 	g.DrawRectangle(&redPen, m_rCurror.left, m_rCurror.top, m_rCurror.Width(), m_rCurror.Height());
	// 	g.DrawRectangle(&redPen, m_rSelect.left, m_rSelect.top, m_rSelect.Width(), m_rSelect.Height());
	// 	g.DrawRectangle(&redPen, m_rOut.left, m_rOut.top, m_rOut.Width(), m_rOut.Height());
	// 	g.DrawRectangle(&redPen, m_rNext.left, m_rNext.top, m_rNext.Width(), m_rNext.Height());
	// 	g.DrawRectangle(&redPen, m_rUp.left, m_rUp.top, m_rUp.Width(), m_rUp.Height());
	// 	g.DrawRectangle(&redPen, m_rPage.left, m_rPage.top, m_rPage.Width(), m_rPage.Height());
	// 	g.DrawRectangle(&redPen, m_rMaxPage.left, m_rMaxPage.top, m_rMaxPage.Width(), m_rMaxPage.Height());
	// 	g.DrawRectangle(&redPen, m_rListData.left, m_rListData.top, m_rListData.Width(), m_rListData.Height());

	Gdiplus::Font font(L"方正细倩简体", m_rClient.Width() * 9 / 1000, FontStyleRegular, UnitPixel);
	SolidBrush percentBrush(Color(255, 212, 214, 222));
	StringFormat stringformat;
	stringformat.SetAlignment(StringAlignmentCenter);
	stringformat.SetLineAlignment(StringAlignmentCenter);
	CString csTemp;
	csTemp.Format(_T("%d"), m_nPage);
	g.DrawString(csTemp.AllocSysString(), -1, &font, RectF(m_rPage.left, m_rPage.top, m_rPage.Width(), m_rPage.Height()), &stringformat, &percentBrush);
	csTemp.Format(_T("%d"), m_nMaxPage);
	g.DrawString(csTemp.AllocSysString(), -1, &font, RectF(m_rMaxPage.left, m_rMaxPage.top, m_rMaxPage.Width(), m_rMaxPage.Height()), &stringformat, &percentBrush);


	DisDatalist(&g);


	//显示缓存，清除HDC
	dc.BitBlt(0, 0, m_rClient.Width(), m_rClient.Height(), pMemDC, 0, 0, SRCCOPY);
	pMemDC->SelectObject(pOldBitmap);
	g.ReleaseHDC(pMemDC->m_hDC);
}


BOOL CSJFXDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_iDataLinstCount = 0;
	m_nPage = 0;
	m_nMaxPage = 0;
	m_rClient.SetRect(0, 0, 1920 * WIDTH / 1920, 830 * HEIGHT / 1080);
	m_rCurror.SetRect(149 * m_rClient.Width() / 1920, 74 * m_rClient.Height() / 830, 364 * m_rClient.Width() / 1920, 93 * m_rClient.Height() / 830);
	m_rSelect.SetRect(375 * m_rClient.Width() / 1920, 71 * m_rClient.Height() / 830, 421 * m_rClient.Width() / 1920, 96 * m_rClient.Height() / 830);
	m_rOut.SetRect(431 * m_rClient.Width() / 1920, 71 * m_rClient.Height() / 830, 480 * m_rClient.Width() / 1920, 98 * m_rClient.Height() / 830);
	m_rUp.SetRect(316 * m_rClient.Width() / 1920, 727 * m_rClient.Height() / 830, 386 * m_rClient.Width() / 1920, 745 * m_rClient.Height() / 830);
	m_rNext.SetRect(479 * m_rClient.Width() / 1920, 727 * m_rClient.Height() / 830, 546 * m_rClient.Width() / 1920, 745 * m_rClient.Height() / 830);
	m_rPage.SetRect(416 * m_rClient.Width() / 1920, 730 * m_rClient.Height() / 830, 447 * m_rClient.Width() / 1920, 745 * m_rClient.Height() / 830);
	m_rMaxPage.SetRect(709 * m_rClient.Width() / 1920, 727 * m_rClient.Height() / 830, 750 * m_rClient.Width() / 1920, 745 * m_rClient.Height() / 830);
	m_rListData.SetRect(56 * m_rClient.Width() / 1920, 154 * m_rClient.Height() / 830, 767 * m_rClient.Width() / 1920, 747 * m_rClient.Height() / 830);
	GetDlgItem(IDC_COM_CURROR)->MoveWindow(m_rCurror.left, m_rCurror.top, m_rCurror.Width(), m_rCurror.Height());


	for (int i = 0; i < 9; i++)//
	{
		m_zhouqizhonglan[i].rNum.SetRect((62 * m_rClient.Width() / 1920), (159 + (63 * i))* m_rClient.Height() / 830, (160 * m_rClient.Width() / 1920), (216 + (63 * i))* m_rClient.Height() / 830);
		m_zhouqizhonglan[i].rBX.SetRect((170 * m_rClient.Width() / 1920), (159 + (63 * i))* m_rClient.Height() / 830, (266 * m_rClient.Width() / 1920), (216 + (63 * i))* m_rClient.Height() / 830);
		m_zhouqizhonglan[i].rCount.SetRect((271 * m_rClient.Width() / 1920), (159 + (63 * i))* m_rClient.Height() / 830, (390 * m_rClient.Width() / 1920), (216 + (63 * i))* m_rClient.Height() / 830);
		m_zhouqizhonglan[i].rOperate.SetRect((688 * m_rClient.Width() / 1920), (170 + (63 * i))* m_rClient.Height() / 830, (748 * m_rClient.Width() / 1920), (200 + (63 * i))* m_rClient.Height() / 830);
	}

	SetComboxCurrent();
	if (!InitDatabaseMigration())
		MessageBox(L"数据库连接失败");
	//双缓存
	CClientDC dc(this);
	pMemDC = new CDC;
	pBitmap = new CBitmap;
	pMemDC->CreateCompatibleDC(&dc);
	pBitmap->CreateCompatibleBitmap(&dc, m_rClient.Width(), m_rClient.Height());

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


HBRUSH CSJFXDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CSJFXDlg::DisDatalist(Graphics *g)
{

	Image imagebx0(L"data/Zhouqibx_0.jpg");
	Image imagebx1(L"data/Zhouqibx_1.jpg");
	Image imagebx2(L"data/Zhouqibx_2.jpg");
	Image imagebx3(L"data/Zhouqibx_3.jpg");
	Image imagebx4(L"data/Zhouqibx_4.jpg");
	Image imagebx5(L"data/Zhouqibx_5.jpg");
	Image imagebx6(L"data/Zhouqibx_6.jpg");
	Image imagebx7(L"data/Zhouqibx_7.jpg");
	Image imagebx8(L"data/Zhouqibx_8.jpg");

	Image imageXQ(L"img/rlkgl_xiangqing.jpg");

	if (m_iDataLinstCount >= 1)
	{
		g->DrawImage(&imagebx0, m_zhouqizhonglan[0].rBX.left, m_zhouqizhonglan[0].rBX.top, m_zhouqizhonglan[0].rBX.Width(), m_zhouqizhonglan[0].rBX.Height());
		g->DrawImage(&imageXQ, m_zhouqizhonglan[0].rOperate.left, m_zhouqizhonglan[0].rOperate.top, m_zhouqizhonglan[0].rOperate.Width(), m_zhouqizhonglan[0].rOperate.Height());
	}
	if (m_iDataLinstCount >= 2)
	{
		g->DrawImage(&imagebx1, m_zhouqizhonglan[1].rBX.left, m_zhouqizhonglan[1].rBX.top, m_zhouqizhonglan[1].rBX.Width(), m_zhouqizhonglan[1].rBX.Height());
		g->DrawImage(&imageXQ, m_zhouqizhonglan[1].rOperate.left, m_zhouqizhonglan[1].rOperate.top, m_zhouqizhonglan[1].rOperate.Width(), m_zhouqizhonglan[1].rOperate.Height());

	}
	if (m_iDataLinstCount >= 3)
	{
		g->DrawImage(&imagebx2, m_zhouqizhonglan[2].rBX.left, m_zhouqizhonglan[2].rBX.top, m_zhouqizhonglan[2].rBX.Width(), m_zhouqizhonglan[2].rBX.Height());
		g->DrawImage(&imageXQ, m_zhouqizhonglan[2].rOperate.left, m_zhouqizhonglan[2].rOperate.top, m_zhouqizhonglan[2].rOperate.Width(), m_zhouqizhonglan[2].rOperate.Height());

	}
	if (m_iDataLinstCount >= 4)
	{
		g->DrawImage(&imageXQ, m_zhouqizhonglan[3].rOperate.left, m_zhouqizhonglan[3].rOperate.top, m_zhouqizhonglan[3].rOperate.Width(), m_zhouqizhonglan[3].rOperate.Height());
		g->DrawImage(&imagebx3, m_zhouqizhonglan[3].rBX.left, m_zhouqizhonglan[3].rBX.top, m_zhouqizhonglan[3].rBX.Width(), m_zhouqizhonglan[3].rBX.Height());
	}
	if (m_iDataLinstCount >= 5)
	{
		g->DrawImage(&imageXQ, m_zhouqizhonglan[4].rOperate.left, m_zhouqizhonglan[4].rOperate.top, m_zhouqizhonglan[4].rOperate.Width(), m_zhouqizhonglan[4].rOperate.Height());
		g->DrawImage(&imagebx4, m_zhouqizhonglan[4].rBX.left, m_zhouqizhonglan[4].rBX.top, m_zhouqizhonglan[4].rBX.Width(), m_zhouqizhonglan[4].rBX.Height());
	}if (m_iDataLinstCount >= 6)
	{
		g->DrawImage(&imageXQ, m_zhouqizhonglan[5].rOperate.left, m_zhouqizhonglan[5].rOperate.top, m_zhouqizhonglan[5].rOperate.Width(), m_zhouqizhonglan[5].rOperate.Height());
		g->DrawImage(&imagebx5, m_zhouqizhonglan[5].rBX.left, m_zhouqizhonglan[5].rBX.top, m_zhouqizhonglan[5].rBX.Width(), m_zhouqizhonglan[5].rBX.Height());
	}if (m_iDataLinstCount >= 7)
	{
		g->DrawImage(&imageXQ, m_zhouqizhonglan[6].rOperate.left, m_zhouqizhonglan[6].rOperate.top, m_zhouqizhonglan[6].rOperate.Width(), m_zhouqizhonglan[6].rOperate.Height());
		g->DrawImage(&imagebx6, m_zhouqizhonglan[6].rBX.left, m_zhouqizhonglan[6].rBX.top, m_zhouqizhonglan[6].rBX.Width(), m_zhouqizhonglan[6].rBX.Height());
	}if (m_iDataLinstCount >= 8)
	{
		g->DrawImage(&imageXQ, m_zhouqizhonglan[7].rOperate.left, m_zhouqizhonglan[7].rOperate.top, m_zhouqizhonglan[7].rOperate.Width(), m_zhouqizhonglan[7].rOperate.Height());
		g->DrawImage(&imagebx7, m_zhouqizhonglan[7].rBX.left, m_zhouqizhonglan[7].rBX.top, m_zhouqizhonglan[7].rBX.Width(), m_zhouqizhonglan[7].rBX.Height());
	}if (m_iDataLinstCount >= 9)
	{
		g->DrawImage(&imageXQ, m_zhouqizhonglan[8].rOperate.left, m_zhouqizhonglan[8].rOperate.top, m_zhouqizhonglan[8].rOperate.Width(), m_zhouqizhonglan[8].rOperate.Height());
		g->DrawImage(&imagebx8, m_zhouqizhonglan[8].rBX.left, m_zhouqizhonglan[8].rBX.top, m_zhouqizhonglan[8].rBX.Width(), m_zhouqizhonglan[8].rBX.Height());
	}


	Gdiplus::Font font(L"方正细倩简体", m_rClient.Width() * 9 / 1000, FontStyleRegular, UnitPixel);
	SolidBrush percentBrush(Color(255, 212, 214, 222));
	StringFormat stringformat;
	stringformat.SetAlignment(StringAlignmentCenter);
	stringformat.SetLineAlignment(StringAlignmentCenter);

	for (int i = 0; i < m_iDataLinstCount; i++)
	{
		g->DrawString(m_zhouqizhonglan[i].strNum.AllocSysString(), -1, &font, RectF(m_zhouqizhonglan[i].rNum.left, m_zhouqizhonglan[i].rNum.top, m_zhouqizhonglan[i].rNum.Width(), m_zhouqizhonglan[i].rNum.Height()), &stringformat, &percentBrush);
		g->DrawString(m_zhouqizhonglan[i].iCount.AllocSysString(), -1, &font, RectF(m_zhouqizhonglan[i].rCount.left, m_zhouqizhonglan[i].rCount.top, m_zhouqizhonglan[i].rCount.Width(), m_zhouqizhonglan[i].rCount.Height()), &stringformat, &percentBrush);
	}

}

BOOL CSJFXDlg::InitDatabaseMigration()
{
	CString csSqlConnect;

	TCHAR pFileName[MAX_PATH];
	int nPos = GetCurrentDirectory(MAX_PATH, pFileName);

	CString csFullPath(pFileName);
	csFullPath += "\\Parameter.ini";
	GetPrivateProfileString(_T("SqlConfiguration"), _T("Sql_ServerAddMigration "), _T(""), m_csSqlIP.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);
	GetPrivateProfileString(_T("SqlConfiguration"), _T("Sql_DatabaseHistory"), _T(""), m_csSqlData.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);
	GetPrivateProfileString(_T("SqlConfiguration"), _T("Sql_Name"), _T(""), m_csSqlName.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);
	GetPrivateProfileString(_T("SqlConfiguration"), _T("Sql_Pwd"), _T(""), m_csSqlPwd.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);
	GetPrivateProfileString(_T("SqlConfiguration"), _T("Img_ServerAdd"), _T(""), m_csImgIP.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);

	csSqlConnect.Format(_T("Driver={MySQL ODBC 5.1 Driver};Server=%s;Database=%s;uid=%s;pwd=%s;"), m_csSqlIP, m_csSqlData, m_csSqlName, m_csSqlPwd);
	try
	{
		g_pConnectMigration.CreateInstance(__uuidof(Connection));
		_bstr_t strConnect = csSqlConnect;
		g_pConnectMigration->Open(strConnect, "", "", adModeUnknown);
	}
	catch (_com_error e)
	{
		return FALSE;
	}
	return  TRUE;
}

void CSJFXDlg::SetComboxCurrent()
{
	try
	{
		_RecordsetPtr pRecordset;
		CString csSql;
		BOOL bFlag = FALSE;
		csSql = (_T("SELECT * FROM cycleinfo WHERE ismigration = 1"));
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		while (!pRecordset->adoEOF)
		{
			bFlag = TRUE;
			CString csMaxNum = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("startdate")->Value;
			m_comCurrect.AddString(csMaxNum);
			pRecordset->MoveNext();
		}
		m_comCurrect.SetCurSel(0);
	}
	catch (CException* e)
	{
	}
}

CString CSJFXDlg::GetSql()
{
	try
	{
		UpdateData();
		_RecordsetPtr pRecordset;
		CString csSql, csName, csName1, csUrrect, csTemp;
		BOOL bFist = TRUE, bTow = TRUE;
		csSql.Format(_T("SELECT * FROM cycleinfo WHERE startdate = '%s' "), m_csCurrect);
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		if (!pRecordset->adoEOF)
		{
			csUrrect = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("currentcycle")->Value;
		}
		else
		{
			return NULL;
		}

		csName.Format(_T("blacklist_%s"), csUrrect);
		csTemp.Format(_T(" SELECT table_name FROM information_schema.TABLES WHERE table_name ='%s'"), csName);
		pRecordset = g_pConnectMigration->Execute(_bstr_t(csTemp), NULL, adCmdText);
		if (pRecordset->adoEOF)
		{
			bFist = FALSE;
		}


		csName1.Format(_T("dynamic_%s"), csUrrect);
		csTemp.Format(_T(" SELECT table_name FROM information_schema.TABLES WHERE table_name ='%s'"), csName1);
		pRecordset = g_pConnectMigration->Execute(_bstr_t(csTemp), NULL, adCmdText);
		if (pRecordset->adoEOF)
		{
			bTow = FALSE;
		}

		if (bFist == FALSE && bTow == FALSE)
		{
			return NULL;
		}
		else if (bFist == FALSE)
		{
			csTemp.Format(_T("select * ,count(*) as num from %s"), csName1);
			return csTemp;
		}
		else if (bFist == FALSE)
		{
			csTemp.Format(_T("select * ,count(*) as num from %s"), csName);
			return csTemp;
		}
		else if (bFist == TRUE && bTow == TRUE)
		{
			CString csReturn;
			csTemp.Format(_T("select * ,count(*) as num from %s"), csName1);
			csReturn += csTemp;
			csReturn += _T(" union ");
			csTemp.Format(_T("select * ,count(*) as num from %s"), csName);
			csReturn += csTemp;
			return csTemp;
		}

	}
	catch (CException* e)
	{
	}
	return NULL;
}


CString CSJFXDlg::GetXiangqingSql(CString csIdcard)
{
	try
	{
		UpdateData();
		_RecordsetPtr pRecordset;
		CString csSql, csName, csName1, csUrrect, csTemp;
		BOOL bFist = TRUE, bTow = TRUE;
		csSql.Format(_T("SELECT * FROM cycleinfo WHERE startdate = '%s' "), m_csCurrect);
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		if (!pRecordset->adoEOF)
		{
			csUrrect = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("currentcycle")->Value;
		}
		else
		{
			return NULL;
		}

		csName.Format(_T("blacklist_%s"), csUrrect);
		csTemp.Format(_T(" SELECT table_name FROM information_schema.TABLES WHERE table_name ='%s'"), csName);
		pRecordset = g_pConnectMigration->Execute(_bstr_t(csTemp), NULL, adCmdText);
		if (pRecordset->adoEOF)
		{
			bFist = FALSE;
		}


		csName1.Format(_T("dynamic_%s"), csUrrect);
		csTemp.Format(_T(" SELECT table_name FROM information_schema.TABLES WHERE table_name ='%s'"), csName1);
		pRecordset = g_pConnectMigration->Execute(_bstr_t(csTemp), NULL, adCmdText);
		if (pRecordset->adoEOF)
		{
			bTow = FALSE;
		}

		if (bFist == FALSE && bTow == FALSE)
		{
			return NULL;
		}
		else if (bFist == FALSE)
		{
			csTemp.Format(_T("select * from %s WHERE id_card = '%s'"), csName1, csIdcard);
			return csTemp;
		}
		else if (bFist == FALSE)
		{
			csTemp.Format(_T("select * from %s WHERE id_card = '%s'"), csName, csIdcard);
			return csTemp;
		}
		else if (bFist == TRUE && bTow == TRUE)
		{
			CString csReturn;
			csTemp.Format(_T("select * from %s WHERE id_card = '%s'"), csName1, csIdcard);
			csReturn += csTemp;
			csReturn += _T(" union ");
			csTemp.Format(_T("select * from %s WHERE id_card = '%s'"), csName, csIdcard);
			csReturn += csTemp;
			return csTemp;
		}

	}
	catch (CException* e)
	{
	}
	return NULL;
}

BOOL CSJFXDlg::GetIdcardImg(CString csImgName, CString csIdcard, CString csSnapshotid, int nIndex)
{
	try
	{
		CString csSql, csTemp;
		_RecordsetPtr pRecordset;

		//取靶向
		csSql.Format(_T("select * from imginfo WHERE id_card = '%s';"), csIdcard);
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		if (!pRecordset->adoEOF)
		{

			CString csImgPath = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("imgpath")->Value;
			int nStart = csImgPath.Find(_T("/Data"));
			csImgPath = csImgPath.Mid(nStart, csImgPath.GetLength() - nStart);

			csTemp.Format(_T("data/%sbx_%d.jpg"), csImgName, nIndex);
			if (m_cTools.DownloadImageFromFileServer(m_csImgIP, IMG_PORT, csImgPath, csTemp) == -1)
			{
				SetNullJpg(csTemp);
			}
		}
		else
		{
			csTemp.Format(_T("data/%sbx_%d.jpg"), csImgName, nIndex);
			SetNullJpg(csTemp);
			return FALSE;
		}

		return TRUE;

	}
	catch (CException* e)
	{
	}

	return FALSE;
}

BOOL CSJFXDlg::GetSnapIdImg(CString csImgName, CString csSnapshotid, CString csPath,CString &csTime,CString &csCameAdd)
{
	try
	{
		CString csSql, csTemp;
		_RecordsetPtr pRecordset;
		//取抓拍图
		csSql.Format(_T("select * from snapshot WHERE Snapshotid = '%s';"), csSnapshotid);
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		if (!pRecordset->adoEOF)
		{
			CString csImgPath = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("FacePhotoName")->Value;
			csTime = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("alarmtime")->Value;
			csCameAdd = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("cameraid")->Value;

			csSql.Format(_T("SELECT * FROM camera WHERE cameraid = '%s';"), csCameAdd);
			pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
			if (!pRecordset->adoEOF)
			{
				csCameAdd = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("addr")->Value;
			}
			int nStart = csImgPath.Find(_T("/Data"));
			csImgPath = csImgPath.Mid(nStart, csImgPath.GetLength() - nStart);
			csTemp.Format(_T("%s/%s.jpg"), csPath, csImgName);
			if (m_cTools.DownloadImageFromFileServer(m_csImgIP, IMG_PORT, csImgPath, csTemp) == -1)
			{
				SetNullJpg(csTemp);
			}
		}
		else
		{
			return FALSE;
		}


		return TRUE;

	}
	catch (CException* e)
	{
	}

	return FALSE;
}
BOOL CSJFXDlg::GetIdcardImgOut(CString csImgName, CString csIdcard, CString csPath)
{
	try
	{
		CString csSql, csTemp;
		_RecordsetPtr pRecordset;

		//取靶向
		csSql.Format(_T("select * from imginfo WHERE id_card = '%s';"), csIdcard);
		pRecordset = g_pConnect->Execute(_bstr_t(csSql), NULL, adCmdText);
		if (!pRecordset->adoEOF)
		{

			CString csImgPath = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("imgpath")->Value;
			int nStart = csImgPath.Find(_T("/Data"));
			csImgPath = csImgPath.Mid(nStart, csImgPath.GetLength() - nStart);

			csTemp.Format(_T("%s/%s.jpg"), csPath, csImgName);
			if (m_cTools.DownloadImageFromFileServer(m_csImgIP, IMG_PORT, csImgPath, csTemp) == -1)
			{
				SetNullJpg(csTemp);
			}
		}
		else
		{
			return FALSE;
		}

		return TRUE;

	}
	catch (CException* e)
	{
	}

	return FALSE;
}
void CSJFXDlg::SetNullJpg(CString csPath)
{
	CFile file;
	if (file.Open(csPath, CFile::modeCreate | CFile::modeWrite))
	{
		char jpgchar[36];
		memset(jpgchar, 0, 36);
		file.Write(jpgchar, 36);
		file.Close();
	}
}
void CSJFXDlg::getListData()
{
	try
	{
		UpdateData();
		_RecordsetPtr pRecordset;
		CString csSql;
		CString csSql1 = GetSql();
		if (csSql1.IsEmpty())
		{
			MessageBox(_T("没有符合条件数据"));
			return;
		}

		int nStart;
		nStart = (m_nPage - 1) * 9;
		csSql.Format(_T("%s group by id_card ORDER BY num  desc  limit %d,9 "), csSql1, nStart);

		pRecordset = g_pConnectMigration->Execute(_bstr_t(csSql), NULL, adCmdText);
		int nListIndex = 0;
		if (pRecordset->adoEOF)
		{
			MessageBox(_T("没有符合条件数据"));
			return;
		}
		while (!pRecordset->adoEOF)
		{
			m_zhouqizhonglan[nListIndex].strNum = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("id_card")->Value;
			m_zhouqizhonglan[nListIndex].iCount = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("num")->Value;

			GetIdcardImg(_T("Zhouqi"),
				(TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("id_card")->Value,
				(TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("snapshotid")->Value, nListIndex);

			nListIndex++;
			pRecordset->MoveNext();
		}
		m_iDataLinstCount = nListIndex;
	}
	catch (CException* e)
	{
	}
}

BOOL CSJFXDlg::GetMaxPaga()
{
	try
	{
		CString csSql;
		m_nMaxPage = 1;
		m_nPage = 1;
		csSql = GetSql();
		if (csSql.IsEmpty())
		{
			return FALSE;
		}
		csSql.Format(_T("%s group by id_card ORDER BY num  desc "), csSql);
		try
		{
			_RecordsetPtr pRecordset;
			USES_CONVERSION;

			pRecordset = g_pConnectMigration->Execute(_bstr_t(csSql), NULL, adCmdText);
			int nTemp = 0;
			while (!pRecordset->adoEOF)
			{
				nTemp++;
				pRecordset->MoveNext();
			}

			if (nTemp > 9)
			{
				m_nMaxPage = nTemp / 9;
				if (nTemp % 9 != 0)
					m_nMaxPage++;
			}
		}
		catch (...)
		{
			return FALSE;
		}

		return  TRUE;
	}
	catch (CException* e)
	{
		return FALSE;
	}

}

void CSJFXDlg::SetUp()
{
	if (1 < m_nPage)
	{
		m_nPage--;
		m_csPaga.Format(_T("%d"), m_nPage);
		UpdateData(FALSE);
		getListData();
	}
	else
		MessageBox(_T("当前页为首页"));
}
void CSJFXDlg::SetNext()
{

	if (m_nPage < m_nMaxPage)
	{
		m_nPage++;
		m_csPaga.Format(_T("%d"), m_nPage);
		UpdateData(FALSE);
		getListData();
	}
	else
		MessageBox(_T("当前页为末页"));
}

char* CStringToChar(CString csData,int &nLen)
{
	try
	{
		DWORD dwNum = WideCharToMultiByte(CP_UTF8, NULL, csData, -1, NULL, NULL, 0, NULL);
		char *cResult = (char *)malloc(dwNum + 1);
		memset(cResult, 0, dwNum + 1);
		WideCharToMultiByte(CP_UTF8, NULL, csData, -1, cResult, dwNum, 0, NULL);
		nLen = dwNum;
		return cResult;
	}
	catch (CException* e)
	{
	}

}


BOOL CSJFXDlg::OutList()
{
	try
	{
		CString csSql;
		CString csGetSql = GetSql();
		if (csGetSql.IsEmpty())
		{
			return FALSE;
		}

		TCHAR pFileName[MAX_PATH];
		int nPos = GetCurrentDirectory(MAX_PATH, pFileName);
		CString csFullPath(pFileName);

		COleDateTime cTime = COleDateTime::GetCurrentTime();
		CString csTime = cTime.Format(_T("%Y%m%d"));

		CString csRootFile = csFullPath + _T("\\Report");
		CString csFormworkFile = csFullPath + _T("\\Report\\Formwork");
		BOOL IsD = PathIsDirectory(csRootFile);
		if (!IsD)
			return FALSE;

		CString strDstPath;
		strDstPath.Format(L"%s/%s", csRootFile, csTime);
		IsD = PathIsDirectory(strDstPath);
		if (!IsD)
			CreateDirectoryW(strDstPath, NULL);

		CString csDstImgPath;
		csDstImgPath.Format(L"%s/img", strDstPath);
		IsD = PathIsDirectory(csDstImgPath);
		if (!IsD)
			CreateDirectoryW(csDstImgPath, NULL);


		CString csParticularsPath;
		csParticularsPath.Format(L"%s/Particulars", strDstPath);
		IsD = PathIsDirectory(csParticularsPath);
		if (!IsD)
			CreateDirectoryW(csParticularsPath, NULL);


		if (!CopyFile(csFormworkFile + _T("\\Report.html"), strDstPath + _T("\\Report.html"), false))
			return FALSE;

		if (!CopyFile(csFormworkFile + _T("\\style.css"), strDstPath + _T("\\style.css"), false))
			return FALSE;

		if (!CopyFile(csFormworkFile + _T("\\img\\top_bj.jpg"), strDstPath + _T("\\img\\top_bj.jpg"), false))
			return FALSE;

		
		csSql.Format(_T("%s group by id_card ORDER BY num  desc "), csGetSql);
		_RecordsetPtr pRecordset;
		pRecordset = g_pConnectMigration->Execute(_bstr_t(csSql), NULL, adCmdText);
		int nTemp = 0;
		if (pRecordset->adoEOF)
			return FALSE;

		CString csEndOut;
		int nIndex = 1;
		while (!pRecordset->adoEOF)
		{
			CString csParticularsPath, csTemp;
			CString csIdcatd = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("id_card")->Value;
			CString csSnapshotid = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("snapshotid")->Value;
			csParticularsPath.Format(_T("%s\\Particulars\\%s.html"), strDstPath, csIdcatd);

			if (!CopyFile(csFormworkFile + _T("\\Particulars.html"), csParticularsPath, false))
				return FALSE;

			if (OutParticulars(csIdcatd, csDstImgPath, csParticularsPath))
			{
			}

			if (GetIdcardImgOut(csSnapshotid,csIdcatd, csDstImgPath))
			{
			}

			csTemp.Format(_T("<tr><td></td>\
						<td>%d</td>\
						<td>%s</td>\
						<td>%s</td>\
						<td><span class = \"photo\"><img src = \"img/%s.jpg\"></span></td>\
						<td><a href = \"Particulars\\%s.html\" class = \"alink\" target = \"_self\">详细信息</a></td>\
						<td></td></tr>\r\n"),
						nIndex++,
						csIdcatd,
						(TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("num")->Value,
						csSnapshotid,
						csIdcatd);
			csEndOut += csTemp;
			pRecordset->MoveNext();
		}


		CFile file;
		BYTE sUNICODE[] = { 0xff, 0xfe };
		if (file.Open(strDstPath + _T("\\Report.html"), CFile::modeReadWrite))
		{
			int filelen = file.GetLength();
			char *imgbuf = new char[filelen + 1];
			file.Read(imgbuf, filelen);
			CString csFile(imgbuf);
			int nFind = csFile.Find(_T("<tbody>"));
			if (nFind == -1)
			{
				return FALSE;
			}
			file.Seek(nFind + 70, CFile::begin);

			if (!csEndOut.IsEmpty())
			{
				int nLen;
				csEndOut += _T("</tbody>\r\n</table>\r\n</body>\r\n</html>");
				char * p = CStringToChar(csEndOut, nLen);
				file.Write(p, nLen);
			}

			file.Close();
		}
		
	}
	catch (CException* e)
	{
		return FALSE; 
	}
	return TRUE;
}

BOOL CSJFXDlg::OutParticulars(CString csIdcard, CString csDstImgPath, CString csPath)
{
	try
	{
		CString csTemp, csEndOut,csForTemp,csTime,csCameAdd;
		csTemp = GetXiangqingSql(csIdcard);
		_RecordsetPtr pRecordset;
		int nIndex = 1;
		pRecordset = g_pConnectMigration->Execute(_bstr_t(csTemp), NULL, adCmdText);
		while (!pRecordset->adoEOF)
		{
			CString csSnapshotid = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("snapshotid")->Value;
			GetSnapIdImg(csSnapshotid, csSnapshotid, csDstImgPath, csTime,csCameAdd);
			csForTemp.Format(_T("<tr><td></td><td>%d</td ><td>%s</td><td>%s</td ><td>%s</td><td><span class = \"photo2\"><img src = \"..//img/%s.jpg\"></span></td><td></td></tr>\r\n"), nIndex++, csSnapshotid, csTime, csCameAdd, csSnapshotid);
			csEndOut += csForTemp;
			pRecordset->MoveNext();
		}

		CFile file;
		BYTE sUNICODE[] = { 0xff, 0xfe };
		if (file.Open(csPath, CFile::modeReadWrite))
		{
			int filelen = file.GetLength();
			char *imgbuf = new char[filelen + 1];
			file.Read(imgbuf, filelen);
			CString csFile(imgbuf);
			int nFind = csFile.Find(_T("<tbody>"));
			if (nFind == -1)
			{
				return FALSE;
			}
			file.Seek(nFind + 50, CFile::begin);
			int nSize = csEndOut.GetLength();
			if (!csEndOut.IsEmpty())
			{
				int nLen;
				csEndOut += _T("</tbody>\r\n</table>\r\n</body>\r\n</html>");
				int nSize1 = csEndOut.GetLength();
				char * p = CStringToChar(csEndOut, nLen);
				file.Write(p, nLen);
			}
			file.Close();
		}
	}
	catch (CException* e)
	{
		return FALSE;
	}
	return TRUE;
}
void CSJFXDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_rSelect.PtInRect(point))
	{
		if (GetMaxPaga())
		{
			getListData();
			InvalidateRect(m_rListData, false);
		}
		else
			MessageBox(_T("查询失败"));

	}
	else if (m_rOut.PtInRect(point))
	{
		if (OutList())
			MessageBox(_T("导出成功"));
		else
			MessageBox(_T("导出失败"));

	}
	else if (m_rNext.PtInRect(point))
	{
		SetNext();
		InvalidateRect(m_rListData, false);
	}
	else if (m_rUp.PtInRect(point))
	{
		SetUp();
		InvalidateRect(m_rListData, false);
	}


	for (int i = 0; i < m_iDataLinstCount; i++)
	{
		if (m_zhouqizhonglan[i].rOperate.PtInRect(point))
		{
			CCrowdAnalyzDlg * m_pReceiver = (CCrowdAnalyzDlg*)GetParent();

			m_pReceiver->m_csLSJLSql = GetXiangqingSql(m_zhouqizhonglan[i].strNum);
			m_pReceiver->m_csLSJLIdcard = m_zhouqizhonglan[i].strNum;
			m_pReceiver->SetLsjl();
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}
