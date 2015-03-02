#!/bin/bash
set -eu

echo P3
N=10
echo $N $N 10

for (( i=0 ; i<N ; i++ ))
do
  for (( j=0 ; j<N ; j++ ))
  do
    V=$(( (i == j) * 10 )) 
    printf "%i %i %i " $V $V 0 
  done
  printf "\n"
done
