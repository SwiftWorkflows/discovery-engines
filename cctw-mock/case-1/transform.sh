#!/bin/bash
set -eu

if [[ ${#*} != 5 ]]
then
  echo "usage: <OUTPUT.NXS> <M> <N> <INPUT.NXS> <ROW>"
  exit 1
fi

OUTPUT=$1
M=$2
N=$3
INPUT=$4
LINE=$5

(( L=LINE+1 ))

# echo "$INPUT $OUTPUT"

OUT=()
SIZE=$(( M * N ))
# set -x
C=$( awk "NR == ${L} { print \$2 }" ${INPUT} )
# echo L C $L $C
# set +x

for P in ${LINE} $(( LINE-1 )) $(( LINE+1 )) $(( LINE-10 )) $(( LINE+10 ))
do
  if (( P >= 0 )) && (( P < SIZE ))
  then
    OUT[${P}]=${C}
  fi
done

k=0
for (( i=0 ; i<M ; i++ ))
do
  for (( j=0 ; j<N ; j++ ))
  do
    echo ${k} ${OUT[${k}]:-0}
    (( k++ )) || true
  done
done > ${OUTPUT}
