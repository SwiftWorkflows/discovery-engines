#!/bin/zsh

# Delay between jobs in minutes
DELAY=$1
shift
COMMAND=${*}

while (( 1 ))
do
  ${COMMAND}
  sleep $(( DELAY * 60 ))
done
