#!/bin/sh
DIR=`dirname $0`
SCRIPT=$DIR/touchedAndCopied.awk
AllBMs="sudoku lcss gc_bench nperm fibheap  knightstour treejoin nqueens "
AllBMs="sudoku treejoin lambda fibheap"
for BM in $AllBMs
do
    for OPT in gc-plain gc-live
    do 
	FILE=output/${BM}_${OPT}_output/garbage-dump.txt
	echo "==== $BM ($OPT) ==== "
	awk -f $SCRIPT $FILE
    done
done
