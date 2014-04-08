#!/bin/sh -eu

DE_HOME=$( cd $( dirname $0 )/.. ; /bin/pwd )

if [[ ${#*} == 2 ]] 
then
    echo "rsync-all.zsh: requires 2 arguments."
    exit 1
fi

DIR=$1
ENTRIES=$2

DE_BIN=${DE_HOME}/bin
DE_DATA=${DE_HOME}/data
DE_LOGS=${DE_HOME}/logs

PADS_LIST=( $( cat ${DE_DATA}/pads.list ) )

if [[ ! -d ${DIR} ]] 
then
    echo "directory does not exist: ${DIR}"
    exit 1
fi

if [[ ! -f ${ENTRIES} ]] 
then
    echo "entries file does not exist: ${ENTRIES}"
    exit 1
fi

PADS_COUNT=${#PADS_LIST[@]}
print "PADS nodes: ${PADS_COUNT}"


mkdir -pv ${DE_LOGS}
LOG=${DE_LOGS}/rsync-all.log
declare LOG

cd ${DIR}
echo PWD=${PWD}

echo ENTRIES=${ENTRIES}

timestamp()
{
  echo "TIMESTAMP: $( date +%s )"
}

while true
do
  timestamp
  read ENTRY
  echo ENTRY=${ENTRY}
  HASH=$( ${DE_BIN}/hash.pl ${ENTRY} )
  INDEX=$(( HASH % PADS_COUNT + 1 ))
  echo INDEX=${INDEX}
  NODE=${PADS_LIST[${INDEX}]}.pads.ci.uchicago.edu
  echo NODE=${NODE}
  rsync --size-only -av ${ENTRY} ${NODE}:/scratch/local/aps-rsync
done < ${ENTRIES} |& tee -a ${LOG}

echo "DONE."

timestamp
