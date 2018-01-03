#if !defined(AFX_COMBOBOXXP_H__69E909B6_B6CA_4CE6_BEF5_2AA1CCAD6A2B__INCLUDED_)
#define AFX_COMBOBOXXP_H__69E909B6_B6CA_4CE6_BEF5_2AA1CCAD6A2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ComboBoxXP.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CComboBoxXP window


class  CComboBoxXP : public CComboBox
{
// Construction
public:
	CComboBoxXP();
	void SkinScroll(HBITMAP hBmp)
	{
		m_hBmpScroll = hBmp;
		//手动弹出下拉列表以便进行列表窗口的子类化
		ShowDropDown(TRUE);
		ShowDropDown(FALSE);
	}
	
private:
	HWND	m_hWndList;
	HBITMAP	m_hBmpScroll;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComboBoxXP)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CComboBoxXP();

	// Generated message map functions
protected:
	virtual void Draw(CDC *pDC);
	
	LRESULT OnCtlColorListBox(WPARAM wParam, LPARAM lParam);
	//{{AFX_MSG(CComboBoxXP)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCbnDropdown();
	afx_msg void OnCbnCloseup();
	//}}AFX_MSG
	CListBox   m_lstBox;
	COLORREF	m_colBackGroundHighLight, m_colTextHighLight,m_colBackGroundHighLight1;			//Colors for listbox in highligh position
	COLORREF	m_colBackGroundNormal, m_colTextNormal;					//Colors for listbox in normal position
	CFont m_font;
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_COMBOBOXXP_H__69E909B6_B6CA_4CE6_BEF5_2AA1CCAD6A2B__INCLUDED_)
