#pragma once

CString UrlEncode(CString szToEncode);
CString Utf8ToStringT(LPSTR str);
char* unicodeToMbcs(WCHAR* zWideFilename);
//base64����
int  EncodeBase64(const unsigned char* pSrc, char* pDst, int nSrcLen, int nMaxLineLen);
//base64����
int  DecodeBase64(const char* pSrc, unsigned char* pDst, int nSrcLen);