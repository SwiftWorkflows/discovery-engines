#!/bin/zsh

TAGS=( merge_status nxmax_status nxfind_status )

for TAG in ${TAGS}
do
  catalog.py -t create_annotation_def ${TAG} text
done
