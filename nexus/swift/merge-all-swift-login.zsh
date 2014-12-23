#!/bin/zsh

set -eu

export ADLB_PRINT_TIME=1

usage()
{
  print "merge-all-swift-tukey-login.zsh <PROCS> <DATA> <LIST>"
}

if [[ ${#*} != 3 ]]
then
  usage
  exit 1
fi

PROCS=$1
DATA=$2
LIST=$3

# Construct Discovery Engines directories:
DE_SWIFT=$( cd $( dirname $0 ) ; /bin/pwd )
DE=$( dirname ${DE_SWIFT} )
DE_BIN=${DE}/bin

# Use location of merge-tiffs.py
PATH=${DE_BIN}:${PATH}

# This is just a test for success
cd ${DATA}
cd -

swift-t -l -n ${PROCS} \
  ${DE_SWIFT}/merge-all.swift --data=${DATA} --list=${LIST}
