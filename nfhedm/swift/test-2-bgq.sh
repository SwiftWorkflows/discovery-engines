#!/bin/bash -eu

if [[ ${#*} != 3 ]]
then
  echo "usage: test-2 <PARAMETERS FILE> <START ROWN> <END ROWN>"
  exit 1
fi

PARAMETERS=$1
START=$2
END=$3

DIR=$( cd $(dirname $0) ; /bin/pwd )
NFHEDM_INSTALL=${HOME}/sfw/ppc64/nfhedm

if [[ ! -f ${NFHEDM_INSTALL}/lib/pkgIndex.tcl ]]
then
  echo "Swift NFHEDM package does not exist!"
  exit 1
fi

export TURBINE_USER_LIB=${NFHEDM_INSTALL}/lib

TEST=test-2

stc -t checkpointing ${DIR}/${TEST}.swift

export MODE=BGQ
export PROJECT=ExM
export QUEUE=default
export TURBINE_LOG=1

DATA=${PWD}/data
cd ${DATA}
echo "PWD: $(pwd)"
turbine-cobalt-run.zsh -n 3 ${DIR}/${TEST}.tcl \
  -p=${DATA}/${PARAMETERS} ${START} ${END}
