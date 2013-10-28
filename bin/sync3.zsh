#!/bin/zsh

# LOG=~wozniak/proj/d-e/logs/sync3.log
LOG=/dev/stdout

SITE1="wozniak#wozniak-laptop-u"
SITE2="lcrc#fusion"
SITE3="wozniak#pads-c10"

GO_USER="wozniak"
GO=cli.globusonline.org
ACCT=${GO_USER}@${GO}

# Sync on file size:
XFER_ARGS=( --preserve-mtime -s 1 )

date_word()
# 2006-10-13_14:26 : Good for file names
{
  date "+%Y-%m-%d_%H:%M"
}

while (( 1 ))
do
  LABEL=( --label $( date_word ) )
  ssh ${ACCT} scp ${SITE1}:de.data ${SITE2}:
  sleep 60
done >> ${LOG}
