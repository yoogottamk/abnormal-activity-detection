#!/bin/bash

HOMEDIR="/home/esw_yoogottam/esw-project/"

FILE_DOWN="${HOMEDIR}SERVER_DOWN"

if [ ! -e "$FILE_DOWN" ]; then
	if ! wget -qO- "localhost:9999/test/" >/dev/null 2>&1; then
	    ./sendMessage.sh "*ALERT!* The Flask server seems to be down.}"

	    touch "$FILE_DOWN"
	fi
fi
