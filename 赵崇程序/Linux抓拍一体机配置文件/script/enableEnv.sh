#!/bin/bash

function main()
{
	ip=$8

	if [ "`ifconfig|grep "${ip}"|awk -F"inet addr:" '{print $2}'|awk '{print $1}'`" == "${ip}" ];then
		${FRAME_ROOT}/bin/fc_env $@ &
	else
		ssh -o StrictHostKeyChecking=no ${ip} "cd ${FRAME_ROOT};. svc_profile.sh;${FRAME_ROOT}/etc/script/enableEnv.sh $@" &
	fi
}
main $@

exit 0
