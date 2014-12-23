#!/bin/zsh -eu

DIRS=( ${*} )

if [[ -z ${GCAT_DEFAULT_CATALOG_ID} ]]
then
  print "Set GCAT_DEFAULT_CATALOG_ID"
  exit 1
fi

DE_HOME=$( cd $( dirname $0 )/../.. && /bin/pwd )
if [[ ${?} != 0 || ${DE_HOME} == "" ]]
then
  print "Could not find DE_HOME!"
  exit 1
fi

declare DE_HOME

source ${DE_HOME}/lib/helpers.zsh
if [[ ${?} != 0 ]]
then
  print "Could not use DE_HOME!"
  exit 1
fi

source ${DE_HOME}/nexus/lib/catalog.zsh

KEYS=( "host" "path" "size_human" "sample" \
       "PI" "date" "beamline" )
for KEY in ${KEYS}
do
  # This fails if the annotation exists
  catalog.py create_annotation_def ${KEY} "text" || true
done

KEYS=( "year" "month" "temperature" "size_bytes" )
for KEY in ${KEYS}
do
  # This fails if the annotation exists
  catalog.py create_annotation_def ${KEY} "int8" || true
done

CATALOG_TAG=${DE_HOME}/nexus/bin/catalog-tag-dataset.zsh

for D in ${DIRS}
do
  ${CATALOG_TAG} ${D}
done

print "catalog-tag-all: DONE."
