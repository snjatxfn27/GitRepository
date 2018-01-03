#!/bin/bash

cur_dir=$(dirname $0)

source /etc/profile
source ~/.bashrc

cd $cur_dir/../..

export PATH=$PATH:/usr/local/bin:/bin
source svc_profile.sh &>/tmp/fcstart.log 
./configure
bash etc/script/start-facecloud.sh &>>/tmp/fcstart.log 

