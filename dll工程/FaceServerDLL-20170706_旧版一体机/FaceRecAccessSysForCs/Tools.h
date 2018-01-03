#if !defined(AFX_TOOLS_H__E231DFDA_C8B4_4681_B30F_EAFE07D4911D__INCLUDED_)
#define AFX_TOOLS_H__E231DFDA_C8B4_4681_B30F_EAFE07D4911D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// Tools window
//定义数据库操作方法
#define getsqlstr(x) (char*)_bstr_t(pSet->GetCollect(_variant_t(x)))
#define getsqlint(x) pSet->GetCollect(_variant_t(x)).iVal;
#define getsqlstr_employee(x) (char*)_bstr_t(pSet_Employee_Table->GetCollect(_variant_t(x)))
#define getsqlint_employee(x) pSet_Employee_Table->GetCollect(_variant_t(x)).iVal;
/*
将数据编码为base64的形式，pSrc为原数据指针，pDst为编码后数据指针
*/
int EncodeBase64(const unsigned char* pSrc, char* pDst, int nSrcLen, int nMaxLineLen);
/*
将base64编码的数据解码
*/
int DecodeBase64(const char* pSrc, unsigned char* pDst, int nSrcLen);
//字符编解码
CString GB2312ToUTF8(const char* str);
//字符集转换UTF-8toGBK
CString UTF8ToGB2312(const char* str);
//登录一体机
int Login(CString strIP,char * pSession);
//登出一体机
int Logout(CString strIP,char * pSession);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLS_H__E231DFDA_C8B4_4681_B30F_EAFE07D4911D__INCLUDED_)