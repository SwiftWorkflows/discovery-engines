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

DATASET_ID=$( catalog.py create_dataset name:${NAME} )

declare DATASET_ID

NEWEST_FILE=$( print *(om[1]) )
DATE=$( stat ${NEWEST_FILE} | awk '$1 == "Modify:" { print $2 " " $3 }' )
DATE=${DATE/.[0-9]*/} # Chop off subsecond resolution
TEMPERATURE=${NAME#*_}
TEMPERATURE=${TEMPERATURE%k}
# declare DATE TEMPERATURE
print "Getting size (${PWD})..."
SIZE_BYTES=$( command du -s  . | zclm 1 )
# declare SIZE_BYTES
SIZE_HUMAN=$( bformat ${SIZE_BYTES} ; true )

# declare SIZE_HUMAN

catalog.py -t add_dataset_annotation ${DATASET_ID} name:${NAME}
catalog.py -t add_dataset_annotation ${DATASET_ID} host:nxrs.msd.anl.gov
catalog.py -t add_dataset_annotation ${DATASET_ID} path:${DIR}
catalog.py -t add_dataset_annotation ${DATASET_ID} date:${DATE}
catalog.py -t add_dataset_annotation ${DATASET_ID} temperature:${TEMPERATURE}
catalog.py -t add_dataset_annotation ${DATASET_ID} sample:LSMO
catalog.py -t add_dataset_annotation ${DATASET_ID} PI:"Ray Osborn"
catalog.py -t add_dataset_annotation ${DATASET_ID} beamline:"ANL APS Sector 6"
catalog.py -t add_dataset_annotation ${DATASET_ID} size_bytes:${SIZE_BYTES}
catalog.py -t add_dataset_annotation ${DATASET_ID} size_human:${SIZE_HUMAN}

catalog.py -t add_dataset_acl ${DATASET_ID} u:rosborn user "r"

ls *.nxs 
