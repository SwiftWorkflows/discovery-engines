#!/bin/zsh

set -eu

export MODE="cluster"
export PROJECT="ExM"
export QUEUE="default"
export ADLB_PRINT_TIME=1

usage()
{
  print "merge-all-swift-tukey.zsh <PROCS> <DATA>"
}

if [[ ${#*} != 2 ]]
then
  usage
  exit 1
fi

PROCS=$1
DATA=$2

# Construct Discovery Engines directories:
DE_SWIFT=$( cd $( dirname $0 ) ; /bin/pwd )
DE=$( dirname ${DE_SWIFT} )
DE_BIN=${DE}/bin

# Use location of merge-tiffs.py
PATH=${DE_BIN}:${PATH}

stc -u ${DE_SWIFT}/merge-all.swift

# This is just a test for success
cd ${DATA}
cd -

turbine-cobalt-run.zsh -t 7 -n ${PROCS} -C ${DATA} \
  ${DE_SWIFT}/merge-all.tcl --data=${DATA}
