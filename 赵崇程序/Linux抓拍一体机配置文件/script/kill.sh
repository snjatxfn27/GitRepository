#!/bin/bash

ps aux|grep fc_dog|grep -v grep|awk '{print $2}'|xargs kill -9 2>/dev/null

chunk -k

sleep 2

ps -ef|egrep "fc_env|fc_monitor|Daemon|sysMgr|dupmanager|cgiServer|mtfs|rtmsn|rtms|rtmf|clrd|faceOpClnt|sysSvc|lde|lda" | grep -v grep|awk '{print $2}'|xargs kill -9 2>/dev/null

declare -i i=0
while [ true ];do
	if [ $i -eq 10 ];then
		break
	fi

	chunkpid=`ps aux|grep chunk|grep -v grep|awk '{print $2}'`
	if [ -z "$chunkpid" ];then
		break
	fi

	i=$i+1
	sleep 1
done

ps -ef|egrep "redis-server|chunk"|grep -v grep|awk '{print $2}'|xargs kill -9 2>/dev/null

