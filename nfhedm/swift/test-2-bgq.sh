#!/bin/bash -eu

if [[ ${#*} != 4 ]]
then
  echo "usage: test-2 <DATA DIRECTORY> <PARAMETERS FILE> <START ROWN> <END ROWN>"
  echo "The parameters file may be relative to the data directory or absolute."
  exit 1
fi

DATA=$1
PARAMETERS=$2
START=$3
END=$4

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
export TURBINE_DEBUG=1
export WALLTIME=${WT:-15}

cd ${DATA}
echo "PWD: $(pwd)"

if [[ ${PARAMETERS} == /* ]]
then
  PARAMETERS_PATH=${PARAMETERS}
else
  PARAMETERS_PATH=${DATA}/${PARAMETERS}
fi
set -x
which turbine-cobalt-run.zsh
turbine-cobalt-run.zsh -n 3 ${DIR}/${TEST}.tcl \
  -p=${PARAMETERS_PATH} ${START} ${END}
