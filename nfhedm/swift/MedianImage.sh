#!/bin/bash

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/clhome/TOMO1/PeaksAnalysisHemant/DIPLIB/Linuxa64/lib/
export LD_LIBRARY_PATH

# NFHEDM=/clhome/TOMO1/PeaksAnalysisHemant/NF_HEDM
NFHEDM=/home/wozniak/sfw/nfhedm/bin

${NFHEDM}/MedianImage $1 $2
