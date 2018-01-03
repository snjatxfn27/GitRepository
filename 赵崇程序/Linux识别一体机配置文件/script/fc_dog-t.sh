#!/bin/bash

sleep 10

while [ true ];do
	sleep 10
	if [ -z "`ps aux|grep "nginx"|grep -v grep`" ];then
        /usr/sbin/nginx -c /etc/nginx/nginx.conf &
    fi
        
	if [ -z "`ps aux|grep "redis-server"|grep -v grep`" ];then
		redis-server $FRAME_ROOT/etc/redis.conf &
	fi

	if [ -z "`ps aux|grep "sysMgr$"|grep -v grep`" ];then
		$FRAME_ROOT/bin/sysMgr &
	fi

	if [ -z "`ps aux|grep "sysSvc$"|grep -v grep`" ];then
		$FRAME_ROOT/bin/sysSvc &
	fi

	if [ -z "`ps aux|grep cgiServer|grep libdupFCGI|grep -v grep`" ];then
		spawn-fcgi -a 127.0.0.1 -p 3282 -F 1 -- $FRAME_ROOT/bin/cgiServer -t 60 -l $FRAME_ROOT/lib/libdupFCGI.so -a false -c 16 -h dupface -s tcp://127.0.0.1:MQ_PORT &
	fi

	if [ -z "`ps aux|grep cgiServer|grep libFCloudMgr|grep -v grep`" ];then
		spawn-fcgi -a 127.0.0.1 -p 3281 -F 1 -- $FRAME_ROOT/bin/cgiServer -t 120 -l $FRAME_ROOT/lib/libFCloudMgr.so -a false -c 16 -h facemgr -s tcp://127.0.0.1:MQ_PORT &
	fi

	if [ -z "`ps aux|grep cgiServer|grep librtParse|grep -v grep`" ];then
		spawn-fcgi -a 127.0.0.1 -p 3283 -F 1 -- $FRAME_ROOT/bin/cgiServer -t 240 -l $FRAME_ROOT/lib/librtParse.so -a false -c 16 -h rtms -s tcp://127.0.0.1:MQ_PORT &
	fi

	if [ -z "`ps aux|grep cgiServer|grep libsysMgrCgi|grep -v grep`" ];then
		spawn-fcgi -a 127.0.0.1 -p 3284 -F 1 -- $FRAME_ROOT/bin/cgiServer -t 240 -l $FRAME_ROOT/lib/libsysMgrCgi.so -a false -c 16 -h sysmgr -s tcp://127.0.0.1:MQ_PORT &
	fi

	if [ -z "`ps aux|grep "lde lde$"|grep -v grep`" ];then
		$FRAME_ROOT/bin/lde lde &
	fi
done

