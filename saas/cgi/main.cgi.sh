#!/bin/bash -e

SESSIONS=$JWWW_ROOT/sessions

if [[ -z $session ]]
then
  f=$( mktemp )
  while (( 1 )) 
  do
    session=${RANDOM}
    if ln $f $SESSIONS/$session 2>&1 > /dev/null
    then
      break
    fi
    sleep 1
  done
  export session
fi

echo session: $session
echo root_directory: $root_directory


mkdir -p $SESSIONS
SESSION=$SESSIONS/$session
if [[ ! -f $SESSION ]]
then
  touch $SESSION
fi

m4 -P $JWWW_ROOT/html/main.html
