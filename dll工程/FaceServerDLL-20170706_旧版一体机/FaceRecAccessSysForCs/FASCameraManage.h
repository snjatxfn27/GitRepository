// FASCameraManage.h: interface for the FASCameraManage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FASCAMERAMANAGE_H__FCC2ACBB_F494_4EBB_BA64_BBB700FCB23D__INCLUDED_)
#define AFX_FASCAMERAMANAGE_H__FCC2ACBB_F494_4EBB_BA64_BBB700FCB23D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//定义camera列表结构体
struct StructCameraList
{
	char acamera_id[20];//摄像机Id
	char aname[50];//摄像机名称
	char aurl[200];//摄像机rtsp
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
class __declspec(dllexport) CFASCameraManage 
{
public:
	CFASCameraManage();
	~CFASCameraManage();
	//一体机摄像机列表查询
	int FASCameraQuery(char * pIP,char * pSession,struct StructCameraList * pStructCameralist,int * pTotal_count,int * pList_size);
	//一体机添加一个摄像机
	int FASCameraAdd(char * pIP,char * pSession,char * pCName,int intCMode,char * pRtsp,char * pDbid,int intNodeid,int intFixedhost,int intPolicyid);
	//一体机删除一个摄像机
	int FASCameraDelete(char * pIP,char * pSession,char * pCameraId);
	//一体机配置一个摄像机
	int FASCameraConfig(char * pIP,char * pSession,char * pCameraId,char * pCName,int intCMode,char * pRtsp,char * pDbid,int intNodeid,int intFixedhost,int intPolicyid);
	//摄像机开始人脸布控(策略手动模式下)
	int FASCameraRecognitionStart(char * pIP,char * pSession,char * pCameraId);
	//摄像机停止人脸布控(策略手动模式下)
	int FASCameraRecognitionStop(char * pIP,char * pSession,char * pCameraId);
};
#endif // !defined(AFX_FASCAMERAMANAGE_H__FCC2ACBB_F494_4EBB_BA64_BBB700FCB23D__INCLUDED_)
