// Tray.cpp: implementation of the CTray class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Tray.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTray::CTray(void)
{
}
CTray::~CTray(void)
{
	DelTray();
}

void CTray::CreateTray(UINT nMenuId, CWnd* pWnd, UINT uCallbackMessage, LPCTSTR szTip, HICON icon, bool isdlg)
{
	//���������Ի���
	//m_popDlg.Create(IDD_DIALOG1,pWnd);
	//��������
	m_NotifyIconData.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	m_NotifyIconData.hWnd = pWnd->GetSafeHwnd();
	m_NotifyIconData.uID = nMenuId;
	m_NotifyIconData.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
	m_NotifyIconData.uCallbackMessage = uCallbackMessage;
	m_NotifyIconData.hIcon = icon;
	strcpy(m_NotifyIconData.szTip, szTip);
	Shell_NotifyIcon(NIM_ADD, &m_NotifyIconData);
	m_bIsEnable = true;
	m_bIsDlg = isdlg;
}
//ɾ������
void CTray::DelTray()
{
	if(m_bIsEnable)
	{
		Shell_NotifyIcon(NIM_DELETE,&m_NotifyIconData);
	//	m_popDlg.DestroyWindow();
		m_bIsEnable = false;
	}
}

//����˵�
LRESULT CTray::OnShowTask(WPARAM wParam, LONG lParam)
{
	static int i = 0;
	if(wParam != m_NotifyIconData.uID)
		return 1;
	else
	{
		if (LOWORD(lParam) == WM_LBUTTONDBLCLK)
		{
		//	DelTray();
		}
		if (LOWORD(lParam) == WM_LBUTTONDOWN)
		{ //���ͼ������������Ի���
			if (m_bIsDlg)
			{
				int x= GetSystemMetrics(SM_CXSCREEN);
				int y= GetSystemMetrics(SM_CYSCREEN);
			//	::SetWindowPos(m_popDlg.m_hWnd,NULL,x-349,y-234,349,207,SWP_NOACTIVATE );
			//	::AnimateWindow(m_popDlg.m_hWnd,1000,AW_VER_NEGATIVE|AW_SLIDE);
			}
			
		}
		if (LOWORD(lParam) == WM_RBUTTONUP)
		{   //�Ҽ������˵�
			CMenu menu, *pSubMenu;
			//���ز˵�
			if(!menu.LoadMenu(m_NotifyIconData.uID))
			{
				return 1;
			}
			//���ز˵��еĵ�һ��
			if(!(pSubMenu = menu.GetSubMenu(0)))
			{
				return 1;
			}
			
			CPoint pos;
			::GetCursorPos(&pos);
			::SetForegroundWindow(m_NotifyIconData.hWnd);
			//���ò˵�����ʾλ��
			::TrackPopupMenu(pSubMenu->m_hMenu,0,pos.x,pos.y,0,m_NotifyIconData.hWnd,NULL);
			menu.DestroyMenu();
		}
	}
	return 0;
}