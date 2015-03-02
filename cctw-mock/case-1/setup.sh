#!/bin/bash
set -eu

if [[ ${#*} != 3 ]]
then
  echo "usage: <M> <N> <OUTPUT>"
  exit 1
fi

M=$1
N=$2
OUTPUT=$3

k=0
for (( i=0 ; i<M ; i++ ))
do
  for (( j=0 ; j<N ; j++ ))
  do
    V=0
    R=${RANDOM}
    (( ${R} % 10 == 0 )) && V=1
    (( ${R} % 9  == 0 )) && V=2
    echo ${k} ${V}
    (( k++ )) || true
  done
done > ${OUTPUT}

mkdir -pv chunks
