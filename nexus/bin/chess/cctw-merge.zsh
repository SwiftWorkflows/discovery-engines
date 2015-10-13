#!/bin/zsh

set -eu

if [[ ${#*} != 3 ]]
then
    print "usage: <OUTPUT_FILE> <OUTPUT_ENTRY> <SUBSETS>"
    print "given: ${*}"
    return 1
fi

OUTPUT_FILE=$1
OUTPUT_ENTRY=$2
SUBSETS=$3

S=$((SUBSETS-1))

INPUTS=()
for (( i=0 ; i<S ; i++ ))
do
  INPUTS+="${OUTPUT_FILE%.nxs}-${i}.nxs"\#${OUTPUT_ENTRY}
done

cctw merge ${INPUTS} \
     -o ${OUTPUT_FILE}\#${OUTPUT_ENTRY}
