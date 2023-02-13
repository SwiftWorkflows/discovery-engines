#!/bin/zsh
set -eu

CONDA=/nfs/chess/sw/nexpy/miniconda3-woz

bash ~/downloads/Miniconda3-2022-07-28.sh -b -p $CONDA

PATH=$CONDA/bin:$PATH

conda install -c conda-forge gxx
conda install -c conda-forge hdf5 libtiff libjpeg libglu
conda install -c conda-forge qt qt-main qt-webengine qtconsole qtconsole-base qtpy
