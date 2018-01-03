delete from sys_parameter;
delete from face_cloud_conf;
delete from host_info;
delete from process_info;
delete from facedb_card_type;
delete from host_disk;
delete from sys_role_info;
delete from sys_user_info;
delete from sys_service_type_info;
delete from sys_service_info;
delete from sys_pri_role_service;
delete from mt_policy_template;
delete from process_lock;

insert into sys_parameter (PARAM_NAME, PARAM_VALUE, MODULE_NAME, STATE)
values ('RTMS_LEADER', '1', 'RTM', 1);

insert into sys_parameter (PARAM_NAME, PARAM_VALUE, MODULE_NAME, STATE)
values ('RTM_THR_NUM', '4', 'RTM', 1);

insert into sys_parameter (PARAM_NAME, PARAM_VALUE, MODULE_NAME, STATE)
values ('RTM_RECO_SERVER', 'NGINX_IP:NGINX_PORT', 'RTM', 1);

insert into sys_parameter (PARAM_NAME, PARAM_VALUE, MODULE_NAME, STATE)
values ('USE_TEST', '1.2', 'CHUNK', 0);

insert into sys_parameter (PARAM_NAME, PARAM_VALUE, MODULE_NAME, STATE)
values ('Best_Angel_Min', '0', 'RTMSN', 1);
insert into sys_parameter (PARAM_NAME, PARAM_VALUE, MODULE_NAME, STATE)
values ('Best_Angel_Max', '15', 'RTMSN', 1);
insert into sys_parameter (PARAM_NAME, PARAM_VALUE, MODULE_NAME, STATE)
values ('Best_Angel_Score', '10', 'RTMSN', 1);
insert into sys_parameter (PARAM_NAME, PARAM_VALUE, MODULE_NAME, STATE)
values ('Normal_Angel_Min', '15', 'RTMSN', 1);
insert into sys_parameter (PARAM_NAME, PARAM_VALUE, MODULE_NAME, STATE)
values ('Normal_Angel_Max', '25', 'RTMSN', 1);
insert into sys_parameter (PARAM_NAME, PARAM_VALUE, MODULE_NAME, STATE)
values ('Normal_Angel_Score', '6', 'RTMSN', 1);
insert into sys_parameter (PARAM_NAME, PARAM_VALUE, MODULE_NAME, STATE)
values ('Info_Angel_Min', '25', 'RTMSN', 1);
insert into sys_parameter (PARAM_NAME, PARAM_VALUE, MODULE_NAME, STATE)
values ('Info_Angel_Max', '360', 'RTMSN', 1);
insert into sys_parameter (PARAM_NAME, PARAM_VALUE, MODULE_NAME, STATE)
values ('Info_Angel_Score', '0', 'RTMSN', 1);

insert into sys_parameter (PARAM_NAME, PARAM_VALUE, MODULE_NAME, STATE)
values ('Best_Pixel_Min', '80', 'RTMSN', 1);
insert into sys_parameter (PARAM_NAME, PARAM_VALUE, MODULE_NAME, STATE)
values ('Best_Pixel_Max', '800', 'RTMSN', 1);
insert into sys_parameter (PARAM_NAME, PARAM_VALUE, MODULE_NAME, STATE)
values ('Best_Pixel_Score', '6', 'RTMSN', 1);
insert into sys_parameter (PARAM_NAME, PARAM_VALUE, MODULE_NAME, STATE)
values ('Normal_Pixel_Min', '50', 'RTMSN', 1);
insert into sys_parameter (PARAM_NAME, PARAM_VALUE, MODULE_NAME, STATE)
values ('Normal_Pixel_Max', '80', 'RTMSN', 1);
insert into sys_parameter (PARAM_NAME, PARAM_VALUE, MODULE_NAME, STATE)
values ('Normal_Pixel_Score', '4', 'RTMSN', 1);
insert into sys_parameter (PARAM_NAME, PARAM_VALUE, MODULE_NAME, STATE)
values ('Info_Pixel_Min', '0', 'RTMSN', 1);
insert into sys_parameter (PARAM_NAME, PARAM_VALUE, MODULE_NAME, STATE)
values ('Info_Pixel_Max', '50', 'RTMSN', 1);
insert into sys_parameter (PARAM_NAME, PARAM_VALUE, MODULE_NAME, STATE)
values ('Info_Pixel_Score', '1', 'RTMSN', 1);

insert into sys_parameter (PARAM_NAME, PARAM_VALUE, MODULE_NAME, STATE)
values ('Best_Alarm_Score', '14', 'RTMSN', 1);
insert into sys_parameter (PARAM_NAME, PARAM_VALUE, MODULE_NAME, STATE)
values ('Normal_Alarm_score', '10', 'RTMSN', 1);
insert into sys_parameter (PARAM_NAME, PARAM_VALUE, MODULE_NAME, STATE)
values ('Info_Alarm_Score', '0', 'RTMSN', 1);

insert into process_lock (process_name,reply_queue,lock_name,is_owner)
values ('fc_env', 'null', 'null', 0);

insert into face_cloud_conf (CONF_ID,CONF_NAME,DB_ID,MQ_ADDR,CHUNK_MAX_FACE,FEATURE_SIZE)
values (1,'test',0,'tcp://127.0.0.1:6379','1000000',1028);

insert into host_info (HOST_ID,HOST_NAME,IP,LOGIN_NAME,LOGIN_PWD,USE_TYPE,CPU_COUNT,MEMORY,DISK_SIZE,MAX_CONNECT,STATE) values (1,'unisibf1','127.0.0.1','root','unisib1000',63,2,2,200,4,1);

insert into host_disk (host_id,disk_id,disk_name,dir,io_capacity,state) 
values (1, 1, 'disk', '/opt/fc_data/fc_disk/1', 1, 1);

insert into facedb_card_type (card_type_id,type_name,crt_date,crt_user)
values (1,'一代身份证',now(),'root');

insert into facedb_card_type (card_type_id,type_name,crt_date,crt_user)
values (2,'二代身份证',now(),'root');

insert into facedb_card_type (card_type_id,type_name,crt_date,crt_user)
values (4,'士官证',now(),'root');

insert into facedb_card_type (card_type_id,type_name,crt_date,crt_user)
values (8,'学生证',now(),'root');

insert into facedb_card_type (card_type_id,type_name,crt_date,crt_user)
values (16,'驾驶证',now(),'root');

insert into facedb_card_type (card_type_id,type_name,crt_date,crt_user)
values (32,'护照',now(),'root');

insert into facedb_card_type (card_type_id,type_name,crt_date,crt_user)
values (64,'港澳通行证',now(),'root');

insert into facedb_card_type (card_type_id,type_name,crt_date,crt_user)
values (128,'其他',now(),'root');

insert into sys_role_info (ROLE_ID, ROLE_NAME, CRT_DATE, CRT_USER, REMARK)
values (1, '超级管理员', now(), 'root', '拥有全部操作权限');
insert into sys_role_info (ROLE_ID, ROLE_NAME, CRT_DATE, CRT_USER, REMARK)
values (2, '管理员', now(), 'root', '拥有除了创建管理员和超级管理员用户外的所有权限');
insert into sys_role_info (ROLE_ID, ROLE_NAME, CRT_DATE, CRT_USER, REMARK)
values (3, '普通用户', now(), 'root', '只有查询比对类权限');

insert into sys_user_info (USER_ID, ROLE_ID, LOGIN, PASSWORD, NAME, SEX, EMPLOYEE_NO, CRT_DATE, CRT_USER, REMARK)
values (1, 1, 'root', 'E10ADC3949BA59ABBE56E057F20F883E', 'root', 1, '001', null, null, null);
insert into sys_user_info (USER_ID, ROLE_ID, LOGIN, PASSWORD, NAME, SEX, EMPLOYEE_NO, CRT_DATE, CRT_USER, REMARK)
values (2, 2, 'admin', 'E10ADC3949BA59ABBE56E057F20F883E', 'admin', 1, '002', null, null, null);
insert into sys_user_info (USER_ID, ROLE_ID, LOGIN, PASSWORD, NAME, SEX, EMPLOYEE_NO, CRT_DATE, CRT_USER, REMARK)
values (3, 3, 'user', 'E10ADC3949BA59ABBE56E057F20F883E', 'user', 1, '003', null, null, null);

insert into sys_service_type_info (TYPE_ID, TYPE_NAME, REMARK)
values (1, '系统基础服务', null);
insert into sys_service_type_info (TYPE_ID, TYPE_NAME, REMARK)
values (2, '系统监控服务', null);
insert into sys_service_type_info (TYPE_ID, TYPE_NAME, REMARK)
values (3, '人脸库服务', null);
insert into sys_service_type_info (TYPE_ID, TYPE_NAME, REMARK)
values (4, '查重服务', null);
insert into sys_service_type_info (TYPE_ID, TYPE_NAME, REMARK)
values (5, '布控服务', null);

insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (44, 3, '特定人脸图片查询', '/facedb/*/persons/*/faces/*/image', 'GET', '特定人脸图片查询', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (45, 3, '特定人脸特征查询', '/facedb/*/persons/*/faces/*/feature', 'GET', '特定人脸特征查询', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (46, 3, '图像比较', '/faceops/image_compare', 'POST', '图像比较', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (47, 3, '人脸检测', '/faceops/image_detection', 'POST', '人脸检测', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (48, 3, '人脸识别', '/faceops/image_recognition', 'POST', '人脸识别', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (49, 3, '特征匹配', '/faceops/feature_matching', 'POST', '特征匹配', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (50, 3, '特征比较', '/faceops/feature_compare', 'POST', '特征比较', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (51, 3, '年龄性别分析', '/faceops/age_sex_analysis', 'POST', '年龄性别分析', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (52, 3, '特征提取', '/faceops/extract_feature', 'POST', '特征提取', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (53, 3, '图片质量分析', '/faceops/image_quality', 'POST', '图片质量分析', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (67, 5, '查询布控服务器状态', '/rtmonitor/manage/host', 'GET', '查询布控服务器状态', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (68, 5, '查询摄像机', '/rtmonitor/manage', 'GET', '查询摄像机', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (69, 5, '添加摄像机', '/rtmonitor/manage', 'PUT', '添加摄像机', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (70, 5, '配置摄像机', '/rtmonitor/manage/*', 'POST', '配置摄像机', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (71, 5, '删除摄像机', '/rtmonitor/manage/*', 'DELETE', '删除摄像机', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (72, 5, '添加兴趣区域', '/rtmonitor/manage/*/roi', 'PUT', '添加兴趣区域', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (73, 5, '删除兴趣区域', '/rtmonitor/manage/*/roi', 'DELETE', '删除兴趣区域', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (74, 5, '修改兴趣区域', '/rtmonitor/manage/*/roi', 'POST', '修改兴趣区域', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (75, 5, '获取兴趣区域', '/rtmonitor/manage/*/roi', 'GET', '获取兴趣区域', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (76, 5, '开始人脸布控', '/rtmonitor/manage/video_recognition/*', 'POST', '开始人脸布控', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (77, 5, '停止人脸布控', '/rtmonitor/manage/video_recognition_stop/*', 'POST', '停止人脸布控', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (78, 5, '实时查看视频', '/rtmonitor/manage/getvideo/*', 'GET', '实时查看视频', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (79, 5, '添加摄像机布控时间计划', '/rtmonitor/manage/*/time_plan', 'PUT', '添加摄像机布控时间计划', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (80, 5, '删除摄像机布控时间计划', '/rtmonitor/manage/*/time_plan', 'DELETE', '删除摄像机布控时间计划', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (81, 5, '修改摄像机布控时间计划', '/rtmonitor/manage/*/time_plan', 'POST', '修改摄像机布控时间计划', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (82, 5, '查询摄像机布控时间计划', '/rtmonitor/manage/*/time_plan', 'GET', '查询摄像机布控时间计划', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (83, 5, '查询告警记录', '/rtmonitor/alarm', 'GET', '查询告警记录', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (84, 5, '删除告警记录', '/rtmonitor/alarm', 'DELETE', '删除告警记录', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (85, 5, '修改告警记录', '/rtmonitor/alarm', 'POST', '修改告警记录', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (86, 5, '获得告警记录中的现场抓拍人脸', '/rtmonitor/alarm/cface/*', 'GET', '获得告警记录中的现场抓拍人脸', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (87, 5, '获得告警记录中的现场抓拍图片', '/rtmonitor/alarm/image/*', 'GET', '获得告警记录中的现场抓拍图片', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (88, 5, '增加时间策略', '/rtmonitor/time_schema', 'PUT', '增加时间策略', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (89, 5, '删除时间策略', '/rtmonitor/time_schema/*', 'DELETE', '删除时间策略', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (90, 5, '调整时间策略', '/rtmonitor/time_schema/*', 'POST', '调整时间策略', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (91, 5, '查询时间策略', '/rtmonitor/time_schema', 'GET', '查询时间策略', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (92, 5, '创建结点', '/rtmonitor/node/create', 'PUT', '创建结点', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (93, 5, '修改结点', '/rtmonitor/node/modify/*', 'POST', '修改结点', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (94, 5, '查询结点子列表信息', '/rtmonitor/node/child/*', 'GET', '查询结点子列表信息', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (95, 5, '查询结点摄像机信息', '/rtmonitor/node/camera/*', 'GET', '查询结点摄像机信息', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (96, 5, '删除结点', '/rtmonitor/node/drop/*', 'DELETE', '删除结点', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (97, 5, '查询布控历史人脸', '/rtmonitor/mt_his_face/search', 'GET', '查询布控历史人脸', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (98, 5, '查询视频检索目录', '/rtmonitor/video_search/dir', 'GET', '查询视频检索目录', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (99, 5, '查询视频检索文件', '/rtmonitor/video_search/file', 'GET', '查询视频检索文件', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (1, 1, '系统日志删除', '/base/log/sys', 'DELETE', '删除系统日志', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (2, 1, '系统日志查询', '/base/log/sys', 'GET', '查询系统日志', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (3, 1, '添加操作员信息', '/base/auth/user', 'PUT', '添加操作员信息', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (4, 1, '删除操作员信息', '/base/auth/user', 'DELETE', '删除操作员信息', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (5, 1, '修改操作员信息', '/base/auth/user', 'POST', '修改操作员信息', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (6, 1, '查询操作员信息', '/base/auth/query/user', 'GET', '查询操作员信息', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (11, 1, '查询所有角色信息', '/base/auth/query/role', 'GET', '查询所有角色信息', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (12, 1, '查询指定角色信息', '/base/auth/query/role/*', 'GET', '查询指定角色信息', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (13, 1, '查询所有角色权限', '/base/auth/query/role_permission', 'GET', '查询所有角色权限', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (14, 1, '查询指定角色权限', '/base/auth/query/role_permission/*', 'GET', '查询指定角色权限', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (7, 1, '修改操作员密码', '/base/auth/user/password', 'PUT', '修改操作员密码', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (8, 1, '重置操作员密码', '/base/auth/user/password', 'POST', '重置操作员密码', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (9, 1, '操作员登录', '/base/auth/login', 'POST', '操作员登录', 0, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (10, 1, '操作员登出', '/base/auth/logout', 'POST', '操作员登出', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (15, 1, '查询用户操作日志', '/base/log/client', 'GET', '查询用户操作日志', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (16, 1, '保存用户操作日志', '/base/log/client', 'POST', '保存用户操作日志', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (17, 1, '删除用户操作日志', '/base/log/client', 'DELETE', '删除用户操作日志', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (18, 1, '查询异步结果', '/base/async', 'GET', '查询异步操作结果', 1, null, null);

insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (19, 3, '人脸库查询', '/facedb', 'GET', '人脸库查询', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (20, 3, '人脸库创建', '/facedb', 'PUT', '人脸库创建', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (21, 3, '人脸库更新', '/facedb', 'POST', '人脸库更新', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (22, 3, '人脸库删除', '/facedb', 'DELETE', '人脸库删除', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (23, 3, '人脸库查询', '/facedb/*', 'GET', '人脸库查询', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (24, 3, '人脸库更新', '/facedb/*', 'POST', '人脸库更新', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (25, 3, '人脸库删除', '/facedb/*', 'DELETE', '人脸库删除', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (26, 3, '证件类型查询', '/facedb/attr/idcard', 'GET', '证件类型查询', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (27, 3, 'person查询', '/facedb/*/persons', 'GET', 'person查询', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (28, 3, 'person创建', '/facedb/*/persons', 'PUT', 'person创建', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (29, 3, 'person修改', '/facedb/*/persons', 'POST', 'person修改', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (30, 3, 'person删除', '/facedb/*/persons', 'DELETE', 'person删除', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (31, 3, '特定person查询', '/facedb/*/persons/*', 'GET', '特定person查询', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (32, 3, '特定person创建', '/facedb/*/persons/*', 'PUT', '特定person创建', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (33, 3, '特定person修改', '/facedb/*/persons/*', 'POST', '特定person修改', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (34, 3, '特定person删除', '/facedb/*/persons/*', 'DELETE', '特定person删除', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (35, 3, '查询人脸列表', '/facedb/*/persons/*/faces', 'GET', '查询人脸列表', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (36, 3, '添加人脸', '/facedb/*/persons/*/faces', 'PUT', '添加人脸', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (37, 3, '人脸修改', '/facedb/*/persons/*/faces', 'POST', '人脸修改', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (38, 3, '人脸删除', '/facedb/*/persons/*/faces', 'DELETE', '人脸删除', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (39, 3, '添加人脸', '/facedb/*/persons/*/image', 'PUT', '添加人脸', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (40, 3, '特定face查询', '/facedb/*/persons/*/faces/*', 'GET', '特定face查询', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (41, 3, '特定face修改', '/facedb/*/persons/*/image/*', 'POST', '特定face修改', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (42, 3, '特定face修改', '/facedb/*/persons/*/faces/*', 'POST', '特定face修改', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (43, 3, '特定face删除', '/facedb/*/persons/*/faces/*', 'DELETE', '特定face删除', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (100, 2, '获取所有节点状态', '/sysmgr/devs', 'GET', '获取所有节点状态', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (101, 2, '设置结点类型', '/sysmgr/devs', 'PUT', '设置结点类型', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (102, 2, '删除结点类型', '/sysmgr/devs', 'DELETE', '删除结点类型', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (103, 2, '查询某个结点详细状态', '/sysmgr/devs/*', 'GET', '查询某个结点详细状态', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (104, 2, '设置节点信息', '/sysmgr/devs/*', 'POST', '设置节点信息', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (105, 2, '删除结点信息', '/sysmgr/devs/*', 'DELETE', '删除结点信息', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (106, 2, '查询所有节点详细状态', '/sysmgr/process', 'GET', '查询所有节点详细状态', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (54, 4, '获取所有查重任务信息', '/dupface', 'GET', '获取所有查重任务信息', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (55, 4, '添加查重任务', '/dupface', 'PUT', '添加查重任务', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (56, 4, '删除所有查重记录及结果', '/dupface', 'DELETE', '删除所有查重记录及结果', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (57, 4, '获取指定查重记录信息', '/dupface/*', 'GET', '获取指定查重记录信息', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (58, 4, '修改指定查重记录信息', '/dupface/*', 'POST', '修改指定查重记录信息', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (59, 4, '删除指定查重记录信息', '/dupface/*', 'DELETE', '删除指定查重记录信息', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (60, 4, '获取指定查重结果的源faceid列表', '/dupface/*/srcfaceid', 'GET', '获取指定查重结果的源faceid列表', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (108, 3, '批量查询人脸', '/facedb/*/faces', 'GET', '批量查询人脸', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (109, 3, '批量注册人员', '/facedb/*/faces', 'PUT', '批量注册人员', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (110, 3, '批量人脸检测', '/faceops/image_detection_batch', 'POST', '批量人脸检测', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (111, 1, '系统信息查询', '/base/about/systeminfo', 'GET', '系统信息查询', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (112, 5, '布控策略添加', '/rtmonitor/mt_policy', 'PUT', '布控策略添加', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (113, 5, '布控策略查询', '/rtmonitor/mt_policy', 'GET', '布控策略查询', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (114, 5, '布控策略修改', '/rtmonitor/mt_policy/*', 'POST', '布控策略修改', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (115, 5, '布控策略删除', '/rtmonitor/mt_policy/delete/*', 'DELETE', '布控策略删除', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (116, 5, '获取视频文件列表', '/rtmonitor/video_search/file/latest/*', 'GET', '获取视频文件列表', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (117, 5, '批量获取告警现场截图', '/rtmonitor/alarm/batch_image', 'GET', '批量获取告警现场截图', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (118, 5, '批量获取告警人脸截图', '/rtmonitor/alarm/batch_cface', 'GET', '批量获取告警人脸截图', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (119, 5, '探测相机地址', '/rtmonitor/manage/probe', 'GET', '', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (120, 5, '探测相机RTSP', '/rtmonitor/manage/probe/rtsp', 'GET', '', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (121, 5, '抓拍统计数清零', '/rtmonitor/manage/*/statreset', 'POST', '抓拍统计数清零', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (122, 2, 'facecloud系统重起', '/sysmgr/reboot', 'PUT', 'facecloud系统重起', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (123, 3, '修改相似度权值', '/facedb/attr/sim_cheat', 'POST', '', 0, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (124, 3, '指定人脸为单次注册', '/facedb/attr/face_pri_up', 'POST', '', 0, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (125, 5, '修改告警结果相似度', '/rtmonitor/alarm/result', 'POST', '修改告警结果相似度', 0, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (126, 3, '指定特征为单次注册', '/facedb/attr/feat_pri_up', 'POST', '', 0, null, null);

insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (61, 4, '获取指定查重的结果列表', '/dupface/*/result', 'GET', '获取指定查重的结果列表', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (62, 4, '删除指定查重的结果列表', '/dupface/*/result', 'DELETE', '删除指定查重的结果列表', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (63, 4, '获取指定查重任务、指定源faceid的目标结果列表', '/dupface/*/srcfaceid/*', 'GET', '获取指定查重任务、指定源faceid的目标结果列表', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (64, 4, '获取指定查重的某一条结果', '/dupface/*/result/*', 'GET', '获取指定查重的某一条结果', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (66, 4, '删除指定查重的某一条结果', '/dupface/*/result/*', 'DELETE', '删除指定查重的某一条结果', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (65, 4, '设置查重结果是否已确认', '/dupface/*/result/*', 'POST', '设置查重结果是否已确认', 1, null, null);
insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
values (107, 4, '批量修改查重记录信息', '/dupface', 'POST', '批量修改查重记录信息', 1, null, null);

insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1057, 1, 108, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1058, 2, 108, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1059, 3, 108, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1060, 1, 109, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1061, 2, 109, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1062, 1, 110, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1063, 2, 110, now(), 'root', null);

insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1064, 1, 111, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1065, 2, 111, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1066, 3, 111, now(), 'root', null);

insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1067, 1, 116, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1068, 2, 116, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1069, 3, 116, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1070, 1, 117, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1071, 2, 117, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1072, 3, 117, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1073, 1, 118, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1074, 2, 118, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1075, 3, 118, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1076, 1, 113, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1077, 2, 113, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1078, 3, 113, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1079, 1, 112, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1080, 2, 112, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1081, 1, 114, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1082, 2, 114, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1083, 1, 115, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1084, 2, 115, now(), 'root', null);

insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1085, 1, 119, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1086, 2, 119, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1087, 3, 119, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1088, 1, 120, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1089, 2, 120, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1090, 3, 120, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1091, 1, 121, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1092, 2, 121, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1093, 1, 122, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1094, 1, 123, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1095, 1, 124, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1096, 1, 125, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1097, 1, 126, now(), 'root', null);

insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1008, 3, 5, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1009, 3, 6, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1010, 3, 7, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1007, 3, 10, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1011, 3, 11, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1012, 3, 12, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1013, 3, 13, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1014, 3, 14, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1015, 3, 16, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1016, 3, 18, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1017, 3, 19, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1018, 3, 23, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1019, 3, 26, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1020, 3, 27, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1021, 3, 31, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1022, 3, 35, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1023, 3, 40, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1024, 3, 44, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1025, 3, 53, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1026, 3, 54, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1027, 3, 57, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1028, 3, 60, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1029, 3, 61, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1030, 3, 63, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1031, 3, 64, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1032, 3, 67, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1033, 3, 68, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1034, 3, 75, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1035, 3, 78, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1036, 3, 82, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1037, 3, 83, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1038, 3, 86, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1039, 3, 91, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1040, 3, 94, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1041, 3, 95, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1042, 3, 97, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1043, 3, 98, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1044, 3, 99, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1045, 3, 100, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1046, 3, 103, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1047, 3, 106, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1048, 3, 45, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1049, 3, 46, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1050, 3, 47, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1051, 3, 48, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1052, 3, 49, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1053, 3, 50, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1054, 3, 51, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1055, 3, 52, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (1056, 3, 87, now(), 'root', null);



insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (613, 1, 1, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (614, 1, 2, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (615, 1, 3, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (616, 1, 4, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (617, 1, 5, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (618, 1, 6, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (619, 1, 7, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (620, 1, 8, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (621, 1, 9, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (622, 1, 10, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (623, 1, 11, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (624, 1, 12, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (625, 1, 13, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (626, 1, 14, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (627, 1, 15, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (628, 1, 16, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (629, 1, 17, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (630, 1, 18, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (631, 1, 19, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (632, 1, 20, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (633, 1, 21, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (634, 1, 22, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (635, 1, 23, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (636, 1, 24, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (637, 1, 25, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (638, 1, 26, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (639, 1, 27, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (640, 1, 28, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (641, 1, 29, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (642, 1, 30, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (643, 1, 31, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (644, 1, 32, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (645, 1, 33, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (646, 1, 34, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (647, 1, 35, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (648, 1, 36, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (649, 1, 37, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (650, 1, 38, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (651, 1, 39, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (652, 1, 40, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (653, 1, 41, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (654, 1, 42, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (655, 1, 43, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (656, 1, 44, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (657, 1, 45, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (658, 1, 46, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (659, 1, 47, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (660, 1, 48, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (661, 1, 49, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (662, 1, 50, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (663, 1, 51, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (664, 1, 52, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (665, 1, 53, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (666, 1, 54, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (667, 1, 55, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (668, 1, 56, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (669, 1, 57, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (670, 1, 58, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (671, 1, 59, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (672, 1, 60, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (673, 1, 61, now(), 'root', null);

insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (674, 1, 62, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (675, 1, 63, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (676, 1, 64, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (677, 1, 65, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (678, 1, 66, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (679, 1, 67, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (680, 1, 68, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (681, 1, 69, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (682, 1, 70, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (683, 1, 71, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (684, 1, 72, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (685, 1, 73, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (686, 1, 74, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (687, 1, 75, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (688, 1, 76, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (689, 1, 77, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (690, 1, 78, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (691, 1, 79, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (692, 1, 80, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (693, 1, 81, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (694, 1, 82, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (695, 1, 83, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (696, 1, 84, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (697, 1, 85, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (698, 1, 86, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (699, 1, 87, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (700, 1, 88, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (701, 1, 89, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (702, 1, 90, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (703, 1, 91, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (704, 1, 92, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (705, 1, 93, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (706, 1, 94, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (707, 1, 95, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (708, 1, 96, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (709, 1, 97, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (710, 1, 98, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (711, 1, 99, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (712, 1, 100, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (713, 1, 101, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (714, 1, 102, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (715, 1, 103, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (716, 1, 104, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (717, 1, 105, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (718, 1, 106, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (826, 1, 107, now(), 'root', null);

insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (719, 2, 1, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (720, 2, 2, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (721, 2, 3, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (722, 2, 4, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (723, 2, 5, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (724, 2, 6, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (725, 2, 7, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (726, 2, 8, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (727, 2, 9, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (728, 2, 10, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (729, 2, 11, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (730, 2, 12, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (731, 2, 13, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (732, 2, 14, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (733, 2, 15, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (734, 2, 16, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (735, 2, 17, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (736, 2, 18, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (737, 2, 19, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (738, 2, 20, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (739, 2, 21, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (740, 2, 22, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (741, 2, 23, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (742, 2, 24, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (743, 2, 25, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (744, 2, 26, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (745, 2, 27, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (746, 2, 28, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (747, 2, 29, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (748, 2, 30, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (749, 2, 31, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (750, 2, 32, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (751, 2, 33, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (752, 2, 34, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (753, 2, 35, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (754, 2, 36, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (755, 2, 37, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (756, 2, 38, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (757, 2, 39, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (758, 2, 40, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (759, 2, 41, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (760, 2, 42, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (761, 2, 43, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (762, 2, 44, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (763, 2, 45, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (764, 2, 46, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (765, 2, 47, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (766, 2, 48, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (767, 2, 49, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (768, 2, 50, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (769, 2, 51, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (770, 2, 52, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (771, 2, 53, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (772, 2, 54, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (773, 2, 55, now(), 'root', null);

insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (774, 2, 56, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (775, 2, 57, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (776, 2, 58, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (777, 2, 59, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (778, 2, 60, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (779, 2, 61, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (780, 2, 62, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (781, 2, 63, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (782, 2, 64, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (783, 2, 65, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (784, 2, 66, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (785, 2, 67, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (786, 2, 68, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (787, 2, 69, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (788, 2, 70, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (789, 2, 71, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (790, 2, 72, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (791, 2, 73, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (792, 2, 74, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (793, 2, 75, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (794, 2, 76, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (795, 2, 77, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (796, 2, 78, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (797, 2, 79, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (798, 2, 80, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (799, 2, 81, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (800, 2, 82, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (801, 2, 83, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (802, 2, 84, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (803, 2, 85, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (804, 2, 86, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (805, 2, 87, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (806, 2, 88, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (807, 2, 89, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (808, 2, 90, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (809, 2, 91, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (810, 2, 92, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (811, 2, 93, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (812, 2, 94, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (813, 2, 95, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (814, 2, 96, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (815, 2, 97, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (816, 2, 98, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (817, 2, 99, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (818, 2, 100, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (819, 2, 101, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (820, 2, 102, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (821, 2, 103, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (822, 2, 104, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (823, 2, 105, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (824, 2, 106, now(), 'root', null);
insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
values (825, 2, 107, now(), 'root', null);

insert into mt_policy_template (mt_policy_id,mt_policy_name,monitor_type,alarm_type,min_similarity,max_return,scene_saved,min_pixel,max_face_num,best_face,zoom_times,time_schema_id,face_storage_day)
values (1,'默认',1,'001',80,10,2,4096,12,0,1,0,30);

insert into time_schema (time_schema_id,time_schema_name,day,month,week_day,time_start,time_end,state)
values (1,'全时段',-1,-1,0,0,240000,1);
insert into time_schema (time_schema_id,time_schema_name,day,month,week_day,time_start,time_end,state)
values (1,'全时段',-1,-1,1,0,240000,1);
insert into time_schema (time_schema_id,time_schema_name,day,month,week_day,time_start,time_end,state)
values (1,'全时段',-1,-1,2,0,240000,1);
insert into time_schema (time_schema_id,time_schema_name,day,month,week_day,time_start,time_end,state)
values (1,'全时段',-1,-1,3,0,240000,1);
insert into time_schema (time_schema_id,time_schema_name,day,month,week_day,time_start,time_end,state)
values (1,'全时段',-1,-1,4,0,240000,1);
insert into time_schema (time_schema_id,time_schema_name,day,month,week_day,time_start,time_end,state)
values (1,'全时段',-1,-1,5,0,240000,1);
insert into time_schema (time_schema_id,time_schema_name,day,month,week_day,time_start,time_end,state)
values (1,'全时段',-1,-1,6,0,240000,1);

insert into time_schema (time_schema_id,time_schema_name,day,month,week_day,time_start,time_end,state)
values (2,'工作日',-1,-1,1,0,240000,1);
insert into time_schema (time_schema_id,time_schema_name,day,month,week_day,time_start,time_end,state)
values (2,'工作日',-1,-1,2,0,240000,1);
insert into time_schema (time_schema_id,time_schema_name,day,month,week_day,time_start,time_end,state)
values (2,'工作日',-1,-1,3,0,240000,1);
insert into time_schema (time_schema_id,time_schema_name,day,month,week_day,time_start,time_end,state)
values (2,'工作日',-1,-1,4,0,240000,1);
insert into time_schema (time_schema_id,time_schema_name,day,month,week_day,time_start,time_end,state)
values (2,'工作日',-1,-1,5,0,240000,1);

insert into time_schema (time_schema_id,time_schema_name,day,month,week_day,time_start,time_end,state)
values (3,'非工作日',-1,-1,0,0,240000,1);
insert into time_schema (time_schema_id,time_schema_name,day,month,week_day,time_start,time_end,state)
values (3,'非工作日',-1,-1,6,0,240000,1);

insert into time_schema (time_schema_id,time_schema_name,day,month,week_day,time_start,time_end,state)
values (4,'夜间',-1,-1,0,0,80000,1);
insert into time_schema (time_schema_id,time_schema_name,day,month,week_day,time_start,time_end,state)
values (4,'夜间',-1,-1,0,200000,240000,1);
insert into time_schema (time_schema_id,time_schema_name,day,month,week_day,time_start,time_end,state)
values (4,'夜间',-1,-1,1,0,80000,1);
insert into time_schema (time_schema_id,time_schema_name,day,month,week_day,time_start,time_end,state)
values (4,'夜间',-1,-1,1,200000,240000,1);
insert into time_schema (time_schema_id,time_schema_name,day,month,week_day,time_start,time_end,state)
values (4,'夜间',-1,-1,2,0,80000,1);
insert into time_schema (time_schema_id,time_schema_name,day,month,week_day,time_start,time_end,state)
values (4,'夜间',-1,-1,2,200000,240000,1);
insert into time_schema (time_schema_id,time_schema_name,day,month,week_day,time_start,time_end,state)
values (4,'夜间',-1,-1,3,0,80000,1);
insert into time_schema (time_schema_id,time_schema_name,day,month,week_day,time_start,time_end,state)
values (4,'夜间',-1,-1,3,200000,240000,1);
insert into time_schema (time_schema_id,time_schema_name,day,month,week_day,time_start,time_end,state)
values (4,'夜间',-1,-1,4,0,80000,1);
insert into time_schema (time_schema_id,time_schema_name,day,month,week_day,time_start,time_end,state)
values (4,'夜间',-1,-1,4,200000,240000,1);
insert into time_schema (time_schema_id,time_schema_name,day,month,week_day,time_start,time_end,state)
values (4,'夜间',-1,-1,5,0,80000,1);
insert into time_schema (time_schema_id,time_schema_name,day,month,week_day,time_start,time_end,state)
values (4,'夜间',-1,-1,5,200000,240000,1);
insert into time_schema (time_schema_id,time_schema_name,day,month,week_day,time_start,time_end,state)
values (4,'夜间',-1,-1,6,0,80000,1);
insert into time_schema (time_schema_id,time_schema_name,day,month,week_day,time_start,time_end,state)
values (4,'夜间',-1,-1,6,200000,240000,1);

insert into time_schema (time_schema_id,time_schema_name,day,month,week_day,time_start,time_end,state)
values (5,'白天',-1,-1,0,60000,240000,1);
insert into time_schema (time_schema_id,time_schema_name,day,month,week_day,time_start,time_end,state)
values (5,'白天',-1,-1,1,60000,240000,1);
insert into time_schema (time_schema_id,time_schema_name,day,month,week_day,time_start,time_end,state)
values (5,'白天',-1,-1,2,60000,240000,1);
insert into time_schema (time_schema_id,time_schema_name,day,month,week_day,time_start,time_end,state)
values (5,'白天',-1,-1,3,60000,240000,1);
insert into time_schema (time_schema_id,time_schema_name,day,month,week_day,time_start,time_end,state)
values (5,'白天',-1,-1,4,60000,240000,1);
insert into time_schema (time_schema_id,time_schema_name,day,month,week_day,time_start,time_end,state)
values (5,'白天',-1,-1,5,60000,240000,1);
insert into time_schema (time_schema_id,time_schema_name,day,month,week_day,time_start,time_end,state)
values (5,'白天',-1,-1,6,60000,240000,1);
commit;

