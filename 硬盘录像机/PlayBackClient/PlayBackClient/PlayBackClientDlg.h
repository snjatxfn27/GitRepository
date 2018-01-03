
// PlayBackClientDlg.h : ͷ�ļ�
//

#pragma once
#define WIDTH 1024
#define HEIGHT 768
#define MAX_TEXT_SHOW_SUM    16
#define  DBINI_PATH						  "\\Parameter.ini"
#define  VIDEO_PATH						  "\\PlaybackVideo\\"
#define  IMG_PATH						  "\\Img\\"
#define  TCPPORT  49999

struct ShowListData	
{
	BOOL	bIsDownlod;
	CRect   rListRect;
	PointF  pointName;
	PointF  pointTime;
	CString csSnapshotID;
	CString csWarningID;//
	CString csIdCard; //idcard
	CString strVideoFile;//
	CString csNameTemp;//�б���ʾ������

	CString strTime;//����ʱ��
	CString csSimilar;//���ƶ�
	CString csName;
	CString csSex;
	CString csAddress;//��ַ
	CString csPhone;//�绰
};

// CPlayBackClientDlg �Ի���
class CPlayBackClientDlg : public CDialogEx
{
// ����
public:
	CPlayBackClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PLAYBACKCLIENT_DIALOG };

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

	CRect m_rDlgRect;
	CRect m_rRightRect;
	CRect m_rLeftRect;
	CRect m_rCloseRect;
	CRect m_rPlayBackRect;
	CButtonST m_wndClose;
	CvvImage m_cimg;


	HANDLE	    m_HPlayVideoEvent; //Event �ź���

	_ConnectionPtr  m_pConnect;
	int  m_nProgressNum;
	int  m_nProgressMax;
	int  m_nSelectIndex;

	BOOL m_bIsDownlod;
	BOOL m_bIsPayVideo;

	CString m_csTcpIP;

	CWinThread * m_TDownlod;
	CWinThread * m_TPlayVideo;
	CString GetiniPath();
	BOOL InitDataBase();
	void InitListRect();
	BOOL GetListMaxSum();

	void ShowVideo(IplImage *m_img, UINT ID);

	void UpdataListData(int nPagesNum);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);

};
