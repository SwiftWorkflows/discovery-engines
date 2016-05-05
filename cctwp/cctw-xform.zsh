#!/bin/zsh

# Print a header
print "CCTW-SPLIT.ZSH"
date "+%b %d %I:%M%p"
print

if [[ ${#*} < 5 ]]
then
    print "usage: <SCRIPT> <INPUT_FILE> <INPUT_ENTRY>"
    print "                <OUTPUT_FILE> <OUTPUT_ENTRY>"
    print "       <OTHER ARGS>*"
    return 1
fi

grep "model name" /proc/cpuinfo | head -1
grep mips /proc/cpuinfo | head -1
print

print "CCTW:"

SCRIPT=$1
INPUT_FILE=$2
INPUT_ENTRY=$3
OUTPUT_FILE=$4
OUTPUT_ENTRY=$5
shift 5

# Optional arguments (-j <threads>, -S i/n subsets)
A=${*}

START=$( date +%s )

set -x
cctw transform --script ${SCRIPT}     \
     ${INPUT_FILE}\#${INPUT_ENTRY}         \
     -o ${OUTPUT_FILE}\#${OUTPUT_ENTRY}    \
     ${=A} \
     --normalization 0
set +x >& /dev/null

STOP=$( date +%s )
print "TIME:" $(( STOP-START ))
