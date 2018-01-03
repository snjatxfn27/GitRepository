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
	//更新人信息
	int FASUpdatePersonInfo(char * pIP,char * pSession,char * pFacedb,char * pPersonid,char * pName,char * pSex,char *pBirth,char * pNativeplace,char * pPhone,char * pAddress,char * pIdCard);
	//新增人信息
	int FASAddPersonInfo(char * pIP,char * pSession,char * pFacedb,char * pName,char * pSex,char *pBirth,char * pNativeplace,char * pPhone,char * pAddress,char * pIdCard,char *pPersonid);
	//修改人脸
	int FASAddFaceInfo(char * pIP,char * pSession,char * pEImage,char *EFeature,char * pFacedb,char * pPersonid); 
	//删除一个人信息
	int FASDeletePersonInfo(char * pIP,char * pSession,char * pFacedb,char * pPersonid);
	//根据idcard删除一个人信息
	int FASDeletePersonInfobyIdcard(char * pIP,char * pSession,char * pFacedb,char * pIdcard);

};

#endif // !defined(AFX_FASFACEMANAGE_H__7A134E50_690D_4B30_B982_FDF9F8F236BB__INCLUDED_)
