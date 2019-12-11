#!/bin/bash
# the textual message should be supplied as the first argument

# convert string into array of strings
read -r CHAT_IDs < "${HOMEDIR}secretdata/CHAT_IDS"
CHAT_IDs=($CHAT_IDs) # https://stackoverflow.com/a/9294015

BOTUID=`cat ${HOMEDIR}secretdata/BOTUID`
BOT_URL="https://api.telegram.org/${BOTUID}/sendMessage"

for id in ${CHAT_IDs[@]}; do
    wget -O- --header="Content-Type: application/json" --post-data="{\"chat_id\": \"$id\", \"parse_mode\": \"Markdown\", \"text\": \"$1\"}" $BOT_URL > /dev/null 2>&1
done