
alias catalog='python ~/.local/lib/python2.6/site-packages/globusonline/catalog/client/examples/catalog.py -default_catalog'

id-column()
{
  zclm 1 | grep -E "[[:digit:]]+" | sed "s/)//"
}

catalog-create-dataset()
{
  local NAME=$1
  catalog create_dataset ${DEFAULT_CATALOG_ID} \
    '{'"\"name\":\"${NAME}\""'}' | read OUTPUT
  print ${OUTPUT#*,}
}

catalog-annotate-dataset()
{
  local DATASET_ID=$1
  local KEY=$2
  local VALUE=$3
  print "annotate-dataset: ${DATASET_ID}: ${KEY}=${VALUE}"
  local JSON=""
  JSON+='{'
  JSON+="\"${KEY}\":\"${VALUE}\""
  JSON+='}'
  catalog add_dataset_annotation ${DEFAULT_CATALOG_ID} ${DATASET_ID} ${JSON}
}

catalog-acl-add()
{
  if [[ ${#*} != 3 ]]
  then
    print "usage: catalog-acl-add <DATASET_ID> <PRINCIPAL> <PERMS>"
    return 1
  fi
  set -x
  local DATASET_ID=$1
  local PRINCIPAL=$2
  local PERMS=$3
  local JSON=""
  JSON+='{"principal":"'${PRINCIPAL}'","principal_type":"user",'
  JSON+='"permission":"'${PERMS}'"}'
  catalog add_dataset_acl ${DEFAULT_CATALOG_ID} ${DATASET_ID} ${JSON}
}

catalog-acl-get()
{
  if [[ ${#*} == 1 ]]
  then
    print "usage: catalog-acl-get <DATASET_ID>"
    return 1
  fi

  local DATASET_ID=$1
  catalog get_dataset_acl ${DEFAULT_CATALOG_ID} ${DATASET_ID}
}

catalog-dataset-exists()
{
  if [[ ${#*} != 1 ]]
  then
    print "usage: catalog-dataset-exists <DATASET_ID>"
    return 1
  fi

  local DATASET_ID=$1
  catalog get_dataset_acl ${DEFAULT_CATALOG_ID} ${DATASET_ID} >& /dev/null
  # Return exit code
}

catalog-query()
{
  if [[ ${#*} != 2 ]]
  then
    print "usage: catalog-query <KEY> <VALUE>"
    return 1
  fi

  local KEY=$1
  local VALUE=$2
  catalog query_datasets ${DEFAULT_CATALOG_ID} \
    ${KEY} LIKE "%"${VALUE}"%" -text | id-column
}

catalog-create-member()
{
  local DATASET_ID=$1
  local ENDPOINT=$2
  local FILEPATH=$3
  local JSON=""
  JSON+='{'
  JSON+="\"data_type\":\"file\","
  JSON+="\"data_uri\":\"globus:///${FILEPATH}\""
  JSON+='}'
  catalog create_members ${DATASET_ID} ${JSON} | read OUTPUT
  print ${OUTPUT#*,*,}
}

catalog-annotate-member()
{
  local DATASET_ID=$1
  local MEMBER_ID=$2
  local KEY=$3
  local VALUE=$4
  print "annotate-member: ${DATASET_ID},${MEMBER_ID}:" \
                          "${KEY}=${VALUE}"
  local JSON=""
  JSON+='{'
  JSON+="\"${KEY}\":\"${VALUE}\""
  JSON+='}'
  catalog add_member_annotation ${DATASET_ID} ${MEMBER_ID} ${JSON}
}
