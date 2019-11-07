#!/bin/bash

PATH="$PATH:/bin"
YK_DIR="/home/esw_yoogottam/esw-project/"
TIME=`date "+%F_%H.log"`

mv "${YK_DIR}server/out" "${YK_DIR}data/$TIME"
