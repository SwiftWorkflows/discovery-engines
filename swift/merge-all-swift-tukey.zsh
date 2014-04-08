#!/bin/zsh

set -eu

export MODE="cluster"
export PROJECT="ExM"
export QUEUE="default"
export ADLB_PRINT_TIME=1

usage()
{
  print "merge-all-swift-tukey.zsh <PROCS> <DATA> <LIST>"
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

stc -u ${DE_SWIFT}/merge-all.swift

# This is just a test for success
cd ${DATA}
cd -

typeset -T LD_LIBRARY_PATH ld_library_path
export LD_LIBRARY_PATH
ld_library_path=( /soft/compilers/gcc/4.8.1/lib
  /gpfs/mira-fs0/software/x86_64/compilers/gcc/4.8.1/lib64 )

typeset LD_LIBRARY_PATH

set -x

turbine-cobalt-run.zsh -t 12:00:00 -n ${PROCS} -C ${DATA} \
  -e LD_LIBRARY_PATH=${LD_LIBRARY_PATH} \
  ${DE_SWIFT}/merge-all.tcl --data=${DATA} --list=${LIST}
