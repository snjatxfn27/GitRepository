#pragma once

//#include "WinGUIClientDlg.h"
// CDeviceDlg 对话框

class CDeviceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDeviceDlg)

public:
	CDeviceDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDeviceDlg();

	//自己的代码
	CRect m_rClient;
	int m_iDeviceCount;//一体机设备数量
	int m_iLineHeight;//list行高像素点
	CString m_strCurRTSP;//当前播放视频流的rtsp地址
	BOOL m_bIsInitRect;//是否初始化了摄像机区域

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
	DEVICESTAT m_devicestat[13];//设备状态及显示位置数组
	void UpdateStatData();//更新设备状态数据
	void PlayVideo(CString strRTSP);

// 对话框数据
	enum { IDD = IDD_DIALOG_DEVICE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
