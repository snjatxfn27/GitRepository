#pragma once


int _GetExecuteResults(_RecordsetPtr & pSet, _ConnectionPtr pConnect, CString csSql);

int __BreakCString(CString csData, CStringArray &arOutCString, CString csFindFlag);

void __ExpandAllNode(HTREEITEM hItem, CTreeCtrl& m_treeShow);

int __EncodeBase64(const unsigned char* pSrc, char* pDst, int nSrcLen, int nMaxLineLen);
/*
��base64��������ݽ���
*/
int __DecodeBase64(const char* pSrc, unsigned char* pDst, int nSrcLen);