// FASCameraManage.h: interface for the FASCameraManage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FASCAMERAMANAGE_H__FCC2ACBB_F494_4EBB_BA64_BBB700FCB23D__INCLUDED_)
#define AFX_FASCAMERAMANAGE_H__FCC2ACBB_F494_4EBB_BA64_BBB700FCB23D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//����camera�б�ṹ��
struct StructCameraList
{
	char acamera_id[20];//�����Id
	char aname[50];//���������
	char aurl[200];//�����rtsp
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
class __declspec(dllexport) CFASCameraManage 
{
public:
	CFASCameraManage();
	~CFASCameraManage();
	//һ���������б��ѯ
	int FASCameraQuery(char * pIP,char * pSession,struct StructCameraList * pStructCameralist,int * pTotal_count,int * pList_size);
	//һ������һ�������
	int FASCameraAdd(char * pIP,char * pSession,char * pCName,int intCMode,char * pRtsp,char * pDbid,int intNodeid,int intFixedhost,int intPolicyid);
	//һ���ɾ��һ�������
	int FASCameraDelete(char * pIP,char * pSession,char * pCameraId);
	//һ�������һ�������
	int FASCameraConfig(char * pIP,char * pSession,char * pCameraId,char * pCName,int intCMode,char * pRtsp,char * pDbid,int intNodeid,int intFixedhost,int intPolicyid);
	//�������ʼ��������(�����ֶ�ģʽ��)
	int FASCameraRecognitionStart(char * pIP,char * pSession,char * pCameraId);
	//�����ֹͣ��������(�����ֶ�ģʽ��)
	int FASCameraRecognitionStop(char * pIP,char * pSession,char * pCameraId);
};
#endif // !defined(AFX_FASCAMERAMANAGE_H__FCC2ACBB_F494_4EBB_BA64_BBB700FCB23D__INCLUDED_)
