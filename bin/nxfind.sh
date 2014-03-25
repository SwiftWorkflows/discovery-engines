#!/bin/sh -eu

# NXFIND.SH
# args: <directory> <nxs> <nxfind.out>

if [[ ${#*} != 3 ]]
then
  echo "nxfind.sh: requires 3 args!"
  exit 1
fi

DIRECTORY=$1
NXS=$2
NXFIND_OUT=$3

NXFIND_COMPLETE=${NXFIND_OUT}.complete
if [[ -f ${NXFIND_COMPLETE} ]]
then
  echo "nxfind.sh: found ${NXFIND_COMPLETE} - exiting"
  exit 0
fi

DE_BIN=$( cd $( dirname $0 ) ; /bin/pwd )

NXFIND=${HOME}/.local/bin/nxfind

${NXFIND} -d ${DIRECTORY} -f ${NXS} > ${NXFIND_OUT} 2>&1
CODE=${?}

echo "CODE: ${CODE}" >> ${NXFIND_OUT}

if [[ ${CODE} != 0 ]]
then
  echo "nxfind.sh: command failed: $0 ${*}"
else
  touch ${NXFIND_COMPLETE}
fi

# Let Swift continue...
exit 0
