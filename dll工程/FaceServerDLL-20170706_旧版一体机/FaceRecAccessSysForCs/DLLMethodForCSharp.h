// DLLMethodForCSharp.h: interface for the DLLMethodForCSharp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DLLMETHODFORCSHARP_H__9CA38F8B_3914_4C03_85E7_1037FC818A34__INCLUDED_)
#define AFX_DLLMETHODFORCSHARP_H__9CA38F8B_3914_4C03_85E7_1037FC818A34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/****************************************
结构体
*****************************************/
//定义camera列表结构体
struct StructCameraList
{
	char acamera_id[32];//摄像机Id
	char aname[64];//摄像机名称
	char aurl[256];//摄像机rtsp
	char acameramode[10];//摄像机视频资源提供模式(0普通摄像机，1英飞拓人脸抓拍摄像机，2大华人脸抓拍摄像机，……)，整型
	char acamerastate[10];//摄像机状态
	char acamerapolicyid[10];//策略id 布控策略，整型
	char acameraidlist[20];//布控人脸库列表,布控库列表（比如 "2+4+8"）字符串
	char acameranodeid[10];//节点nodeid 指定将布控挂在某个目录下，需要先建立布控目录树，整型 默认0
	char acamerafixedhost[10];//服务器编号，该参数缺省时表示随机连接一台服务器，否则表示固定绑定到一台服务器上，整型 默认0
	char acamerabelonghost[20];//当前摄像机挂载服务器,运行时信息,不可修改
	char acameramanctrl[20];//0:自动模式1:手动模式,运行时信息,不可修改
	char acamerainit[20];//手动初始状态，含义同camera_state,运行时信息,不可修改
	char acameraalarm[20];//相机自上次清零后的告警统计数
	char acamerasnap[20];//相机自上次清零后的抓拍数量
	char acameralastcleartime[30];//上次清零时间 2012/07/12 09:02:00
};

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
//定义人脸库列表结构体
struct StructDblist
{
	char db_id[20];
	char name[50];
	char url[200];
};
//定义创建人脸库返回值结构体
struct StructDbCreateReturn
{
	char pFacedData_id[10];
	char pFacedData_name[50];
	char pFacedData_createdate[20];
	char pFacedData_user[50];
	char pFacedData_state[10];
	char pFacedData_similarity[10];
	char pFacedData_person_count[10];
	char pFacedData_face_count[10];
	char pFacedData_inst_num[10];
	};

//定义策略列表结构体
struct StructRecognitionList
{
	char aId_card[50];//身份证号
	char aSimilarity[5];//相似度
	// 	char aPserson_id[10];//persion_Id
	// 	char aName[50];//姓名
	// 	char aBirth[50];//出生日期
	// 	char aSex[50];//性别
	// 	char aCard_type[50];//证件类型
	// 	char aNative_place[50];//籍贯
	// 	char aAddr[50];//地址
	// 	char aPhone[50];//手机号	
	// 	char aDb_id[6];//所在人脸库;
	// 	char aCreate_date[6];//人员信息地址
	// 	char aDefaut_face_image_url[6];//face 图片地址
	
	};
/****************************************
	登录管理头文件
*****************************************/
	//登录一体机
	extern "C" __declspec(dllexport) int DeviceLogin(char * pIP,char * pSession);
	//登出一体机
	extern "C" __declspec(dllexport) int DeviceLogout(char * pIP,char * pSession);
/****************************************
	摄像机管理头文件
*****************************************/
	//一体机摄像机列表查询
	extern "C" __declspec(dllexport) int FASCameraQuery(char * pIP,char * pSession,StructCameraList * pStructCameralist,int * pTotal_count,int * pList_size);
	extern "C" __declspec(dllexport) int FASCameraQuery_Release(char * pIP,char * pSession,StructCameraList * pStructCameralist,int * pTotal_count,int * pList_size);
	//一体机添加一个摄像机
	extern "C" __declspec(dllexport) int FASCameraAdd(char * pIP,char * pSession,char * pCName,int intCMode,char * pRtsp,char * pDbid,int intNodeid,int intFixedhost,int intPolicyid);
	//一体机删除一个摄像机
	extern "C" __declspec(dllexport) int FASCameraDelete(char * pIP,char * pSession,char * pCameraId);
	//一体机配置一个摄像机
	extern "C" __declspec(dllexport) int FASCameraConfig(char * pIP,char * pSession,char * pCameraId,char * pCName,int intCMode,char * pRtsp,char * pDbid,int intNodeid,int intFixedhost,int intPolicyid);
	//摄像机开始人脸布控(策略手动模式下)
	extern "C" __declspec(dllexport) int FASCameraRecognitionStart(char * pIP,char * pSession,char * pCameraId);
	//摄像机停止人脸布控(策略手动模式下)
	extern "C" __declspec(dllexport) int FASCameraRecognitionStop(char * pIP,char * pSession,char * pCameraId);
/****************************************
	策略管理头文件
*****************************************/

	//策略列表查询 pPolicy_id为指定策略id，若空字符串时，返回所有策略。
	extern "C" __declspec(dllexport) int FASPolicyQuery(char * pIP,char * pSession,char * pPolicy_id,struct StructPolicyList * pStructPolicylist,int * pmt_policy_num);
	//添加一个策略
	extern "C" __declspec(dllexport) int FASPolicyAdd(char * pIP,char * pSession,char * pPolicyname,int intmonitor_type,char * palarm_type,int inttime_schema_id,int intsimilarity,int intsimilarity2,int intsimilarity3,int intmax_return,int intscene_saved,int intmin_pixel,int intmax_face_num,int intzoom_times,int intstorage_day, int intbest_face,int intspeed_mode,int intface_adjust,int intmin_point,int intskip_dup,int intskip_time);
	//删除一个策略
	extern "C" __declspec(dllexport) int FASPolicyDelete(char * pIP,char * pSession,char * pMt_policy_id);
	//配置一个策略
	extern "C" __declspec(dllexport) int FASPolicyModify(char * pIP,char * pSession,char * pMt_policy_id,char * pPolicyname,int intmonitor_type,char * palarm_type,int inttime_schema_id,int intsimilarity,int intsimilarity2,int intsimilarity3,int intmax_return,int intscene_saved,int intmin_pixel,int intmax_face_num,int intzoom_times,int intstorage_day, int intbest_face,int intspeed_mode,int intface_adjust,int intmin_point,int intskip_dup,int intskip_time);

/****************************************
	人脸库管理头文件
*****************************************/
	
	//查询人脸库
	extern "C" __declspec(dllexport) int FASFaceDBQuery(char * pIP,char * pSession,struct StructDblist * pStructDblist,int * pTotal_count,int * pList_size);
	//创建人脸库
	extern "C" __declspec(dllexport) int FASFaceDBCreate(char * pIP,char * pSession,char * pDbName,struct StructDbCreateReturn * pStructDbCreateReturn);
	//删除人脸库
	extern "C" __declspec(dllexport) int FASFaceDBDelete(char * pIP,char * pSession,char * pDbName);

/****************************************
	人脸库中信息管理头文件
*****************************************/
	//更新人信息
	extern "C" __declspec(dllexport) int FASUpdatePersonInfo(char * pIP,char * pSession,char * pFacedb,char * pPersonid,char * pName,char * pSex,char *pBirth,char * pNativeplace,char * pPhone,char * pAddress,char * pIdCard);
	//新增人信息
	extern "C" __declspec(dllexport) int FASAddPersonInfo(char * pIP,char * pSession,char * pFacedb,char * pName,char * pSex,char *pBirth,char * pNativeplace,char * pPhone,char * pAddress,char * pIdCard,char *pPersonid);
	//修改人脸
	extern "C" __declspec(dllexport) int FASAddFaceInfo(char * pIP,char * pSession,char * pEImage,char *EFeature,char * pFacedb,char * pPersonid); 
	//删除一个人信息
	extern "C" __declspec(dllexport) int FASDeletePersonInfo(char * pIP,char * pSession,char * pFacedb,char * pPersonid);
	//根据idcard删除一个人信息
	extern "C" __declspec(dllexport) int FASDeletePersonInfobyIdcard(char * pIP,char * pSession,char * pFacedb,char * pIdcard);
/****************************************
	人脸识别头文件
*****************************************/
	//人脸提取特征
	extern "C" __declspec(dllexport) int FASFeatureDetect(char * pIP,char * pSession,char * pImage,char *pEImage,char *pEFeature);
	//人脸识别
	extern "C" __declspec(dllexport) int FASImgRecongnition(char * pIP,char * pSession,char * pFacedb,char *pImage,char *pTop,char *pSimilarity,int * plistsize,struct StructRecognitionList * pStructRecognitionlist);
	//特征识别
	extern "C" __declspec(dllexport) int FASFeatureMatching(char * pIP,char * pSession,char * pFacedb,char *pFeature,char *pTop,char *pSimilarity,int * plistsize,struct StructRecognitionList * pStructRecognitionlist);
	//性别识别
	extern "C" __declspec(dllexport) int FASImgAgeSexAnalysis(char * pIP,char * pSession,struct StructRecognitionList * pStructRecognitionlist,char * pImage);
#endif