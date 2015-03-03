#!/bin/bash
set -eu

# NXS2PPM
# Convert NXS file to PPM image format

BLACK="0 0 0"
RED="10 0 0"
GREEN="0 10 0"
BLUE="0 0 10"
YELLOW="10 10 0"
PURPLE="9 0 9"
WHITE="10 10 10"
COLORS=( BLACK GREEN BLUE RED YELLOW PURPLE WHITE )

if [[ ${#*} != 4 ]]
then
  echo "usage: <M> <N> <INPUT.EPS> <OUTPUT.PPM>"
  exit 1
fi

M=$1
N=$2
INPUT=$3
OUTPUT=$4

# Header
{
  echo "P3"
  echo "${M} ${N} 10"
} > ${OUTPUT}

i=0
while read LINE V
do
  # Translate
  if (( V == 0 ))
  then
    COLOR=${BLACK}
  else
    j=$(( V % 7 ))
    C=${COLORS[j]}
    COLOR=${!C}
  fi
  printf "%s " ${COLOR}

  # Loop
  (( i++ )) || true
  if (( i % M == 0 ))
  then
    printf "\n"
    i=0
  fi
done < ${INPUT} >> ${OUTPUT}
