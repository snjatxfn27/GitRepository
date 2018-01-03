#!/bin/bash

function main()
{
	name=$1
	ip=$2

	if [ "`ifconfig|grep "${ip}"|awk -F"inet addr:" '{print $2}'|awk '{print $1}'`" == "${ip}" ];then
		ps -ef|grep ${FRAME_ROOT}|grep fc_env|grep -v grep|grep "svcName ${name} "|awk '{print $2}'|xargs kill -9 2>/dev/null
	else
		ssh ${ip} "cd ${FRAME_ROOT};. svc_profile.sh;${FRAME_ROOT}/etc/script/killEnv.sh $@" &
	fi
}
main $@

exit 0
