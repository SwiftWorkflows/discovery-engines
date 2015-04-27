#!/bin/zsh -f
set -eu

zparseopts -D -E D:=D

set -x

cd ${D[2]}
exec nice -n 15 nxmerge ${*}
