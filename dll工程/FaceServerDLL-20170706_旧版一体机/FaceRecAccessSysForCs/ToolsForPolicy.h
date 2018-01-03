// ToolsForPolicy.h: interface for the ToolsForPolicy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLSFORPOLICY_H__39F78499_84BF_4192_BB08_CFBE29CE860C__INCLUDED_)
#define AFX_TOOLSFORPOLICY_H__39F78499_84BF_4192_BB08_CFBE29CE860C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//布控策略查询
int PolicyQuery(CString strIP,CString strSession,CString strPolicy_id,struct StructPolicyList * pStructPolicylist,int * mt_policy_num);
//添加一个策略
int PolicyAdd(CString strIP,CString strSession,CString strPolicyname,int intmonitor_type,CString stralarm_type,int inttime_schema_id,int intsimilarity,int intsimilarity2,int intsimilarity3,int intmax_return,int intscene_saved,int intmin_pixel,int intmax_face_num,double douzoom_times,int intstorage_day, int intbest_face,int intspeed_mode,int intface_adjust,int intmin_point,int intskip_dup,int intskip_time);
//删除策略
int PolicyDelete(CString strIP,CString strSession,CString strMt_policy_id);
//修改策略	
int PolicyModify(CString strIP,CString strSession,CString strMt_policy_id,CString strPolicyname,int intmonitor_type,CString stralarm_type,int inttime_schema_id,int intsimilarity,int intsimilarity2,int intsimilarity3,int intmax_return,int intscene_saved,int intmin_pixel,int intmax_face_num,double douzoom_times,int intstorage_day, int intbest_face,int intspeed_mode,int intface_adjust,int intmin_point,int intskip_dup,int intskip_time);
#endif
