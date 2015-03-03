#!/bin/bash
set -eu

# NXS2EPS
# Convert NXS file to viewable EPS

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
convert $T $EPS
rm $T
