#!/bin/zsh

set -eu

if [[ ${#} != 1 ]]
then
    print "requires F"
    return 1
fi

F=$1

if [[ ! -d f$F ]]
then
    print "Directory f$F does not exist!"
    return 1
fi

LIST=f$F/list.txt
if [[ -f ${LIST} ]]
then
    print "List exists: ${LIST}"
    return 1
fi

pl f$F/*.tiff > ${LIST}
wc -l ${LIST}

