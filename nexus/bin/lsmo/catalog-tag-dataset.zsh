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
# declare NAME
DATASET_ID=$( catalog.py create_dataset name:${NAME} )
# declare DATASET_ID
SAMPLE=${NAME%_*}
FRACTION=${SAMPLE#lsmo}
TEMPERATURE=${NAME#*_}
TEMPERATURE=${TEMPERATURE%k}

print "Annotating: ${NAME}"
ANNOTATIONS=(

  # Text  
  name:${NAME}
  PI:"Ray Osborn"
  beamline:"ANL APS Sector 6"
  material:lsmo
  sample:${SAMPLE}
  host:nxrs.msd.anl.gov
  path:${DIR}

  # Int8s 
  year:2013
  month:10

  # Floats
  fraction:${FRACTION}
  temperature_K:${TEMPERATURE}
)

catalog.py -t add_dataset_annotation ${DATASET_ID} ${ANNOTATIONS}
catalog.py -t add_dataset_acl ${DATASET_ID} u:rosborn user "r"

get_md5sum()
{
  (( ${#*} == 1 )) || return 1
  FILE=$1
  MD5=${FILE}.md5
  if [[ -f ${MD5} ]] 
    then 
    cat ${MD5}
  else
    md5sum ${FILE} | cut -d ' ' -f 1 | tee ${MD5}
  fi
  return 0
}

cd ${DIR}
pwd
NXS_LIST=( $( find . -name "*.nxs" ) )
foreach NXS in ${NXS_LIST}
do
  print "Annotating member: ${NXS}"
  DATE=$( stat ${NXS} | awk '$1 == "Modify:" { print $2 " " $3 }' )
  DATE=${DATE/.[0-9]*/} # Chop off subsecond resolution
  SIZE_BYTES=$( command du -s ${NXS} | zclm 1 )
  declare SIZE_BYTES
  SIZE_HUMAN=$( bformat ${SIZE_BYTES} ; true )
  SCAN=${NXS:h} # ZSH dirname
  SCAN=${SCAN:t} # ZSH basename
  FILE=${DIR}/${NXS}
  FILE=${FILE:a} # Clean up path
  MD5=$( get_md5sum ${NXS} )

  ANNOTATIONS=( 
    size_bytes:${SIZE_BYTES}
    size_human:${SIZE_HUMAN}
    date:${DATE}
    scan:${SCAN}
    path:${FILE}
    md5:${MD5}
  )

  MEMBER_ID=$( catalog.py -t create_members ${DATASET_ID} file ${NXS} )
  catalog.py -t add_dataset_annotation ${MEMBER_ID} ${ANNOTATIONS}
done
