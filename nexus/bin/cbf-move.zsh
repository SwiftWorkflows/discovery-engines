#!/bin/zsh

set -eu

RAMDISK=/ramdisk
INBOX=${RAMDISK}/inbox
OUTBOX=${RAMDISK}/outbox

REMOTE_HOST=nxrs.msd.anl.gov
REMOTE_DIR=/home/bessrc/data1/2015-1

mkdir -p ${OUTBOX}

while true
do
  # Poll loop
  while true
  do
    FILES=( $( ls ${INBOX} ) ) 
    if (( ${#FILES} > 0 )) 
    then
      mv -v ${FILES} ${OUTBOX}
      break
    else
      sleep 10
    fi
  done
  xfer.zsh ${REMOTE_HOST} ${REMOTE_DIR} ${FILES} &
done
