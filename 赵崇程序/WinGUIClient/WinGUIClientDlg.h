
// WinGUIClientDlg.h : ͷ�ļ�
//

#pragma once
#include "TopDlg.h"
#include "VideoDlg.h"
#include "DeviceDlg.h"
#include "BottomDlg.h"
#include "RTAlarmDlg.h"
#include "RTAlarmInfoDlg.h"
#include "HistoryDlg.h"

// CWinGUIClientDlg �Ի���
class CWinGUIClientDlg : public CDialogEx
{
// ����
public:
	CWinGUIClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

	//��ʾ���򻮷�
	CRect m_rClient, m_rTop, m_rVideo, m_rDevice, m_rBottom, m_rRTAlarm, m_rRTAlarmInfo, m_rHistory;
	CTopDlg *m_dlgTop;
	CVideoDlg *m_dlgVideo;
	CDeviceDlg *m_dlgDevice;
	CBottomDlg *m_dlgBottom;
	CRTAlarmDlg *m_dlgRTAlarm;
	CRTAlarmInfoDlg *m_dlgRTAlarmInfo;
	CHistoryDlg *m_dlgHistory;
	void InitClients();
	void RunDialogs();

// �Ի�������
	enum { IDD = IDD_WINGUICLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
