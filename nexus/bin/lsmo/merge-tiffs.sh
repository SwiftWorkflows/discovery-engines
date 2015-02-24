#!/bin/sh

# Simple workaround to accept/ignore background TIF

set -x

pwd

DE=$( cd $( dirname $0 )/../.. ; /bin/pwd )
source ${DE}/bin/python-settings-nxrs.sh

MERGE_TIFFS_PY="nxmerge"

${MERGE_TIFFS_PY} ${*}
CODE=${?}

echo "CODE: ${CODE}"

if [[ ${CODE} != 0 ]]
then
  echo "NEXUS_ERROR: command failed: $0 $*"
  exit 1
fi

ARGS=( ${*} )
for (( i=0 ; i<${#*} ; i++ ))
do
  if [[ ${ARGS[i]} == "-d" ]]
    then
    DATASET_NAME=${ARGS[i+1]}
  fi
  if [[ ${ARGS[i]} == "-p" ]]
    then
    PREFIX=${ARGS[i+1]}
  fi
done

# catalog.py -stn add_dataset_annotation ${DATASET_NAME}

### NEED TO ADD A MEMBER FOR EACH PREFIX.NXS ???

# Let Swift continue...
exit 0
