#!/bin/zsh -eu

alias catalog='python ~/.local/lib/python2.6/site-packages/globusonline/catalog/client/examples/catalog.py'

if [[ ${#*} != 2 ]]
then
  print "usage catalog-delete-dataset.zsh <CATALOG> <DATASET>"
  exit 1
fi

catalog delete_dataset $1 $2 true
