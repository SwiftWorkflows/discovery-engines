#!/bin/zsh

set -eux

zparseopts -D -E D=D d:=d p:=p e:=e o:=o 

ARGS=( ${d} ${p} ${e} ${o} )

set -x
DIRC=${d[2]}   # Directory containing CBFs: Child: sample/label/temp/fN
DIRP=${DIRC:h} # ZSH dirname: Parent: sample/label/temp
DIRL=${DIRP:h} # ZSH dirname: Parent: sample/label
DIRS=${DIRL:h} # ZSH dirname: Parent: sample

F=${DIRC:h}      # f1 or f2
TEMPK=${DIRP:t}  # E.g., 300K
TEMP=${TEMPK%K}  # E.g., 300
LABEL=${DIRL:h}  # E.g., jv000_12
SAMPLE=${DIRS:h} # E.g., nav2o500

print NXMERGE.SH in ${DIRC} 

if (( ${#o} > 0 )) 
then
  OUTPUT=${o[2]}
fi

NXSTACK=${HOME}/proj/nexusformat/src/nexusformat/scripts/nxstack.py

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
  set -x
  LOG=${o[2]%.nxs}.log
  print ${NXSTACK} ${ARGS} >& ${LOG}
  # nxsetup -s ${D} -l jv0004_3 ${d} -t 300 -f f1 f2 f3
  print nxsetup -s ${SAMPLE} -l ${LABEL} ${d} -t ${TEMP} -f f1 f2 f3
  # nxlink -s nav2o5_04 -l jv0004_3 -d align -t 300 -f f1 -m f1_mask.nxs
  print nxlink -s ${SAMPLE} -l ${LABEL} ${d} -f ${F} -m ${F}_mask.nxs
  print catalog-tag-dataset.zsh ${DIRS}
  # nxmax -d nav2o5_04/jv0004_3 -f nav2o5_04_align.nxs -p f1/data
  print nxmax -d ${SAMPLE}/${LABEL} -f ${OUTPUT} -p ${F}/data
  # nxfind -d nav2o5_04/jv0004_3 -f nav2o5_04_align.nxs -p f1/data
  print nxfind -d ${SAMPLE}/${LABEL} -f ${OUTPUT} -p ${F}/data
  # chmod a+r ${OUTPUT}
fi
