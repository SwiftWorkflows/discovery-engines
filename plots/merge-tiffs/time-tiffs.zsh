#!/bin/zsh

set -eu

zmodload zsh/stat

for C in {1..119}
do
  DATA=time-tifs-${C}.data
  rank -c ${C} ${TO}/output.txt | grep Processing | \
    awk -f time-tifs.awk > ${DATA}
  zstat -H S ${DATA}
  if [[ ${S[size]} == 0 ]]
  then
    print "Empty file: ${DATA}"
    rm -v ${DATA}
  fi
done

swift_plotter.zsh -s time-tiffs.{cfg,eps} time-tifs-*.data
convert time-tiffs.{eps,png}
