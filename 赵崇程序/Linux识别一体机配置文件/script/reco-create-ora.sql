/*
 * copyright 2012 reconova tech co., ltd. all rights reserved.
 *
 * created by cyberman wo on dec 13th, 2012.
 */

-- 用于初始化

set serveroutput on

create table face_db
(
    dbid        number          primary key, 
    dbname      varchar2(32)   not null, 
    crt_date    date, 
    crt_user    varchar2(32), 
    sim         number          default -1, 
    state       number          default 0, 
    personnum   number          default 0,
    facenum	number		default 0,
    node_inst_num number        default 0
) nologging;

create sequence seq_dbid minvalue 0 maxvalue 9999 start with 0;
create table dbid_recycle
(
    dbid        number		primary key
);

create sequence seq_personid minvalue 1 start with 1 cache 300;

create table facedb_card_type
(
    card_type_id number(4)      primary key,
    type_name   varchar2(32),
    crt_date    date,
    crt_user    varchar2(32),
    remark      varchar2(255)
);

create table face_cloud_conf
(
    conf_id     number            not null,
    conf_name   varchar2(64)      not null,
    db_id       number            not null,
    mq_addr     varchar2(64),
    chunk_max_face number,
    feature_size number
);

create table chunk_info
(
    chunk_id    number           not null,
    chunk_inst_id number         not null,
    host_id     number           not null,
    process_id  number           default 0,
    chunk_state number           default 0,
    db_id       number           not null,
    last_update_time date,
    face_count  number           not null
);
create unique index idx_chunk_id on chunk_info(chunk_id,chunk_inst_id);

create table face_id_seq
(
    db_id       number          primary key,
    cur_face_seq number         default 0
) nologging;

create table face_id_recycle
(
    face_seq_id number          not null,
    db_id       number          not null
) nologging;
create index idx_faceid_recy_dbid on face_id_recycle(db_id);

create table work_ctrl
(
    work_serial number          primary key,
    action      number          not null,
    part_month  number          not null
) nologging;

create sequence seq_ws cache 100;

create table work_chunk
(
    work_serial number		not null,
    chunk_id    number		not null,
    chunk_inst_id number        not null,
    deal_state  number		default 0,
    part_month  number		not null
);
create index idx_workchunk_ws on work_chunk(work_serial);
create index idx_workchunk_ds on work_chunk(deal_state);

create table inc_face
(
    work_serial number          not null,
    dbid        number          default -1,
    faceid      number          default -1,
    personid    number          default -1,
    birth       number          default -1,
    sex         number          default -1,
    card_type_id number(4)	default 0,
    address     varchar2(255),
    native_place varchar2(255),
    phone       varchar2(20),
    name        varchar2(128),
    id_card     varchar2(20),
    part_month  number
) nologging;
create index idx_incface_ws on inc_face(work_serial);

create table host_info
(
    host_id     number          primary key,
    host_name   varchar2(32)    not null,
    ip          varchar2(32)    not null,
    login_name  varchar2(32)    not null,
    login_pwd   varchar2(32)    not null,
    use_type    number          not null,
    cpu_count   number          not null,
    memory      number          not null,
    disk_size   number          not null,
    max_connect number          not null,
    state       number          not null
);

create table process_info
(
    process_id  number          primary key,
    process_name varchar2(32)   not null,
    host_id     number          not null,
    port        number          not null,
    state       number          default 1
);

create table dup_info
(
    dup_id      number          primary key, 
    dupname     varchar2(255),
    address     varchar2(255),
    sim         number          default 0,
    runbeg      number          default 0,
    runend      number          default 0,
    agebeg      number          default 0,
    ageend      number          default 0,
    gender      number          default 0,
    status      number          default 0,
    progress    number          default 0,
    finishtime  number          default 0,
    exectime    number          default 0,
    ageswitch   number          default 0,
    sexswitch   number          default 0,
    delsame     number			default 0,
    priority    number          default 0,
	dup_type	number			default 0,
	rt_beg		date,
	rt_end		date,
	alarm_intval	number,
	snapshot_beg	number,
	snapshot_end	number
);

create table dup_rt_camera_file
(
	dup_id		number,
	camera_id	number,
	file_id		varchar2(20)
);

create sequence seq_dupid cache 10;

create table dup_srcid
(
    dup_id      number,
    src_db_id   number,
    execcount   number           default 0
);
create index idx_dupid_srcid on dup_srcid(dup_id);

create table dup_dstid
(
    dup_id      number,
    dst_db_id   number
);
create index idx_dupid_dstid on dup_dstid(dup_id);

create table dup_res
(
    dup_res_id  number          primary key, 
    dup_id      number,
    src_person_id number,
    src_face_id number,
    dst_person_id number,
    dst_face_id number,
    sim         number,
    src_db_id   number,
    dst_db_id   number,
    verify	number		default 0,
	dst_res_num	number	default 0,
	dst_res_seq	number	default 0
);
create index idx_dupid_res on dup_res(dup_id);

create table dup_res_rt
(
	dup_res_id	number			primary key,
	dup_id		number,
	src_db_id	number,
	src_person_id	number,
	src_face_id	number,
	sim			number,
	verify		number		default 0,
	host_id		number,
	camera_id	number,
	file_id		varchar2(20),
	image_id	varchar2(255),
	face_id		varchar2(255),
	alarm_time	date,
	snapshot_num	number,
	snapshot_seq	number
);
create index idx_dupid_res_rt on dup_res_rt(dup_id);

create sequence seq_dupresid cache 100;

-- create or replace trigger tr_delete_dup_id
--     before delete on dup_info
--     for each row
-- begin
--     delete from dup_res where dup_id=:old.dup_id;
--     delete from dup_srcid where dup_id=:old.dup_id;
--     delete from dup_dstid where dup_id=:old.dup_id;
-- 	delete from dup_rt_camera_file where dup_id=:old.dup_id;
-- 	delete from dup_res_rt where dup_id=:old.dup_id;
-- end;
-- /

-- create or replace trigger tr_delete_faceid
--     before delete on face
--     for each row
-- begin
--     delete from dup_res where src_face_id=:old.faceid or dst_face_id=:old.faceid;
-- end;
-- /

-- create or replace trigger tr__pid_dbid__dbid
--     after insert or delete 
--     on pid_dbid 
--     for each row 
-- begin
--     if inserting then
--         update face_db set personnum=personnum+1 where dbid=:new.dbid;
--     else
--         update face_db set personnum=personnum-1 where dbid=:old.dbid;
--     end if;
-- end;
-- /
-- 
-- create or replace trigger tr__face_facedb
--     after insert or delete
--     on face
--     for each row
-- begin
--     if inserting then
--         update face_db set facenum=facenum+1 where dbid=:new.db_id;
--     else
--         update face_db set facenum=facenum-1 where dbid=:old.db_id;
--     end if;
-- end;
-- /


-- create or replace trigger tr__face__personid
--     after insert or delete 
--     on face 
--     for each row 
-- begin
--     if inserting then
--         update person_info set facenum=facenum+1 where personid=:new.personid;
--     else
--         update person_info set facenum=facenum-1 where personid=:old.personid;
--     end if;
-- end;
-- /

-- create table
create table sys_parameter
(
  param_name  varchar2(32) not null,
  param_value varchar2(32) not null,
  module_name varchar2(32) not null,
  state       number(2) default 1 not null
);
-- create/recreate indexes 
create unique index sys_parameter_index1 on sys_parameter (param_name);


-- create table
create table CAMERA_INFO
(
  camera_id    NUMBER(6) not null,
  camera_name  VARCHAR2(32) not null,
  rtsp         VARCHAR2(256) not null,
  fixed_host   NUMBER(8) not null,
  mt_policy_id NUMBER(8) not null,
  state        NUMBER(2) default 1 not null
);
-- create/recreate indexes 
create unique index camera_info_index1 on camera_info (camera_id);
create unique index camera_info_index2 on camera_info (camera_name);
-- create unique index camera_info_index3 on camera_info (rtsp);


-- Create table
create table MT_POLICY_TEMPLATE
(
  mt_policy_id     NUMBER(8) not null,
  mt_policy_name   VARCHAR2(64) not null,
  monitor_type     NUMBER(2) not null,
  alarm_type       VARCHAR2(3) not null,
  min_similarity   NUMBER(3,1) not null,
  max_return       NUMBER(4) not null,
  scene_saved      NUMBER(2) not null,
  min_pixel        NUMBER(9) not null,
  max_face_num     NUMBER(2) not null,
  best_face        NUMBER(1) not null,
  zoom_times       NUMBER(3,2) not null,
  time_schema_id   NUMBER(6) not null,
  face_storage_day NUMBER(6) not null,
  speed_mode       NUMBER(4) default 0,
  face_adjust      NUMBER(4) default 0,
  min_point		   NUMBER(8) default 0,
  skip_dup         NUMBER(2) default 0,
  skip_time		   NUMBER(8) default 0,
  snap_sense	   NUMBER(2) default 3
);
-- Create/Recreate primary, unique and foreign key constraints 
alter table MT_POLICY_TEMPLATE 
	add constraint PK_MT_POLICY_TEMPLATE primary key (MT_POLICY_ID);

-- create table
create table camera_run_ctrl
(
  camera_id      number(6) not null,
  camera_state   number(2) not null,
  belong_host    number(4) not null,
  host_state     number(2) not null,
  man_init_state number(2) not null,
  run_state      number(2) default 1 not null,
  last_clear_time      date default sysdate,
  alarm_count          NUMBER(12) default 0,
  snap_count           NUMBER(12) default 0
);

-- create/recreate indexes 
create unique index camera_id on camera_run_ctrl (camera_id);


-- create table
create table time_schema
(
  time_schema_id   number(6) not null,
  time_schema_name varchar2(16) not null,
  day              number(2) not null,
  month            number(2) not null,
  week_day         number(2) not null,
  time_start       number(6) not null,
  time_end         number(6) not null,
  state            number(2) default 1 not null
);
-- create/recreate indexes 
create index time_schema_index1 on time_schema (time_schema_id);


-- create table
create table camera_search_db
(
  camera_id number(4) not null,
  db_id     number(4) not null,
  state     number(2) default 1 not null
);
-- create/recreate indexes 
create index camera_search_db_index1 on camera_search_db (camera_id);
create index camera_search_db_index2 on camera_search_db (db_id);

-- create sequence 
create sequence camera_id_seq
minvalue 1
maxvalue 999999999
start with 10
increment by 1
cache 20;

-- create sequence 
create sequence time_schema_id_seq
minvalue 1
maxvalue 999999999
start with 10
increment by 1
cache 20;

-- Create sequence 
create sequence MT_POLICY_ID_SEQ
minvalue 1
maxvalue 999999999
start with 30
increment by 1
cache 10;

-- create table
create table camera_roi
(
  camera_id      number(6) not null,
  roi_left       number not null,
  roi_top        number not null,
  roi_right      number not null,
  roi_bottom     number not null,
  state          number(2) default 1 not null
);
-- create/recreate indexes 
create unique index camera_roi_index1 on camera_roi (camera_id);

-- create table
create table mt_face_set
(
  face_set_id number not null,
  host_id     number not null,
  camera_id   number not null,
  start_time  date not null,
  end_time    date not null
);
-- create/recreate indexes 
create unique index idx_face_set_id on mt_face_set (face_set_id);

-- create table
create table mt_camera_max_id
(
  camera_id   number not null,
  face_set_id number not null
);
-- create/recreate indexes 
create unique index idx_mt_face_set_id on mt_camera_max_id (face_set_id);
create unique index idx_camera_id on mt_camera_max_id (camera_id);

create sequence face_set_id_seq
minvalue 1
maxvalue 999999999999
start with 10
increment by 1
cache 100;

-- create sequence 
create sequence node_id_seq
minvalue 1
maxvalue 999999999
start with 10
increment by 1
cache 100;

-- create table
create table tree_node_def
(
  node_id   number(9) not null,
  node_name varchar2(32) not null,
  parent_node_id   number(9) not null,
  state       number(2) default 1 not null
);
-- create/recreate indexes 
create unique index tree_node_def_index1 on tree_node_def (node_id);
create index tree_node_def_index2 on tree_node_def (parent_node_id);

-- create table
create table tree_node_camera
(
  node_id     number(9) not null,
  camera_id   number(6) not null,
  state       number(2) default 1 not null
);
-- create/recreate indexes 
create index tree_node_camera_index1 on tree_node_camera (node_id);
create index tree_node_camera_index2 on tree_node_camera (camera_id);

-- create table
create table host_io
(
  host_id     number not null,
  io_capacity number not null,
  state       number not null
);
-- create/recreate indexes 
create unique index host_io_index1 on host_io (host_id);

-- create table
create table host_disk
(
  host_id     number(4) not null,
  disk_id     number(5) not null,
  disk_name   varchar2(64) not null,
  dir         varchar2(64) not null,
  io_capacity number(9),
  state       number(2) default 1 not null
);

-- create table
create table camera_time_plan
(
  time_schema_id number(6) not null,
  camera_id      number(6) not null,
  exe_start      date not null,
  exe_end        date not null,
  state          number(2) default 1 not null
);
-- create/recreate indexes 
create index camera_time_plan_index1 on camera_time_plan (time_schema_id);
create unique index camera_time_plan_index2 on camera_time_plan (camera_id,time_schema_id);

-- create table
create table video_file
(
  file_id     varchar2(20) not null,
  camera_id   number(6) not null,
  file_name   varchar2(256) not null,
  play_offset number,
  play_time   date,
  state       integer not null,
  exception_times integer default 0 not null,
  progress  integer default 0 not null
);

-- create/recreate indexes 
create unique index video_file_index2 on video_file (camera_id,file_name);
create index video_file_index3 on video_file (camera_id);
-- create/recreate primary, unique and foreign key constraints 
alter table video_file
  add constraint pk_video_file primary key (file_id);


-- create table
create table video_file_slip
(
  camera_id   number(6) not null,
  slip_index  number(6) not null,
  file_name   varchar2(256) not null,
  start_time  number,
  end_time    number,
  state       integer not null,
  play_offset number,
  exception_times integer default 0 not null,
  progress    integer default 0 not null
);

-- create/recreate indexes 
create unique index video_file_slip_index2 on video_file_slip(camera_id,file_name,slip_index);
create index video_file_slip_index3 on video_file_slip(camera_id);

-- create table
create table sys_system_runtime_log
(
  log_id        number(14) not null,
  log_type      number(2),
  host_id       number,
  host_name     varchar2(255),
  process_id    number,
  process_name  varchar2(255),
  process_no    number(14),
  thread_no     number(14),
  module_name   varchar2(32),
  log_level     number,
  description   varchar2(255),
  crt_date      date
);
-- create/recreate indexes 
create unique index sys_system_runtime_log_index1 on sys_system_runtime_log (log_id);
create sequence seq_syslogid minvalue 0 start with 1 cache 20;

-- create table
create table sys_user_busi_op_log
(
  log_id        number(14) not null,
  user_id      number(8),  
  busi_name     varchar2(64),
  op_time    date,
  description   varchar2(255),
  user_name     varchar2(32),
  host_ip     varchar2(32)
);
-- create/recreate indexes 
create unique index sys_user_busi_op_log_index1 on sys_user_busi_op_log (log_id);
create sequence seq_clntlogid minvalue 0 start with 1 cache 20;

-- create table
create table sys_pri_role_service
(
  pri_id     number(8) not null,
  role_id    number(4),
  service_id number(8),
  crt_date   date,
  crt_user   varchar2(32),
  remark     varchar2(255)
);
-- create/recreate indexes
create unique index sys_pri_role_service_index1 on sys_pri_role_service (pri_id);

-- create table
create table sys_role_info
(
  role_id   number(8) not null,
  role_name varchar2(32),
  crt_date  date,
  crt_user  varchar2(32),
  remark    varchar2(128)
);
-- create/recreate indexes
create unique index sys_role_info_index1 on sys_role_info (role_id);

-- create table
create table sys_service_info
(
  service_id      number(8) not null,
  type_id         number(4),
  service_name    varchar2(64),
  service_address varchar2(128),
  function        varchar2(32),
  description     varchar2(255),
  login_require   number(1) default 1,
  crt_date        date,
  crt_user        varchar2(32)
);
-- create/recreate indexes
create unique index sys_service_info_index1 on sys_service_info (service_id);

-- create table
create table sys_service_type_info
(
  type_id   number(4) not null,
  type_name varchar2(64),
  remark    varchar2(255)
);
-- create/recreate indexes
create unique index sys_service_type_info_index1 on sys_service_type_info (type_id);

-- create table
create table sys_user_info
(
  user_id     number(8) not null,
  role_id     number(8),
  login       varchar2(32),
  password    varchar2(32) default '123456',
  name        varchar2(32),
  sex         number(1),
  employee_no varchar2(32),
  crt_date    date,
  crt_user    varchar2(32),
  remark      varchar2(255)
);
-- create/recreate indexes
create unique index sys_user_info_index1 on sys_user_info (user_id);
create sequence seq_auth_userid minvalue 0 start with 100 cache 20;

-- create table
create table process_lock
(
  process_name varchar2(64) not null,
  reply_queue  varchar2(64) not null,
  lock_name    varchar2(64) not null,
  is_owner     number(2) default 0 not null
);
-- create/recreate indexes 
create unique index process_lock_index1 on process_lock (process_name);
create index process_lock_index2 on process_lock (lock_name);

exit

