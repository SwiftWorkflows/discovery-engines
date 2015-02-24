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

DE_HOME=$( cd $( dirname $0 )/../../.. && /bin/pwd )
[[ ${?} != 0 || ${DE_HOME} == "" ]] && crash "Could not find DE_HOME!"

source ${DE_HOME}/lib/helpers.zsh || \
  crash "Could not source helpers.zsh!"

cd ${DIR}

# Get absolute path
DIR=$( /bin/pwd )

NAME=$( basename ${DIR} )
# declare NAME
DATASET_ID=$( catalog.py create_dataset name:${NAME} )
# declare DATASET_ID

catalog.py -t add_dataset_acl ${DATASET_ID} u:rosborn user "r"

SAMPLE=${NAME%_*}
FRACTION=${SAMPLE#bfap}

print "Annotating: ${NAME}"
ANNOTATIONS=(
  # Text
  name:${NAME}
  PI:"Ray Osborn"
  beamline:"ANL APS Sector 6"
  material:bfap
  sample:${SAMPLE}
  host:nxrs.msd.anl.gov
  path:${DIR}
  # Int8s
  year:2013
  month:10
  # Float
  fraction:${FRACTION}
)

catalog.py -t add_dataset_annotation ${DATASET_ID} ${ANNOTATIONS}
@ catalog.py -t add_dataset_acl ${DATASET_ID} rosborn user "r"

annotate_nxs()
# NXS is in PWD
{
  NXS=$1
  print "Annotating member: ${NXS}"

  TEMPERATURE=$( ${DE_HOME}/nexus/bin/nexus-temperature.py ${NXS} )
  DATE=$( stat ${NXS} | awk '$1 == "Modify:" { print $2 " " $3 }' )
  DATE=${DATE/.[0-9]*/} # Chop off subsecond resolution
  FILE=${DIR}/${LABEL}/${NXS}
  FILE=${FILE:a} # Clean up path
  NXS_BASE=${NXS:t} # ZSH basename

  ANNOTATIONS=(
    # Text
    "date:${DATE}"
    label:${LABEL}
    path:${FILE}
    host:nxrs.msd.anl.gov
    # Float
    temperature_K:${TEMPERATURE}
  )

  print -l $ANNOTATIONS
  print

  MEMBER_ID=$( catalog.py -t create_members ${DATASET_ID} file ${NXS_BASE} )
  catalog.py -t add_member_annotation ${DATASET_ID} ${MEMBER_ID} ${ANNOTATIONS}
}

LABELS=( kt*(/) )
for LABEL in ${LABELS}
do
  push ${LABEL}
  printf "Looking for *.nxs in: ${PWD}\n\n"
  NXS_LIST=( *.nxs )
  foreach NXS in ${NXS_LIST}
  do
    annotate_nxs ${NXS}
  done
  pop
done

print "Done with ${NAME}."
