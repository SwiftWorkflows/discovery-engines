#!/bin/sh -eu

# NXMAX.SH
# args: <directory> <nxs> <nxmax.out>

if [[ ${#*} != 3 ]]
then
  echo "nxmax.sh: requires 3 args!"
  exit 1
fi

DIRECTORY=$1
NXS=$2
NXMAX_OUT=$3

NXMAX_COMPLETE=${NXMAX_OUT}.complete
if [[ -f ${NXMAX_COMPLETE} ]]
then
  echo "nxmax.sh: found ${NXMAX_COMPLETE} - exiting"
  exit 0
fi

DE_BIN=$( cd $( dirname $0 ) ; /bin/pwd )

NXMAX=${HOME}/.local/bin/nxmax

${NXMAX} -d ${DIRECTORY} -f ${NXS} > ${NXMAX_OUT} 2>&1
CODE=${?}

echo "CODE: ${CODE}" >> ${NXMAX_OUT}

if [[ ${CODE} != 0 ]]
then
  echo "nxmax.sh: command failed: $0 ${*}"
else
  touch ${NXMAX_COMPLETE}
fi

# Let Swift continue...
exit 0
