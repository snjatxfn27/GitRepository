#!/bin/sh
echo "begin reboot: ${FRAME_ROOT}"
cd ${FRAME_ROOT}/
source ./svc_profile.sh
cd etc/script
./kill.sh
sleep 1 
./start-facecloud.sh
sleep 1 
echo "reboot finish"
