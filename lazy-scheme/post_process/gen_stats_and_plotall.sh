
#!/bin/bash 
DIR=`dirname $0`
PROGNAME=`basename $0`
TestBMs="  test testsmall small"
SmallBMs=" nperm fibheap lambda sudoku lcss nqueens huffman "
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

limit["nqueens"]=10101
limit["nperm"]=27428
limit["fibheap"]=37043
limit["treejoin"]=1616533
limit["sudoku"]=4066
limit["lcss"]=22243
limit["lambda"]=20466
limit["knightstour"]=677800
limit["fft"]=172762
limit["huffman"]=100070
## UNUSED BMs
limit["small"]=17
limit["test"]=247
limit["testsmall"]=267

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
freq["huffman"]=100;

declare -A xtics
declare -A ytics
xtics["small"]=1		; ytics["small"]=1
xtics["test"]=25		; ytics["test"]=25
xtics["testsmall"]=25		; ytics["testsmall"]=25
xtics["fft"]=2050		; ytics["fft"]=250
xtics["sudoku"]=0.25e5		; ytics["sudoku"]=0.6e3
xtics["lcss"]=.9e5		; ytics["lcss"]=4e3
xtics["gc_bench"]=1.9e5   	; ytics["gc_bench"]=70e3
xtics["nperm"]=8.5e5		; ytics["nperm"]=3.5e3
xtics["fibheap"]=8e5		; ytics["fibheap"]=10e3
xtics["knightstour"]=8.5e5	; ytics["knightstour"]=40e3
xtics["treejoin"]=13e5  	; ytics["treejoin"]=330e3
xtics["nqueens"]=3.5e5    	; ytics["nqueens"]=4.6e3
xtics["lambda"]=19e5		; ytics["lambda"]=3e3
xtics["huffman"]=19e5		; ytics["huffman"]=3e3

declare -A xlimit
declare -A ylimit
xlimit["fft"]=2050		; ylimit["fft"]=250
xlimit["lcss"]=1e5:1.7e5	; ylimit["lcss"]=0:24e3
xlimit["small"]=1		; ylimit["small"]=1
xlimit["test"]=25		; ylimit["test"]=25
xlimit["testsmall"]=25		; ylimit["testsmall"]=25
xlimit["sudoku"]=.8e5:1.0e5	; ylimit["sudoku"]=1.8e3:4.2e3
xlimit["gc_bench"]=0:4e5	; ylimit["gc_bench"]=0:20e3
xlimit["nperm"]=5e5:9e5	        ; ylimit["nperm"]=12e3:28e3
xlimit["fibheap"]=2.6e6:2.9e6	; ylimit["fibheap"]=31000:38000
xlimit["knightstour"]=850000	; ylimit["knightstour"]=40000
xlimit["treejoin"]=3.9e6:4.5e6	; ylimit["treejoin"]=1320e3:1650e3
xlimit["nqueens"]=3e5:4e5	; ylimit["nqueens"]=0:23e3
xlimit["lambda"]=50e5:56e5	; ylimit["lambda"]=:
xlimit["huffman"]=:             ; ylimit["huffman"]=:

if [ "x$BMs" = "x" ]; then
    BMs=$AllBMs
fi

for bm in $BMs
do
    for opt in gc-plain gc-live gc-freq=${freq[$bm]}
    #for opt in gc-plain gc-live
    do 
      	echo "Processing $bm [$opt]"
      	$DIR/gen_gc_stats.sh $OPTARG \
      	    -bmdir ../benchmarks/programs/$bm \
      	    -gcopt $opt -heap ${limit[$bm]}
     done
    bash $DIR/val.plot $bm ${xtics[$bm]} ${ytics[$bm]}
    bash $DIR/window.plot $bm ${xlimit[$bm]} ${ylimit[$bm]} 
    gnuplot $DIR/plots/${bm}.gnu
    gnuplot $DIR/plots/${bm}_win.gnu

    echo "$bm Done"
done
exit


