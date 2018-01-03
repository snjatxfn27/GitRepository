#pragma once
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

class __declspec(dllexport) CFASStatusQuery 
{
public:
	CFASStatusQuery();
	~CFASStatusQuery();
	//��ѯ������
	int FASFaceDBQuery(char * pIP,char * pSession,struct StructDblist * pStructDblist,int * pTotal_count,int * pList_size);
	//����������
	int FASFaceDBCreate(char * pIP,char * pSession,char * pDbName,struct StructDbCreateReturn * pStructDbCreateReturn);
	//ɾ��������
	int FASFaceDBDelete(char * pIP,char * pSession,char * pDbName);
	//ָ��ID����������ϸ��ϸ��Ϣ��ѯ
	int FASFaceDBInfo(char * pIP,char * pSession,char * pDbid,struct StructDbInfo * pStructDbinfo);
};
