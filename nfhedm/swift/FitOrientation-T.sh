#!/bin/bash -eu

# FitOrientation-T
# Runs FitOrientation-T.swift on the local machine

if [[ ${#*} != 5 ]]
then
  echo "usage: FitOrientation-T <DATA DIRECTORY> <PARAMETERS FILE> <START ROWN> <END ROWN> <MICROSTRUCTURE>"
  echo "The parameters file may be relative to the data directory or absolute."
  exit 1
fi

echo "FitOrientation-T.sh ..."

set -x

DATA=$1
PARAMETERS=$2
START=$3
END=$4
MICROSTRUCTURE=$5

DIR=$( cd $(dirname $0) ; /bin/pwd )
NFHEDM_INSTALL=${HOME}/sfw/nfhedm

if [[ ! -f ${NFHEDM_INSTALL}/lib/pkgIndex.tcl ]]
then
  echo "Swift NFHEDM package does not exist!"
  exit 1
fi

export TURBINE_USER_LIB=${NFHEDM_INSTALL}/lib

SCRIPT=FitOrientation-T

stc -u -t checkpointing ${DIR}/${SCRIPT}.swift

export MODE=BGQ
export PROCS=${PROCS:-2}
export PROJECT=ExM
export QUEUE=default
export TURBINE_LOG=1
export TURBINE_DEBUG=1
export WALLTIME=${WT:-15}
export PPN=4

cd ${DATA}
echo "PWD: $(pwd)"

if [[ ${PARAMETERS} == /* ]]
then
  PARAMETERS_PATH=${PARAMETERS}
else
  PARAMETERS_PATH=${DATA}/${PARAMETERS}
fi
set -x
turbine -n ${PROCS} ${DIR}/${SCRIPT}.tcl \
  -p=${PARAMETERS_PATH} -m=${MICROSTRUCTURE} ${START} ${END}
