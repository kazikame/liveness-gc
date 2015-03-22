#!/bin/sh 
TEST=$1
MIN=${3-0}
MAX=$2
FULLTEST=../benchmarks/programs/$TEST/anf-$TEST.scm
LOG=run.out.$$
GCOPT=gc-plain


RANGE=`expr $MAX - $MIN`
while [ $RANGE -gt 1 ]; do 
    CUR=`expr \( $MIN  + $MAX \) / 2`
    echo "Running with Memory = $CUR"
    python collector_gc.py $FULLTEST $CUR $GCOPT > $LOG 2>&1
    if grep -q "Memory Exhausted" $LOG; then
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
