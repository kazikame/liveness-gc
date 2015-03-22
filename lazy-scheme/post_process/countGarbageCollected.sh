#!/bin/sh
AllBMs="lambda sudoku lcss gc_bench nperm fibheap  knightstour treejoin nqueens "
AllBMs="lambda sudoku treejoin fibheap"
for BM in $AllBMs
do
    for OPT in gc-plain gc-live
    do 
	FILE=output/${BM}_${OPT}_output/GC_STATS.txt
	CELLS=`grep -v "\/\/" $FILE | wc -l`
	FINISH=`grep -n Finish $FILE | cut -d: -f1`
	ALL=`cat $FILE | wc -l `
	
	TAIL=`expr \( $ALL - $FINISH \) - 1`
	GARBAGE=`expr $CELLS - $TAIL`
	
	echo "$GARBAGE cells collected for $BM [$OPT]"
    done
done
