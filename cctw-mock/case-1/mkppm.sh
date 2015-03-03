#!/bin/bash
set -eu

# MKPPM
# Dummy script to create PPM file.
# Usage: ./mkppm.sh > out.ppm

N=10

# Header
echo "P3"
echo "${N} ${N} 10"

# Data
for (( i=0 ; i<N ; i++ ))
do
  for (( j=0 ; j<N ; j++ ))
  do
    V=$(( (i == j) * 10 ))
    printf "%i %i %i " $V $V 0
  done
  printf "\n"
done
