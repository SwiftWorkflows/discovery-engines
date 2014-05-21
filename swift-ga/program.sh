#!/bin/bash -eu

python generate_hdf.py cfg.py cfg.hdf cfg.sh

source ./cfg.sh
stc ga-1.swift

# Create the base population winner
touch nobody.winner

export TURBINE_LOG=0
turbine -n 10 ga-1.tcl --individuals=$individuals --ngenerations=$ngenerations
