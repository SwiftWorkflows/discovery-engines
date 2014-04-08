#!/bin/zsh

set -eu

usage()
{
  print "count-tifs.zsh <DATA> <LIST>"
}

if [[ ${#*} != 2 ]]
then
  usage
  exit 1
fi

DATA=$1
LIST=$2

COUNT=0

DIRS=( $( < ${LIST} ) )
for D in ${DIRS}
do
  TIFS=( ${DATA}/${D}/*.tif )
  (( COUNT = COUNT + ${#TIFS} ))
done

print ${COUNT}
