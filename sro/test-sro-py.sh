#!/bin/sh
set -eu

THIS=$( dirname $0 )
export PYTHONPATH=$THIS

python test-sro.py $*
