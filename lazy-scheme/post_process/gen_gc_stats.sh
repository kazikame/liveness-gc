#! /bin/bash

PROGNAME=`basename $0`
DIRNAME=`dirname $0`
PPONLY="false"
ANFREUSE="false"
CPWD=`pwd`
echo "Current dir is `pwd`"
DIRNAME=`echo $DIRNAME | sed -e "s|\.|$CPWD|"`
echo "Script home is $DIRNAME"

parse()
{
    if [ $# -eq 0 ]; then
	help
	exit 0
    fi

    while [ $# -gt 0 ]; do
	ARG=$1
	shift
	if [ "$ARG" = "-pp" ]; then
	    PPONLY="true"
	elif [ "$ARG" = "-help" -o "$ARG" = "-h" ]; then
	    help
	    exit 0
	elif [ "$ARG" =  "-bmdir" ]; then
	    TEST="$1"
	    shift
	elif [ "$ARG" =  "-gcopt" ]; then
	    GCOPT="$1"
	    shift
	elif [ "$ARG" =  "-heap" ]; then
	    HEAPSIZE="$1"
	    shift
	elif [ "$ARG" =  "-reuseanf" ]; then
	    ANFREUSE="true"
	else
	    echo ""
	    echo "      ERROR!!! Bag argument or filename" $ARG
	    echo ""
	    help
	    exit 1
	fi
    done
}

help()
{
    echo ""
    echo " $PROGNAME : Generates garbage collection statistics for Scheme programs"
    echo ""
    echo " Usage:"
    echo "   $PROGNAME [-pp] [-h|-help] -bmdir </path/to/benchmark/directory> -gcopt <gc_option> -heap <size>"
    echo ""
    echo " Options:"
    echo "   -pp       : post processing only. Scheme simulator is not run."
    echo "   -h|-help  : this help."
    echo "   -bmdir    : benchmark directory to process"
    echo "   -gcopt    : garbage collection type -- one of 'gc-disable', 'gc-plain', 'gc-live', 'gc-freq'"
    echo "   -heap     : heap size (half heap for copying collector)"
    echo "   -reuseanf : reuse anf file for the test (if anf file exists)"
    echo ""
    echo " Options -bmdir, -gcopt, and -heap should all be specified together." 
    echo ""
}

# main program
parse $*

if [ "$PPONLY" = "false" -a ! -d $TEST ]; then
    echo "Error! Bad Benchmark"
    help
    exit 1
fi

TESTNAME=`basename $TEST`
OUTPUT=$CPWD/output/${TESTNAME}_${GCOPT}_output
mkdir -p $OUTPUT;

LOG=$OUTPUT/runtime
echo -n 'Start At : ' > $LOG
date                 >> $LOG

STATSFILE=GC_STATS.txt
SCMFILE=$TEST/${TESTNAME}.scm
ANFFILE=$TEST/anf-${TESTNAME}.scm
if [ "$PPONLY" = "false" ]; then
    # run the test
    export LD_LIBRARY_PATH=.:$D_LIBRARY_PATH
    if [ "$ANFREUSE" = "false" -o ! -f $ANFFILE ]; then
      python anf.py $SCMFILE > $ANFFILE
    fi
    python collector_gc.py $ANFFILE $HEAPSIZE $GCOPT 
    if [ $? = 0 ]; then
       mv output/$STATSFILE $OUTPUT/$STATSFILE
       mv output/garbage-dump.txt $OUTPUT/
    else
       help
       exit 1
    fi
fi

cd $OUTPUT/
if [ "$PPONLY" = "true" -a ! -f $STATSFILE ]; then
    echo "Error! No stats file for $TESTNAME"
    help
    exit 1
fi

STATOUTFILES="rch use gbg" # ignore lag void for now
for file in $STATOUTFILES; do
    rm -f ${file}.out
done

PPSCRIPT=$DIRNAME/process
PPSCRIPT=$DIRNAME/process_fast
OUTFILE=general_stats.out
$PPSCRIPT $STATSFILE  > $OUTFILE

for file in $STATOUTFILES; do
    infile="${file}.out"
    sortfile="${file}.sort"
    sort -n $infile        > $sortfile
done

for file in rch.sort use.sort gbg.sort; do
    endtimefile="endtime"
    et=`awk '{print $2+0}' $endtimefile`
    echo "$et 0"           >> $file
done

echo -n 'End At   : '>> $LOG
date                 >> $LOG

