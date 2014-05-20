#!/bin/bash -eu

PATH=~wilde/swift/rev/swift-0.94.1/bin:$PATH

# PATH=/home/wozniak/Downloads/swift-0.94.1/bin:${PATH}

usage()
{
  echo "usage: runImages.sh <SWIFT SCRIPTS DIRECTORY> <PARAMETERS FILE>"
}

if [[ ${#*} != 2 ]]
then
  usage
  exit 1
fi

SWIFT_SCRIPTS_DIR=$1
PARAMETERS_FILE=$2

NDISTANCES=$( awk '$1 ~ /^nDistances/ { print $2 }' ${PARAMETERS_FILE} )
NRFILESPERLAYER=$( awk '$1 ~ /^NrFilesPerLayer/ { print $2 }' ${PARAMETERS_FILE} )

echo "PARAMETERS_FILE: ${PARAMETERS_FILE}"
echo "NDISTANCES:      ${NDISTANCES}"
echo "NRFILESPERLAYER: ${NRFILESPERLAYER}"

set -x
swift -sites.file ${SWIFT_SCRIPTS_DIR}/sites.xml             \
      -tc.file    ${SWIFT_SCRIPTS_DIR}/tc                    \
      -config     ${SWIFT_SCRIPTS_DIR}/cf                    \
                  ${SWIFT_SCRIPTS_DIR}/ProcessImages.swift   \
      -paramfile=${PARAMETERS_FILE}                          \
      -NrLayers=${NDISTANCES}                                \
      -NrFilesPerLayer=${NRFILESPERLAYER}

# -sites.file sites-wozniak.xml
