#pragma once

//执行sql
int _GetExecuteResults(_RecordsetPtr & pSet, _ConnectionPtr pConnect, CString csSql);
//拆分字符串
int __BreakCString(CString csData, CStringArray &arOutCString, CString csFindFlag);
//展开tree
void __ExpandAllNode(HTREEITEM hItem, CTreeCtrl& m_treeShow);
//将base64编码的数据编码
int __EncodeBase64(const unsigned char* pSrc, char* pDst, int nSrcLen, int nMaxLineLen);
//将base64编码的数据解码
int __DecodeBase64(const char* pSrc, unsigned char* pDst, int nSrcLen);

void g_StringLanType(char *szDstLan, char *szLanCn, char *szLanEn);

