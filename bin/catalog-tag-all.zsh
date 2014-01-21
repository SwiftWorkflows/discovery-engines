#!/bin/zsh -eu

ENDPOINT=$1
shift
DIRS=( ${*} )

if [[ -z ${DEFAULT_CATALOG_ID} ]]
then
  print "Set DEFAULT_CATALOG_ID"
  exit 1
fi

DE_HOME=$( cd $( dirname $0 )/.. && /bin/pwd )
if [[ ${?} != 0 || ${DE_HOME} == "" ]]
then
  print "Could not find DE_HOME!"
  exit 1
fi

source ${DE_HOME}/lib/helpers.zsh
if [[ ${?} != 0 ]]
then
  print "Could not use DE_HOME!"
  exit 1
fi

source ${DE_HOME}/lib/catalog.zsh

KEYS=( "host" "path" "size" "date" "temperature" "sample" \
       "PI" "date" "beamline" )
for KEY in ${KEYS}
do
  # This fails if the annotation exists
  catalog create_annotation_def ${DEFAULT_CATALOG_ID} ${KEY} "text" || true
done

CATALOG_TAG=${DE_HOME}/bin/catalog-tag-dataset.zsh

for D in ${DIRS}
do
  ${CATALOG_TAG} ${ENDPOINT} ${D}
done

print "catalog-tag-all: DONE."
