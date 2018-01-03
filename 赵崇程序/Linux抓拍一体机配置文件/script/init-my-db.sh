#!/bin/bash

rootpwd=$1
if [ -z "${rootpwd}" ];then
	echo "input root pwd"
	exit 0
fi

dbuser=`grep "\[mysql\]" ${FRAME_ROOT}/etc/sys.conf |awk -F"DBUSER=" '{print $2}'|awk -F" " '{print $1}'`
dbpwd=`grep "\[mysql\]" ${FRAME_ROOT}/etc/sys.conf |awk -F"DBPWD=" '{print $2}'|awk -F" " '{print $1}'`
database=`grep "\[mysql\]" ${FRAME_ROOT}/etc/sys.conf |awk -F"DATABASE=" '{print $2}'|awk -F" " '{print $1}'`

cd ${FRAME_ROOT}/etc/script

mysql -uroot -p${rootpwd} < create-my-user.sql;

mysql -u${dbuser} -p${dbpwd} -D${database} < reco-create-my.sql

mysql -u${dbuser} -p${dbpwd} -D${database} < conf-table.sql

cd - &>/dev/null

