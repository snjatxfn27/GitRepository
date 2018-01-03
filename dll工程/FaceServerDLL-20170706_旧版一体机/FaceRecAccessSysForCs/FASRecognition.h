// FASRecognition.h: interface for the FASRecognition class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FASRECOGNITION_H__C1642042_8ECF_4901_93E6_E8DBF71C6C75__INCLUDED_)
#define AFX_FASRECOGNITION_H__C1642042_8ECF_4901_93E6_E8DBF71C6C75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
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
class __declspec(dllexport) CFASRecognition
{
public:
	CFASRecognition();
	~CFASRecognition();
	//������ȡ����
	int FASFeatureDetect(char * pIP,char * pSession,char * pImage,char *pEImage,char *pEFeature);
	//����ʶ��
	int FASImgRecongnition(char * pIP,char * pSession,char * pFacedb,char *pImage,char *pTop,char *pSimilarity,int * plistsize,struct StructRecognitionList * pStructRecognitionlist);
	//ͼƬ�����Ա�������ѯ
	//����ʶ��
	int FASImgRecongnitionForSelect(char * pIP,char * pSession,char * pFacedb,char *pImage,char *pTop,char * pSimilarity,char * pAge,char * pSex,char * pNative_place,char * pAddr,char * pName,char * pPhone,char * pId_card, int * plistsize,struct StructRecognitionList * pStructRecognitionlist);
	//����ʶ��
	int FASFeatureMatching(char * pIP,char * pSession,char * pFacedb,char *pFeature,char *pTop,char *pSimilarity,int * plistsize,struct StructRecognitionList * pStructRecognitionlist);
	//�Ա�ʶ��
	int FASImgAgeSexAnalysis(char * pIP,char * pSession,struct StructRecognitionList * pStructRecognitionlist,char * pImage);
};
#endif // !defined(AFX_FASRECOGNITION_H__C1642042_8ECF_4901_93E6_E8DBF71C6C75__INCLUDED_)
