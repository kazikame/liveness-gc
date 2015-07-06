
#!/bin/bash 
DIR=`dirname $0`
PROGNAME=`basename $0`
TestBMs="  test testsmall small"
SmallBMs=" nperm fibheap lambda sudoku lcss nqueens gc_bench "
LargeBMs=" treejoin knightstour "
AllBMs="$SmallBMs $LargeBMs"
help()
{
    echo ""
    echo " $EXE : Plots reachable vs live object statistics for all benchmarks"
    echo "        using gnuplot"
    echo ""
    echo " Usage:"
    echo "   $PROGNAME [-genStats] [-bm <benchmark>]"
    echo ""
    echo " Currently supports the following benchmarks:"
    echo "$AllBMs"
    echo " Multiple \"-bm\" options can be specified."
    echo " If \"-bm\" option is not used, then all benchmarks are run."
    echo ""
    echo "   Output file is named <benchmark>.eps."
    echo ""
}

OPTARG="-pp"
while [ $# -gt 0 ]; do
    ARG=$1
    shift
    if [ "$ARG" = "-genStats" ]; then
        OPTARG=""
    elif [ "$ARG" = "-bm" ]; then
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

declare -A limit

limit["nqueens"]=22729
limit["nperm"]=27429
limit["fibheap"]=38001
limit["treejoin"]=1616535
limit["sudoku"]=4075
limit["lcss"]=22243
limit["lambda"]=20466
limit["gc_bench"]=204850
limit["fft"]=3300
limit["small"]=17
limit["test"]=247
limit["testsmall"]=267
limit["knightstour"]=508245

declare -A freq

freq["sudoku"]=2050;
freq["fft"]=2050;
freq["lcss"]=1000;
freq["small"]=1;
freq["test"]=1;
freq["testsmall"]=1;
freq["gc_bench"]=4000;
freq["nperm"]=145;
freq["fibheap"]=700;
freq["knightstour"]=8500;
freq["treejoin"]=1300;
freq["nqueens"]=3000;
freq["lambda"]=100;

declare -A xtics
declare -A ytics
xtics["sudoku"]=2050; ytics["sudoku"]=250
xtics["fft"]=2050; ytics["fft"]=250
xtics["lcss"]=100000; ytics["lcss"]=9000
xtics["small"]=1; ytics["small"]=1
xtics["test"]=25; ytics["test"]=25
xtics["testsmall"]=25; ytics["testsmall"]=25
xtics["gc_bench"]=400000; ytics["gc_bench"]=20000
xtics["nperm"]=145000; ytics["nperm"]=35000
xtics["fibheap"]=70000; ytics["fibheap"]=30000
xtics["knightstour"]=850000; ytics["knightstour"]=40000
xtics["treejoin"]=130000; ytics["treejoin"]=110000
xtics["nqueens"]=6000000; ytics["nqueens"]=105000
xtics["lambda"]=90000; ytics["lambda"]=10000

if [ "x$BMs" = "x" ]; then
    BMs=$AllBMs
fi

for bm in $BMs
do
    for opt in gc-plain gc-live
    #for opt in gc-plain gc-live gc-freq=${freq[$bm]}
    do 
     	echo "Processing $bm [$opt]"
     	$DIR/gen_gc_stats.sh $OPTARG \
     	    -bmdir ../benchmarks/programs/$bm \
     	    -gcopt $opt -heap ${limit[$bm]}
    done
    bash $DIR/val.plot $bm ${xtics[$bm]} ${ytics[$bm]}
    echo "$bm Done"
done
exit


