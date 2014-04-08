#!/bin/bash -eu

export PYTHONPATH=${HOME}/.local/lib/python2.6/site-packages/globusonline/catalog/client/examples

DE_BIN=$( dirname $0 )

python ${DE_BIN}/catalog-tag-dataset.py ${*}

