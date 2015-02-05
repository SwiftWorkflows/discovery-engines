#!/bin/zsh -eu

# FitOrientation-T
# Runs FitOrientation-T.swift on the BG/Q

if [[ ${#*} != 5 ]]
then
  echo "usage: test-2 <DATA DIRECTORY> <PARAMETERS FILE> <START ROWN> <END ROWN> <MICROSTRUCTURE>"
  echo "The parameters file may be relative to the data directory or absolute."
  exit 1
fi

DATA=$1
PARAMETERS=$2
START=$3
END=$4
MICROSTRUCTURE=$5

DIR=$( cd $(dirname $0) ; /bin/pwd )
NFHEDM_INSTALL=$( cd ${DIR}/.. ; /bin/pwd )

if [[ ! -f ${NFHEDM_INSTALL}/lib/pkgIndex.tcl ]]
then
  echo "Swift NFHEDM package does not exist!"
  exit 1
fi

export SWIFT_PATH=${NFHEDM_INSTALL}/lib

SCRIPT=FitOrientation-T

declare PROCS PPN TURBINE_LOG WALLTIME

# cd ${DATA}
# echo "PWD: $(pwd)"

if [[ ${PARAMETERS} == /* ]]
then
  PARAMETERS_PATH=${PARAMETERS}
else
  PARAMETERS_PATH=${DATA}/${PARAMETERS}
fi

# set -x
which swift-t
swift-t -m cobalt -n ${PROCS}                      \
  -s ${PWD}/swift/settings-bgq.sh                  \
  ${DIR}/${SCRIPT}.swift                           \
  -p=${PARAMETERS_PATH} -m=${MICROSTRUCTURE}       \
   ${START} ${END}

#   -e TURBINE_LEADER_HOOK="$( < ${DIR}/hook.tcl )"  \
