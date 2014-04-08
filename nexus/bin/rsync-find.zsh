#!/bin/zsh -eu

DE_HOME=$( cd $( dirname $0 )/.. ; /bin/pwd )
source ${DE_HOME}/lib/helpers.zsh
if [[ ${?} != 0 ]] 
then
    print "Could not find discovery engines directory."
    exit 1
fi

[[ ${#*} == 1 ]] || crash 1 "rsync-all.zsh: requires 1 argument."

DE_BIN=${DE_HOME}/bin

ENTRY=$1
PADS_LIST=( $( < ~wozniak/pads.list ) ) 

PADS_COUNT=${#PADS_LIST}
print "PADS nodes: ${PADS_COUNT}"

HASH=$( ${DE_BIN}/hash.pl ${ENTRY} )
INDEX=$(( HASH % PADS_COUNT + 1 ))
# declare INDEX
NODE=${PADS_LIST[${INDEX}]}.pads.ci.uchicago.edu
declare NODE
