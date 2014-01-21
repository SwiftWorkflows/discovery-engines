
push()
{
  pushd ${*} > /dev/null
}

pop()
{
  popd ${*} > /dev/null
}

line()
# Nth line in file
{
  local N=$1
  local FILE=$2
  sed -n ${N}p ${FILE}
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
  CODE=$1
  shift
  MSG="${*}"
  bail ${CODE} ${MSG}
  exit ${CODE}
}

line_count()
# Clean wc -l
{
  wc -l ${*} | read -A R
  print ${R[1]}
}

timestamp()
{
  print "TIMESTAMP: $( date +%s )"
}

zclm()
# Select columns from input without awk
{
  local L
  local -Z c
  local A C i
  C=( ${*} )
  while read L
  do
    A=( $( print -- ${L} ) )
    N=${#C}
    for (( i=1 ; i<=N ; i++ ))
    do
      c=${C[i]}
      print -n "${A[c]}"
      (( i < N )) && print -n " "
    done
    printf "\n"
  done
  return 0
}
