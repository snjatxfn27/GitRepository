#!/bin/bash

function main()
{
	ip=$2

	if [ "`ifconfig|grep "${ip}"|awk -F"inet addr:" '{print $2}'|awk '{print $1}'`" == "${ip}" ];then
		${FRAME_ROOT}/bin/fc_monitor $@ &
	else
		ssh -o StrictHostKeyChecking=no ${ip} "cd ${FRAME_ROOT};. svc_profile.sh;${FRAME_ROOT}/etc/script/enableMonitor.sh $@" &
	fi
}
main $@

exit 0
