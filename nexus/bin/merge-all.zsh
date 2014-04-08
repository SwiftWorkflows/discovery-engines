#!/bin/zsh

set -eux

PROJECT=/projects/ExM/DE

cd ${PROJECT}
cd data

for DIR in lsmo34_090k
do
  # 1.26s
  merge_tiffs -d ${DIR} -p darkbeg
  merge_tiffs -d ${DIR} -p ff1scan -b darkbeg -o 0 -s 0.1
  merge_tiffs -d ${DIR} -p fb1scan -b darkbeg -o 0 -s 0.1 -r
  merge_tiffs -d ${DIR} -p ff2scan -b darkbeg -o 0 -s 0.1
  merge_tiffs -d ${DIR} -p fb2scan -b darkbeg -o 0 -s 0.1 -r
  merge_tiffs -d ${DIR} -p sfscan -b darkbeg -o 0 -s 0.1
  merge_tiffs -d ${DIR} -p sbscan -b darkbeg -o 0 -s 0.1 -r
  merge_tiffs -d ${DIR} -p ubfscan -b darkbeg -o 0 -s 0.1
  merge_tiffs -d ${DIR} -p ubbscan -b darkbeg -o 0 -s 0.1 -r
done
