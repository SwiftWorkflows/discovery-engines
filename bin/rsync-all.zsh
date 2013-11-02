#!/bin/zsh -eu

DIR=$1
ENTRIES=$2
PADS_LIST=( $( < ~wozniak/pads.list ) ) 

DE_HOME=$( cd $( dirname $0 ) ; /bin/pwd )

line()
# Nth line in file
{ 
  local N=$1
  local FILE=$2
  sed -n ${N}p ${FILE}
}

assert()
{
  ERR=$1
  shift
  MSG="${*}"
  check ${ERR} "${MSG}" || exit ${ERR}
  return 0
}

exitcode()
{
  ERR=$?
  MSG="${*}"
  assert ${ERR} "${MSG}"
}

# If CODE is non-zero, print MSG and return CODE
check()
{
  CODE=$1
  shift
  MSG=${*}

  if [[ ${CODE} != 0 ]]
    then
    print ${MSG}
    return ${CODE}
  fi
  return 0
}

line_count()
# Clean wc -l
{
  wc -l ${*} | read -A R
  print ${R[1]}
}

[[ -d ${DIR} ]] 
exitcode "directory does not exist: ${DIR}"

[[ -f ${ENTRIES} ]] 
exitcode "entries file does not exist: ${ENTRIES}"

PADS_COUNT=$( line_count ${PADS_LIST} )
print "PADS nodes: ${PADS_COUNT}"

LOG=${DE_HOME}/logs/rsync-all.log

while (( 1 )) 
do 
  read ENTRY
  declare ENTRY
  HASH=$( ${DE_HOME}/hash.pl ${ENTRY} )
  INDEX=$(( HASH % PADS_COUNT + 1 ))
  declare INDEX
  NODE=${PADS_LIST[${INDEX}]}.pads.ci.uchicago.edu
  declare NODE
done < ${ENTRIES} >>& ${LOG}
