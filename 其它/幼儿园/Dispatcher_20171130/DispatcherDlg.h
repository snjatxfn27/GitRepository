
// DispatcherDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDispatcherDlg 对话框
class CDispatcherDlg : public CDialogEx
{
// 构造
public:
	CDispatcherDlg(CWnd* pParent = NULL);	// 标准构造函数

	BOOL m_bQuit;
	BOOL m_bConnDisconnect;
	/*snapshot表中process_status的意义：
	0:该条记录还未处理
	3:该条记录已经成功处理完成
	11:处理该条记录时DLL调用失败
	12:处理该条记录时返回Json错误
	13:处理该条记录时未能提取出人脸特征
	14:比对函数返回非0，FSAMatchPerson
	15:比对函数返回的Json解析错误
	16:比对函数未能比中人脸
	17:写warninfo表时，打开数据库失败
	*/
	//返回值 0：成功；1：DLL调用失败 2:Json解析失败 3：提取feature失败
	int ProcessOneSnapData(int iSnapshotid, CString strFacePhotoName, CString strFullPhotoName,int &iAge,int &iGender);
	
	_ConnectionPtr conn;
	CString m_strOutput[13];
	CString m_strTime[13];
	
	void PutInfoToList(CString strInfo);
	CString GetSimFromDB();
	BOOL UpdateFaceDBsFromDB();
	BOOL UpdateCameraIDFromDB();
	BOOL SetVoiceQueueWarnig(CString csType,  int nSnapId);
	BOOL IsWarnig(int iSnapshotid);

	BOOL GetOneSnapshotFromDB(int &iSnapshotid, CString &strFacePhotoName, CString &strFullPhotoName);
	BOOL ConnectDB();
	BOOL InsertAlarmToDB(CString strSnapshotId, CString strIDCard, CString strSim, CString strFaceDB);
	BOOL UpdateProcessStatusToDB(int iSnapshotid, int iStatus, int iAge, int iGender);
		// 对话框数据
	enum { IDD = IDD_DISPATCHER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_staticTop;
	CStatic m_staticSim;
	CListCtrl m_listOutput;
	char chSession[2048];
	BOOL m_bLogIn;
};
