#!/bin/bash -eu

usage()
{
  echo "usage: <PARAMETERS> <MICROSTRUCTURE>"
}

if [[ ${#*} != 2 ]]
then
  usage
  exit 1
fi

PARAMETERS=$1
MICROSTRUCTURE=$2

NFHEDM_HOME=$( cd $(dirname $0)/.. ; /bin/pwd )

# DATA=${NFHEDM_HOME}/data
# DATA=${HOME}/wozniak/data

# cd ${DATA}

VALGRIND=${VALGRIND:-}

echo "PWD: $(pwd)"
${VALGRIND} ${NFHEDM_HOME}/bin/fo-nlopt ${PARAMETERS} 135 ${MICROSTRUCTURE}
