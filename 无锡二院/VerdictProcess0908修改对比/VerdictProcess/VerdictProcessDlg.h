
// VerdictProcessDlg.h : 头文件
//

#pragma once

typedef struct STRU_DEVICE_INFO
{
	BOOL    bIsonline;
	CString csDeviceIp;
	CString csDescription;
	CString csType;
	CString csFaceDbs;
	CString csFacedbBlacekList;//黑名单
	CString csFacedbWhiteList;//白名单
	CString csFacedbDynamicList;//动态
	CString csFacedbTemporaryList;//临时

	STRU_DEVICE_INFO()
	{
		bIsonline = FALSE;
		csDeviceIp = "";
		csDescription = "";
		csFacedbBlacekList = "";
		csFacedbWhiteList = "";
		csFacedbDynamicList = "";
		csFacedbTemporaryList = "";
	}

}DEVICE_INFO, *pDEVICE_INFO;


// CVerdictProcessDlg 对话框
class CVerdictProcessDlg : public CDialogEx
{
// 构造
public:
	CVerdictProcessDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_VERDICTPROCESS_DIALOG };

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
	CString m_csSqlIP;
	CString m_csSqlData;
	CString m_csSqlDatabase;
	CString m_csCurrent;
	CString m_csAppearNum;
	CString m_csCurrentNum;
	CString m_csAge;
	CString m_csAgeResults;
	CString m_csSimilarity;
	CString m_csTimeOut;

	BOOL InitDatabase();
	void Start();
	CString GetiniPath();
	BOOL IsCycleInner(CString csCurrentDate, int  &nCurrent);
	BOOL SetCycleNext();
	BOOL ManageWarninfo();
	BOOL GetDeviceFaceDb();
	BOOL WriteBlacekLis(CString csWarninfoId, CString csSnapShotid, CString csIdCard, CString csSimilarity,  CString csCameraid, CString csInserttime,int nCurrent);
	BOOL WriteDynamicList(CString csWarninfoId, CString csSnapShotid, CString csIdCard, CString csSimilarity,  CString csCameraid, CString csInserttime, int nCurrent);
	BOOL WriteTemporaryList( CString csSnapShotid, CString csIdCard, CString csSimilarity, CString csCameraid, CString csInserttime, int nCurrent);
	BOOL ManageSnapshot(CString csStartTime, CString csEndTime, CString nCurrent, BOOL bIsdelete);
	BOOL VerdictSnapshot();
	BOOL GetDbTableMaxItem(CString csItem, CString csTable, CString &csMaxItem);
	BOOL GetComputerSimilar(CString &csSim);
	BOOL IsAgeCorrect(int nNum, CString csIdcard, CString csCurrect);

	void SetNextCurrentValue(int csCurrent);
	void IsOpenSql(BOOL bFlag);
	BOOL GetIni();

	BOOL OnIdCardGetSnpId(CString csIdcard, CString &csSnapid,CString &csPath);
	BOOL VerdictTime(CString csSnapId, CString csTime, CString csPath, CString csIp, char pSession[37], CString &csSim,  CString &csIDCard);
	BOOL GetTeIdcard(CString csSnapid, CString &csIDcard);
	void MaintainVerdictTime(CString csTime);

	///维护表
	BOOL CreateTableALL(CString csCurrentCycle);
	BOOL GetPersonId(CString  csIdcard, CString  &csPossonId);
	BOOL GetEmpIdcard(CString &csIdcard);

	BOOL CreateTable(CString csTable);
	BOOL DeleteIdcardImgInfo(CString csStartTime, CString csEndTime);
	BOOL DeleteComputerIdcard(CString csStartTime, CString csEndTime);
	BOOL DeleteEmploy(CString csIdcdrd);
	BOOL DeleteImg(CString csIdcdrd);
	BOOL InitDatabaseMove();
	void CloseDatabaseMove();
	BOOL m_bOpenDataMove;
	BOOL MoveComputerTemLis(CString csCurrentCycle, CString csItem, int nTop);
	BOOL MoveBlacekLis(CString csCurrentCycle);
	BOOL MoveDynamicList(CString csCurrentCycle);
	BOOL MoveTemporaryList(CString csCurrentCycle);
	BOOL MoveTempEmpToDynEmp(CString csIdcard);

	BOOL MoveTempListToDynList(CString csCurrentCycle, CString csIdcard);
	BOOL bbbbbbbbbbbb;
	afx_msg void OnDestroy();
};
