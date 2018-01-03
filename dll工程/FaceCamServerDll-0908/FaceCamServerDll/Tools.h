#pragma once

CString UrlEncode(CString szToEncode);
CString Utf8ToStringT(LPSTR str);
char* unicodeToMbcs(WCHAR* zWideFilename);
//base64±àÂë
int  EncodeBase64(const unsigned char* pSrc, char* pDst, int nSrcLen, int nMaxLineLen);
//base64½âÂë
int  DecodeBase64(const char* pSrc, unsigned char* pDst, int nSrcLen);