
create table face_db
(
    dbid        int          	primary key, 
    dbname      varchar(32)   	not null, 
    crt_date    datetime, 
    crt_user    varchar(32), 
    sim         double          default -1, 
    state       int          	default 0, 
    personnum   bigint		default 0,
    facenum	bigint		default 0,
    node_inst_num int 		default 0
) engine=MyISAM;

create table reco_sequence (name varchar(32) not null,current_value bigint not null,inc int not null default 1,primary key(name)) engine=MyISAM;
insert into reco_sequence values('dbid',0,1);
insert into reco_sequence values('personid',0,1);

delimiter //

create function reco_currval(n varchar(32)) returns bigint
begin
declare _cur bigint;
set _cur=(select current_value from reco_sequence where name = n);
return _cur;
end;
//

create function reco_nextval(n varchar(32)) returns bigint
begin
update reco_sequence set current_value = current_value + inc where name = n;
return reco_currval(n);
end;
//

delimiter ;


create table dbid_recycle
(
    dbid        int		primary key
);

create table facedb_card_type
(
    card_type_id bigint		primary key,
    type_name   varchar(32),
    crt_date    datetime,
    crt_user    varchar(32),
    remark      varchar(255)
);

create table face_cloud_conf
(
    conf_id     int		not null,
    conf_name   varchar(64)	not null,
    db_id       int		not null,
    mq_addr     varchar(64),
    chunk_max_face bigint,
    feature_size int
);

create table chunk_info
(
    chunk_id    bigint		not null,
    chunk_inst_id int		not null,
    host_id     int		not null,
    process_id  int		default 0,
    chunk_state int		default 0,
    db_id       int		not null,
    last_update_time datetime,
    face_count  int		not null
);

create unique index idx_chunk_id on chunk_info(chunk_id,chunk_inst_id);

create table face_id_recycle
(
    face_seq_id bigint		not null,
    db_id       int		not null
);
create index idx_faceid_recy_dbid on face_id_recycle(db_id);

create table work_ctrl
(
    work_serial bigint		primary key,
    action      int		not null,
    part_month  int		not null
) engine=MyISAM;

insert into reco_sequence values('ws',0,1);

create table work_chunk
(
    work_serial bigint		not null,
    chunk_id    bigint		not null,
    chunk_inst_id int		not null,
    deal_state  int		default 0,
    part_month  int		not null
) engine=MyISAM;
create index idx_workchunk_ws on work_chunk(work_serial);
create index idx_workchunk_ds on work_chunk(deal_state);

create table inc_face
(
    work_serial bigint		not null,
    dbid        int		default -1,
    faceid      bigint		default -1,
    personid    bigint		default -1,
    birth       int		default -1,
    sex         int		default -1,
    card_type_id int		default 0,
    address     varchar(255),
    native_place varchar(255),
    phone       varchar(20),
    name        varchar(128),
    id_card     varchar(20),
    part_month  int 
);
create index idx_incface_ws on inc_face(work_serial);

create table host_info
(
    host_id     int		primary key,
    host_name   varchar(32)	not null,
    ip          varchar(32)	not null,
    login_name  varchar(32)	not null,
    login_pwd   varchar(32)	not null,
    use_type    bigint		not null,
    cpu_count   int		not null,
    memory      int		not null,
    disk_size   int		not null,
    max_connect int		not null,
    state       int		not null
);

create table process_info
(
    process_id  int		primary key,
    process_name varchar(32)	not null,
    host_id     int		not null,
    port        int		not null,
    state       int		default 1
);

create table dup_info
(
    dup_id      int		primary key, 
    dupname     varchar(255),
    address     varchar(255),
    sim         double		default 0,
    runbeg      int		default 0,
    runend      int		default 0,
    agebeg      int		default 0,
    ageend      int		default 0,
    gender      int		default 0,
    status      int		default 0,
    progress    double		default 0,
    finishtime  int		default 0,
    exectime    int		default 0,
    ageswitch   int		default 0,
    sexswitch   int		default 0,
    delsame     int		default 0,
    priority    int		default 0,
	dup_type	int		default 0,
	rt_beg		datetime,
	rt_end		datetime,
	alarm_intval	int,
	snapshot_beg	int,
	snapshot_end	int
) engine=MyISAM;

create table dup_rt_camera_file
(
	dup_id		int,
	camera_id	int,
	file_id		varchar(20)
) engine=MyISAM;

insert into reco_sequence values('dupid',0,1);
insert into reco_sequence values('dupresid',0,1);

create table dup_srcid
(
    dup_id      int,
    src_db_id   int,
    execcount   int		default 0
) engine=MyISAM;
create index idx_dupid_srcid on dup_srcid(dup_id);

create table dup_dstid
(
    dup_id      int,
    dst_db_id   int
) engine=MyISAM;
create index idx_dupid_dstid on dup_dstid(dup_id);

create table dup_res
(
    dup_res_id  int		primary key, 
    dup_id      int,
    src_person_id bigint,
    src_face_id bigint,
    dst_person_id bigint,
    dst_face_id bigint,
    sim         double,
    src_db_id   int,
    dst_db_id   int,
    verify	int		default 0,
	dst_res_num	int	default 0,
	dst_res_seq	int	default 0
) engine=MyISAM;
create index idx_dupid_res on dup_res(dup_id);

create table dup_res_rt
(
	dup_res_id	int		primary key,
	dup_id		int,
	src_db_id	int,
	src_person_id	bigint,
	src_face_id	bigint,
	sim			double,
	verify		int		default 0,
	host_id		int,
	camera_id	int,
	file_id		varchar(20),
	image_id	varchar(255),
	face_id		varchar(255),
	alarm_time	datetime,
	snapshot_num	int,
	snapshot_seq          int
);
create index idx_dupid_res_rt on dup_res_rt(dup_id);

-- delimiter //
-- create trigger tr_delete_dup_id
--     before delete on dup_info
--     for each row
-- begin
--     delete from dup_res where dup_id=old.dup_id;
--     delete from dup_srcid where dup_id=old.dup_id;
--     delete from dup_dstid where dup_id=old.dup_id;
-- 	delete from dup_rt_camera_file where dup_id=old.dup_id;
-- 	delete from dup_res_rt where dup_id=old.dup_id;
-- end;
-- //
-- delimiter ;

create table sys_parameter
(
  param_name  varchar(32) not null,
  param_value varchar(32) not null,
  module_name varchar(32) not null,
  state       int(2) default 1 not null
);

create unique index sys_parameter_index1 on sys_parameter (param_name);

create table camera_info
(
  camera_id    int(6) not null,
  camera_name  varchar(32) not null,
  rtsp         varchar(256) not null,
  fixed_host   int(4) not null,
  mt_policy_id int(8) not null,
  state        int(2) default 1 not null
);

create unique index camera_info_index1 on camera_info (camera_id);
create unique index camera_info_index2 on camera_info (camera_name);
-- create unique index camera_info_index3 on camera_info (rtsp(40));

-- Create table
create table MT_POLICY_TEMPLATE
(
  mt_policy_id     int(8) not null,
  mt_policy_name   varchar(64) not null,
  monitor_type     int(2) not null,
  alarm_type       varchar(3) not null,
  min_similarity   double not null,
  max_return       int(4) not null,
  scene_saved      int(2) not null,
  min_pixel        int(9) not null,
  max_face_num     int(2) not null,
  best_face        int(1) not null,
  zoom_times       double not null,
  time_schema_id   int(6) not null,
  face_storage_day int(6) not null,
  speed_mode       int(4) default 0,
  face_adjust      int(4) default 0,
  min_point		   int(8) default 0,
  skip_dup         int(2) default 0,
  skip_time		   int(8) default 0,
  snap_sense	   int(2) default 3
);
create unique index mt_policy_template_index1 on MT_POLICY_TEMPLATE (mt_policy_id);	

create table CAMERA_RUN_CTRL
(
  camera_id      int(6) primary key,
  camera_state   int(2) not null,
  belong_host    int(4) not null,
  host_state     int(2) not null,
  man_init_state int(2) not null,
  run_state      int(2) default 1 not null,
  last_clear_time      datetime ,
  alarm_count          int(12) default 0,
  snap_count           int(12) default 0
);

-- create unique index CAMERA_ID on CAMERA_RUN_CTRL (CAMERA_ID);

create table TIME_SCHEMA
(
  time_schema_id   int(6) not null,
  time_schema_name varchar(16) not null,
  day              int(2) not null,
  month            int(2) not null,
  week_day         int(2) not null,
  time_start       int(6) not null,
  time_end         int(6) not null,
  state            int(2) default 1 not null
) engine=MyISAM;

create index TIME_SCHEMA_INDEX1 on TIME_SCHEMA (TIME_SCHEMA_ID);

create table CAMERA_SEARCH_DB
(
  camera_id int(4) not null,
  db_id     int(4) not null,
  state     int(2) default 1 not null
);

create index CAMERA_SEARCH_DB_INDEX1 on CAMERA_SEARCH_DB (CAMERA_ID);
create index CAMERA_SEARCH_DB_INDEX2 on CAMERA_SEARCH_DB (DB_ID);

insert into reco_sequence values('camera_id_seq',9,1);
insert into reco_sequence values('time_schema_id_seq',9,1);
insert into reco_sequence values('mt_policy_id_seq',9,1);

create table CAMERA_ROI
(
  camera_id      int(6) not null,
  roi_left       int not null,
  roi_top        int not null,
  roi_right      int not null,
  roi_bottom     int not null,
  state          int(2) default 1 not null
);

create unique index CAMERA_ROI_INDEX1 on CAMERA_ROI (CAMERA_ID);

create table Mt_face_set
(
  Face_set_id bigint not null,
  Host_id     int not null,
  Camera_id   int not null,
  Start_time  datetime not null,
  End_time    datetime not null
);

create unique index IDX_Face_set_id on Mt_face_set (Face_set_id);

create table Mt_camera_max_id
(
  Camera_id   int not null,
  Face_set_id bigint not null
);

create unique index IDX_mt_Face_set_id on Mt_camera_max_id (Face_set_id);
create unique index IDX_Camera_id on Mt_camera_max_id (Camera_id);

insert into reco_sequence values('face_set_id_seq',9,1);
insert into reco_sequence values('node_id_seq',9,1);

create table tree_node_def
(
  Node_id   int(9) not null,
  Node_Name varchar(32) not null,
  Parent_node_id   int(9) not null,
  state       int(2) default 1 not null
);

create unique index tree_node_def_INDEX1 on tree_node_def (Node_id);
create index tree_node_def_INDEX2 on tree_node_def (Parent_node_id);

create table tree_node_camera
(
  Node_id     int(9) not null,
  Camera_id   int(6) not null,
  state       int(2) default 1 not null
);

create index tree_node_camera_INDEX1 on tree_node_camera (Node_id);
create index tree_node_camera_INDEX2 on tree_node_camera (Camera_id);

create table HOST_IO
(
  host_id     int not null,
  io_capacity int not null,
  state       int not null
);

create unique index HOST_IO_INDEX1 on HOST_IO (HOST_ID);

create table HOST_DISK
(
  host_id     int(4) not null,
  disk_id     int(5) not null,
  disk_name   varchar(64) not null,
  dir         varchar(64) not null,
  io_capacity int(9),
  state       int(2) default 1 not null
);

create table CAMERA_TIME_PLAN
(
  time_schema_id int(6) not null,
  camera_id      int(6) not null,
  exe_start      datetime not null,
  exe_end        datetime not null,
  state          int(2) default 1 not null
) engine=MyISAM;

create index CAMERA_TIME_PLAN_INDEX1 on CAMERA_TIME_PLAN (TIME_SCHEMA_ID);
create unique index CAMERA_TIME_PLAN_INDEX2 on CAMERA_TIME_PLAN (CAMERA_ID,TIME_SCHEMA_ID);

create table VIDEO_FILE
(
  file_id     varchar(20) not null,
  camera_id   int(6) not null,
  file_name   varchar(256) not null,
  play_offset int,
  play_time   datetime,
  state       int not null,
  exception_times int default 0 not null,
  progress    int default 0 not null
);

create unique index VIDEO_FILE_INDEX2 on VIDEO_FILE (CAMERA_ID,FILE_NAME(60));
create index VIDEO_FILE_INDEX3 on VIDEO_FILE (CAMERA_ID);

alter table VIDEO_FILE
  add constraint PK_VIDEO_FILE primary key (FILE_ID);

create table VIDEO_FILE_SLIP
(
  camera_id   int(6) not null,
  slip_index  int,
  file_name   varchar(256) not null,
  start_time  int,
  end_time    int,
  state       int not null,
  play_offset int,
  exception_times int default 0 not null,
  progress    int default 0 not null
);

create unique index VIDEO_FILE_SLIP_INDEX2 on VIDEO_FILE_SLIP(CAMERA_ID,FILE_NAME(60),SLIP_INDEX);
create index VIDEO_FILE_SLIP_INDEX3 on VIDEO_FILE_SLIP(CAMERA_ID);
  
create table PROCESS_LOCK
(
  process_name varchar(64) not null,
  reply_queue  varchar(64) not null,
  lock_name    varchar(64) not null,
  is_owner     int default 0 not null
);
create unique index PROCESS_LOCK_INDEX1 on PROCESS_LOCK (PROCESS_NAME);
create index PROCESS_LOCK_INDEX2 on PROCESS_LOCK (LOCK_NAME);  


create table sys_system_runtime_log
(
	log_id				bigint	auto_increment primary key,
	log_type			int,
	host_id				int,
	host_name			varchar(255),
	process_id		int,
	process_name  varchar(255),
  process_no    int,
  thread_no     int,
  module_name   varchar(32),
  log_level     int,
  description   varchar(255),
  crt_date      datetime
);

create table sys_user_busi_op_log
(
  log_id       bigint auto_increment primary key,
  user_id      int,  
  busi_name    varchar(64),
  op_time      datetime,
  description   varchar(255),
  user_name     varchar(32),
  host_ip     varchar(32)
);

create table sys_pri_role_service
(
  pri_id     int primary key,
  role_id    int,
  service_id int,
  crt_date   datetime,
  crt_user   varchar(32),
  remark     varchar(255)
);

create table sys_role_info
(
  role_id   int primary key,
  role_name varchar(32),
  crt_date  datetime,
  crt_user  varchar(32),
  remark    varchar(128)
);

create table sys_service_info
(
  service_id      int primary key,
  type_id         int,
  service_name    varchar(64),
  service_address varchar(128),
  function        varchar(32),
  description     varchar(255),
  login_require   int default 1,
  crt_date        datetime,
  crt_user        varchar(32)
);

create table sys_service_type_info
(
  type_id   int primary key,
  type_name varchar(64),
  remark    varchar(255)
);

create table sys_user_info
(
  user_id     int auto_increment primary key,
  role_id     int,
  login       varchar(32),
  password    varchar(32) default '123456',
  name        varchar(32),
  sex         int,
  employee_no varchar(32),
  crt_date    datetime,
  crt_user    varchar(32),
  remark      varchar(255)
);
