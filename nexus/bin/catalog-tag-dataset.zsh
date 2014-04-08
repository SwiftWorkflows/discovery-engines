#!/bin/zsh -eu

if [[ -z ${GCAT_DEFAULT_CATALOG_ID} ]]
then
  print "Set GCAT_DEFAULT_CATALOG_ID"
  exit 1
fi

usage()
{
  print "usage: catalog-tag-dataset.zsh <DIRECTORY>"
}

if [[ ${#*} != 1 ]]
then
  usage
  exit 1
fi

DIR=$1

DE_HOME=$( cd $( dirname $0 )/../.. && /bin/pwd )
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

cd ${DIR}

# Get absolute path
DIR=$( /bin/pwd )

NAME=$( basename ${DIR} )

declare NAME

DATASET_ID=$( catalog.py create_dataset ${NAME} )

declare DATASET_ID

NEWEST_FILE=$( print *(om[1]) )
DATE=$( stat ${NEWEST_FILE} | awk '$1 == "Modify:" { print $2 $3 }' )
TEMPERATURE=${NAME#*_}
declare DATE TEMPERATURE
print "Getting size..."
SIZE=$( du -s . | zclm 1 )

catalog-annotate-dataset ${DATASET_ID} name:${NAME}
catalog-annotate-dataset ${DATASET_ID} host:mira
catalog-annotate-dataset ${DATASET_ID} path:${DIR}
catalog-annotate-dataset ${DATASET_ID} date:${DATE}
catalog-annotate-dataset ${DATASET_ID} temperature:${TEMPERATURE}
catalog-annotate-dataset ${DATASET_ID} sample:LSMO
catalog-annotate-dataset ${DATASET_ID} PI:"Ray Osborn"
catalog-annotate-dataset ${DATASET_ID} beamline:"ANL APS Sector 6"
catalog-annotate-dataset ${DATASET_ID} size:${SIZE}

catalog-acl-add ${DATASET_ID} "*" "r"
