// ComboBoxXP.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "./ComboBoxXP.h"
#include "skinscrollwnd.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define GWL_WNDPROC         (-4)

static WNDPROC g_pWndProc = 0;

extern "C" static LRESULT FAR PASCAL BitComboBoxListBoxProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	return CallWindowProc(g_pWndProc, hWnd, nMsg, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////
// CComboBoxXP

CComboBoxXP::CComboBoxXP()
{
	m_colBackGroundHighLight = RGB(8,53,136);//背景颜色
	m_colBackGroundHighLight1 = RGB(6,81,149);//下拉框滑动选择颜色失去焦点颜色
	m_colTextHighLight = RGB(144,149,187);//字体颜色
	m_colBackGroundNormal = RGB(8, 53, 136);//RGB(223,230,240);//背景前景颜色失去焦点颜色
	m_colTextNormal = RGB(144, 149, 187);//下拉框字体颜色
	m_hWndList = NULL;
}

CComboBoxXP::~CComboBoxXP()
{
}


BEGIN_MESSAGE_MAP(CComboBoxXP, CComboBox)
	//{{AFX_MSG_MAP(CComboBoxXP)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()

	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CTLCOLORLISTBOX, OnCtlColorListBox)
	ON_CONTROL_REFLECT(CBN_DROPDOWN, OnCbnDropdown)
	ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCbnCloseup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComboBoxXP message handlers

void CComboBoxXP::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
 	if( lpDrawItemStruct->itemID == -1 )
 		return;
 
 	CDC			dc;	
 	dc.Attach(lpDrawItemStruct->hDC);
 	
 	CBrush pBrush;
 	CString		sItem;
 
	if(lpDrawItemStruct->itemState & ODS_FOCUS)
	{
		pBrush.CreateSolidBrush(m_colBackGroundHighLight1);
		dc.FillRect(&lpDrawItemStruct->rcItem, &pBrush);
		dc.SetTextColor(m_colTextHighLight);
	}
	else
	{
		pBrush.CreateSolidBrush(m_colBackGroundNormal);
		dc.FillRect(&lpDrawItemStruct->rcItem, &pBrush);
		dc.SetTextColor(m_colTextNormal);
	}

	
	if(m_lstBox.GetSafeHwnd())
	{
		// Copy the text of the item to a string
		GetLBText(lpDrawItemStruct->itemID, sItem);
		dc.SetBkMode(TRANSPARENT);

		// Draw the text after the images left postion
		lpDrawItemStruct->rcItem.left = 4;
		dc.SelectObject(m_lstBox.GetFont());
		CFont *pOldFont = dc.SelectObject(&m_font);
		dc.DrawText(sItem, &lpDrawItemStruct->rcItem, DT_VCENTER | DT_SINGLELINE);
		dc.SelectObject(pOldFont);
	}


	if(lpDrawItemStruct->itemAction & ODA_DRAWENTIRE)
		Invalidate(); 	
}

LRESULT CComboBoxXP::OnCtlColorListBox(WPARAM wParam, LPARAM lParam)
{
	//*
	// Here we need to get a reference to the listbox of the combobox
	// (the dropdown part). We can do it using 
	//TRACE("OnCtlColorListBox()\n");
	if (this->m_lstBox.m_hWnd == 0) {
		HWND hWnd = (HWND)lParam;

		if (hWnd != 0 && hWnd != m_hWnd) 
		{
			// Save the handle
			m_lstBox.m_hWnd = hWnd;
			m_lstBox.ShowWindow(SW_HIDE);
			SkinWndScroll(&m_lstBox,m_hBmpScroll);
			m_hWndList=m_lstBox.m_hWnd;
			// Subclass ListBox
			g_pWndProc = (WNDPROC)GetWindowLong(m_lstBox.m_hWnd, GWL_WNDPROC);
			SetWindowLong(m_lstBox.m_hWnd, GWL_WNDPROC, (LONG)BitComboBoxListBoxProc);
		}
	}

	//*/	
	return DefWindowProc(WM_CTLCOLORLISTBOX, wParam, lParam);
}

void CComboBoxXP::OnDestroy() 
{
	if (m_lstBox.GetSafeHwnd() != NULL)
		m_lstBox.UnsubclassWindow();

  CComboBox::OnDestroy();
}

void CComboBoxXP::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	Draw(&dc);	
	// Do not call CComboBox::OnPaint() for painting messages
}
void CComboBoxXP::OnCbnDropdown()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!m_hWndList) return ;
	CWnd *pFrame=CWnd::FromHandle(::GetParent(::GetParent(m_hWndList)));
	CRect rc;
	GetWindowRect(&rc);
	CRect rc2;
	this->GetDroppedControlRect(&rc2);
	int nHei;
	int nLineHei=GetItemHeight(0);
	if(nLineHei*GetCount()<rc2.Height()-2)
		nHei=nLineHei*GetCount()+2;
	else
		nHei=rc2.Height();
	pFrame->SetWindowPos(&wndTopMost,rc.left,rc.bottom,rc2.Width(),nHei,0);
	pFrame->ShowWindow(SW_SHOW);
}
void CComboBoxXP::OnCbnCloseup()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!m_hWndList) return ;
	::ShowWindow(::GetParent(::GetParent(m_hWndList)),SW_HIDE);
}
void CComboBoxXP::Draw(CDC *pDC)
{
 	CDC dc;
 	dc.Attach(pDC->m_hDC);
 	
 	CRect rc;
 	GetClientRect(&rc);
 	dc.FillSolidRect(rc,GetSysColor(COLOR_WINDOW));
	BITMAP bitRight;
	{
		int nIDRight = IDB_BITMAP1;
		ASSERT(nIDRight);
		CDC dcMem;
		dcMem.CreateCompatibleDC(&dc);
		CBitmap bmpComboRight;
		bmpComboRight.LoadBitmap(nIDRight);
		bmpComboRight.GetBitmap(&bitRight);
		CBitmap *pOldBmp = dcMem.SelectObject(&bmpComboRight);
		dc.BitBlt(rc.right - bitRight.bmWidth ,rc.top,bitRight.bmWidth,bitRight.bmHeight,&dcMem,0,0,SRCCOPY);
		dcMem.SelectObject(pOldBmp);
	}

 	CString sz;
 	GetWindowText(sz);
 	rc.left += 4;
 
 	dc.SetBkMode(TRANSPARENT);
	
 	RECT rcRepaint;
 	GetClientRect(&rcRepaint);
 	rcRepaint.left = rcRepaint.left + 2;
 	rcRepaint.right = rcRepaint.right - bitRight.bmWidth-1;
	rcRepaint.top += 2; rcRepaint.bottom -= 2;
 	//Set the normal/highlight color when its repainted 
 	if( GetFocus() && GetFocus()->m_hWnd == m_hWnd)
 	{
 		dc.FillSolidRect(&rcRepaint,m_colBackGroundHighLight);
 		dc.SetTextColor(m_colTextHighLight);
 	}
	else
 	{
 		dc.FillSolidRect(&rcRepaint,m_colBackGroundNormal);
		dc.SetTextColor(m_colTextNormal);
 	}
	
 	{
		CRect rcClient;
		GetClientRect(rcClient);
		CRect rcText(rcClient);
		rcText.DeflateRect(6,4,bitRight.bmWidth+2,4);
		//rcText.left+=1;
		CFont *pOldFont = dc.SelectObject(&m_font);
		dc.DrawText(sz,rcText,DT_VCENTER | DT_SINGLELINE);
		dc.SelectObject(pOldFont);
	}
	
 	if( m_lstBox )
 	{
 		 CDC *pDC = m_lstBox.GetDC();
 		 m_lstBox.GetClientRect(&rc);
 
 		 CBrush pBrush;
 		 pBrush.CreateSolidBrush(m_colBackGroundNormal);
 		 
  	    pDC->FillRect(&rc,&pBrush);
	
 		pDC->SetTextColor(m_colTextNormal);
		
 		RECT rcItemData;
 		CString szItemString;

 		for(int i =0;i<m_lstBox.GetCount();i++)
 		{
 			m_lstBox.GetText(i,szItemString);
 			m_lstBox.GetItemRect(i,&rcItemData);
 			
 			rcItemData.left += 5;
 
 			pDC->SetBkMode(TRANSPARENT);
 			pDC->SetTextColor(m_colTextNormal);
 
 			//nListBoxTextLeft = rcItemData.left;
 			
			//CFont* def_font = pDC->SelectObject(&m_Font);
 			CFont *pOldFont = pDC->SelectObject(&m_font);
			pDC->DrawText(szItemString,&rcItemData, DT_VCENTER | DT_SINGLELINE);
 			pDC->SelectObject(pOldFont);
			DeleteObject(pOldFont);
 		}

		 ReleaseDC(pDC);
	
 	}

 
	//draw border
	{
		//CRect rcClient;
	//	GetClientRect(rcClient);
	//	CBrush br(RGB( 255, 0, 255));//外边框颜色
	//	dc.FrameRect(rcClient,&br);
	}

	dc.Detach();
}

void CComboBoxXP::PreSubclassWindow() 
{
	ModifyStyle(NULL,CBS_DROPDOWNLIST | CBS_OWNERDRAWFIXED | CBS_HASSTRINGS);
	
	if(!m_font.GetSafeHandle())
		m_font.CreatePointFont(100,L"宋体");	
	
	CComboBox::PreSubclassWindow();
}

void CComboBoxXP::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
		lpMeasureItemStruct->itemHeight = 21;
}

int CComboBoxXP::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if(!m_font.GetSafeHandle())
		m_font.CreatePointFont(100,L"宋体");	

	return 0;
}

BOOL CComboBoxXP::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;	
	return CComboBox::OnEraseBkgnd(pDC);
}


