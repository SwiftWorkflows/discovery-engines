#!/bin/bash

set -eu

DIR=$( cd $(dirname $0) ; /bin/pwd )
NFHEDM_INSTALL=${HOME}/sfw/ppc64/nfhedm

if [[ ! -f ${NFHEDM_INSTALL}/lib/pkgIndex.tcl ]]
then
  echo "Swift NFHEDM package does not exist!"
  exit 1
fi

export TURBINE_USER_LIB=${NFHEDM_INSTALL}/lib

stc -t checkpointing ${DIR}/test-1.swift

export MODE=BGQ
export PROJECT=ExM
export QUEUE=default
export TURBINE_LOG=1
turbine-cobalt-run.zsh -n 4 ${DIR}/test-1.tcl
