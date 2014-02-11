#!/bin/zsh -eu

NAMES=( lsmo34_090k lsmo39_130k lsmo48_090k lsmo42_150k )
PREFIXES=( ff1 ff2 fb1 fb2 sf sb ubf ubb )

# If 1, overwrite debug directory
FORCE=0
# Verbosity level: 0, 1, or 2
VERBOSE=0
while getopts "fv" OPTION
do
  case ${OPTION} in
    f) FORCE=1 ;;
    v) (( VERBOSE = VERBOSE+1 )) ;;
    *) # ZSH prints an error message.
      exit 1 ;;
  esac
done

if (( VERBOSE == 2 ))
then
  set -x
fi

if [[ ! -d data ]]
then
  print "Run from directory containing directory data!"
  exit 1
fi

mkdir_checked()
{
  if [[ ${#*} != 1 ]]
  then
    print "mkdir_checked(): must have single argument!"
    return 1
  fi

  DIR=$1

  (( VERBOSE )) && V="-v"

  if (( FORCE ))
  then
    mkdir -pv ${DIR}
  else
    if [[ -d ${DIR} ]]
    then
      print "Directory ${DIR} already exists!"
      return 1
    fi
    mkdir -v ${DIR}
  fi
}

(( VERBOSE )) && alias cp="cp -v"

mkdir_checked debug

for NAME in ${NAMES}
do
  SRC=data/${NAME}
  DST=debug/${NAME}

  mkdir_checked ${DST}
  cp ${SRC}/dark* ${DST}
  for PREFIX in ${PREFIXES}
  do
    cp ${SRC}/${PREFIX}scan-0000{0..9}.raw.tif{,.metadata} ${DST}
  done
done
