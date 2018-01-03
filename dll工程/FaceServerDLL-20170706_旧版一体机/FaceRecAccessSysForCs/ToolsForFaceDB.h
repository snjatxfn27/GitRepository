// ToolsForFaceDB.h: interface for the ToolsForFaceDB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLSFORFACEDB_H__E77AB622_6B27_4F32_B3F8_476D17C863F3__INCLUDED_)
#define AFX_TOOLSFORFACEDB_H__E77AB622_6B27_4F32_B3F8_476D17C863F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//һ����������б��ѯ
int FaceDBQuery(CString strIP,CString strSession,struct StructDblist * pStructDblist,int * pTotal_count,int * pList_size);
//ָ��ID����������ϸ��ϸ��Ϣ��ѯ
int GetOneFaceDBInfo(CString strIP,CString strSession,CString strDbid,struct StructDbInfo * pStructDbinfo);
//һ�������������
int FaceDBCreate(CString strIP,CString strSession,CString strDbName,struct StructDbCreateReturn * pStructDbCreateReturn);
//һ���ɾ��һ��ָ�����Ƶ�������
int FaceDBDelete(CString strIP,CString strSession,CString strDbName);
#endif // !defined(AFX_TOOLSFORFACEDB_H__E77AB622_6B27_4F32_B3F8_476D17C863F3__INCLUDED_)
