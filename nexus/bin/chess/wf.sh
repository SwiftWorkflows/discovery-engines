#!/bin/zsh
set -eu

print WF.SH $JOB_ID on $(hostname -s) at $(date_nice)
print

umask 000

WRAPPER_NXS=$1
SUBPATH=$2
shift 2

OUT_NXS=$6

PATH=/nfs/chess/sw/nexpy/anaconda/bin:$PATH

APS=/nfs/chess/aps/rosenkranz-311-2
cd $APS

declare SUBPATH WRAPPER_NXS OUT_NXS
print
print $SUBPATH | tr / ' ' | read SAMPLE LABEL TEMPERATURE F
declare SAMPLE LABEL TEMPERATURE F
print

if [[ ! -f $OUT_NXS ]]
then
    /home/wozniak/proj/d-e/mt2/bin/mt2 ${*}
    print $SUBPATH >> $APS/stack_done.txt
else
  print "skipping stack"
fi

if grep -q $F/nxfind <( h5dump -xH $WRAPPER_NXS )
then
    print "skipping nxfind"
else
  print nxlink...
  START=$( date +%s )
  nxlink -d $SAMPLE/$LABEL/$TEMPERATURE -f $F -m $APS/pilatus_mask
  STOP=$( date +%s )
  print nxlink time: $(( STOP - START ))
  print nxfind...
  START=$( date +%s )
  nxfind -f $WRAPPER_NXS -p $F/data -t 500
  STOP=$( date +%s )
  print nxfind time: $(( STOP - START ))
fi

if ! grep -q "^"$SAMPLE/$LABEL/$F $APS/template.txt
then
    print "Template wrapper not found:" $SAMPLE/$LABEL/$F
    exit 0
fi

PARENT=$( grep "^"$SAMPLE/$LABEL $APS/template.txt | clm 2 )
declare PARENT

set -x
nxcopy -f $PARENT -o $WRAPPER_NXS

TRANSFORM_DONE=$APS/${SUBPATH}${F}_transform.done
if [[ ! -f $TRANSFORM_DONE ]]
then
    nxtransform -s $SAMPLE -l $LABEL -d $TEMPERATURE -f $F -p $PARENT
    print $SUBPATH >> $APS/transform_done.txt
    touch $TRANSFORM_DONE
else
  print "skipping transform"
fi

TRANSFORM_DONES=( $APS/${SUBPATH}f{1..3}_transform.done )
stat --format "" $TRANSFORM_DONES || exit 0

nxcombine -s $SAMPLE -l $LABEL -d $TEMPERATURE -f f1 f2 f3

print "WF.SH done at" $(date_nice)
