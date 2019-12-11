#!/bin/bash

HOMEDIR="/home/esw_yoogottam/esw-project/server/"

timestamp="$( date +%s )"
last="$( cat ${HOMEDIR}live )"

timediff="$( echo "$timestamp - $last" | bc )"
LAST_SEEN=$( date -d @"$last" +"%H:%M:%S %d %b %Y" )

if [ ! -e ${HOMEDIR}ESP_DOWN ]; then
	if (( timediff > 60 )); then
	    ./sendMessage.sh "*ALERT!* No activity from ESP since _${LAST_SEEN}_"

	    touch ${HOMEDIR}ESP_DOWN
	fi
fi
