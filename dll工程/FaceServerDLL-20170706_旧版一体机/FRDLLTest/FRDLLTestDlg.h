// FRDLLTestDlg.h : header file
//

#if !defined(AFX_FRDLLTESTDLG_H__513A900F_9302_4AF9_9083_5F11C83AB640__INCLUDED_)
#define AFX_FRDLLTESTDLG_H__513A900F_9302_4AF9_9083_5F11C83AB640__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
//下发注册
class _declspec(dllimport) CFaceAccessSys
{
public:
	CFaceAccessSys(void);
	~CFaceAccessSys(void);
	//批量注册
	int BatchInfoPublish(char * pLocalDbId,char * pLocalDbIp,char * pDbName,char * pDbUser,char * pDbPwd,char * pFaceDbIp,char * pFaceDbId,int *pi_Count_Success);
};
//人脸库管理
//定义人脸库列表结构体
struct StructDblist
{
	char db_id[20];
	char name[50];
	char url[200];
};
//定义人脸库信息结构体
struct StructDbInfo
{
	char db_id[20];
	char name[50];
	char create_date[20];
	char user[20];
	char similarity[3];
	char state[10];
	char person_count[10];
	char face_count[10];
	char inst_num[10];
};
//定义创建人脸库返回值结构体
struct StructDbCreateReturn
{
	char pFacedData_id[10];
	char pFacedData_name[50];
	char pFacedData_createdate[20];
	char pFacedData_user[50];
	char pFacedData_state[10];
	char pFacedData_similarity[10];
	char pFacedData_person_count[10];
	char pFacedData_face_count[10];
	char pFacedData_inst_num[10];
};
class __declspec(dllimport) CFASStatusQuery
{
public:
	CFASStatusQuery();
	~CFASStatusQuery();
	//一体机获取人脸库列表
	int FASFaceDBQuery(char * pIP,char * pSession,struct StructDblist * pStructDblist,int * pTotal_count,int * pList_size);
	//一体机创建人脸库
	int FASFaceDBCreate(char * pIP,char * pSession,char * pDbName,struct StructDbCreateReturn * pStructDbCreateReturn);
	//一体机删除人脸库
	int FASFaceDBDelete(char * pIP,char * pSession,char * pDbName);
	//指定ID的人脸库详细详细信息查询
	int FASFaceDBInfo(char * pIP,char * pSession,char * pDbid,struct StructDbInfo * pStructDbinfo);
};
//登录与登出
class __declspec(dllimport) CFASLoginOrLogout
{
public:
	CFASLoginOrLogout();
	~CFASLoginOrLogout();
	//int DeviceLogin(char * pIP,char * &pSession);
	//登录系统
	int DeviceLogin(char * pIP,char * pSession);
	//登出系统
	int DeviceLogout(char * pIP,char * pSession);
};
//摄像机管理
//定义camera列表结构体
struct StructCameraList
{
	char atotal_count[20];
	char alist_size[20];
	char acamera_id[20];
	char aname[50];
	char aurl[200];
	char acameramode[10];
	char acamerastate[10];
	char acamerapolicyid[10];
	char acameraidlist[20];
	char acameranodeid[10];
	char acamerafixedhost[10];
	char acamerabelonghost[20];
	char acameramanctrl[20];
	char acamerainit[20];
	char acameraalarm[20];
	char acamerasnap[20];
	char acameralastcleartime[30];
};
class __declspec(dllimport) CFASCameraManage 
{
public:
	CFASCameraManage();
	~CFASCameraManage();
	//一体机摄像机列表查询
	int FASCameraQuery(char * pIP,char * pSession,struct StructCameraList * pStructCameralist,int * pTotal_count,int * pList_size);
	//一体机添加一个摄像机
	int FASCameraAdd(char * pIP,char * pSession,char * pCName,int intCMode,char * pRtsp,char * pDbid,int intNodeid,int intFixedhost,int intPolicyid);
	//一体机删除一个摄像机
	int FASCameraDelete(char * pIP,char * pSession,char * pCameraId);
	//一体机配置一个摄像机（修改）
	int FASCameraConfig(char * pIP,char * pSession,char * pCameraId,char * pCName,int intCMode,char * pRtsp,char * pDbid,int intNodeid,int intFixedhost,int intPolicyid);
	//摄像机开始人脸布控(策略手动模式下)
	int FASCameraRecognitionStart(char * pIP,char * pSession,char * pCameraId);
	//摄像机停止人脸布控(策略手动模式下)
	int FASCameraRecognitionStop(char * pIP,char * pSession,char * pCameraId);
};	
//策略管理
//定义策略列表结构体
struct StructPolicyList
{
	char aPolicy_id[10];//策略Id
	char aname[50];//策略name
	char aPolicymonitor_type[10];//布控类型
	char aPolicyalarm_type[10];//报警类型:1-黑名单布控，有黑名单中人物出现时告警,2-白名单布控，出现人物不在名单时告警,3-抓拍+黑名单,4-抓拍+白名单,5-抓拍
	char aPolicytime_schema_id[10];//时间方案//手工布控方案填0
	char aPolicypolicysimilarity[10];//相似度//普通告警阀值，不填数据库默认为0，抓拍不处理该级别的告警
	char aPolicymax_return[10];//可选(<50)
	char aPolicyscene_saved[10];//可选,默认2-都保存//是否保存告警现场图片,1-都不保存,2-都保存,3-大图保存,人脸不保存,4-大图不保存，人脸保存
	char aPolicymin_pixel[10];// 可选,单位为象素
	char aPolicymax_face_num[10];//每帧最多抓拍人脸个数(1-32)可选,单帧最大抓拍人脸数，系统上限是32，配置超过32以32计算，不提示错误
	char aPolicyzoom_times[10];//可选
	char aPolicyface_storage_day[10];//人脸数据保存天数（1-30）;
	char aPolicybest_face[10];//0:普通抓拍；1：最佳人脸抓拍
	char aPolicyspeed_mode[10];// 0:正常模式 1:分片模式 只对ftp有效，//可选，不填时服务端以0处理
	char aPolicyface_adjust[10]; //0:不进行人脸角度分析，1:进行人脸角度分析，//可选，不填时服务端以0处理
	char aPolicymin_point[10];//最小角度，大于此角度的人脸丢弃
	char aPolicyskip_dup[10]; //0:不进行人脸去重分析，1:进行人脸去重，//可选，不填时服务端以0处理
	char aPolicyskip_time[10];//去重时间周期，单位秒，表示这个时间周期内有重复的人脸则不再抓拍
};
class __declspec(dllimport) CFASPolicyManage
{
public:
	CFASPolicyManage();
	~CFASPolicyManage();
	//策略列表查询
	int FASPolicyQuery(char * pIP,char * pSession,char * pPolicy_id,struct StructPolicyList * pStructPolicylist,int * pmt_policy_num);
	//添加一个策略
	int FASPolicyAdd(char * pIP,char * pSession,char * pPolicyname,int intmonitor_type,char * palarm_type,int inttime_schema_id,int intsimilarity,int intsimilarity2,int intsimilarity3,int intmax_return,int intscene_saved,int intmin_pixel,int intmax_face_num,double douzoom_times,int intstorage_day, int intbest_face,int intspeed_mode,int intface_adjust,int intmin_point,int intskip_dup,int intskip_time);
	//删除一个策略
	int FASPolicyDelete(char * pIP,char * pSession,char * pMt_policy_id);
	//配置一个策略
	int FASPolicyModify(char * pIP,char * pSession,char * pMt_policy_id,char * pPolicyname,int intmonitor_type,char * palarm_type,int inttime_schema_id,int intsimilarity,int intsimilarity2,int intsimilarity3,int intmax_return,int intscene_saved,int intmin_pixel,int intmax_face_num,double douzoom_times,int intstorage_day, int intbest_face,int intspeed_mode,int intface_adjust,int intmin_point,int intskip_dup,int intskip_time);
};

//定义策略列表结构体
struct StructRecognitionList
{
	char aId_card[50];//身份证号
	char aSimilarity[5];//相似度
};
class __declspec(dllimport) CFASRecognition
{
public:
	CFASRecognition();
	~CFASRecognition();
	//人脸提取特征
	int FASFeatureDetect(char * pIP,char * pSession,char * pImage,char *pEImage,char *pEFeature);
	//人脸识别
	int FASImgRecongnition(char * pIP,char * pSession,char * pFacedb,char *pImage,char *pTop,char *pSimilarity,int *listsize,struct StructRecognitionList * pStructRecognitionlist);
	//人脸识别条件查询
	int FASImgRecongnitionForSelect(char * pIP,char * pSession,char * pFacedb,char *pImage,char *pTop,char * pSimilarity,char * pAge,char * pSex,char * pNative_place,char * pAddr,char * pName,char * pPhone,char * pId_card, int * plistsize,struct StructRecognitionList * pStructRecognitionlist);
	//特征脸识别
	int FASFeatureMatching(char * pIP,char * pSession,char * pFacedb,char *pFeature,char *pTop,char *pSimilarity,int *listsize,struct StructRecognitionList * pStructRecognitionlist);
	//年龄性别识别
	int FASImgAgeSexAnalysis(char * pIP,char * pSession,struct StructRecognitionList * pStructRecognitionlist,char * pImage);
};
// CFRDLLTestDlg dialog

class CFRDLLTestDlg : public CDialog
{
// Construction
public:
	CFRDLLTestDlg(CWnd* pParent = NULL);	// standard constructor
	//base64编码
	int EncodeBase64(const unsigned char* pSrc, char* pDst, int nSrcLen, int nMaxLineLen);
// Dialog Data
	//{{AFX_DATA(CFRDLLTestDlg)
	enum { IDD = IDD_FRDLLTEST_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFRDLLTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CFRDLLTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton();
	afx_msg void OnLogoutbtn();
	afx_msg void OnFacedbqBtn();
	afx_msg void OnCreatefacedbbtn();
	afx_msg void OnDeletefacedb();
	afx_msg void OnCameraqueryBtn();
	afx_msg void OnCameraaddBtn();
	afx_msg void OnCameradeleteBtn();
	afx_msg void OnCameraconfigBtn();
	afx_msg void OnPolicyqueryBtn();
	afx_msg void OnPolicyaddBtn();
	afx_msg void OnPolicymodifyBtn();
	afx_msg void OnPolicydeleteBtn();
	afx_msg void OnReconBtn();
	afx_msg void OnButtonDbinfo();
	afx_msg void OnCamerarecstartBtn();
	afx_msg void OnCamerarecstopBtn();
	afx_msg void OnGetfeatureBtn();
	afx_msg void OnAgeandsexBtn();
	afx_msg void OnFeaturematchBtn();
	afx_msg void OnReconSelectBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CFaceAccessSys m_FaceAccess;
	CFASStatusQuery m_CFASStatusQuery;
    CFASLoginOrLogout m_CFASLoginOrLogout;
	CFASCameraManage m_CFASCameraManage;
	CFASPolicyManage m_CFASPolicyManage;
	CFASRecognition m_CFASRecognition;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRDLLTESTDLG_H__513A900F_9302_4AF9_9083_5F11C83AB640__INCLUDED_)
