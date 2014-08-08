#!/bin/zsh -lu

DOC=swift.tex

if ! which inotifywait 
then
  print "Not found: inotifywait"
  exit 1
fi

./lx.zsh || exit 1

while ((1))
do
  inotifywait -e close_write ${DOC}
  print "changed: ${DOC}"
  ./lx.zsh
  sleep 1
done
