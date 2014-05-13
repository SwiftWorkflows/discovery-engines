#!/bin/bash

/clhome/TOMO1/PeaksAnalysisHemant/NF_HEDM/./ImageProcessing $1 $2 $3
RC=$?
if [[ $RC != 0 ]] 
then 
    echo "Warning: RC=0"
fi
exit 0

