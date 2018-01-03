// DLLMethodForCSharp.h: interface for the DLLMethodForCSharp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DLLMETHODFORCSHARP_H__9CA38F8B_3914_4C03_85E7_1037FC818A34__INCLUDED_)
#define AFX_DLLMETHODFORCSHARP_H__9CA38F8B_3914_4C03_85E7_1037FC818A34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/****************************************
�ṹ��
*****************************************/
//����camera�б�ṹ��
struct StructCameraList
{
	char acamera_id[32];//�����Id
	char aname[64];//���������
	char aurl[256];//�����rtsp
	char acameramode[10];//�������Ƶ��Դ�ṩģʽ(0��ͨ�������1Ӣ��������ץ���������2������ץ�������������)������
	char acamerastate[10];//�����״̬
	char acamerapolicyid[10];//����id ���ز��ԣ�����
	char acameraidlist[20];//�����������б�,���ؿ��б����� "2+4+8"���ַ���
	char acameranodeid[10];//�ڵ�nodeid ָ�������ع���ĳ��Ŀ¼�£���Ҫ�Ƚ�������Ŀ¼�������� Ĭ��0
	char acamerafixedhost[10];//��������ţ��ò���ȱʡʱ��ʾ�������һ̨�������������ʾ�̶��󶨵�һ̨�������ϣ����� Ĭ��0
	char acamerabelonghost[20];//��ǰ��������ط�����,����ʱ��Ϣ,�����޸�
	char acameramanctrl[20];//0:�Զ�ģʽ1:�ֶ�ģʽ,����ʱ��Ϣ,�����޸�
	char acamerainit[20];//�ֶ���ʼ״̬������ͬcamera_state,����ʱ��Ϣ,�����޸�
	char acameraalarm[20];//������ϴ������ĸ澯ͳ����
	char acamerasnap[20];//������ϴ�������ץ������
	char acameralastcleartime[30];//�ϴ�����ʱ�� 2012/07/12 09:02:00
};

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
//�����������б�ṹ��
struct StructDblist
{
	char db_id[20];
	char name[50];
	char url[200];
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

//��������б�ṹ��
struct StructRecognitionList
{
	char aId_card[50];//���֤��
	char aSimilarity[5];//���ƶ�
	// 	char aPserson_id[10];//persion_Id
	// 	char aName[50];//����
	// 	char aBirth[50];//��������
	// 	char aSex[50];//�Ա�
	// 	char aCard_type[50];//֤������
	// 	char aNative_place[50];//����
	// 	char aAddr[50];//��ַ
	// 	char aPhone[50];//�ֻ���	
	// 	char aDb_id[6];//����������;
	// 	char aCreate_date[6];//��Ա��Ϣ��ַ
	// 	char aDefaut_face_image_url[6];//face ͼƬ��ַ
	
	};
/****************************************
	��¼����ͷ�ļ�
*****************************************/
	//��¼һ���
	extern "C" __declspec(dllexport) int DeviceLogin(char * pIP,char * pSession);
	//�ǳ�һ���
	extern "C" __declspec(dllexport) int DeviceLogout(char * pIP,char * pSession);
/****************************************
	���������ͷ�ļ�
*****************************************/
	//һ���������б��ѯ
	extern "C" __declspec(dllexport) int FASCameraQuery(char * pIP,char * pSession,StructCameraList * pStructCameralist,int * pTotal_count,int * pList_size);
	extern "C" __declspec(dllexport) int FASCameraQuery_Release(char * pIP,char * pSession,StructCameraList * pStructCameralist,int * pTotal_count,int * pList_size);
	//һ������һ�������
	extern "C" __declspec(dllexport) int FASCameraAdd(char * pIP,char * pSession,char * pCName,int intCMode,char * pRtsp,char * pDbid,int intNodeid,int intFixedhost,int intPolicyid);
	//һ���ɾ��һ�������
	extern "C" __declspec(dllexport) int FASCameraDelete(char * pIP,char * pSession,char * pCameraId);
	//һ�������һ�������
	extern "C" __declspec(dllexport) int FASCameraConfig(char * pIP,char * pSession,char * pCameraId,char * pCName,int intCMode,char * pRtsp,char * pDbid,int intNodeid,int intFixedhost,int intPolicyid);
	//�������ʼ��������(�����ֶ�ģʽ��)
	extern "C" __declspec(dllexport) int FASCameraRecognitionStart(char * pIP,char * pSession,char * pCameraId);
	//�����ֹͣ��������(�����ֶ�ģʽ��)
	extern "C" __declspec(dllexport) int FASCameraRecognitionStop(char * pIP,char * pSession,char * pCameraId);
/****************************************
	���Թ���ͷ�ļ�
*****************************************/

	//�����б��ѯ pPolicy_idΪָ������id�������ַ���ʱ���������в��ԡ�
	extern "C" __declspec(dllexport) int FASPolicyQuery(char * pIP,char * pSession,char * pPolicy_id,struct StructPolicyList * pStructPolicylist,int * pmt_policy_num);
	//���һ������
	extern "C" __declspec(dllexport) int FASPolicyAdd(char * pIP,char * pSession,char * pPolicyname,int intmonitor_type,char * palarm_type,int inttime_schema_id,int intsimilarity,int intsimilarity2,int intsimilarity3,int intmax_return,int intscene_saved,int intmin_pixel,int intmax_face_num,int intzoom_times,int intstorage_day, int intbest_face,int intspeed_mode,int intface_adjust,int intmin_point,int intskip_dup,int intskip_time);
	//ɾ��һ������
	extern "C" __declspec(dllexport) int FASPolicyDelete(char * pIP,char * pSession,char * pMt_policy_id);
	//����һ������
	extern "C" __declspec(dllexport) int FASPolicyModify(char * pIP,char * pSession,char * pMt_policy_id,char * pPolicyname,int intmonitor_type,char * palarm_type,int inttime_schema_id,int intsimilarity,int intsimilarity2,int intsimilarity3,int intmax_return,int intscene_saved,int intmin_pixel,int intmax_face_num,int intzoom_times,int intstorage_day, int intbest_face,int intspeed_mode,int intface_adjust,int intmin_point,int intskip_dup,int intskip_time);

/****************************************
	���������ͷ�ļ�
*****************************************/
	
	//��ѯ������
	extern "C" __declspec(dllexport) int FASFaceDBQuery(char * pIP,char * pSession,struct StructDblist * pStructDblist,int * pTotal_count,int * pList_size);
	//����������
	extern "C" __declspec(dllexport) int FASFaceDBCreate(char * pIP,char * pSession,char * pDbName,struct StructDbCreateReturn * pStructDbCreateReturn);
	//ɾ��������
	extern "C" __declspec(dllexport) int FASFaceDBDelete(char * pIP,char * pSession,char * pDbName);

/****************************************
	����������Ϣ����ͷ�ļ�
*****************************************/
	//��������Ϣ
	extern "C" __declspec(dllexport) int FASUpdatePersonInfo(char * pIP,char * pSession,char * pFacedb,char * pPersonid,char * pName,char * pSex,char *pBirth,char * pNativeplace,char * pPhone,char * pAddress,char * pIdCard);
	//��������Ϣ
	extern "C" __declspec(dllexport) int FASAddPersonInfo(char * pIP,char * pSession,char * pFacedb,char * pName,char * pSex,char *pBirth,char * pNativeplace,char * pPhone,char * pAddress,char * pIdCard,char *pPersonid);
	//�޸�����
	extern "C" __declspec(dllexport) int FASAddFaceInfo(char * pIP,char * pSession,char * pEImage,char *EFeature,char * pFacedb,char * pPersonid); 
	//ɾ��һ������Ϣ
	extern "C" __declspec(dllexport) int FASDeletePersonInfo(char * pIP,char * pSession,char * pFacedb,char * pPersonid);
	//����idcardɾ��һ������Ϣ
	extern "C" __declspec(dllexport) int FASDeletePersonInfobyIdcard(char * pIP,char * pSession,char * pFacedb,char * pIdcard);
/****************************************
	����ʶ��ͷ�ļ�
*****************************************/
	//������ȡ����
	extern "C" __declspec(dllexport) int FASFeatureDetect(char * pIP,char * pSession,char * pImage,char *pEImage,char *pEFeature);
	//����ʶ��
	extern "C" __declspec(dllexport) int FASImgRecongnition(char * pIP,char * pSession,char * pFacedb,char *pImage,char *pTop,char *pSimilarity,int * plistsize,struct StructRecognitionList * pStructRecognitionlist);
	//����ʶ��
	extern "C" __declspec(dllexport) int FASFeatureMatching(char * pIP,char * pSession,char * pFacedb,char *pFeature,char *pTop,char *pSimilarity,int * plistsize,struct StructRecognitionList * pStructRecognitionlist);
	//�Ա�ʶ��
	extern "C" __declspec(dllexport) int FASImgAgeSexAnalysis(char * pIP,char * pSession,struct StructRecognitionList * pStructRecognitionlist,char * pImage);
#endif