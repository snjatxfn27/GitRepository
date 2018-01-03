// FASFaceManage.h: interface for the FASFaceManage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FASFACEMANAGE_H__7A134E50_690D_4B30_B982_FDF9F8F236BB__INCLUDED_)
#define AFX_FASFACEMANAGE_H__7A134E50_690D_4B30_B982_FDF9F8F236BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class __declspec(dllexport) CFASFaceManage
{
public:
	CFASFaceManage();
	~CFASFaceManage();
	//��������Ϣ
	int FASUpdatePersonInfo(char * pIP,char * pSession,char * pFacedb,char * pPersonid,char * pName,char * pSex,char *pBirth,char * pNativeplace,char * pPhone,char * pAddress,char * pIdCard);
	//��������Ϣ
	int FASAddPersonInfo(char * pIP,char * pSession,char * pFacedb,char * pName,char * pSex,char *pBirth,char * pNativeplace,char * pPhone,char * pAddress,char * pIdCard,char *pPersonid);
	//�޸�����
	int FASAddFaceInfo(char * pIP,char * pSession,char * pEImage,char *EFeature,char * pFacedb,char * pPersonid); 
	//ɾ��һ������Ϣ
	int FASDeletePersonInfo(char * pIP,char * pSession,char * pFacedb,char * pPersonid);
	//����idcardɾ��һ������Ϣ
	int FASDeletePersonInfobyIdcard(char * pIP,char * pSession,char * pFacedb,char * pIdcard);

};

#endif // !defined(AFX_FASFACEMANAGE_H__7A134E50_690D_4B30_B982_FDF9F8F236BB__INCLUDED_)
