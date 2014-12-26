#!/bin/zsh

set -eu

zparseopts -D -E D=D d:=d p:=p e:=e o:=o 

ARGS=( ${d} ${p} ${e} ${o} )

DIR=${d[2]:h} # ZSH dirname

print NXMERGE.SH in ${DIR} 

if (( ${#o} > 0 )) 
then
  OUTPUT=${DIR}/${o[2]}.nxs
  if [[ -f ${OUTPUT} ]] 
  then
    echo "Output already exists: ${OUTPUT}"
    exit 0
  fi
fi

LOG=${o[2]%.nxs}.log
nice nxmerge ${ARGS} >& ${LOG} 
