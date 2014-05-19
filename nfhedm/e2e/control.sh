#!/bin/bash -eu

# CONTROL.SH
# Master control script- runs the end-to-end workflow
# Notes are at:
#   https://docs.google.com/document/d/1SA9u6i3OUtjPF7nZoVMSZ2Dp-JxUaHqJqmTqr4EDN6s

usage()
{
  echo   ""
  echo   "control.sh: usage: "
  echo   ""
  printf "\t control.sh <PARAMETERS FILE> <START> <STOP>\n"
  echo   ""
  echo   "Environment:"
}

while getopts "h" OPTION
do
  case ${OPTION} in
    h)
      usage
      exit 0
      ;;
  esac
done

E2E_DIR=$(    cd $( dirname $0 )  ; /bin/pwd )
NFHEDM_DIR=$( cd ${E2E_DIR}/..    ; /bin/pwd )
NFHEDM_SWIFT_DIR=${NFHEDM_DIR}/swift



${NFHEDM_SWIFT_DIR}/runImages.sh

# convert data endianism

# ${E2E_DIR}/run-remote.sh

