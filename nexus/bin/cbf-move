#!/bin/zsh

set -eu

usage()
{
  print "usage: cbf-move <OPTS>* <DIRECTORY> <TARGET>"
  print "\t OPTS:      -h for help"
  print "\t DIRECTORY: the local directory"
  print "\t TARGET:    the rsync target, i.e., user@host:/dirs"
}

zparseopts -D -E h=H
if [[ ${#H} != 0 ]]
then
  usage 
  exit 0
fi

if [[ ${#*} != 1 ]]
then
  usage
  exit 1
fi

DIRECTORY=$1
TARGET=$2

REMOTE_HOST=nxrs.msd.anl.gov
REMOTE_DIR=/home/bessrc/sharedbigdata/data1/osborn-2015-1

alias RSYNC='rsync -rtv --exclude "*tmp"'

DONE=0
while true
do
  [[ -f ${DIRECTORY}/done.txt ]] && DONE=1
  RSYNC ${DIRECTORY} ${REMOTE_HOST}:${REMOTE_DIR}
  (( DONE )) && break
  sleep 10
done

print rm -r ${DIRECTORY}
