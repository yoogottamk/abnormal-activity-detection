#!/bin/bash

HOMEDIR="/home/esw_yoogottam/esw-project/"

timestamp="$( date +%s )"
last="$( cat ${HOMEDIR}live )"

timediff="$( echo "$timestamp - $last" | bc )"

CHAT_IDs=( 630804492 879466464 623858814 )
BOT_URL='https://api.telegram.org/bot853098987:AAF87AC_3qzruYsN9Cjpk6aKilK5Om9RJu4/sendMessage'
LAST_SEEN=$( date -d @"$last" +"%H:%M:%S %d %b %Y" )

if [ ! -e ${HOMEDIR}ESP_DOWN ]; then
	if (( timediff > 60 )); then
	    for id in ${CHAT_IDs[@]}; do
		wget -O- --header="Content-Type: application/json" --post-data="{\"chat_id\": \"$id\", \"parse_mode\": \"Markdown\", \"text\": \"*ALERT!* No activity from ESP since _${LAST_SEEN}_\"}" $BOT_URL > /dev/null 2>&1
	    done

	    touch ${HOMEDIR}ESP_DOWN
	fi
fi
