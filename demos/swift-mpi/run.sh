#!/bin/sh

set -eu

DATAFILE="sample.data"
if [ ! -f ${DATAFILE} ]
then
  echo "Creating data file: ${DATAFILE}"
  turbine-write-doubles ${DATAFILE} 1 2 3 4
fi

stc -u demo.swift

export TURBINE_USER_LIB=${PWD}
export ADLB_DEBUG=0
export TURBINE_LOG=0

turbine -l -n 6 demo.tcl --data=${DATAFILE}
