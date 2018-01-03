#pragma once

//#include "WinGUIClientDlg.h"
// CDeviceDlg �Ի���

class CDeviceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDeviceDlg)

public:
	CDeviceDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDeviceDlg();

	//�Լ��Ĵ���
	CRect m_rClient;
	int m_iDeviceCount;//һ����豸����
	int m_iLineHeight;//list�и����ص�
	CString m_strCurRTSP;//��ǰ������Ƶ����rtsp��ַ
	BOOL m_bIsInitRect;//�Ƿ��ʼ�������������

	struct DEVICESTAT
	{
		CString strDeviceName;
		BOOL bIsOnline;
		int iCameraCount;
		CString strCameraAddr[2];
		CString strCameraRTSP[2];
		CRect rCameraRect[2];
		int iCameraState[2];
	};
	DEVICESTAT m_devicestat[13];//�豸״̬����ʾλ������
	void UpdateStatData();//�����豸״̬����
	void PlayVideo(CString strRTSP);

// �Ի�������
	enum { IDD = IDD_DIALOG_DEVICE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
