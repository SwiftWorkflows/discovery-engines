#!/bin/sh
set -eu

SWIFT=/home/wozniak/Public/sfw/stc/bin/swift-t
export TURBINE_LOG=0
$SWIFT cctw.swift $*
