#!/bin/zsh -efu

DE_HOME=$( cd $( dirname $0 )/.. ; /bin/pwd )
source ${DE_HOME}/lib/helpers.zsh
if [[ ${?} != 0 ]] 
then
    print "Could not find discovery engines directory."
    exit 1
fi

[[ ${#*} == 2 ]] || crash 1 "rsync-all.zsh: requires 2 arguments".

DIR=$1
ENTRIES=$2
PADS_LIST=( $( < ~wozniak/pads.list ) ) 

[[ -d ${DIR} ]] || crash 1 "directory does not exist: ${DIR}"

[[ -f ${ENTRIES} ]] || crash 1 "entries file does not exist: ${ENTRIES}"

PADS_COUNT=${#PADS_LIST}
print "PADS nodes: ${PADS_COUNT}"

DE_BIN=${DE_HOME}/bin
DE_LOGS=${DE_HOME}/logs

mkdir -pv ${DE_LOGS}
LOG=${DE_LOGS}/rsync-all.log
declare LOG

cd ${DIR}
declare PWD

declare ENTRIES

while true  
do 
  print ok1
  read ENTRY
  print ok
  declare ENTRY
  HASH=$( ${DE_BIN}/hash.pl ${ENTRY} )
  INDEX=$(( HASH % PADS_COUNT + 1 ))
  declare INDEX
  NODE=${PADS_LIST[${INDEX}]}.pads.ci.uchicago.edu
  declare NODE
  rsync --size-only -av ${ENTRY} ${NODE}:/scratch/local/aps-rsync
done < ${ENTRIES} |& tee -a ${LOG}
