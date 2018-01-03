#pragma once
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

class __declspec(dllexport) CFASStatusQuery 
{
public:
	CFASStatusQuery();
	~CFASStatusQuery();
	//查询人脸库
	int FASFaceDBQuery(char * pIP,char * pSession,struct StructDblist * pStructDblist,int * pTotal_count,int * pList_size);
	//创建人脸库
	int FASFaceDBCreate(char * pIP,char * pSession,char * pDbName,struct StructDbCreateReturn * pStructDbCreateReturn);
	//删除人脸库
	int FASFaceDBDelete(char * pIP,char * pSession,char * pDbName);
	//指定ID的人脸库详细详细信息查询
	int FASFaceDBInfo(char * pIP,char * pSession,char * pDbid,struct StructDbInfo * pStructDbinfo);
};
