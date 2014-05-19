#!/bin/zsh

set -eu

INSTALL=${HOME}/wozniak/sfw/e2e

mkdir -pv ${INSTALL}
cp -uv *.sh ${INSTALL}
