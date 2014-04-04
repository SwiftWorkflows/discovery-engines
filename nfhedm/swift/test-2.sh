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

DIR=$( cd $(dirname $0) ; /bin/pwd )
NFHEDM_INSTALL=$( cd ${DIR}/.. ; /bin/pwd )

if [[ ! -f ${NFHEDM_INSTALL}/lib/pkgIndex.tcl ]]
then
  echo "Swift NFHEDM package does not exist!"
  exit 1
fi
export TURBINE_USER_LIB=${NFHEDM_INSTALL}/lib

stc swift/test-2.swift

DATA=${HOME}/wozniak/data
cd ${DATA}
echo "PWD: $(pwd)"
export TURBINE_LOG=0
turbine -l ${NFHEDM_INSTALL}/swift/test-2.tcl -p=${PARAMETERS} ${START} ${END}
