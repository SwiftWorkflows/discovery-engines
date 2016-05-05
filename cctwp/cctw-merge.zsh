#!/bin/zsh
set -eu

print "CCTW-MERGE"
date "+%B %d %I:%M%p"
print

if [[ ${#*} != 3 ]]
then
    print "usage: <OUTPUT_FILE> <OUTPUT_ENTRY> <SUBSETS>"
    print "given: ${*}"
    return 1
fi

OUTPUT_FILE=$1
OUTPUT_ENTRY=$2
SUBSETS=$3

grep "model name" /proc/cpuinfo | head -1
grep mips /proc/cpuinfo | head -1
print

print "CCTW:"
START=$( date +%s )

S=$((SUBSETS-1))

INPUTS=()
for (( i=0 ; i<S ; i++ ))
do
  INPUTS+="${OUTPUT_FILE%.nxs}-${i}.nxs"\#${OUTPUT_ENTRY}
done

cctw merge ${INPUTS} \
     -o ${OUTPUT_FILE}\#${OUTPUT_ENTRY}

STOP=$( date +%s )
print "TIME:" $(( STOP-START ))
