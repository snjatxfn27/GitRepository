
// DispatcherDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDispatcherDlg �Ի���
class CDispatcherDlg : public CDialogEx
{
// ����
public:
	CDispatcherDlg(CWnd* pParent = NULL);	// ��׼���캯��

	BOOL m_bQuit;
	BOOL m_bConnDisconnect;
	/*snapshot����process_status�����壺
	0:������¼��δ����
	3:������¼�Ѿ��ɹ��������
	11:���������¼ʱDLL����ʧ��
	12:���������¼ʱ����Json����
	13:���������¼ʱδ����ȡ����������
	*/
	//����ֵ 0���ɹ���1��DLL����ʧ�� 2:Json����ʧ�� 3����ȡfeatureʧ��
	int ProcessOneSnapData(int iSnapshotid, CString strFacePhotoName, CString strFullPhotoName,int &iAge);
	
	_ConnectionPtr conn;
	CString m_strOutput[13];
	CString m_strTime[13];
	void PutInfoToList(CString strInfo);
	CString GetSimFromDB();
	BOOL GetOneSnapshotFromDB(int &iSnapshotid, CString &strFacePhotoName, CString &strFullPhotoName);
	BOOL ConnectDB();
	BOOL InsertAlarmToDB(CString strSnapshotId, CString strIDCard, CString strSim, CString strFaceDB);
	BOOL UpdateProcessStatusToDB(int iSnapshotid, int iStatus, int iAge);
	BOOL DeleteOneSnapshot(int iSnapshotid, CString strFacePhotoName, CString strFullPhotoName);
	// �Ի�������
	enum { IDD = IDD_DISPATCHER_DIALOG };

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
public:
	CStatic m_staticTop;
	CStatic m_staticSim;
	CListCtrl m_listOutput;
	char chSession[2048];
	BOOL m_bLogIn ;

};
