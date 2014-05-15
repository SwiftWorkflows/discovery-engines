#!/bin/bash -eu

python generate_hdf.py cfg.py cfg.hdf cfg.sh

source ./cfg.sh
stc ga-1.swift
turbine ga-1.tcl --individuals=$individuals --ngenerations=$ngenerations
