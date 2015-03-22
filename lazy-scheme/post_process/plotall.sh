#!/bin/bash 
DIR=`dirname $0`
PROGNAME=`basename $0`
AllBMs="   lambda sudoku fft lcss gc_bench nperm fibheap knightstour treejoin nqueens "
help()
{
    echo ""
    echo " $EXE : Plots reachable vs live object statistics for all benchmarks"
    echo "        using gnuplot"
    echo ""
    echo " Usage:"
    echo "   $PROGNAME [-bm <benchmark>]"
    echo ""
    echo " Currently supports the following benchmarks:"
    echo "$AllBMs"
    echo " Multiple \"-bm\" options can be specified."
    echo " If \"-bm\" option is not used, then all benchmarks are run."
    echo ""
    echo "   Output file is named <benchmark>.eps."
    echo ""
}

OPTARG="-pp -reuseanf"
while [ $# -gt 0 ]; do
    ARG=$1
    shift
    if [ "$ARG" = "-bm" ]; then
	BMs="$BMs $1"
	shift
    else
        echo ""
        echo "      ERROR!!! Bag argument or filename" $ARG
        echo ""
        help
        exit 1
    fi
done

if [ "x$BMs" = "x" ]; then
    BMs=$AllBMs
fi

for bm in $BMs
do
    bash $DIR/val.plot $bm 
    echo "$bm Done"
done
exit


