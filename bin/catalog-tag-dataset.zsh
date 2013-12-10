#!/bin/zsh -eu

if [[ -z ${DEFAULT_CATALOG_ID} ]]
then
  print "Set DEFAULT_CATALOG_ID"
  exit 1
fi

usage()
{
  print "usage: catalog-tag-dataset.zsh <ENDPOINT> <DIRECTORY>"
}

if [[ ${#*} != 2 ]]
then
  usage
  exit 1
fi

ENDPOINT=$1
DIR=$2

cd ${DIR}

# Get absolute path
DIR=$( /bin/pwd )

NAME=$( basename ${DIR} )

declare ENDPOINT NAME

DATASET_ID=$( catalog-create-dataset ${NAME} )

declare DATASET_ID

KEYS=( "host" "path" "size" )
for KEY in ${KEYS}
do
  # This fails if the annotation exists
  catalog create_annotation_def ${KEY} "text" || true
done

catalog-annotate-dataset ${DATASET_ID} "host" "mira"
catalog-annotate-dataset ${DATASET_ID} "path" ${DIR}

print "Getting size..."
SIZE=$( du -s . )
declare SIZE
catalog-annotate-dataset ${DATASET_ID} "size" ${SIZE}

print "DONE."
