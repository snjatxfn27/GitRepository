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
	//�����ʶ�����Ƿ����
	bool m_bIsEnable;

	bool m_bIsDlg;

	//�����Ի��������������MFC���һ���Ի���Ȼ���øöԻ������ɵ�һ����CPopDlg��
	//CPopDlg  m_popDlg;
	public:
	/*
	����������������
	������    nMenuId: ���̲˵�ID
	pWnd:���򴰿�ָ��
	uCallbackMessage:������ϢID
	szTip:��꾭������ʱ��ʾ������
	icon:����ͼ��
	*/
	void CreateTray(UINT nMenuId, CWnd* pWnd, UINT uCallbackMessage, LPCTSTR szTip, HICON icon, bool isdlg);

	//��������ͼ���¼�
	LRESULT OnShowTask(WPARAM wParam, LONG lParam);

	//ɾ������
	void DelTray();
};
#endif // !defined(AFX_TRAY_H__720ECF3B_D552_4DCC_9760_572E592CDC49__INCLUDED_)
