#!/bin/bash

set -e

stc swift/test-0.swift

turbine swift/test-0.tcl -a Params.txt ANYTHING




