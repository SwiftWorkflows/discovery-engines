#!/bin/bash

set -e

stc swift/test-1.swift

export TURBINE_USER_LIB=${PWD}

if [[ ! -f pkgIndex.tcl ]]
then
  echo "Swift NFHEDM package does not exist!"
  exit 1
fi

export TURBINE_LOG=0
turbine swift/test-1.tcl


