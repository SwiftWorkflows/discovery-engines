#!/bin/bash -eu

# PATH=~wilde/swift/rev/swift-0.94.1/bin:$PATH

PATH=/home/wozniak/Downloads/swift-0.94.1/bin:${PATH}

if [[ ${#*} != 1 ]]
then
  echo "Provide a parameters file!"
  exit 1
fi

PARAMETERS_FILE=$1

NDISTANCES=$( awk '$1 ~ /^nDistances/ { print $2 }' ${PARAMETERS_FILE} )
NRFILESPERLAYER=$( awk '$1 ~ /^NrFilesPerLayer/ { print $2 }' ${PARAMETERS_FILE} )

echo "PARAMETERS_FILE: ${PARAMETERS_FILE}"
echo "NDISTANCES:      ${NDISTANCES}"
echo "NRFILESPERLAYER: ${NRFILESPERLAYER}"

set -x
swift -sites.file sites-etc.xml -tc.file tc -config cf ProcessImages.swift \
  -paramfile=${PARAMETERS_FILE} \
  -NrLayers=${NDISTANCES}       \
  -NrFilesPerLayer=${NRFILESPERLAYER} 

# -sites.file sites-wozniak.xml 
