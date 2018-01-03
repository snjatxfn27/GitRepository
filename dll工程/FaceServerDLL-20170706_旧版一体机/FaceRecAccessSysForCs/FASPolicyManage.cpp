// FASPolicyManage.cpp: implementation of the FASPolicyManage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FaceRecAccessSys.h"
#include "FASPolicyManage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFASPolicyManage::CFASPolicyManage()
{

}

CFASPolicyManage::~CFASPolicyManage()
{

}

//策略列表查询
int CFASPolicyManage::FASPolicyQuery(char * pIP,char * pSession,char * pPolicy_id,struct StructPolicyList * pStructPolicylist,int * pmt_policy_num)
{	
	CString strIP;
	CString strSession;
	CString strPolicy_id;
	strIP.Format("%s",pIP);
	strSession.Format("%s",pSession);
	strPolicy_id.Format("%s",pPolicy_id);
	int m_result = PolicyQuery(strIP,strSession,strPolicy_id,pStructPolicylist,pmt_policy_num);
	if(m_result != 0)
	{
		return m_result;
	}
	return 0;
}

//添加一个策略
int CFASPolicyManage::FASPolicyAdd(char * pIP,char * pSession,char * pPolicyname,int intmonitor_type,char * palarm_type,int inttime_schema_id,int intsimilarity,int intsimilarity2,int intsimilarity3,int intmax_return,int intscene_saved,int intmin_pixel,int intmax_face_num,double douzoom_times,int intstorage_day, int intbest_face,int intspeed_mode,int intface_adjust,int intmin_point,int intskip_dup,int intskip_time)
{	
	CString strIP;
	CString strSession;
	CString strPolicyname;
	CString strPolicyalarm_type;
	strIP.Format("%s",pIP);
	strSession.Format("%s",pSession);
	strPolicyname.Format("%s",pPolicyname);
	strPolicyalarm_type.Format("%s",palarm_type);
	int m_result = PolicyAdd(strIP,strSession,strPolicyname,intmonitor_type,strPolicyalarm_type,inttime_schema_id,intsimilarity,intsimilarity2,intsimilarity3,intmax_return,intscene_saved,intmin_pixel,intmax_face_num,douzoom_times,intstorage_day,intbest_face,intspeed_mode,intface_adjust,intmin_point,intskip_dup,intskip_time);
	if(m_result != 0)
	{
		return m_result;
	}
	return 0;
}

//修改一个策略
int CFASPolicyManage::FASPolicyModify(char * pIP,char * pSession,char * pMt_policy_id,char * pPolicyname,int intmonitor_type,char * palarm_type,int inttime_schema_id,int intsimilarity,int intsimilarity2,int intsimilarity3,int intmax_return,int intscene_saved,int intmin_pixel,int intmax_face_num,double douzoom_times,int intstorage_day, int intbest_face,int intspeed_mode,int intface_adjust,int intmin_point,int intskip_dup,int intskip_time)
{	
	CString strIP;
	CString strSession;
	CString strMt_policy_id;
	CString strPolicyname;
	CString strPolicyalarm_type;
	strIP.Format("%s",pIP);
	strSession.Format("%s",pSession);
	strPolicyname.Format("%s",pPolicyname);
	strPolicyalarm_type.Format("%s",palarm_type);
	strMt_policy_id.Format("%s",pMt_policy_id);
	int m_result = PolicyModify(strIP,strSession,strMt_policy_id,strPolicyname,intmonitor_type,strPolicyalarm_type,inttime_schema_id,intsimilarity,intsimilarity2,intsimilarity3,intmax_return,intscene_saved,intmin_pixel,intmax_face_num,douzoom_times,intstorage_day,intbest_face,intspeed_mode,intface_adjust,intmin_point,intskip_dup,intskip_time);
	if(m_result != 0)
	{
		return m_result;
	}
	return 0;
}

//删除一个策略
int CFASPolicyManage::FASPolicyDelete(char * pIP,char * pSession,char * pMt_policy_id)
{	
	CString strIP;
	CString strSession;
	CString strMt_policy_id;
	CString strPolicyname;
	CString strPolicyalarm_type;
	strIP.Format("%s",pIP);
	strSession.Format("%s",pSession);
	strMt_policy_id.Format("%s",pMt_policy_id);
	int m_result = PolicyDelete(strIP,strSession,strMt_policy_id);
		if(m_result != 0)
		{
			return m_result;
		}
	return 0;
}
