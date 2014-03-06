#!/bin/bash

set -e

NFHEDM_HOME=$( cd $(dirname $0)/.. ; /bin/pwd )

cd ${NFHEDM_HOME}/data

../bin/fop-nlopt Parameters.txt 135
