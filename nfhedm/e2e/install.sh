#!/bin/bash -eu

if [[ ${#*} == 1 ]]
then
  INSTALL=$1
else
  INSTALL=${HOME}/wozniak/sfw/e2e
fi

E2E_DIR=$(    cd $( dirname $0 )  ; /bin/pwd )
NFHEDM_DIR=$( cd ${E2E_DIR}/..    ; /bin/pwd )
NFHEDM_SWIFT_DIR=${NFHEDM_DIR}/swift

mkdir -pv ${INSTALL}/swift
cp -uv *.sh ${INSTALL}
