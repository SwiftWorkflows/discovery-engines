#!/bin/bash

MIPS=$( grep mips /proc/cpuinfo | head -1 )
CPU=$( grep "model name" /proc/cpuinfo | head -1 )
OUTPUT=result-$JOB_ID.txt
echo $HOSTNAME $MIPS $CPU > $OUTPUT

sleep 10

