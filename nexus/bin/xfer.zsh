#!/bin/zsh 

set -eu

REMOTE_HOST=$1
REMOTE_DIR=$2
shift 2
FILES=${*}

@ nice rsync ${FILES} ${REMOTE_HOST}:${REMOTE_DIR}
print rm ${FILES}
