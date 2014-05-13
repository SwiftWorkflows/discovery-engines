#!/bin/bash

# NFHEDM=/clhome/TOMO1/PeaksAnalysisHemant/NF_HEDM
NFHEDM=/home/wozniak/sfw/nfhedm/bin
${NFHEDM}/ImageProcessing $1 $2 $3
RC=${?}
if [[ $RC != 0 ]] 
then 
    echo "Warning: RC!=0"
fi
exit 0

