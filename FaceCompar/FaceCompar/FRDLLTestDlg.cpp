// FRDLLTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FRDLLTest.h"
#include "FRDLLTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFRDLLTestDlg dialog

CFRDLLTestDlg::CFRDLLTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFRDLLTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFRDLLTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFRDLLTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFRDLLTestDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFRDLLTestDlg, CDialog)
	//{{AFX_MSG_MAP(CFRDLLTestDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON, OnButton)
	ON_BN_CLICKED(IDC_LOGOUTBTN, OnLogoutbtn)
	ON_BN_CLICKED(IDC_FACEDBQ_BTN, OnFacedbqBtn)
	ON_BN_CLICKED(IDC_CREATEFACEDBBTN, OnCreatefacedbbtn)
	ON_BN_CLICKED(IDC_DELETEFACEDB, OnDeletefacedb)
	ON_BN_CLICKED(IDC_CAMERAQUERY_BTN, OnCameraqueryBtn)
	ON_BN_CLICKED(IDC_CAMERAADD_BTN, OnCameraaddBtn)
	ON_BN_CLICKED(IDC_CAMERADELETE_BTN, OnCameradeleteBtn)
	ON_BN_CLICKED(IDC_CAMERACONFIG_BTN, OnCameraconfigBtn)
	ON_BN_CLICKED(IDC_POLICYQUERY_BTN, OnPolicyqueryBtn)
	ON_BN_CLICKED(IDC_POLICYADD_BTN, OnPolicyaddBtn)
	ON_BN_CLICKED(IDC_POLICYMODIFY_BTN, OnPolicymodifyBtn)
	ON_BN_CLICKED(IDC_POLICYDELETE_BTN, OnPolicydeleteBtn)
	ON_BN_CLICKED(IDC_RECON_BTN, OnReconBtn)
	ON_BN_CLICKED(IDC_BUTTON_DBINFO, OnButtonDbinfo)
	ON_BN_CLICKED(IDC_CAMERARECSTART_BTN, OnCamerarecstartBtn)
	ON_BN_CLICKED(IDC_CAMERARECSTOP_BTN, OnCamerarecstopBtn)
	ON_BN_CLICKED(IDC_GETFEATURE_BTN, OnGetfeatureBtn)
	ON_BN_CLICKED(IDC_AGEANDSEX_BTN, OnAgeandsexBtn)
	ON_BN_CLICKED(IDC_FEATUREMATCH_BTN, OnFeaturematchBtn)
	ON_BN_CLICKED(IDC_RECSEL_BTN, OnReconSelectBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFRDLLTestDlg message handlers

BOOL CFRDLLTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFRDLLTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFRDLLTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CFRDLLTestDlg::OnButton()
{
	// TODO: Add your control notification handler code heres
	//BatchInfoPublish(char * strDbId,char * strDbIp,char * strDbName,char * strDbUser,char * strDbPwd);
	int icount =0 ;
	//m_FaceAccess.BatchInfoPublish("9",(LPSTR)(LPCTSTR)theApp.m_strAdoIP,"facerecaccess","root","cr",(LPSTR)(LPCTSTR)theApp.m_strDeviceIP,"11",&icount);
	m_FaceAccess.BatchInfoPublish("62",(LPSTR)(LPCTSTR)theApp.m_strAdoIP,"facedetectsys","root","cr",(LPSTR)(LPCTSTR)theApp.m_strDeviceIP,"7",&icount);
}

void CFRDLLTestDlg::OnLogoutbtn()
{
	// TODO: Add your control notification handler code here
	char pSession[19];
	char m_pResult[10];
	int m_result = m_CFASLoginOrLogout.DeviceLogin((LPSTR)(LPCTSTR)theApp.m_strDeviceIP,pSession);
	if (m_result == 0)
	{
		int m_result = m_CFASLoginOrLogout.DeviceLogout((LPSTR)(LPCTSTR)theApp.m_strDeviceIP,pSession);
		itoa(m_result,m_pResult,10);
		AfxMessageBox(m_pResult);
	}else 
	{	
		itoa(m_result,m_pResult,10);
		AfxMessageBox(m_pResult);
	}
}

void CFRDLLTestDlg::OnFacedbqBtn()
{
	// TODO: Add your control notification handler code here
	char pSession[19];
	struct StructDblist sDblist [20];
	int intTotalcount,intListsize;
	m_CFASLoginOrLogout.DeviceLogin((LPSTR)(LPCTSTR)theApp.m_strDeviceIP,pSession);
	m_CFASStatusQuery.FASFaceDBQuery((LPSTR)(LPCTSTR)theApp.m_strDeviceIP,pSession,sDblist,&intTotalcount,&intListsize);
	int i =0;
	while (i<sizeof(sDblist)/sizeof(sDblist[0]))
	{
		sDblist[i].db_id;
		sDblist[i].name;
		sDblist[i].url;
		i++;
	}
}

void CFRDLLTestDlg::OnCreatefacedbbtn() 
{
	char pSession[19];
	char pDbName[50];
	CString strDbName = "人脸库2";
	struct StructDbCreateReturn pStructDbCreateReturn[1];
	strcpy(pDbName,strDbName);
	m_CFASLoginOrLogout.DeviceLogin((LPSTR)(LPCTSTR)theApp.m_strDeviceIP,pSession);
	m_CFASStatusQuery.FASFaceDBCreate((LPSTR)(LPCTSTR)theApp.m_strDeviceIP,pSession,pDbName,pStructDbCreateReturn);
	strDbName=pDbName;
	AfxMessageBox(strDbName);
}

void CFRDLLTestDlg::OnDeletefacedb() 
{
	char pSession[19];
	char pDbName[50];
	CString strDbName = "人脸库7";
	strcpy(pDbName,strDbName);
	m_CFASLoginOrLogout.DeviceLogin((LPSTR)(LPCTSTR)theApp.m_strDeviceIP,pSession);
	m_CFASStatusQuery.FASFaceDBDelete((LPSTR)(LPCTSTR)theApp.m_strDeviceIP,pSession,pDbName);
	strDbName=pDbName;
	AfxMessageBox(strDbName);
}

void CFRDLLTestDlg::OnCameraqueryBtn() 
{	
	int intTotalcount = 0;
	int	intListsize = 0;
	char pSession[19];
	char * pIp=(LPSTR)(LPCTSTR)theApp.m_strDeviceIP;
	struct StructCameraList pStructCameralist [20];
	m_CFASLoginOrLogout.DeviceLogin((LPSTR)(LPCTSTR)theApp.m_strDeviceIP,pSession);
	m_CFASCameraManage.FASCameraQuery(pIp,pSession,pStructCameralist,&intTotalcount,&intListsize);
	AfxMessageBox(pIp);
}
//一体机添加一个摄像机
void CFRDLLTestDlg::OnCameraaddBtn() 
{
	char pSession[19];
	char * pIp=(LPSTR)(LPCTSTR)theApp.m_strDeviceIP;
	char * pCName="摄像机2";
	char * pRtsp="rtsp://admin:12345@192.168.0.64/h264/ch1/main/av_stream";
	char * pDbid="1";
	int	intCMode = 0;
	int intNodeid = 0;
	int	intFixedhost = 0;
	int	intPolicyid = 1;
	m_CFASLoginOrLogout.DeviceLogin(pIp,pSession);
	m_CFASCameraManage.FASCameraAdd(pIp,pSession,pCName,intCMode,pRtsp,pDbid,intNodeid,intFixedhost,intPolicyid);
}
//删除一个摄像机
void CFRDLLTestDlg::OnCameradeleteBtn() 
{
	char pSession[19];
	char * pIp=(LPSTR)(LPCTSTR)theApp.m_strDeviceIP;
	char * pCameraId = "35";
	m_CFASLoginOrLogout.DeviceLogin(pIp,pSession);
	m_CFASCameraManage.FASCameraDelete(pIp,pSession,pCameraId);
	
}
//一体机配置一个摄像机
void CFRDLLTestDlg::OnCameraconfigBtn() 
{
	char pSession[19];
	char * pIp=(LPSTR)(LPCTSTR)theApp.m_strDeviceIP;
	char * pCameraId = "35";
	char * pCName="test2";
	char * pRtsp="rtsp://admin:unisib1000@192.168.1.233/h264/ch1/main/av_stream";
	char * pDbid="1+2+3";
	int	intCMode = 0;
	int intNodeid = 0;
	int	intFixedhost = 0;
	int	intPolicyid = 1;
	m_CFASLoginOrLogout.DeviceLogin(pIp,pSession);
	m_CFASCameraManage.FASCameraConfig(pIp,pSession,pCameraId,pCName,intCMode,pRtsp,pDbid,intNodeid,intFixedhost,intPolicyid);
}
	// 开始布控
void CFRDLLTestDlg::OnCamerarecstartBtn() 
{
	char pSession[19];
	char * pIp=(LPSTR)(LPCTSTR)theApp.m_strDeviceIP;
	char * pCameraId = "43";
	m_CFASLoginOrLogout.DeviceLogin(pIp,pSession);
	m_CFASCameraManage.FASCameraRecognitionStart(pIp,pSession,pCameraId);
}
	// 停止布控
void CFRDLLTestDlg::OnCamerarecstopBtn() 
{
	char pSession[19];
	char * pIp=(LPSTR)(LPCTSTR)theApp.m_strDeviceIP;
	char * pCameraId = "43";
	m_CFASLoginOrLogout.DeviceLogin(pIp,pSession);
	m_CFASCameraManage.FASCameraRecognitionStop(pIp,pSession,pCameraId);
}

//策略管理
void CFRDLLTestDlg::OnPolicyqueryBtn() 
{
	int intmt_policy_num = 0;
	char pSession[19];
	char * pIp=(LPSTR)(LPCTSTR)theApp.m_strDeviceIP;
	char * pPolicy_id=" ";
	struct StructPolicyList pStructPolicylist [20];
	m_CFASLoginOrLogout.DeviceLogin((LPSTR)(LPCTSTR)theApp.m_strDeviceIP,pSession);
	m_CFASPolicyManage.FASPolicyQuery(pIp,pSession,pPolicy_id,pStructPolicylist,&intmt_policy_num);	
}

void CFRDLLTestDlg::OnPolicyaddBtn() 
{
	char pSession[19]={'1','6','1','1','0','1','1','5','1','9','5','6','0','0','0','5','4','0'};
	char * pIp=(LPSTR)(LPCTSTR)theApp.m_strDeviceIP;
	char * pPolicyname="测试1";
	char * pPolicyalarm_type="001";
	int	intmonitor_type = 3;
	int inttime_schema_id = 2;
	int	intsimilarity = 80;
	int	intsimilarity2 = 80;
	int	intsimilarity3 = 80;
	int	intmax_return = 10;
	int	intscene_saved = 1;
	int intmin_pixel = 1024;
	int	intmax_face_num = 12;
	double douzoom_times = 0.1;
	int	intstorage_day = 1;
	int	intbest_face = 1;
	int	intspeed_mode = 0;
	int	intface_adjust = 0;
	int	intmin_point = 1;
	int	intskip_dup = 1;
	int	intskip_time = 10;
	m_CFASLoginOrLogout.DeviceLogin(pIp,pSession);
	m_CFASPolicyManage.FASPolicyAdd(pIp,pSession,pPolicyname,intmonitor_type,pPolicyalarm_type,inttime_schema_id,intsimilarity,intsimilarity2,intsimilarity3,intmax_return,intscene_saved,intmin_pixel,intmax_face_num,douzoom_times,intstorage_day,intbest_face,intspeed_mode,intface_adjust,intmin_point,intskip_dup,intskip_time);
}

void CFRDLLTestDlg::OnPolicymodifyBtn() 
{
	char pSession[19];
	char * pIp=(LPSTR)(LPCTSTR)theApp.m_strDeviceIP;
	char * pPolicyid = "9";
	char * pPolicyname="修改";
	char * pPolicyalarm_type="001";
	int	intmonitor_type = 1;
	int inttime_schema_id = 1;
	int	intsimilarity = 50;
	int	intsimilarity2 = 80;
	int	intsimilarity3 = 80;
	int	intmax_return = 10;
	int	intscene_saved = 2;
	int intmin_pixel = 1024;
	int	intmax_face_num = 12;
	double douzoom_times = 0.1;
	int	intstorage_day = 1;
	int	intbest_face = 1;
	int	intspeed_mode = 0;
	int	intface_adjust = 0;
	int	intmin_point = 10;
	int	intskip_dup = 1;
	int	intskip_time = 10;
	m_CFASLoginOrLogout.DeviceLogin(pIp,pSession);
	m_CFASPolicyManage.FASPolicyModify(pIp,pSession,pPolicyid,pPolicyname,intmonitor_type,pPolicyalarm_type,inttime_schema_id,intsimilarity,intsimilarity2,intsimilarity3,intmax_return,intscene_saved,intmin_pixel,intmax_face_num,douzoom_times,intstorage_day,intbest_face,intspeed_mode,intface_adjust,intmin_point,intskip_dup,intskip_time);
	
}

void CFRDLLTestDlg::OnPolicydeleteBtn() 
{
	char pSession[19];
	char * pIp=(LPSTR)(LPCTSTR)theApp.m_strDeviceIP;
	char * pPolicyid = "11";
	m_CFASLoginOrLogout.DeviceLogin(pIp,pSession);
	m_CFASPolicyManage.FASPolicyDelete(pIp,pSession,pPolicyid);
}

void CFRDLLTestDlg::OnButtonDbinfo() 
{
	// TODO: Add your control notification handler code here
	char pSession[19];
	char pDbId[10];
	CString strDbid = "58";
	struct StructDbInfo pStructDbinfo[1];
	strcpy(pDbId,strDbid);
	m_CFASLoginOrLogout.DeviceLogin((LPSTR)(LPCTSTR)theApp.m_strDeviceIP,pSession);
	m_CFASStatusQuery.FASFaceDBInfo((LPSTR)(LPCTSTR)theApp.m_strDeviceIP,pSession,pDbId,pStructDbinfo);
}

void CFRDLLTestDlg::OnReconBtn() 
{
	char pSession[19];
	char * pIp=(LPSTR)(LPCTSTR)theApp.m_strDeviceIP;//一体机ip	
	char * pFacedb = "13";//人脸库id		
	char * pImage = "";//抓拍比对base64 img
	char * pTop = "10";//本次返回最大匹配结果上限
	char * pSimilarity ="10";//本次比对相似度
	int    intlistsize = 0;
	StructRecognitionList pStructRecognitionlist [50];
	//CString strFileAllName = "D:/work/VC6.0_workspace/FRDLLTest/img/3.txt";
	CString strFileAllName = "D:/work/VC6.0_workspace/FRDLLTest/img/curface_281.b64";
	CFile file;
	file.Open(strFileAllName,CFile::modeRead);
	int filelen = file.GetLength();
	//BYTE *imagebuf = new BYTE[filelen];
	char *imagebuf = new char[filelen*2];
	file.Read(imagebuf,filelen);
	file.Close();         
	pImage = imagebuf;
	int login = m_CFASLoginOrLogout.DeviceLogin(pIp,pSession);
	int a =m_CFASRecognition.FASImgRecongnition(pIp,pSession,pFacedb,pImage,pTop,pSimilarity,&intlistsize,pStructRecognitionlist);
	int ba =intlistsize;
	int i = 0;
//	while (true)
	{
		pStructRecognitionlist[0].aId_card;
		pStructRecognitionlist[0].aSimilarity;
		pStructRecognitionlist[1].aId_card;
		pStructRecognitionlist[1].aSimilarity;
		i++;
	}
}

void CFRDLLTestDlg::OnReconSelectBtn() 
{
	char pSession[19];
	char * pIp=(LPSTR)(LPCTSTR)theApp.m_strDeviceIP;//一体机ip	
	char * pFacedb = "13";//人脸库id		
	char * pImage = "";//抓拍比对base64 img
	char * pTop = "10";//本次返回最大匹配结果上限
	char * pSimilarity ="0";//本次比对相似度
	char * pAge = "1-80";//年龄
	char * pSex = "male";//性别
	char * pNative_place = "北京";//籍贯
	char * pAddr = "公司";//地址
	char * pName = "李";//姓名
	char * pPhone = "1";//电话
	char * pId_card = "2";//证件号

	int    intlistsize = 0;
	StructRecognitionList pStructRecognitionlist [50];
	//CString strFileAllName = "D:/work/VC6.0_workspace/FRDLLTest/img/3.txt";
	CString strFileAllName = "D:/work/VC6.0_workspace/FRDLLTest/img/curface_281.b64";
	CFile file;
	file.Open(strFileAllName,CFile::modeRead);
	int filelen = file.GetLength();
	//BYTE *imagebuf = new BYTE[filelen];
	char *imagebuf = new char[filelen*2];
	file.Read(imagebuf,filelen);
	file.Close();         
	pImage = imagebuf;
	int login = m_CFASLoginOrLogout.DeviceLogin(pIp,pSession);
	int a =m_CFASRecognition.FASImgRecongnitionForSelect(pIp,pSession,pFacedb,pImage,pTop,pSimilarity,pAge,pSex,pNative_place,pAddr,pName,pPhone,pId_card,&intlistsize,pStructRecognitionlist);
	int ba =intlistsize;
	int i = 0;
	//	while (true)
	{
		pStructRecognitionlist[0].aId_card;
		pStructRecognitionlist[0].aSimilarity;
		pStructRecognitionlist[1].aId_card;
		pStructRecognitionlist[1].aSimilarity;
		i++;
	}
}

void CFRDLLTestDlg::OnFeaturematchBtn() 
{
	char pSession[19];
	char * pIp=(LPSTR)(LPCTSTR)theApp.m_strDeviceIP;//一体机ip	
	char * pFacedb = "1";//人脸库id		
	char * pImage = "";//抓拍比对base64 img
	char * pFeature1;
	char * pTop = "10";//本次返回最大匹配结果上限
	char * pSimilarity ="10";//本次比对相似度
	int    intlistsize = 0;
	StructRecognitionList pStructRecognitionlist [50];
	//CString strFileAllName = "D:/work/VC6.0_workspace/FRDLLTest/img/3.txt";
	CString strFileAllName = "D:/work/VC6.0_workspace/FRDLLTest/img/curface_281.b64";
	CFile file;
	file.Open(strFileAllName,CFile::modeRead);
	int filelen = file.GetLength();
	//BYTE *imagebuf = new BYTE[filelen];
	char *imagebuf = new char[filelen*2];
	file.Read(imagebuf,filelen);
	file.Close();         
	pImage = imagebuf;
	char *pEImage = new char[filelen*2];
	char *pFeature = new char[filelen*2];
	m_CFASLoginOrLogout.DeviceLogin(pIp,pSession);
	int b = m_CFASRecognition.FASFeatureDetect(pIp,pSession,pImage,pEImage,pFeature);
	pFeature1 = pFeature;
	int a =m_CFASRecognition.FASFeatureMatching(pIp,pSession,pFacedb,pFeature1,pTop,pSimilarity,&intlistsize,pStructRecognitionlist);
	int ba =intlistsize;
	int i = 0;
	//	while (true)
	{
		pStructRecognitionlist[0].aId_card;
		pStructRecognitionlist[0].aSimilarity;
		pStructRecognitionlist[1].aId_card;
		pStructRecognitionlist[1].aSimilarity;
		i++;
	}
	delete pEImage;
	delete pFeature;

}

void CFRDLLTestDlg::OnGetfeatureBtn()
{
	char pSession[19];
	char * pIp=(LPSTR)(LPCTSTR)theApp.m_strDeviceIP;
	char * pFacedb = "1";
	//CString strFileAllName = "D:/work/VC6.0_workspace/FRDLLTest/img/阿不都沙拉木·阿布都热西提.jpg";
	//CString strFileAllName = "D:/work/VC6.0_workspace/FRDLLTest/img/曹茁.jpg";00a990969fd5d581d74b74347f866ef3
	CString strFileAllName = "D:/work/VC6.0_workspace/FRDLLTest/img/ids_in_daxinggong_md5/00c39292e73c574c3beb78708591f2f9.jpg";
	//CString strFileAllName = "D:/work/VC6.0_workspace/FRDLLTest/img/ids_in_daxinggong_md5/0b2715c223c3acf30146770b0ce91a5c.jpg";
	CFile file;
	file.Open(strFileAllName,CFile::modeRead);
	int filelen = file.GetLength();
	BYTE *imagebuf = new BYTE[filelen*2];
	char *pImage = new char[filelen*2];
	memset(pImage,'\0',filelen*2);
	file.Read(imagebuf,filelen*2);
	file.Close();
	int len = EncodeBase64(imagebuf,pImage,filelen,1024*1024*6);
	char *pEImage = new char[filelen*2];
	char *pEFeature = new char[filelen*2];
	m_CFASLoginOrLogout.DeviceLogin(pIp,pSession);
	int b = m_CFASRecognition.FASFeatureDetect(pIp,pSession,pImage,pEImage,pEFeature);
	CString strFeature = pEFeature;
	file.Open("feature.txt",CFile::modeCreate | CFile::modeWrite);
	file.Write(strFeature.GetBuffer(strFeature.GetLength()),strFeature.GetLength());
	file.Close();
}
//base64编码
int CFRDLLTestDlg::EncodeBase64(const unsigned char* pSrc, char* pDst, int nSrcLen, int nMaxLineLen)
{  
	char EnBase64Tab[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	unsigned char c1, c2, c3;    // 输入缓冲区读出3个字节
	int nDstLen = 0;             // 输出的字符计数
	int nLineLen = 0;            // 输出的行长度计数
	int nDiv = nSrcLen / 3;      // 输入数据长度除以3得到的倍数
	int nMod = nSrcLen % 3;      // 输入数据长度除以3得到的余数
	   
	// 每次取3个字节，编码成4个字符
	for (int i = 0; i < nDiv; i ++)
	{
		// 取3个字节
		c1 = *pSrc++;
		c2 = *pSrc++;
		c3 = *pSrc++;
		
		// 编码成4个字符
		*pDst++ = EnBase64Tab[c1 >> 2];
		*pDst++ = EnBase64Tab[((c1 << 4) | (c2 >> 4)) & 0x3f];
		*pDst++ = EnBase64Tab[((c2 << 2) | (c3 >> 6)) & 0x3f];
		*pDst++ = EnBase64Tab[c3 & 0x3f];
		nLineLen += 4;
		nDstLen += 4;
		
		// 输出换行？
		if (nLineLen > nMaxLineLen - 4)
		{
			*pDst++ = '\r';
			*pDst++ = '\n';
			nLineLen = 0;
			nDstLen += 2;
		}
	}
	
	   // 编码余下的字节
	if (nMod == 1)
	{
		c1 = *pSrc++;
		*pDst++ = EnBase64Tab[(c1 & 0xfc) >> 2];
		*pDst++ = EnBase64Tab[((c1 & 0x03) << 4)];
		*pDst++ = '=';
		*pDst++ = '=';
		nLineLen += 4;
		nDstLen += 4;
	}
	else if (nMod == 2)
	{
		c1 = *pSrc++;
		c2 = *pSrc++;
		*pDst++ = EnBase64Tab[(c1 & 0xfc) >> 2];
		*pDst++ = EnBase64Tab[((c1 & 0x03) << 4) | ((c2 & 0xf0) >> 4)];
		*pDst++ = EnBase64Tab[((c2 & 0x0f) << 2)];
		*pDst++ = '=';
		nDstLen += 4;
	}
	// 输出加个结束符
	*pDst = '\0';
	
	return nDstLen;
}

void CFRDLLTestDlg::OnAgeandsexBtn() 
{
	char pSession[19];
	char * pIp=(LPSTR)(LPCTSTR)theApp.m_strDeviceIP;
	char * pFacedb = "1";
	char * pImage = "";
	struct StructRecognitionList pStructRecognitionlist [20];
	CString strFileAllName = "D:/work/VC6.0_workspace/FRDLLTest/img/3.txt";
	CFile file;
	file.Open(strFileAllName,CFile::modeRead);
	int filelen = file.GetLength();
	char *imagebuf = new char[filelen*3];
	file.Read(imagebuf,filelen);
	file.Close();
	pImage = imagebuf;
	m_CFASLoginOrLogout.DeviceLogin(pIp,pSession);
	m_CFASRecognition.FASImgAgeSexAnalysis(pIp,pSession,pStructRecognitionlist,pImage);	
	
}
