#!/bin/zsh -eu

if [[ -z ${DEFAULT_CATALOG_ID} ]]
then
  print "Set DEFAULT_CATALOG_ID"
  exit 1
fi

catalog.py get_datasets ${DEFAULT_CATALOG_ID} -t | clm 1 | grep -E "[[:digit:]]+" | sed "s/)//"
