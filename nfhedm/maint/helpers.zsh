
# Copyright 2013 University of Chicago and Argonne National Laboratory
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License
# Reusable shell helpers

KB=1024
MB=$(( 1024*KB ))
GB=$(( 1024*MB ))

# Verbose operation
@()
{
  print
  print ${*}
  print
  ${*}
}

abort()
{
  MSG=${*}
  print ${MSG}
  exit 1
}

assert()
{
  ERR=$1
  shift
  MSG="${*}"
  check ${ERR} "${MSG}" || exit ${ERR}
  return 0
}

exitcode()
{
  ERR=$?
  MSG="${*}"
  assert ${ERR} "${MSG}"
}

# If CODE is non-zero, print MSG and return CODE
check()
{
  CODE=$1
  shift
  MSG=${*}

  if [[ ${CODE} != 0 ]]
    then
    print ${MSG}
    return ${CODE}
  fi
  return 0
}

bail()
{
  CODE=$1
  shift
  MSG="${*}"
  print "${MSG}"
  set +x
}

crash()
{
  local CODE=1
  while getopts "c" OPTION
  do
    case ${OPTION}
      in
      (c) CODE=${OPTARG} ;;
    esac
  done
  shift $(( OPTIND-1 ))

  shift
  MSG="${*}"
  bail ${CODE} ${MSG}
  exit ${CODE}
}

checkvar()
{
  local VAR=$1

  if [[ ${(P)VAR} == "" ]]
    then
    crash "Not set: ${VAR}"
  fi
  return 0
}

checkvars()
{
  local VARS
  VARS=( ${*} )
  local V
  for V in ${VARS}
   do
   checkvar ${V}
  done
  return 0
}

checkint()
{
  local VAR=$1

  checkvar ${VAR}
  if [[ ${(P)VAR} != <-> ]]
  then
    crash "Not an integer: ${VAR}"
  fi
  return 0
}

date_nice()
# Human-readable date: minute resolution
{
  date "+%m/%d/%Y %I:%M%p"
}

date_path()
# E.g., 2006/10/13/14/26/12 : Good for path names
{
  date "+%Y/%m/%d/%H/%M/%S"
}

nanos()
{
  date +%s.%N
}

compile()
{
  SWIFT=$1
  TCL=$2
  shift 2

  if [[ ! -f ${TCL} || ${SWIFT} -nt ${TCL} ]]
  then
    stc ${*} ${SWIFT} ${TCL}
    return ${?}
  fi
  return 0
}

uptodate()
{
  F1=$1
  F2=$2
  if [[ ${F1} == "" || ${F2} == "" ]]
    then
    print "uptodate: requires two files!"
  fi
  if [[ -f ${F2} && ${F2} -nt ${F1} ]]
  then
    return 0
  fi
  return 1
}

rm0()
{
  (( ! ${#*} )) && return
  rm ${*}
}

within()
{
  local TIME=$1
  shift
  local START STOP DIFF LEFT
  START=$( nanos )
  ${*}
  STOP=$( nanos )
  DIFF=$(( STOP-START ))
  if (( DIFF < 0 ))
    then
    print "TIME exceeded (${DIFF} > ${TIME})!"
    return 1
  fi
  LEFT=$(( TIME-LEFT ))
  sleep ${LEFT}
  return 0
}

login_ip()
# Obtain a visible IP address for this node
{
  ifconfig | grep "inet addr" | head -1 | cut -f 2 -d : | cut -f 1 -d ' '
}

scan()
# Use shoot to output the contents of a scan
{
  [[ $1 == "" ]] && return
  typeset -g -a $1
  local i=1
  local T
  while read T
  do
    eval "${1}[${i}]='${T}'"
    (( i++ ))
  done
}

shoot()
# print out an array loaded by scan()
{
  local i
  local N
  N=$( eval print '${#'$1'}' )
    # print N $N
  for (( i=1 ; i <= N ; i++ ))
  do
    eval print -- "$"${1}"["${i}"]"
  done
}

scan_kv()
{
  [[ $1 == "" ]] && return 1
  typeset -g -A $1
  while read T
  do
   A=( ${T} )
   KEY=${A[1]%:} # Strip any tailing :
   VALUE=${A[2,-1]}
   eval "${1}[${KEY}]='${VALUE}'"
  done
  return 0
}

shoot_kv()
{
  local VAR=$1
  eval print -a -C 2 \"'${(kv)'$VAR'[@]}'\"
  return 0
}

tformat()
# Convert seconds to hh:mm:ss
{
  local -Z 2 T=$1
  local -Z 2 M

  if (( T <= 60 ))
  then
    print "${T}"
  elif (( T <= 3600 ))
  then
    M=$(( T/60 ))
    print "${M}:$( tformat $(( T%60 )) )"
  else
    print "$(( T/3600 )):$( tformat $(( T%3600 )) )"
  fi
}

bformat()
# Format byte counts
{
  local BYTES=$1
  local LENGTH=${2:-3}
  local UNIT
  local UNITS
  UNITS=( "B" "KB" "MB" "GB" "TB" )

  local B=${BYTES}
  for (( UNIT=0 ; UNIT < 4 ; UNIT++ ))
   do
   (( B /= 1024 ))
   (( B == 0 )) && break
  done

  local RESULT=${UNITS[UNIT+1]}
  if [[ ${RESULT} == "B" ]]
    then
    print "${BYTES} B"
  else
    local -F BF=${BYTES}
    local MANTISSA=$(( BF / (1024 ** UNIT ) ))
    MANTISSA=$( significant ${LENGTH} ${MANTISSA} )
    print "${MANTISSA} ${RESULT}"
  fi

  return 0
}

significant()
# Report significant digits from floating point number
{
  local DIGITS=$1
  local NUMBER=$2

  local -F FLOAT=${NUMBER}
  local RESULT
  local DOT=0
  local LZ=1 # Leading zeros
  local C
  local i=1
  while (( 1 ))
   do
    C=${FLOAT[i]}
    [[ ${C} != "0" ]] && [[ ${C} != "." ]] && break
    [[ ${C} == "." ]] && DOT=1
    RESULT+=${C}
    (( i++ ))
  done
  while (( ${DIGITS} > 0 ))
  do
    C=${FLOAT[i]}
    if [[ ${C} == "" ]]
      then
      (( ! DOT )) && RESULT+="." && DOT=1
      C="0"
    fi
    RESULT+=${C}
    [[ ${C} == "." ]] && (( DIGITS++ )) && DOT=1
    (( i++ ))
    (( DIGITS-- ))
  done
  if (( ! DOT )) # Extra zeros to finish out integer
    then
    local -i J=${NUMBER}
    # J=${J}
    while (( ${#RESULT} < ${#J} ))
     do
     RESULT+="0"
    done
  fi
  print ${RESULT}
  return 0
}

turbine_stats_walltime()
# Extract the average walltime from the Turbine STATS
{
  LOG=$1
  if [[ ${LOG} == "" ]]
  then
    print "turbine_stats_walltime(): Not given: LOG"
  fi
  grep walltime ${LOG} | zclm -1 | avg
}

# Local variables:
# mode: sh
# sh-basic-offset: 2
# End:
