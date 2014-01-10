#!/bin/sh

set -eu

stc demo.swift

export ADLB_DEBUG=0
export TURBINE_LOG=0

turbine -n 6 demo.tcl
