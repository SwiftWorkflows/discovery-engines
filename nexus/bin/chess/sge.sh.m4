changecom(`dnl')#!/bin/bash

# Define convenience macros
# This simply does environment variable substition when m4 runs
define(`getenv', `esyscmd(printf -- "$`$1'")')

# To set the run time name of the job to "JOBNAME"
#$ -N getenv(JOBNAME)

# To make sure that the .e and .o file arrive in the working directory
#$ -cwd
#$ -o getenv(DIRECTORY)/jobs
#$ -e getenv(DIRECTORY)/jobs
#$ -j y 

#$ -pe sge_pe getenv(CORES)
#$ -l m_core=getenv(CORES)

echo running on: $(hostname)
echo PWD:        $PWD
echo user:       $USER
echo start_date: $( date "+%m/%d/%Y %I:%M%p" )
echo start_secs: $( date "+%s" )

LD_LIBRARY_PATH=/home/wozniak/Public/sfw/CBFlib-0.9.3.1/lib
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/wozniak/downloads/Qt/5.5/gcc_64/lib
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/wozniak/Public/sfw/hdf5-1.8.11/lib
export LD_LIBRARY_PATH
PATH=$PATH:/nfs/chess/aux/user/wozniak/cctw/bin

OUTPUT=getenv(DIRECTORY)/jobs/job-getenv(JOBNAME)-$JOB_ID.out
echo redirecting job output to: $OUTPUT

getenv(COMMAND) > $OUTPUT 2>&1 

echo stop_date: $( date "+%m/%d/%Y %I:%M%p" )
echo stop_secs: $( date "+%s" )
