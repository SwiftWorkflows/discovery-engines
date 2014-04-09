#!/bin/zsh

if [[ ${#*} != 3 ]]
then
  usage
  exit 1
fi

MIN_RANK=$1
MAX_RANK=$2
LOGFILE=$3

for (( i=${MIN_RANK} ; i < MAX_RANK ; i++ ))
do
  print $i $( rank -c ${i} ${LOGFILE} | grep -c exec: )
done
