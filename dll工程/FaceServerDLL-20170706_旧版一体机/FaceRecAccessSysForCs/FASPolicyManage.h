// FASPolicyManage.h: interface for the FASPolicyManage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FASPOLICYMANAGE_H__424DEFD2_B2EC_4D90_B542_BC134B1D87DA__INCLUDED_)
#define AFX_FASPOLICYMANAGE_H__424DEFD2_B2EC_4D90_B542_BC134B1D87DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//定义策略列表结构体
struct StructPolicyList
{
	char aPolicy_id[10];//策略Id
	char aname[50];//策略name
	char aPolicymonitor_type[10];//布控类型
	char aPolicyalarm_type[10];//报警类型:1-黑名单布控，有黑名单中人物出现时告警,2-白名单布控，出现人物不在名单时告警,3-抓拍+黑名单,4-抓拍+白名单,5-抓拍
	char aPolicytime_schema_id[10];//时间方案//手工布控方案填0
	char aPolicypolicysimilarity[10];//相似度//普通告警阀值，不填数据库默认为0，抓拍不处理该级别的告警
	char aPolicymax_return[10];//可选(<50)
	char aPolicyscene_saved[10];//可选,默认2-都保存//是否保存告警现场图片,1-都不保存,2-都保存,3-大图保存,人脸不保存,4-大图不保存，人脸保存
	char aPolicymin_pixel[10];// 可选,单位为象素
	char aPolicymax_face_num[10];//每帧最多抓拍人脸个数(1-32)可选,单帧最大抓拍人脸数，系统上限是32，配置超过32以32计算，不提示错误
	char aPolicyzoom_times[10];//可选
	char aPolicyface_storage_day[10];//人脸数据保存天数（1-30）;
	char aPolicybest_face[10];//0:普通抓拍；1：最佳人脸抓拍
	char aPolicyspeed_mode[10];// 0:正常模式 1:分片模式 只对ftp有效，//可选，不填时服务端以0处理
	char aPolicyface_adjust[10]; //0:不进行人脸角度分析，1:进行人脸角度分析，//可选，不填时服务端以0处理
	char aPolicymin_point[10];//最小角度，大于此角度的人脸丢弃
	char aPolicyskip_dup[10]; //0:不进行人脸去重分析，1:进行人脸去重，//可选，不填时服务端以0处理
	char aPolicyskip_time[10];//去重时间周期，单位秒，表示这个时间周期内有重复的人脸则不再抓拍
};
class __declspec(dllexport) CFASPolicyManage
{
public:
	CFASPolicyManage();
	~CFASPolicyManage();
	//策略列表查询 pPolicy_id为指定策略id，若空字符串时，返回所有策略。
	int FASPolicyQuery(char * pIP,char * pSession,char * pPolicy_id,struct StructPolicyList * pStructPolicylist,int * pmt_policy_num);
	//添加一个策略
	int FASPolicyAdd(char * pIP,char * pSession,char * pPolicyname,int intmonitor_type,char * palarm_type,int inttime_schema_id,int intsimilarity,int intsimilarity2,int intsimilarity3,int intmax_return,int intscene_saved,int intmin_pixel,int intmax_face_num,double douzoom_times,int intstorage_day, int intbest_face,int intspeed_mode,int intface_adjust,int intmin_point,int intskip_dup,int intskip_time);
	//删除一个策略
	int FASPolicyDelete(char * pIP,char * pSession,char * pMt_policy_id);
	//配置一个策略
	int FASPolicyModify(char * pIP,char * pSession,char * pMt_policy_id,char * pPolicyname,int intmonitor_type,char * palarm_type,int inttime_schema_id,int intsimilarity,int intsimilarity2,int intsimilarity3,int intmax_return,int intscene_saved,int intmin_pixel,int intmax_face_num,double douzoom_times,int intstorage_day, int intbest_face,int intspeed_mode,int intface_adjust,int intmin_point,int intskip_dup,int intskip_time);
};
#endif // !defined(AFX_FASPOLICYMANAGE_H__424DEFD2_B2EC_4D90_B542_BC134B1D87DA__INCLUDED_)
