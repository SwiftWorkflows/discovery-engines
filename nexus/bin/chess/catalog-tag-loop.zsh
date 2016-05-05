#!/bin/zsh

set -eu

THIS=$( dirname $0 )
DATA_ROOT=$1

cd ${DATA_ROOT}

LAST_STAMP=0
while true
do
  STAMP=$( stat --format %Y ${DATA_ROOT}/**/*(om[1]) )
  if (( STAMP > LAST_STAMP ))
  then
      ${THIS}/catalog-tag-all.zsh 
  fi
done
