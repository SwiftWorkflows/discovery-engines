#!/bin/zsh
set -eu

CONDA=/nfs/chess/sw/nexpy/miniconda3-woz

bash ~/downloads/Miniconda3-2022-07-28.sh -b -p $CONDA

pt $CONDA

cicf gxx
cicf hdf5 libtiff libjpeg libglu
cicf qt qt-main qt-webengine qtconsole qtconsole-base qtpy
