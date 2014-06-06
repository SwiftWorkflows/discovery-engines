#!/bin/bash

set -eux

DIR=$( cd $(dirname $0) ; /bin/pwd )
NFHEDM_INSTALL=$( cd ${DIR}/.. ; /bin/pwd )

if [[ ! -f ${NFHEDM_INSTALL}/pkgIndex.tcl ]]
then
  echo "Swift NFHEDM package does not exist!"
  exit 1
fi

export TURBINE_USER_LIB=${NFHEDM_INSTALL}/lib

stc ${DIR}/test-1.swift

export TURBINE_LOG=0
turbine ${DIR}/test-1.tcl


