#!/bin/bash

if [ "${1}" = "--help" ] || [ "${1}" = "-help" ];then
	echo "Useage: command systempwd"
	exit 1
fi

if [ -z "${1}" ];then
	echo "Useage: command systempwd"
	exit 1
fi

syspwd=$1
sqluser=`grep "\[oracle\]" ${FRAME_ROOT}/etc/sys.conf |awk -F"DBUSER=" '{print $2}'|awk -F" " '{print $1}'`
sqlpwd=`grep "\[oracle\]" ${FRAME_ROOT}/etc/sys.conf |awk -F"DBPWD=" '{print $2}'|awk -F" " '{print $1}'`

rm -rf create-user.sql
cp create-user-t.sql create-user.sql

sed -i "s#VSPACE#${sqluser}#g" create-user.sql
sed -i "s#VSQLUSER#${sqluser}#g" create-user.sql
sed -i "s#VSQLPWD#${sqlpwd}#g" create-user.sql
sed -i "s#VORACLE_BASE#${ORACLE_BASE}#g" create-user.sql

sqlplus system/${syspwd} @create-user.sql

sqlplus ${sqluser}/${sqlpwd} @reco-create-ora.sql

sqlplus ${sqluser}/${sqlpwd} @conf-table.sql



