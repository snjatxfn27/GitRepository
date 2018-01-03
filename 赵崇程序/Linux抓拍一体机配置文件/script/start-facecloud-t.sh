#!/bin/bash

redis-server ${FRAME_ROOT}/etc/redis.conf &

sleep 1

${FRAME_ROOT}/bin/sysMgr &

sleep 3

${FRAME_ROOT}/bin/sysSvc &

sleep 1

spawn-fcgi -a 127.0.0.1 -p 3281 -F 1 -- ${FRAME_ROOT}/bin/cgiServer -t 120 -l ${FRAME_ROOT}/lib/libFCloudMgr.so -a false -c 16 -h facemgr -s tcp://127.0.0.1:MQ_PORT &

spawn-fcgi -a 127.0.0.1 -p 3282 -F 1 -- ${FRAME_ROOT}/bin/cgiServer -t 60 -l ${FRAME_ROOT}/lib/libdupFCGI.so -a false -s "tcp://127.0.0.1:MQ_PORT" -c 16 -h dupface &

spawn-fcgi -a 127.0.0.1 -p 3283 -F 1 -- ${FRAME_ROOT}/bin/cgiServer -t 240 -l ${FRAME_ROOT}/lib/librtParse.so -a false -s "tcp://127.0.0.1:MQ_PORT" -c 16 -h rtms &

spawn-fcgi -a 127.0.0.1 -p 3284 -F 1 -- ${FRAME_ROOT}/bin/cgiServer -t 240 -l ${FRAME_ROOT}/lib/libsysMgrCgi.so -a false -s "tcp://127.0.0.1:MQ_PORT" -c 16 -h sysmgr &

${FRAME_ROOT}/bin/lde lde &

$FRAME_ROOT/etc/script/fc_dog.sh &

