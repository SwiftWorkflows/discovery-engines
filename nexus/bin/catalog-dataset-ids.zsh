#!/bin/zsh -eu

if [[ -z ${GCAT_DEFAULT_CATALOG_ID} ]]
then
  print "Set GCAT_DEFAULT_CATALOG_ID"
  exit 1
fi

catalog.py get_datasets -t | clm 1 | grep -E "[[:digit:]]+" | sed "s/)//"
if [[ ${pipestatus[1]} != 0 ]]
then
  print "catalog operation failed!"
  exit 1
fi
exit 0
