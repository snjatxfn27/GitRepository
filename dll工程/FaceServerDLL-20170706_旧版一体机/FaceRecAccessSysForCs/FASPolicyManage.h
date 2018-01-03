// FASPolicyManage.h: interface for the FASPolicyManage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FASPOLICYMANAGE_H__424DEFD2_B2EC_4D90_B542_BC134B1D87DA__INCLUDED_)
#define AFX_FASPOLICYMANAGE_H__424DEFD2_B2EC_4D90_B542_BC134B1D87DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
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
class __declspec(dllexport) CFASPolicyManage
{
public:
	CFASPolicyManage();
	~CFASPolicyManage();
	//�����б��ѯ pPolicy_idΪָ������id�������ַ���ʱ���������в��ԡ�
	int FASPolicyQuery(char * pIP,char * pSession,char * pPolicy_id,struct StructPolicyList * pStructPolicylist,int * pmt_policy_num);
	//���һ������
	int FASPolicyAdd(char * pIP,char * pSession,char * pPolicyname,int intmonitor_type,char * palarm_type,int inttime_schema_id,int intsimilarity,int intsimilarity2,int intsimilarity3,int intmax_return,int intscene_saved,int intmin_pixel,int intmax_face_num,double douzoom_times,int intstorage_day, int intbest_face,int intspeed_mode,int intface_adjust,int intmin_point,int intskip_dup,int intskip_time);
	//ɾ��һ������
	int FASPolicyDelete(char * pIP,char * pSession,char * pMt_policy_id);
	//����һ������
	int FASPolicyModify(char * pIP,char * pSession,char * pMt_policy_id,char * pPolicyname,int intmonitor_type,char * palarm_type,int inttime_schema_id,int intsimilarity,int intsimilarity2,int intsimilarity3,int intmax_return,int intscene_saved,int intmin_pixel,int intmax_face_num,double douzoom_times,int intstorage_day, int intbest_face,int intspeed_mode,int intface_adjust,int intmin_point,int intskip_dup,int intskip_time);
};
#endif // !defined(AFX_FASPOLICYMANAGE_H__424DEFD2_B2EC_4D90_B542_BC134B1D87DA__INCLUDED_)
