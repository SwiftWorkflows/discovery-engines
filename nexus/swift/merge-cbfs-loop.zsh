#!/bin/zsh 

set -eu

DATA=/home/bessrc/sharedbigdata/data2/osborn-2015-1

THIS=$( dirname $0 )

while true 
do
  for D in ${DATA}/nav2*
  do
    ${THIS}/merge-cbfs-swift.zsh 2 ${D}
  done

  print "MERGED."
  date_nice
  print Sleeping...
  sleep 60 
done
