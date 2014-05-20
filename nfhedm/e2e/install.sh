#!/bin/bash -eu

if [[ ${#*} == 1 ]]
then
  INSTALL=$1
else
  INSTALL=${HOME}/wozniak/sfw/nfhedm
fi

E2E_DIR=$(    cd $( dirname $0 )  ; /bin/pwd )
NFHEDM_DIR=$( cd ${E2E_DIR}/..    ; /bin/pwd )
NFHEDM_SWIFT_DIR=${NFHEDM_DIR}/swift

mkdir -pv ${INSTALL}/{e2e,swift}
cp -uv {control,run-remote}.sh ${INSTALL}/e2e
SWIFT_FILES=(  ${NFHEDM_SWIFT_DIR}/{runImages.sh,ProcessImages.swift} )
SWIFT_FILES+=( ${NFHEDM_SWIFT_DIR}/{sites.xml,tc,cf} )
SWIFT_FILES+=( ${NFHEDM_SWIFT_DIR}/convert-bin-l2n.swift )
SWIFT_FILES+=( ${NFHEDM_SWIFT_DIR}/convert-bin-l2n.sh )
cp -uv ${SWIFT_FILES[@]} ${INSTALL}/swift

{
  echo "Installed by:   ${USER}"
  echo "Installed from: ${NFHEDM_DIR}"
  echo "Installed on:   $(date)"
} > ${INSTALL}/install.txt
