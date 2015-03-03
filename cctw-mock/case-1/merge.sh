#!/bin/bash
set -eu

# Sum 2nd columns of input files into output files

OUTPUT=$1
INPUT1=$2
INPUT2=$3
shift

# echo "merge: ${INPUT1} ${INPUT2} to: ${OUTPUT}"

crash()
{
  echo ${*}
  exit 1
}

# Open files to be merged
[[ -f ${INPUT1} ]] || crash "Not found: ${INPUT1}"
[[ -f ${INPUT2} ]] || crash "Not found: ${INPUT2}"
exec 11< ${INPUT1}
exec 12< ${INPUT2}

while true
do
  read LINE V1 <&11 || exit 0 # Exit when out of input
  read LINE V2 <&12 # If this fails, there was a real error
  V=$(( V1 + V2 ))
  echo ${LINE} ${V}
done > ${OUTPUT}

exit 0
