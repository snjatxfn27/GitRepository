#!/bin/bash

UPDATE_DIR=${FRAME_ROOT}/etc/script/update

function main()
{
	versionNumber=$1
	dbselect=`grep "using=" ${FRAME_ROOT}/etc/sys.conf|awk -F= '{print $2}'|awk '{print $1}'`

	if [ "${dbselect}" == "mysql" ];then
		dbuser=`grep "\[mysql\]" ${FRAME_ROOT}/etc/sys.conf |awk -F"DBUSER=" '{print $2}'|awk -F" " '{print $1}'`
		dbpwd=`grep "\[mysql\]" ${FRAME_ROOT}/etc/sys.conf |awk -F"DBPWD=" '{print $2}'|awk -F" " '{print $1}'`
		database=`grep "\[mysql\]" ${FRAME_ROOT}/etc/sys.conf |awk -F"DATABASE=" '{print $2}'|awk -F" " '{print $1}'`

		rm -rf ${UPDATE_DIR}/up_r6.sql
		touch ${UPDATE_DIR}/up_r6.sql
		if [ "$versionNumber" -le "4838" ];then
			echo "alter table face_db engine=MyISAM;" > $UPDATE_DIR/up_r6.sql
			echo "alter table reco_sequence engine=MyISAM;" >> $UPDATE_DIR/up_r6.sql
		fi

		if [ "$versionNumber" -le "5464" ];then
			echo "select dbid from face_db;" > $UPDATE_DIR/query_id_my.sql
			for dbid in `mysql -u$dbuser -p$dbpwd -D$database < $UPDATE_DIR/query_id_my.sql|grep -v dbid`;do
				echo "alter table face_$dbid add (pri int default 0);" >> $UPDATE_DIR/up_r6.sql
			done
		fi

		if [ "$versionNumber" -le "5476" ];then
			echo "alter table CAMERA_RUN_CTRL add (last_clear_time datetime,alarm_count int(12) default 0,snap_count int(12) default 0); " >> $UPDATE_DIR/up_r6.sql
		fi

		if [ "$versionNumber" -le "5973" ];then
			echo "alter table dup_info add (dup_type int default 0);" >> $UPDATE_DIR/up_r6.sql
			echo "alter table dup_info add (rt_beg datetime);" >> $UPDATE_DIR/up_r6.sql
			echo "alter table dup_info add (rt_end datetime);" >> $UPDATE_DIR/up_r6.sql
			echo "alter table dup_info add (alarm_intval int);" >> $UPDATE_DIR/up_r6.sql
			echo "alter table dup_info add (snapshot_beg int);" >> $UPDATE_DIR/up_r6.sql
			echo "alter table dup_info add (snapshot_end int);" >> $UPDATE_DIR/up_r6.sql

			echo "create table dup_rt_camera_file(
				dup_id		int,
				camera_id	int,
				file_id		varchar(20),
				foreign key(dup_id)
					references dup_info(dup_id)) engine=MyISAM;" >> $UPDATE_DIR/up_r6.sql

			echo "create table dup_res_rt(
				dup_res_id		int			primary key,
				dup_id			int,
				src_db_id		int,
				src_person_id	bigint,
				src_face_id		bigint,
				sim				double,
				verify			int			default 0,
				host_id			int,
				camera_id		int,
				file_id			varchar(20),
				image_id		varchar(255),
				face_id			varchar(255),
				alarm_time		datetime,
				snapshot_num	int,
				snapshot_seq	int);" >> $UPDATE_DIR/up_r6.sql
			echo "create index idx_dupid_res_rt on dup_res_rt(dup_id);" >> $UPDATE_DIR/up_r6.sql

			echo "drop trigger tr_delete_dup_id;" >> $UPDATE_DIR/up_r6.sql
			echo "delimiter //
				create trigger tr_delete_dup_id
				before delete on dup_info
				for each row
				begin
				delete from dup_res where dup_id=old.dup_id;
				delete from dup_srcid where dup_id=old.dup_id;
				delete from dup_dstid where dup_id=old.dup_id;
				delete from dup_rt_camera_file where dup_id=old.dup_id;
				delete from dup_res_rt where dup_id=old.dup_id;
				end;
				//
				delimiter ;" >> $UPDATE_DIR/up_r6.sql

		fi

		if [ "$versionNumber" -le "5960" ];then
			echo "alter table dup_res add (dst_res_num int default 0,dst_res_seq int default 0);" >> $UPDATE_DIR/up_r6.sql
		fi

		if [ "$versionNumber" -le "5973" ];then
			echo "drop index camera_info_index3 on camera_info;" >> $UPDATE_DIR/up_r6.sql
		fi

		if [ "$versionNumber" -lt "6093" ];then
			echo "insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
				values (124, 3, '指定人脸为单次注册', '/facedb/attr/face_pri_up', 'POST', '', 0, null, null);" >> $UPDATE_DIR/up_r6.sql
			echo "insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
				values (125, 5, '修改告警结果相似度', '/rtmonitor/alarm/result', 'POST', '修改告警结果相似度', 0, null, null);" >> $UPDATE_DIR/up_r6.sql
			echo "insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
				values (126, 3, '指定特征为单次注册', '/facedb/attr/feat_pri_up', 'POST', '', 0, null, null);" >> $UPDATE_DIR/up_r6.sql

			echo "insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
				values (1095, 1, 124, now(), 'root', null);" >> $UPDATE_DIR/up_r6.sql
			echo "insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
				values (1096, 1, 125, now(), 'root', null);" >> $UPDATE_DIR/up_r6.sql
			echo "insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
				values (1097, 1, 126, now(), 'root', null);" >> $UPDATE_DIR/up_r6.sql
		fi

		if [ "$versionNumber" -lt "6204" ];then
			echo "alter table dup_rt_camera_file drop foreign key dup_id;" >> $UPDATE_DIR/up_r6.sql
			echo "alter table dup_srcid drop foreign key dup_id;" >> $UPDATE_DIR/up_r6.sql
			echo "alter table dup_dstid drop foreign key dup_id;" >> $UPDATE_DIR/up_r6.sql
			echo "alter table dup_res drop foreign key dup_id;" >> $UPDATE_DIR/up_r6.sql
			echo "drop trigger tr_delete_dup_id;" >> $UPDATE_DIR/up_r6.sql
		fi

		mysql -u${dbuser} -p${dbpwd} -D${database} < ${UPDATE_DIR}/up_r6.sql
	elif [ "$dbselect" == "oracle" ];then
		dbuser=`grep "\[oracle\]" ${FRAME_ROOT}/etc/sys.conf |awk -F"DBUSER=" '{print $2}'|awk -F" " '{print $1}'`
		dbpwd=`grep "\[oracle\]" ${FRAME_ROOT}/etc/sys.conf |awk -F"DBPWD=" '{print $2}'|awk -F" " '{print $1}'`

		rm -rf $UPDATE_DIR/up_r6.sql
		touch ${UPDATE_DIR}/up_r6.sql
		if [ "$versionNumber" -le "5464" ];then
			echo "set serveroutput on;" > $UPDATE_DIR/query_id_ora.sql
			echo "select 'mark_result',dbid from face_db;" >> $UPDATE_DIR/query_id_ora.sql

			for dbid in `sqlplus $dbuser/$dbpwd @$UPDATE_DIR/query_id_ora.sql|grep mark_result|awk '{print $2}'`;do
				echo "alter table face_$dbid add (pri number default 0);" >> $UPDATE_DIR/up_r6.sql
			done

		fi
		
		if [ "$versionNumber" -le "5476" ];then
			echo "alter table CAMERA_RUN_CTRL add (last_clear_time date default sysdate,alarm_count NUMBER(12) default 0,snap_count NUMBER(12) default 0); " >> $UPDATE_DIR/up_r6.sql
		fi

		if [ "$versionNumber" -le "5973" ];then
			echo "alter table dup_info add (dup_type number default 0);" >> $UPDATE_DIR/up_r6.sql
			echo "alter table dup_info add (rt_beg date);" >> $UPDATE_DIR/up_r6.sql
			echo "alter table dup_info add (rt_end date);" >> $UPDATE_DIR/up_r6.sql
			echo "alter table dup_info add (alarm_intval number);" >> $UPDATE_DIR/up_r6.sql
			echo "alter table dup_info add (snapshot_beg number);" >> $UPDATE_DIR/up_r6.sql
			echo "alter table dup_info add (snapshot_end number);" >> $UPDATE_DIR/up_r6.sql

			echo "create table dup_rt_camera_file(
				dup_id		number,
				camera_id	number,
				file_id		varchar2(20),
				foreign key(dup_id)
					references dup_info(dup_id));" >> $UPDATE_DIR/up_r6.sql

			echo "create table dup_res_rt(
				dup_res_id		number		primary key,
				dup_id			number,
				src_db_id		number,
				src_person_id	number,
				src_face_id		number,
				sim				number,
				verify			number		default 0,
				host_id			number,
				camera_id		number,
				file_id			varchar2(20),
				image_id		varchar2(255),
				face_id			varchar2(255),
				alarm_time		date,
				snapshot_num	number,
				snapshot_seq	number);" >> $UPDATE_DIR/up_r6.sql
			echo "create index idx_dupid_res_rt on dup_res_rt(dup_id);" >> $UPDATE_DIR/up_r6.sql

			echo "drop trigger tr_delete_dup_id;" >> $UPDATE_DIR/up_r6.sql
			echo "create or replace trigger tr_delete_dup_id
				before delete on dup_info
				for each row
				begin
				delete from dup_res where dup_id=:old.dup_id;
				delete from dup_srcid where dup_id=:old.dup_id;
				delete from dup_dstid where dup_id=:old.dup_id;
				delete from dup_rt_camera_file where dup_id=:old.dup_id;
				delete from dup_res_rt where dup_id=:old.dup_id;
				end;
				/" >> $UPDATE_DIR/up_r6.sql
		fi

		if [ "$versionNumber" -le "5960" ];then
			echo "alter table dup_res add (dst_res_num number default 0,dst_res_seq number default 0);" >> $UPDATE_DIR/up_r6.sql
		fi

		if [ "$versionNumber" -le "5973" ];then
			echo "drop index camera_info_index3;" >> $UPDATE_DIR/up_r6.sql
		fi

		if [ "$versionNumber" -lt "6093" ];then
			echo "insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
				values (124, 3, '指定人脸为单次注册', '/facedb/attr/face_pri_up', 'POST', '', 0, null, null);" >> $UPDATE_DIR/up_r6.sql
			echo "insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
				values (125, 5, '修改告警结果相似度', '/rtmonitor/alarm/result', 'POST', '修改告警结果相似度', 0, null, null);" >> $UPDATE_DIR/up_r6.sql
			echo "insert into sys_service_info (SERVICE_ID, TYPE_ID, SERVICE_NAME, SERVICE_ADDRESS, FUNCTION, DESCRIPTION, LOGIN_REQUIRE, CRT_DATE, CRT_USER)
				values (126, 3, '指定特征为单次注册', '/facedb/attr/feat_pri_up', 'POST', '', 0, null, null);" >> $UPDATE_DIR/up_r6.sql

			echo "insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
				values (1095, 1, 124, sysdate, 'root', null);" >> $UPDATE_DIR/up_r6.sql
			echo "insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
				values (1096, 1, 125, sysdate, 'root', null);" >> $UPDATE_DIR/up_r6.sql
			echo "insert into sys_pri_role_service (PRI_ID, ROLE_ID, SERVICE_ID, CRT_DATE, CRT_USER, REMARK)
				values (1097, 1, 126, sysdate, 'root', null);" >> $UPDATE_DIR/up_r6.sql
		fi

		if [ "$versionNumber" -lt "6204" ];then
			echo "select 'mark_result',constraint_name from user_constraints where table_name='DUP_RT_CAMERA_FILE' or table_name='DUP_SRCID' or table_name='DUP_DSTID' or table_name='DUP_RES' and r_constraint_name is not null;" >> $UPDATE_DIR/query_dup_constraint.sql

			for constraint_table in `sqlplus $dbuser/$dbpwd @$UPDATE_DIR/query_dup_constraint.sql|grep mark_result|awk '{print $2"_"$3}'`;do
				constraint=`echo $constraint_table|awk -F"_" '{print $1}'`
				tablename=`echo $constraint_table|awk -F"_" '{print $2}'`
				echo "alter table $tablename drop constraint $constraint;" >> $UPDATE_DIR/up_r6.sql
			done

			echo "drop trigger tr_delete_dup_id;" >> $UPDATE_DIR/up_r6.sql
		fi

		echo "exit" >> ${UPDATE_DIR}/up_r6.sql
		sqlplus ${dbuser}/${dbpwd} @${UPDATE_DIR}/up_r6.sql
	else
		echo "error db: ${dbselect}"
	fi
}
main $@

