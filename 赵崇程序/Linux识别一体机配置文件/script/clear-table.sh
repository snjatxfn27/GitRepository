#!/bin/bash

#${FRAME_ROOT}/bin/clrmtff

if [ "$1" = "--help" ];then
	echo "Usage: clear-table.sh sqluser sqlpwd"
	exit 1
fi

SQLUSER=$1
SQLPWD=$2

if [ -z "${SQLUSER}" ] || [ -z "${SQLPWD}" ];then
	echo "Please input sqluser sqlpwd"
	exit 1
fi

sqlplus ${SQLUSER}/${SQLPWD} @${FRAME_ROOT}/etc/script/reco-del-ora.sql

rm -rf ${FRAME_ROOT}/bin/chunk_db
#${FRAME_ROOT}/bin/clrAlarmDb ${SQLUSER}

