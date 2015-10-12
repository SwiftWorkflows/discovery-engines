#!/bin/zsh

if [[ ${#*} < 5 ]]
then
    print "usage: <SCRIPT> <INPUT_FILE> <INPUT_ENTRY>"
    print "                <OUTPUT_FILE> <OUTPUT_ENTRY>"
    print "       <OTHER ARGS>*"
    return 1
fi

SCRIPT=$1
INPUT_FILE=$2
INPUT_ENTRY=$3
OUTPUT_FILE=$2
OUTPUT_ENTRY=$3
shift 5
A=${*}

echo cctw transform --script ${SCRIPT}     \
     ${INPUT_FILE}\#${INPUT_ENTRY}         \
     -o ${OUTPUT_FILE}\#${OUTPUT_ENTRY}    \
     ${=A} \
     --normalization 0
