#!/bin/sh

# Simple workaround to accept/ignore background TIF

DE_BIN=$( cd $( dirname $0 ) ; /bin/pwd )

MERGE_TIFFS_PY=${DE_BIN}/merge-tiffs.py

# Drop background TIF: its location is implied
shift

${MERGE_TIFFS_PY} ${*}
