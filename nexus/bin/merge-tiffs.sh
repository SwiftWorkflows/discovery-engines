#!/bin/sh

# Simple workaround to accept/ignore background TIF

set -x

pwd

DE=$( cd $( dirname $0 )/../.. ; /bin/pwd )
source ${DE}/bin/python-settings.sh

MERGE_TIFFS_PY=${DE}/nexus/bin/merge-tiffs.py

# Drop background TIF: its location is implied
shift

${MERGE_TIFFS_PY} ${*}
CODE=${?}

echo "CODE: ${CODE}"

if [[ ${CODE} != 0 ]]
then
  echo "NEXUS_ERROR: command failed: $0 $*"
  exit 1
fi

# set -e

which catalog.py

# Let Swift continue...
exit 0
