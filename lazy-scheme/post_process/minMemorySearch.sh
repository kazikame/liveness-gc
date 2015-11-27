#!/bin/sh 
TEST=$1
MIN=${3-0}
MAX=$2
FULLTEST=../benchmarks/programs/$TEST/$TEST.scm
LOG=run.out.$$
GCOPT=gc-live


RANGE=`expr $MAX - $MIN`
while [ $RANGE -gt 1 ]; do 
    CUR=`expr \( $MIN  + $MAX \) / 2`
    echo "Running with Memory = $CUR"
    ./Simulator $FULLTEST $CUR $GCOPT > $LOG 2>&1
    if grep -q "No Sufficient Memory" $LOG; then
	# increase MEMORY
	MIN=$CUR
    else
	# decrease MEMORY
	MAX=$CUR
    fi
    RANGE=`expr $MAX - $MIN`
done
echo MIN=$MIN
echo MAX=$MAX
exit
