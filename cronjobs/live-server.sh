#!/bin/bash

HOMEDIR="/home/esw_yoogottam/esw-project/"

CHAT_IDs=( 630804492 879466464 623858814 )
BOT_URL='https://api.telegram.org/bot853098987:AAF87AC_3qzruYsN9Cjpk6aKilK5Om9RJu4/sendMessage'
FILE_DOWN="${HOMEDIR}SERVER_DOWN"

if [ ! -e "$FILE_DOWN" ]; then
	if ! wget "localhost:9999/test/"; then
	    for id in ${CHAT_IDs[@]}; do
		wget -O- --header="Content-Type: application/json" --post-data="{\"chat_id\": \"$id\", \"parse_mode\": \"Markdown\", \"text\": \"*ALERT!* The Flask server seems to be down.\"}" $BOT_URL > /dev/null 2>&1
	    done

	    touch "$FILE_DOWN"
	fi
fi
