changecom(`dnl')#!/bin/bash

# Define convenience macros
# This simply does environment variable substition when m4 runs
define(`getenv', `esyscmd(printf -- "$`$1'")')
define(`getenv_nospace', `esyscmd(printf -- "$`$1'")')

# To set the run time name of the job to "JOBNAME"
#$ -N getenv(JOBNAME)

# To make sure that the .e and .o file arrive in the working directory
#$ -cwd

echo running on: $(hostname)
date "+%m/%d/%Y %I:%M%p"

LD_LIBRARY_PATH=/home/wozniak/Public/sfw/CBFlib-0.9.3.1/lib
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/wozniak/downloads/Qt/5.5/gcc_64/lib
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/wozniak/Public/sfw/hdf5-1.8.11/lib
export LD_LIBRARY_PATH
PATH=$PATH:/nfs/chess/aux/user/wozniak/cctw/bin

getenv(COMMAND)

date "+%m/%d/%Y %I:%M%p"
