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

SETTINGS=0
while getopts "hs:" OPTION
do
  case ${OPTION} in
    h) usage ; exit 0     ;;
    s) SETTINGS=${OPTARG} ;;
  esac
done
shift $((OPTIND-1))

if [[ ${#*} != 3 ]]
then
  usage
  exit 1
fi

PARAMETERS=$1
START=$2
STOP=$3

crash()
{
  MSG=$1
  echo ${MSG}
  exit 1
}

if [[ ${SETTINGS} != 0 ]]
then
  source ${SETTINGS} || crash "Settings file failed: ${SETTINGS}"
fi

E2E_DIR=$(    cd $( dirname $0 )  ; /bin/pwd )
NFHEDM_DIR=$( cd ${E2E_DIR}/..    ; /bin/pwd )
NFHEDM_SWIFT_DIR=${NFHEDM_DIR}/swift

echo
echo "RUNNING CONTROL.SH ${*}"
echo

# Extract parameters from the parameters file
DATA=$( awk '$1 ~ /^DataDirectory/ { print $2 }' ${PARAMETERS} )
echo "DATA=${DATA}"
REDUCED=$( awk '$1 ~ /^ReducedFileName/ { print $2 }' ${PARAMETERS} )
echo "REDUCED=${REDUCED}"
REDUCED_DIR=$( dirname ${REDUCED} )
REDUCED_CONVERTED_DIR=${DATA/NFTest/NFTest/converted}

# Make a unique control directory
CONTROL_ID=$( date "+%Y-%m-%d_%H:%M" )
CONTROL_DIR=${HOME}/nfhedm-control/${CONTROL_ID}
i=1
while [[ -d ${CONTROL_DIR} ]]
do
  CONTROL_DIR=${HOME}/nfhedm-control/${CONTROL_ID}_$((++i))
done
export CONTROL_DIR
mkdir -pv ${CONTROL_DIR}

# Set PATH
PATH=${HOME}/swift-t/turbine/bin:${PATH}
PATH=${HOME}/wozniak/sfw/nfhedm/bin:${PATH}

# We want to handle errors in the stages
set +e

# Run a significant stage of the workflow
run_stage()
{
  STAGE=$1
  shift
  echo
  echo "STAGE: ${STAGE}"
  echo "RUN:   ${*}"
  echo
  local START=$( date +%s )
  LOG=${CONTROL_DIR}/stage-${STAGE}.log

  # Run the stage:
  ${*} >& ${LOG}
  # Check for errors:
  [[ ${?} == 0 ]] || crash "STAGE: ${STAGE} failed: see ${LOG}"

  local STOP=$( date +%s )
  echo "STAGE: ${STAGE}: DURATION: $((STOP-START)) seconds"
}

# run_stage 1:RUN_IMAGES ${NFHEDM_SWIFT_DIR}/runImages.sh ${NFHEDM_SWIFT_DIR} ${PARAMETERS}

# run_stage 3:CONVERT ${NFHEDM_SWIFT_DIR}/convert-bin-l2n.sh \
#   ${NFHEDM_SWIFT_DIR} ${DATA} ${REDUCED_DIR}

run_stage 4:REMOTE ${E2E_DIR}/run-remote.sh \
  ${REDUCED_CONVERTED_DIR}/${REDUCED_DIR} ${REDUCED_DIR} ${PARAMETERS} ${START} ${STOP}
