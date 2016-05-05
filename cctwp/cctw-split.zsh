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
OUTPUT_FILE=$4
OUTPUT_ENTRY=$5
shift 5

# Optional arguments (-j <threads>, -S i/n subsets)
A=${*}

# Print a header
print "CCTW.ZSH"
date "+%m/%d/%Y %I:%M%p"
print

set -x
cctw transform --script ${SCRIPT}     \
     ${INPUT_FILE}\#${INPUT_ENTRY}         \
     -o ${OUTPUT_FILE}\#${OUTPUT_ENTRY}    \
     ${=A} \
     --normalization 0
