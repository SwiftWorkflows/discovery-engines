#!/bin/bash -eu

DE_HOME=$( cd $( dirname $0 )/.. ; /bin/pwd )

if [[ ${#*} == 1 ]] 
then
    echo "rsync-all.zsh: requires 1 argument."
    exit 1
fi

DE_BIN=${DE_HOME}/bin

ENTRY=$1
PADS_LIST=( $( cat ${DE_HOME}/data/pads.list ) ) 

PADS_COUNT=${#PADS_LIST[@]}
print "PADS nodes: ${PADS_COUNT}"

HASH=$( ${DE_BIN}/hash.pl ${ENTRY} )
INDEX=$(( HASH % PADS_COUNT + 1 ))
# declare INDEX
NODE=${PADS_LIST[${INDEX}]}.pads.ci.uchicago.edu
echo "NODE=${NODE}"
