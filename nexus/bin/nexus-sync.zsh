#!/bin/zsh -eu

LOG=~wozniak/proj/d-e/logs/go-sync3.log
# LOG=/dev/stdout

mkdir -p $( dirname ${LOG} )

SITE1="wozniak#s11pilatus:/~/sharedbigdata/data1/osborn-2014-1"
SITE2="alcf#dtn_mira:/~/DE/2014-04"

GO_USER="wozniak"
GO=cli.globusonline.org
ACCT=${GO_USER}@${GO}

DELAY=60

# Sync on file size:
XFER_ARGS=( -r --preserve-mtime -s 1 )

date_word()
# 2006-10-13_14-26 : Good for file names
{
  date "+%Y-%m-%d_%H-%M"
}

{
while (( 1 ))
do
  print
  LABEL=( --label $( date_word ) )
  print "Starting sync: ${LABEL}"
  date
  ssh ${ACCT} scp ${XFER_ARGS} ${LABEL} ${SITE1} ${SITE2}
  sleep ${DELAY}
done < /dev/null
date
print DONE
} |& tee -a ${LOG}
