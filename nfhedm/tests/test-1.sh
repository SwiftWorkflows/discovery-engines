#!/bin/bash

set -e

if [[ ${#*} != 1 ]]
then
  echo "Not given: PARAMETERS"
  exit 1
fi

PARAMETERS=$1

NFHEDM_HOME=$( cd $(dirname $0)/.. ; /bin/pwd )

# DATA=${NFHEDM_HOME}/data
DATA=${HOME}/wozniak/data

cd ${DATA}

echo "PWD: $(pwd)"
${VALGRIND} ${NFHEDM_HOME}/bin/fo-nlopt ${PARAMETERS} 135
