#!/bin/zsh
set -eu

path+=~/.local.bak/bin
LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${HOME}/Public/sfw/Python-2.7.10/lib

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

NAME=$1
declare NAME

DE_HOME=$( cd $( dirname $0 )/../../.. && /bin/pwd )
[[ ${?} != 0 || ${DE_HOME} == "" ]] && crash "Could not find DE_HOME!"

source ${DE_HOME}/lib/helpers.zsh || \
  crash "Could not source helpers.zsh!"

# Get absolute path: Should be in sample directory
DIR=$( /bin/pwd )

SAMPLE=${NAME%_*}
# FRACTION=${SAMPLE#}

ANNOTATIONS=(
  # Text
  name:${NAME}
  PI:"Ray Osborn"
  beamline:"CHESS A2"
  sample:${SAMPLE}
  # host:nxrs.msd.anl.gov
  host:lnx201.classe.cornell.edu
  path:${DIR}
  # Int8s
  year:2016
  month:05
  # Float
  # fraction:${FRACTION}
)

TMPFILE=$( mktemp /tmp/catalog-tag-XXXX )
if catalog -tn get_dataset_annotations ${NAME} > ${TMPFILE}
then
  print "Dataset exists: ${NAME}"
  DATASET_ID=$( awk '/id:/ {print substr($0, 4)}' < ${TMPFILE} )
  declare DATASET_ID
else
  print "Creating dataset: ${NAME}"
  DATASET_ID=$( catalog.py create_dataset name:${NAME} )
  declare DATASET_ID
  @ catalog.py -t add_dataset_annotation ${DATASET_ID} ${ANNOTATIONS}
  @ catalog.py -t add_dataset_acl ${DATASET_ID} rosborn user "r"
fi
rm ${TMPFILE}

# Convert to array
T=${TAGGED}
unset TAGGED
typeset -ga TAGGED
TAGGED=( ${=T} )

annotate_nxs()
# NXS is in PWD
{
  LABEL=$1
  NXS=$2

  REALDIR=${DIR:a}
  FILE=${DIR}/${LABEL}/${NXS}
  FILE=${FILE:a} # Canonicalize path
  NXS_BASE=${NXS:t} # ZSH basename

  # Reverse associated array lookup
  if [[ ${TAGGED[(i)${LABEL}/${NXS_BASE}]} != ${UNTAGGED} ]]
  then
    print "Already tagged: ${NXS_BASE}"
    return
  fi

  print "Annotating member: ${NXS}"
  # TEMPERATURE=$( ${DE_HOME}/nexus/bin/nexus-temperature ${NXS} )
  # TEMPERATURE=100
  t=${NXS%.nxs}

  TEMPERATURE=${t#${NAME}_} # Pull out temperature path component
  DATE=$( stat ${NXS} | awk '$1 == "Modify:" { print $2 " " $3 }' )
  DATE=${DATE/.[0-9]*/} # Chop off subsecond resolution
  ANNOTATIONS=(
    # Text
    "date:${DATE}"
    label:${LABEL}
    path:${FILE}
    # host:nxrs.msd.anl.gov
    host:lnx201.classe.cornell.edu
  )
  if [[ ${TEMPERATURE} != "nan" ]]
  then
    ANNOTATIONS+=temperature:${TEMPERATURE}
  fi

  print -l $ANNOTATIONS
  print

  print "Creating member: ${LABEL}/${NXS_BASE}"
  if ! MEMBER_ID=$( catalog.py -t create_members ${DATASET_ID} \
                                  file ${LABEL}/${NXS_BASE} )
  then
    print "Error adding member!"
    print ${MEMBER_ID} # May contain error message
    exit 1
  fi
  # MEMBER_ID=67
  value MEMBER_ID
  catalog.py -t add_member_annotation ${DATASET_ID} ${MEMBER_ID} ${ANNOTATIONS}
}

LABELS=( *(/) )
for LABEL in ${LABELS}
do
  push ${LABEL}
  printf "Looking for *.nxs in: ${PWD}\n"
  NXS_LIST=( *.nxs )
  foreach NXS in ${NXS_LIST}
  do
    annotate_nxs ${LABEL} ${NXS}
    print
  done
  pop
done

print "Done with ${NAME}."
print
