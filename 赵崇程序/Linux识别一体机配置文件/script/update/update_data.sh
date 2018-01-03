#!/bin/bash

srcFrameRoot=$1
newFrameRoot=$2
oldVersion=$3
newVersion=$4

cd $newFrameRoot
source svc_profile.sh
sh etc/script/kill.sh
mkdir -p $newFrameRoot/bin/chunk_db

if [ ! -z "$oldVersion" ];then
	if [ "$oldVersion" -gt "5464" ];then
		mv $srcFrameRoot/bin/chunk_db/face_*.db $newFrameRoot/bin/chunk_db 2>/dev/null
	fi
fi

mkdir -p $newFrameRoot/pic/
mv $srcFrameRoot/pic/* $newFrameRoot/pic/ 2>/dev/null
