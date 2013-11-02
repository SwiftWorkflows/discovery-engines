
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
