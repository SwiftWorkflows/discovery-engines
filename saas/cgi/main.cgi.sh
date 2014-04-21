#!/bin/bash -eu

SESSIONS=$JWWW_ROOT/sessions

mkdir -p $SESSIONS

export session=${session:-none}
change_session=${change_session:-none}
if [[ $session == "none" || $change_session == "Reset" ]]
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
  SESSION_D=$SESSIONS/$session.d
  mkdir -p $SESSION_D
  export current_directory="/tmp"
  echo $current_directory  > $SESSION_D/current_directory.txt
  date "+%Y/%m/%d %I:%M%p" > $SESSION_D/created_human.txt
  date "+%s"               > $SESSION_D/created_seconds.txt
fi

SESSION_D=$SESSIONS/$session.d
export current_directory=${current_directory:-none}
if [[ $current_directory == "none" ]] 
then
  current_directory=$( cat $SESSION_D/current_directory.txt )
fi
export created_human=${created_human:-none}
if [[ $created_human == "none" ]] 
then
  created_human=$( cat $SESSION_D/created_human.txt )
fi

m4 -P $JWWW_ROOT/html/main.html
