#!/bin/bash

set -e

if [[ ${#*} != 3 ]]
then
  echo "Not given: PARAMETERS"
  exit 1
fi

PARAMETERS=$1
START=$2
END=$3

stc swift/test-2.swift

export TURBINE_USER_LIB=${PWD}

if [[ ! -f pkgIndex.tcl ]]
then
  echo "Tcl FOP package does not exist!"
  exit 1
fi

NFHEDM_HOME=$( cd $(dirname $0)/.. ; /bin/pwd )

cd ${NFHEDM_HOME}/data

echo "PWD: $(pwd)"

export TURBINE_LOG=0
turbine -n 3 ${NFHEDM_HOME}/swift/test-2.tcl -p=${PARAMETERS} ${START} ${END}
