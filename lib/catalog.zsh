
alias catalog='python ~/.local/lib/python2.6/site-packages/globusonline/catalog/client/examples/catalog.py -default_catalog'

catalog-create-dataset()
{
  local NAME=$1
  catalog create_dataset '{'"\"name\":\"${NAME}\""'}' | read OUTPUT
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
  catalog add_dataset_annotation ${DATASET_ID} ${JSON}
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
