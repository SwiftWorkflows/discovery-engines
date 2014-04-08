#!/bin/zsh -eu

if [[ ${#*} != 2 ]]
then
  print "usage catalog-delete-dataset.zsh <CATALOG> <DATASET>"
  exit 1
fi

catalog.py delete_dataset $1 $2 true
