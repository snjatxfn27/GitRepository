#!/bin/bash

sleep 10

while [ true ];do
	sleep 20
	if [ -z "`ps aux|grep "chatter"|grep -v grep`" ];then
	cd /usr/unisib/app/chatter
        nohup ./chatter >/dev/null 2>&1 &
        fi
done

