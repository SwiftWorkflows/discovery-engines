#!/bin/bash
set -eu

if [[ ${#*} != 2 ]]
then
  echo "usage: <N> <OUTPUT>"
  exit 1
fi

N=$1
OUTPUT=$2

for (( i=0 ; i<N ; i++ ))
do
  V=0
  R=${RANDOM}
  (( ${R} % 10  == 0 )) V=1
  (( ${R} % 100 == 0 )) V=2
  printf "%i %i"
done
