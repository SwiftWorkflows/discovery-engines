#!/bin/zsh

if [[ ${#*} != 2 ]]
then
    print "usage: <OUTPUT_FILE> <OUTPUT_ENTRY> <SUBSETS>"
    return 1
fi

OUTPUT_FILE=$1
OUTPUT_ENTRY=$2
SUBSETS=$3

S=$((SUBSETS-1))

INPUTS=$( eval print "${OUTPUT_FILE%.nxs}-{0..${S}}.nxs" ) 

echo cctw merge ${^INPUTS}\#${OUTPUT_ENTRY} \
     -o ${OUTPUT_FILE}\#${OUTPUT_ENTRY}
