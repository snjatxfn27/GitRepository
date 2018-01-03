#pragma once


int _GetExecuteResults(_RecordsetPtr & pSet, _ConnectionPtr pConnect, CString csSql);

int __BreakCString(CString csData, CStringArray &arOutCString, CString csFindFlag);

void __ExpandAllNode(HTREEITEM hItem, CTreeCtrl& m_treeShow);

int __EncodeBase64(const unsigned char* pSrc, char* pDst, int nSrcLen, int nMaxLineLen);
/*
将base64编码的数据解码
*/
int __DecodeBase64(const char* pSrc, unsigned char* pDst, int nSrcLen);