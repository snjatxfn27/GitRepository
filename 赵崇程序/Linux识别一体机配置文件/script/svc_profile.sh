#!/bin/bash

if [ ! -z "${FRAME_ROOT}" ];then
	PATH=${PATH//:${FRAME_ROOT}\/bin/}; export PATH
fi

GlobalDatabaseName="orcl"
if [ -f /etc/oratab ];then
	GlobalDatabaseName=`cat /etc/oratab|tail -n 1|awk -F: '{print $1}'`
	ORACLE_ROOT=`cat /etc/oratab|tail -n 1|awk -F: '{print $2}'`; export ORACLE_ROOT
fi

GLOBAL_DB_NAME=${GlobalDatabaseName}; export GLOBAL_DB_NAME

if [ -f /usr/local/bin/oraenv ];then
	echo "${GlobalDatabaseName}" > dbname
	. /usr/local/bin/oraenv < dbname 1>/dev/null
	rm -rf dbname
fi

cd $(dirname 0)
FRAME_ROOT=`pwd`; export FRAME_ROOT
cd - 1>/dev/null

LOG_ROOT=${FRAME_ROOT}/logs; export LOG_ROOT
PATH=${PATH}:/usr/lib/oracle/11.2/client64/bin:${FRAME_ROOT}/bin:${FRAME_ROOT}/3rdTools/bin; export PATH
LD_LIBRARY_PATH=/usr/lib/oracle/11.2/client64/lib:${FRAME_ROOT}/lib:${FRAME_ROOT}/3rdTools/lib:${ORACLE_ROOT}/lib; export LD_LIBRARY_PATH

loginuser=$(whoami)
if [ "${loginuser}" == "root" ];then
	echo "${FRAME_ROOT}/logs/core.%e.%p" > /proc/sys/kernel/core_pattern

	ulimit -c unlimited
	ulimit -n 10000
fi

#MQ_IP=127.0.0.1; export MQ_IP
MQ_PORT=6379; export MQ_PORT

#NGINX_IP=127.0.0.1; export NGINX_IP
NGINX_PORT=8000; export NGINX_PORT

#LOCAL_IP=127.0.0.1; export LOCAL_IP

