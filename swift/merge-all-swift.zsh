#!/bin/zsh

set -eu

usage()
{
  print "merge-all-swift.zsh <PROCS> <DATA>"
}

if [[ ${#*} != 2 ]]
then
  usage
  exit 1
fi

PROCS=$1
DATA=$2

# Construct Discovery Engines directories:
DE_SWIFT=$( dirname $0 )
DE=$( dirname ${DE_SWIFT} )
DE_BIN=${DE}/bin

# Use location of merge-tiffs.py
PATH=${DE_BIN}:${PATH}

stc -u ${DE_SWIFT}/merge-all.swift

cd ${DATA}

turbine -l -n ${PROCS} ${DE_SWIFT}/merge-all.tcl --data=${DATA}
