// ToolsForFaceDB.h: interface for the ToolsForFaceDB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLSFORFACEDB_H__E77AB622_6B27_4F32_B3F8_476D17C863F3__INCLUDED_)
#define AFX_TOOLSFORFACEDB_H__E77AB622_6B27_4F32_B3F8_476D17C863F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//一体机人脸库列表查询
int FaceDBQuery(CString strIP,CString strSession,struct StructDblist * pStructDblist,int * pTotal_count,int * pList_size);
//指定ID的人脸库详细详细信息查询
int GetOneFaceDBInfo(CString strIP,CString strSession,CString strDbid,struct StructDbInfo * pStructDbinfo);
//一体机创建人脸库
int FaceDBCreate(CString strIP,CString strSession,CString strDbName,struct StructDbCreateReturn * pStructDbCreateReturn);
//一体机删除一个指定名称的人脸库
int FaceDBDelete(CString strIP,CString strSession,CString strDbName);
#endif // !defined(AFX_TOOLSFORFACEDB_H__E77AB622_6B27_4F32_B3F8_476D17C863F3__INCLUDED_)
