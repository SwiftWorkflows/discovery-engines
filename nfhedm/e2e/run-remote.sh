#!/bin/bash -eu

# RUN-REMOTE.SH

# Run this from Orthros to launch remote execution on the BG/Q
# You should already have a shared SSH session to REMOTE_HOST

usage()
{
  echo   ""
  echo   "run-remote-swift-t.zsh: usage: "
  echo   ""
  printf "\t run-remote-swift-t.zsh <LOCAL DATA DIRECTORY> <START> <STOP>\n"
  echo   ""
  echo   "Environment:"
  local W=-15 # Field width
  echo   ""
  printf "\t %${W}s Set the remote hostname\n"  REMOTE_HOST:
  printf "\t %${W}s Set the remote username\n"  REMOTE_USER:
  printf "\t %${W}s Set the script file\n"      REMOTE_SCRIPT:
  printf "\t %${W}s Set the data directory\n"   REMOTE_DATA:
  printf "\t %${W}s Set the parameters file\n"  REMOTE_PARAMS:
  echo   ""
}

if [[ ${#*} != 3 ]]
then
  usage
  exit 1
fi

DATA=$1   # E.g., /data/tomo1/NFTest/Au_Reduced2-converted
START=$2
STOP=$3

REMOTE_HOST=${REMOTE_HOST:-cetus.alcf.anl.gov}
REMOTE_USER=${REMOTE_USER:-wozniak}
REMOTE=${REMOTE_USER}@${REMOTE_HOST}

REMOTE_HOME=/home/${REMOTE_USER}
REMOTE_SCRIPT=${REMOTE_HOME}/proj/ppc64/d-e/nfhedm/swift/FitOrientation-T.sh

REMOTE_DATA=${REMOTE_DATA:-${REMOTE_HOME}/Au_Reduced2-converted}
REMOTE_PARAMS=${REMOTE_PARAMS:-${REMOTE_DATA}/ParametersGoldApril14.txt}

set -x

rsync -avz ${DATA}/ ${REMOTE}:${REMOTE_DATA}

ssh ${REMOTE} ${REMOTE_SCRIPT} ${REMOTE_DATA} ${REMOTE_PARAMS} ${START} ${STOP}
