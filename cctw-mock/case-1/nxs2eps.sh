#!/bin/bash
set -eu

if [ ${#*} != 4 ]
then
  echo "usage: <M> <N> <NXS> <EPS>"
  exit 1
fi

M=$1
N=$2
NXS=$3
EPS=$4

T=$( mktemp --suffix .ppm )
./nxs2ppm.sh 10 10 $NXS $T
echo T $T
convert $T $EPS
# rm $T
