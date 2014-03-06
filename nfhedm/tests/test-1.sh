#!/bin/bash

set -e

NFHEDM_HOME=$( cd $(dirname $0)/.. ; /bin/pwd )

cd ${NFHEDM_HOME}/data

${VALGRIND} ../bin/fop-nlopt Parameters.txt 135
