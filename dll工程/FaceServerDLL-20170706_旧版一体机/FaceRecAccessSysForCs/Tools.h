#if !defined(AFX_TOOLS_H__E231DFDA_C8B4_4681_B30F_EAFE07D4911D__INCLUDED_)
#define AFX_TOOLS_H__E231DFDA_C8B4_4681_B30F_EAFE07D4911D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// Tools window
//�������ݿ��������
#define getsqlstr(x) (char*)_bstr_t(pSet->GetCollect(_variant_t(x)))
#define getsqlint(x) pSet->GetCollect(_variant_t(x)).iVal;
#define getsqlstr_employee(x) (char*)_bstr_t(pSet_Employee_Table->GetCollect(_variant_t(x)))
#define getsqlint_employee(x) pSet_Employee_Table->GetCollect(_variant_t(x)).iVal;
/*
�����ݱ���Ϊbase64����ʽ��pSrcΪԭ����ָ�룬pDstΪ���������ָ��
*/
int EncodeBase64(const unsigned char* pSrc, char* pDst, int nSrcLen, int nMaxLineLen);
/*
��base64��������ݽ���
*/
int DecodeBase64(const char* pSrc, unsigned char* pDst, int nSrcLen);
//�ַ������
CString GB2312ToUTF8(const char* str);
//�ַ���ת��UTF-8toGBK
CString UTF8ToGB2312(const char* str);
//��¼һ���
int Login(CString strIP,char * pSession);
//�ǳ�һ���
int Logout(CString strIP,char * pSession);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLS_H__E231DFDA_C8B4_4681_B30F_EAFE07D4911D__INCLUDED_)