#!/bin/bash -eu

python generate_hdf.py cfg.py cfg.hdf cfg.sh

source ./cfg.sh
stc ga-1.swift

export TURBINE_LOG=0
turbine ga-1.tcl --individuals=$individuals --ngenerations=$ngenerations
