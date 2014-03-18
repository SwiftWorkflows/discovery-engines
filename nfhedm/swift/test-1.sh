#!/bin/bash

set -e

stc swift/test-1.swift

export TURBINE_USER_LIB=${PWD}

if [[ ! -f pkgIndex.tcl ]]
then
  echo "Tcl FOP package does not exist!"
  exit 1
fi

turbine swift/test-1.tcl


