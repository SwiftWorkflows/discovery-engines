#!/bin/sh -eu

# NXFIND.SH
# args: <directory> <nxs> <nxmax.out> <nxfind.out>

if [[ ${#*} != 4 ]]
then
  echo "nxfind.sh: requires 4 args!"
  exit 1
fi

DIRECTORY=$1
NXS=$2
NXMAX_OUT=$3 # This is not read - just for synchronization
NXFIND_OUT=$4

NXFIND_COMPLETE=${NXFIND_OUT}.complete
if [[ -f ${NXFIND_COMPLETE} ]]
then
  echo "nxfind.sh: found ${NXFIND_COMPLETE} - exiting"
  exit 0
fi

DE=$( cd $( dirname $0 )/.. ; /bin/pwd )
source ${DE}/bin/python-settings.sh

# Use nxfind in PATH
NXFIND=nxfind

NXS_FILE=$( basename ${NXS} )
${NXFIND} -d ${DIRECTORY} -f ${NXS_FILE} 2>&1 | tee ${NXFIND_OUT} 
CODE=${?}

echo "CODE: ${CODE}" | tee -a ${NXFIND_OUT}

if [[ ${CODE} != 0 ]]
then
  echo "nxfind.sh: command failed: $0 ${*}"
  exit ${CODE}
else
  touch ${NXFIND_COMPLETE}
fi

# Let Swift continue...
exit 0
