#!/bin/zsh
set -eu

CREATE_DEFS=1
zparseopts -D -E d=d
(( ${#d} > 0 )) && CREATE_DEFS=0

DIRS=( ${*} )

if [[ -z ${GCAT_DEFAULT_CATALOG_ID} ]]
then
  print "Set GCAT_DEFAULT_CATALOG_ID"
  exit 1
fi

THIS=$( cd $( dirname $0 ) && /bin/pwd )
DE_HOME=$( cd ${THIS}/../../.. && /bin/pwd )
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

${DE_HOME}/nexus/bin/catalog-list-files | scan P
UNTAGGED=$(( ${#P} + 1 )) # Indicates not found
TAGGED=${P}
export TAGGED UNTAGGED

if (( CREATE_DEFS ))
then
  # Text
  KEYS=( "name" "PI" "host" "path" "size_human" "sample" \
    "beamline" )
  for KEY in ${KEYS}
  do
    # This fails if the annotation exists
    catalog.py create_annotation_def ${KEY} "text" || true
  done

  # Integers
  KEYS=( "year" "month" "size_bytes" )
  for KEY in ${KEYS}
  do
    # This fails if the annotation exists
    catalog.py create_annotation_def ${KEY} "int8" || true
  done

  # Floats
  KEYS=( "fraction" "temperature" )
  for KEY in ${KEYS}
  do
    # This fails if the annotation exists
    catalog.py create_annotation_def ${KEY} "float8" || true
  done

  # Dates
  catalog.py create_annotation_def "date" "timestamptz" || true
fi

CATALOG_TAG=${THIS}/catalog-tag-dataset.zsh

for D in ${DIRS}
do
  if [[ ! -d ${D} ]]
  then
    print "No such directory: ${D}"
    exit 1
  fi
  push ${D}
  ${CATALOG_TAG} ${D}
  pop
done

print "catalog-tag-all: DONE."
