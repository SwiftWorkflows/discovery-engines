#!/bin/bash

set -e

if [[ ${#*} != 1 ]]
then
  echo "Not given: PARAMETERS"
  exit 1
fi

PARAMETERS=$1

NFHEDM_HOME=$( cd $(dirname $0)/.. ; /bin/pwd )

cd ${NFHEDM_HOME}/data

pwd 
${VALGRIND} ../bin/fo-nlopt ${PARAMETERS} 135
