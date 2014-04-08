#!/bin/bash

set -e

if [[ ${#*} != 3 ]]
then
  echo "usage: test-2 <PARAMETERS FILE> <START ROWN> <END ROWN>"
  exit 1
fi

PARAMETERS=$1
START=$2
END=$3

stc -r ${PWD} swift/test-2.swift

# export TURBINE_USER_LIB=${PWD}

if [[ ! -f pkgIndex.tcl ]]
then
  echo "Tcl FOP package does not exist!"
  exit 1
fi

NFHEDM_HOME=$( cd $(dirname $0)/.. ; /bin/pwd )

cd ${NFHEDM_HOME}/data

echo "PWD: $(pwd)"

export TURBINE_LOG=0
export MODE=cluster
export PROJECT=ExM
export QUEUE=pubnet
turbine-cobalt-run.zsh -n 3 ${NFHEDM_HOME}/swift/test-2.tcl \
 -p=${PARAMETERS} ${START} ${END}
