#!/bin/zsh -eu

alias catalog='python ~/.local/lib/python2.6/site-packages/globusonline/catalog/client/examples/catalog.py -default_catalog'

if [[ -z ${DEFAULT_CATALOG_ID} ]]
then
  print "Set DEFAULT_CATALOG_ID"
  exit 1
fi

catalog-create-dataset()
{
  local NAME=$1
  catalog create_dataset '{'"\"name\":\"${NAME}\""'}' | read OUTPUT
  print ${OUTPUT#*,}
}

catalog-annotate-dataset()
{
  local DATASET_ID=$1
  local KEY=$2
  local VALUE=$3
  print "annotate-dataset: ${DATASET_ID}: ${KEY}=${VALUE}"
  local JSON=""
  JSON+='{'
  JSON+="\"${KEY}\":\"${VALUE}\""
  JSON+='}'
  catalog add_dataset_annotation ${DATASET_ID} ${JSON}
}

catalog-create-member()
{
  local DATASET_ID=$1
  local ENDPOINT=$2
  local FILEPATH=$3
  local JSON=""
  JSON+='{'
  JSON+="\"data_type\":\"file\","
  JSON+="\"data_uri\":\"globus:///${FILEPATH}\""
  JSON+='}'
  catalog create_members ${DATASET_ID} ${JSON} | read OUTPUT
  print ${OUTPUT#*,*,}
}

catalog-annotate-member()
{
  local DATASET_ID=$1
  local MEMBER_ID=$2
  local KEY=$3
  local VALUE=$4
  print "annotate-member: ${DATASET_ID},${MEMBER_ID}:" \
                          "${KEY}=${VALUE}"
  local JSON=""
  JSON+='{'
  JSON+="\"${KEY}\":\"${VALUE}\""
  JSON+='}'
  catalog add_member_annotation ${DATASET_ID} ${MEMBER_ID} ${JSON}
}

usage()
{
  print "usage: catalog-tag-dataset.zsh <ENDPOINT> <DIRECTORY>"
}

push()
{
  pushd ${*} > /dev/null
}

pop()
{
  popd ${*} > /dev/null
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

catalog-annotate-dataset ${DATASET_ID} "host" "mira" &
catalog-annotate-dataset ${DATASET_ID} "path" ${DIR} || true
wait

print "Looking for files..."
FILES=( *(.) )
print "Found ${#FILES} files."

for F in ${FILES}
do
  declare F
  MEMBER_ID=$( catalog-create-member \
               ${DATASET_ID} ${ENDPOINT} ${DIR}/${F} )
  declare MEMBER_ID
  du -h ${F} | read SIZE N
  catalog-annotate-member ${DATASET_ID} ${MEMBER_ID} "size" ${SIZE}
done

print "DONE."
