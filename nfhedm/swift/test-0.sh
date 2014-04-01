#!/bin/bash

set -e

DIR=$( cd $(dirname $0) ; /bin/pwd )

stc ${DIR}/test-0.swift

turbine ${DIR}/test-0.tcl -a Params.txt ANYTHING
