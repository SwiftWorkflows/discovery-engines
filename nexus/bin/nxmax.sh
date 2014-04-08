#!/bin/sh -eu

# NXMAX.SH
# args: <directory> <nxs> <nxmax.out>

if [[ ${#*} != 3 ]]
then
  echo "nxmax.sh: requires 3 args!"
  exit 1
fi

DE=$( cd $( dirname $0 )/../.. ; /bin/pwd )
source ${DE}/bin/python-settings.sh

DIRECTORY=$1
NXS=$2
NXMAX_OUT=$3

NXMAX_COMPLETE=${NXMAX_OUT}.complete
if [[ -f ${NXMAX_COMPLETE} ]]
then
  echo "nxmax.sh: found ${NXMAX_COMPLETE} - exiting"
  exit 0
fi

# Find nxmax in PATH
NXMAX=nxmax

${NXMAX} -d ${DIRECTORY} -f ${NXS} | tee ${NXMAX_OUT}
CODE=${?}

echo "CODE: ${CODE}" >> ${NXMAX_OUT}

if [[ ${CODE} != 0 ]]
then
  echo "nxmax.sh: command failed: $0 ${*}"
  exit 1
else
  touch ${NXMAX_COMPLETE}
fi

echo NXMAX DONE: $( date "+%m/%d/%Y %I:%M:%S%p" )

# Let Swift continue...
exit 0
