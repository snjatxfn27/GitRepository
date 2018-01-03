// Tools.h: interface for the Tools class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLS_H__0A7B10E1_0AB7_4089_AC1D_370D4DC1FA47__INCLUDED_)
#define AFX_TOOLS_H__0A7B10E1_0AB7_4089_AC1D_370D4DC1FA47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct sBatchRegistration
{
	CString csLibraryId;
	CString csFaceIP;
	CString csPath;
};


struct StructSetTreeData
{
	CString csID;
	CString csName;
	CString csTime;
	CString csUrl;
	CString csIP;
	CString CSAamerastate;
	int nFlag;
	int nComputeStart;
};


struct StructSetComputer
{
	int nFlag;
	CString csIP;
	CString csOnlyid;
	CString csDescription;
	CString csLasttime;
	CString csIsonline;
};

/*
�����ݱ���Ϊbase64����ʽ��pSrcΪԭ����ָ�룬pDstΪ���������ָ��
*/
int __EncodeBase64(const unsigned char* pSrc, char* pDst, int nSrcLen, int nMaxLineLen);
/*
��base64��������ݽ���
*/
int __DecodeBase64(const char* pSrc, unsigned char* pDst, int nSrcLen);
//�ַ������
CString GB2312ToUTF8(const char* str);
//featureDetect
void FeatureDetect(CString strIP,CString strSession,CString strEImage,char *EFeature);
//��¼һ���
CString Login(CString strIP);

void __ExpandAllNode(HTREEITEM hItem, CTreeCtrl& m_treeShow);

int __BreakCString(CString csData, CStringArray &arOutCString,CString csFindFlag);


void __SetFontSize(CWnd &m_tree);

void __PromptInformation(CString csInfo);
void __PromptInformation(int nInfo);

BOOL __IsDigital(CString strItem);

#endif // !defined(AFX_TOOLS_H__0A7B10E1_0AB7_4089_AC1D_370D4DC1FA47__INCLUDED_)

/////////////////////////////////////////////////////////////////////////////


//�·�ע��
class _declspec(dllimport) CFaceAccessSys
{
public:
	CFaceAccessSys(void);
	~CFaceAccessSys(void);
	//����ע��
	int BatchInfoPublish(char * pLocalDbId, char * pLocalDbIp, char * pDbName, char * pDbUser, char * pDbPwd, char * pFaceDbIp, char * pFaceDbId, int *pi_Count_Success);
};
//���������
//�����������б�ṹ��
struct StructDblist
{
	char db_id[20];
	char name[50];
	char url[200];
};
//������������Ϣ�ṹ��
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
//���崴�������ⷵ��ֵ�ṹ��
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
	//һ�����ȡ�������б�
	int FASFaceDBQuery(char * pIP, char * pSession, struct StructDblist * pStructDblist, int * pTotal_count, int * pList_size);
	//һ�������������
	int FASFaceDBCreate(char * pIP, char * pSession, char * pDbName, struct StructDbCreateReturn * pStructDbCreateReturn);
	//һ���ɾ��������
	int FASFaceDBDelete(char * pIP, char * pSession, char * pDbName);
	//ָ��ID����������ϸ��ϸ��Ϣ��ѯ
	int FASFaceDBInfo(char * pIP, char * pSession, char * pDbid, struct StructDbInfo * pStructDbinfo);
};
//��¼��ǳ�
class __declspec(dllimport) CFASLoginOrLogout
{
public:
	CFASLoginOrLogout();
	~CFASLoginOrLogout();
	//int DeviceLogin(char * pIP,char * &pSession);
	//��¼ϵͳ
	int DeviceLogin(char * pIP, char * pSession);
	//�ǳ�ϵͳ
	int DeviceLogout(char * pIP, char * pSession);
};
//���������
//����camera�б�ṹ��
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
	//һ���������б��ѯ
	int FASCameraQuery(char * pIP, char * pSession, struct StructCameraList * pStructCameralist, int * pTotal_count, int * pList_size);
	//һ������һ�������
	int FASCameraAdd(char * pIP, char * pSession, char * pCName, int intCMode, char * pRtsp, char * pDbid, int intNodeid, int intFixedhost, int intPolicyid);
	//һ���ɾ��һ�������
	int FASCameraDelete(char * pIP, char * pSession, char * pCameraId);
	//һ�������һ����������޸ģ�
	int FASCameraConfig(char * pIP, char * pSession, char * pCameraId, char * pCName, int intCMode, char * pRtsp, char * pDbid, int intNodeid, int intFixedhost, int intPolicyid);
	//�������ʼ��������(�����ֶ�ģʽ��)
	int FASCameraRecognitionStart(char * pIP, char * pSession, char * pCameraId);
	//�����ֹͣ��������(�����ֶ�ģʽ��)
	int FASCameraRecognitionStop(char * pIP, char * pSession, char * pCameraId);
};
//���Թ���
//��������б�ṹ��
struct StructPolicyList
{
	char aPolicy_id[10];//����Id
	char aname[50];//����name
	char aPolicymonitor_type[10];//��������
	char aPolicyalarm_type[10];//��������:1-���������أ��к��������������ʱ�澯,2-���������أ��������ﲻ������ʱ�澯,3-ץ��+������,4-ץ��+������,5-ץ��
	char aPolicytime_schema_id[10];//ʱ�䷽��//�ֹ����ط�����0
	char aPolicypolicysimilarity[10];//���ƶ�//��ͨ�澯��ֵ���������ݿ�Ĭ��Ϊ0��ץ�Ĳ�����ü���ĸ澯
	char aPolicymax_return[10];//��ѡ(<50)
	char aPolicyscene_saved[10];//��ѡ,Ĭ��2-������//�Ƿ񱣴�澯�ֳ�ͼƬ,1-��������,2-������,3-��ͼ����,����������,4-��ͼ�����棬��������
	char aPolicymin_pixel[10];// ��ѡ,��λΪ����
	char aPolicymax_face_num[10];//ÿ֡���ץ����������(1-32)��ѡ,��֡���ץ����������ϵͳ������32�����ó���32��32���㣬����ʾ����
	char aPolicyzoom_times[10];//��ѡ
	char aPolicyface_storage_day[10];//�������ݱ���������1-30��;
	char aPolicybest_face[10];//0:��ͨץ�ģ�1���������ץ��
	char aPolicyspeed_mode[10];// 0:����ģʽ 1:��Ƭģʽ ֻ��ftp��Ч��//��ѡ������ʱ�������0����
	char aPolicyface_adjust[10]; //0:�����������Ƕȷ�����1:���������Ƕȷ�����//��ѡ������ʱ�������0����
	char aPolicymin_point[10];//��С�Ƕȣ����ڴ˽Ƕȵ���������
	char aPolicyskip_dup[10]; //0:����������ȥ�ط�����1:��������ȥ�أ�//��ѡ������ʱ�������0����
	char aPolicyskip_time[10];//ȥ��ʱ�����ڣ���λ�룬��ʾ���ʱ�����������ظ�����������ץ��
};
class __declspec(dllimport) CFASPolicyManage
{
public:
	CFASPolicyManage();
	~CFASPolicyManage();
	//�����б��ѯ
	int FASPolicyQuery(char * pIP, char * pSession, char * pPolicy_id, struct StructPolicyList * pStructPolicylist, int * pmt_policy_num);
	//���һ������
	int FASPolicyAdd(char * pIP, char * pSession, char * pPolicyname, int intmonitor_type, char * palarm_type, int inttime_schema_id, int intsimilarity, int intsimilarity2, int intsimilarity3, int intmax_return, int intscene_saved, int intmin_pixel, int intmax_face_num, double douzoom_times, int intstorage_day, int intbest_face, int intspeed_mode, int intface_adjust, int intmin_point, int intskip_dup, int intskip_time);
	//ɾ��һ������
	int FASPolicyDelete(char * pIP, char * pSession, char * pMt_policy_id);
	//����һ������
	int FASPolicyModify(char * pIP, char * pSession, char * pMt_policy_id, char * pPolicyname, int intmonitor_type, char * palarm_type, int inttime_schema_id, int intsimilarity, int intsimilarity2, int intsimilarity3, int intmax_return, int intscene_saved, int intmin_pixel, int intmax_face_num, double douzoom_times, int intstorage_day, int intbest_face, int intspeed_mode, int intface_adjust, int intmin_point, int intskip_dup, int intskip_time);
};

//��������б�ṹ��
struct StructRecognitionList
{
	char aId_card[50];//���֤��
	char aSimilarity[5];//���ƶ�
};
class __declspec(dllimport) CFASRecognition
{
public:
	CFASRecognition();
	~CFASRecognition();
	//������ȡ����
	int FASFeatureDetect(char * pIP, char * pSession, char * pImage, char *pEImage, char *pEFeature);
	//����ʶ��
	int FASImgRecongnition(char * pIP, char * pSession, char * pFacedb, char *pImage, char *pTop, char *pSimilarity, int *listsize, struct StructRecognitionList * pStructRecognitionlist);
	//����ʶ��������ѯ
	int FASImgRecongnitionForSelect(char * pIP, char * pSession, char * pFacedb, char *pImage, char *pTop, char * pSimilarity, char * pAge, char * pSex, char * pNative_place, char * pAddr, char * pName, char * pPhone, char * pId_card, int * plistsize, struct StructRecognitionList * pStructRecognitionlist);
	//������ʶ��
	int FASFeatureMatching(char * pIP, char * pSession, char * pFacedb, char *pFeature, char *pTop, char *pSimilarity, int *listsize, struct StructRecognitionList * pStructRecognitionlist);
	//�����Ա�ʶ��
	int FASImgAgeSexAnalysis(char * pIP, char * pSession, struct StructRecognitionList * pStructRecognitionlist, char * pImage);
};