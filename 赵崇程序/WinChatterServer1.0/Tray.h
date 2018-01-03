// Tray.h: interface for the CTray class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRAY_H__720ECF3B_D552_4DCC_9760_572E592CDC49__INCLUDED_)
#define AFX_TRAY_H__720ECF3B_D552_4DCC_9760_572E592CDC49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "PopDlg.h"

class CTray
{
	public:
	CTray(void);
	~CTray(void);

	private:
	NOTIFYICONDATA m_NotifyIconData;
	//定义标识托盘是否可用
	bool m_bIsEnable;

	bool m_bIsDlg;

	//弹出对话框变量（就是用MFC添加一个对话框，然后用该对话框生成的一个类CPopDlg）
	//CPopDlg  m_popDlg;
	public:
	/*
	函数名：创建托盘
	参数：    nMenuId: 托盘菜单ID
	pWnd:程序窗口指针
	uCallbackMessage:托盘消息ID
	szTip:鼠标经过托盘时显示的文字
	icon:托盘图标
	*/
	void CreateTray(UINT nMenuId, CWnd* pWnd, UINT uCallbackMessage, LPCTSTR szTip, HICON icon, bool isdlg);

	//处理托盘图标事件
	LRESULT OnShowTask(WPARAM wParam, LONG lParam);

	//删除托盘
	void DelTray();
};
#endif // !defined(AFX_TRAY_H__720ECF3B_D552_4DCC_9760_572E592CDC49__INCLUDED_)
