#!/bin/sh
set -eu

THIS=$( dirname $0 )
SRO_HOME=$( cd $THIS/../.. ; /bin/pwd )
export PYTHONPATH=$SRO_HOME/lib:$SRO_HOME/py

echo $PYTHONPATH

SCRIPT=$1
shift
python $THIS/$SCRIPT $*
