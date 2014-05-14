#!/bin/bash -eu

set -x

START=$1
STOP=$2

REMOTE_HOST=cetus.alcf.anl.gov
REMOTE_USER=wozniak
REMOTE=${REMOTE_USER}@${REMOTE_HOST}

REMOTE_HOME=/home/${REMOTE_USER}
REMOTE_SCRIPT=${REMOTE_HOME}/proj/ppc64/d-e/nfhedm/swift/FitOrientation-T.sh

REMOTE_DATA=${REMOTE_HOME}/Au_Reduced2-converted
REMOTE_PARAMS=${REMOTE_DATA}/ParametersGoldApril14.txt

ssh ${REMOTE} ${REMOTE_SCRIPT}  ${REMOTE_DATA} ${REMOTE_PARAMS} ${START} ${STOP}
 