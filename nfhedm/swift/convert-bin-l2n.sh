#!/bin/bash -eu

usage()
{
  echo "convert-bin-l2n.sh <SWIFT SCRIPT DIRECTORY> <DATA DIRECTORY> <REDUCED NAME>"
}

if [[ ${#*} != 3 ]]
then
  usage
  exit 1
fi

SWIFT_SCRIPT_DIR=$1
DATA=$2
REDUCED_DIR=$3

set -x

mkdir -pv ${DATA}/converted/${REDUCED_DIR}

stc -u ${SWIFT_SCRIPT_DIR}/convert-bin-l2n.swift
export TURBINE_LOG=0
turbine ${SWIFT_SCRIPT_DIR}/convert-bin-l2n.tcl --data=${DATA} --reduced=${REDUCED_DIR}
