#!/bin/bash

set -ex

NFHEDM_INSTALL=${HOME}/sfw/nfhedm
DATA=${HOME}/proj/d-e/nfhedm/data

if [[ ${#*} != 3 ]]
then
  echo "usage: test-2 <PARAMETERS FILE> <START ROWN> <END ROWN>"
  exit 1
fi

PARAMETERS=$1
START=$2
END=$3

TEST_DIR=$(  cd $(dirname $0)  ; /bin/pwd )
SWIFT_DIR=$( cd ${TEST_DIR}/.. ; /bin/pwd )

if [[ ! -f ${NFHEDM_INSTALL}/lib/pkgIndex.tcl ]]
then
  echo "Swift NFHEDM package does not exist!"
  exit 1
fi
export TURBINE_USER_LIB=${NFHEDM_INSTALL}/lib

stc ${TEST_DIR}/test-2.swift

cd ${DATA}
echo "PWD: $(pwd)"
export TURBINE_LOG=0
turbine -l ${TEST_DIR}/test-2.tcl -p=${PARAMETERS} ${START} ${END}
