#!/bin/zsh

set -eu

usage()
{
  print "merge-cbfs-swift.zsh <PROCS> <DATA>"
}

if [[ ${#*} != 2 ]]
then
  usage
  exit 1
fi

PROCS=$1
# The sample directory
DATA=$2

if [[ ! -d ${DATA} ]]
then
  print "Directory does not exist: ${DATA}"
  exit 1
fi

# Construct Discovery Engines directories:
DE_SWIFT=$( cd $( dirname $0 ) ; /bin/pwd )
DE=$( dirname ${DE_SWIFT} )
DE_BIN=${DE}/bin

# Use location of merge-tiffs.py
PATH=${DE_BIN}:${PATH}

stc -u ${DE_SWIFT}/merge-cbfs.swift

export TURBINE_LOG=0
nice turbine -l -n ${PROCS} ${DE_SWIFT}/merge-cbfs.tic --data=${DATA}
