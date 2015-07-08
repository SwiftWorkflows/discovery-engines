#!/bin/zsh

set -eux

zparseopts -D -E D=D d:=d p:=p e:=e o:=o

ARGS=( ${d} ${p} ${e} ${o} )

set -x
DIRC=${d[2]}   # Directory containing CBFs: Child: sample/label/temp/fN
DIRP=${DIRC:h} # ZSH dirname: Parent: sample/label/temp
DIRL=${DIRP:h} # ZSH dirname: Parent: sample/label
DIRS=${DIRL:h} # ZSH dirname: Parent: sample
DIRD=${DIRS:h} # ZSH dirname: Parent: data (e.g. /path/to/osborn-2015-1)

F=${DIRC:h}      # f1 or f2
TEMPERATURE_K=${DIRP:t}  # E.g., 300K
TEMPERATURE=${TEMPERATURE_K%K}  # E.g., 300
LABEL=${DIRL:t}  # E.g., jv000_12
SAMPLE=${DIRS:t} # E.g., nav2o500

print "NXMERGE.SH: ${DIRC}"

if (( ${#o} > 0 ))
then
  OUTPUT=${o[2]}
fi

NXSTACK=${HOME}/proj/nxpeaks/src/nxpeaks/scripts/nxstack.py

if [[ ! -f ${DIRC}/done.txt ]]
then
  print "Could not find: ${DIRC}/done.txt"
  print "No done.txt!"
  exit 0
fi

if [[ -f ${OUTPUT} ]]
then
  print "Output already exists: ${OUTPUT}"
  exit 0
else
  LOG=${o[2]%.nxs}.log

  pwd
  which nxwork

  # nxwork -s sm2ru3ge5 -l db0073b-1 -d 120K -t 120 -f f1 f2 f3 -p sm2ru3ge5_100K.nxs
  cd ${DIRD}
  nxwork -s ${SAMPLE} -l ${LABEL} -d ${TEMPERATURE_K} # > ${LOG}
fi
exit 1
