#!/bin/bash
set -eu

# Sum 2nd columns of input files into output files

OUTPUT=$1
INPUT1=$2
INPUT2=$3
shift

# Open files to be merged
exec 11< ${INPUT1}
exec 12< ${INPUT2}

echo "merge: $INPUT1 $INPUT2"

while true
do
  read LINE V1 <&11 || exit 0 # Exit when out of input
  read LINE V2 <&12 # If this fails, there was a real error
  V=$(( V1 + V2 ))
  echo ${LINE} ${V}
done > ${OUTPUT}

exit 0
