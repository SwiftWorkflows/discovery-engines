#!/bin/zsh

set -eu
set -x

crash()
{
  MSG=$1
  print ${MSG}
  exit 1
}

make tests/test-placebo.x
tests/test-placebo.x >& tests/test-placebo.out
grep "z: 4" tests/test-placebo.out || crash "tests/test-placebo.x failed!"

print "tests complete."
