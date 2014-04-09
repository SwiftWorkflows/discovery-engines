#!/bin/zsh

# MERGE-RESULTS
# Extract result statistics from log

usage()
{
  print "usage: merge-results <OUTPUT>"
}

if [[ ${#*} != 1 ]]
then
  usage
  return 1
fi

LOG=$1

print PROCS:     $( grep    "PROCS"            ${LOG} )
print COMPLETED: $( grep -c "NEXUS: COMPLETE:" ${LOG} )
print ERRORS:    $( grep -c "NEXUS_ERROR:"     ${LOG} )
