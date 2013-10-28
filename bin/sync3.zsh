#!/bin/zsh -eu

LOG=~wozniak/proj/d-e/logs/sync3.log
# LOG=/dev/stdout

mkdir -p $( dirname ${LOG} )

SITE1="wozniak#wozniak-laptop-u"
SITE2="lcrc#fusion"
SITE3="wozniak#pads-c10"

GO_USER="wozniak"
GO=cli.globusonline.org
ACCT=${GO_USER}@${GO}

# Sync on file size:
XFER_ARGS=( -r --preserve-mtime -s 1 )

date_word()
# 2006-10-13_14-26 : Good for file names
{
  date "+%Y-%m-%d_%H-%M"
}

while (( 1 ))
do
  print
  LABEL=( --label $( date_word ) )
  print "Starting sync: ${LABEL}"
  ssh ${ACCT} scp ${XFER_ARGS} ${LABEL} ${SITE1}:d-e.d ${SITE2}:d-e.d
  ssh ${ACCT} scp ${XFER_ARGS} ${LABEL} ${SITE2}:d-e.d ${SITE3}:d-e.d
  sleep ${DELAY}
done < /dev/null >> ${LOG}
