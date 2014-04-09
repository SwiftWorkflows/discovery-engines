#!/bin/zsh -eu

TOPDIR=/projects/DE/2014-04

EXPECTED_CBFS=3600

export PPN=8

DE_HOME=$( cd $( dirname $0 )/../.. && /bin/pwd )
if [[ ${?} != 0 || ${DE_HOME} == "" ]]
then
  print "Could not find DE_HOME!"
  exit 1
fi

ready()
{
  DIR=$1
  pushd ${DIR}
  CBFS=( $( print *.cbf ) )
  if [[ ${#CBFS} != ${EXPECTED_CBFS} ]]
  then
    return 1
  fi
  return 0
}

DIRS=( $( find ${TOPDIR} -maxdepth 1 ) )
for D in ${DIRS}
do
  if [[ -f ${D}/merge.complete ]]
  then
    continue
  fi
  if ! ready ${D}
  then
    continue
  fi

  JOBS_DIR=${DE_HOME}/jobs
  mkdir -p ${JOBS_DIR}
  JOB_NUMBER=$( date +%s )
  JOB=${JOBS_DIR}/list-${JOB_NUMBER}.txt
  print ${D} > ${JOB}

  ${DE_HOME}/nexus/swift/merge-all-swift-tukey.zsh 8 ${TOPDIR} ${JOB}
done
