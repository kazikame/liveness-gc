#!/bin/sh
DIR=`dirname $0`
SCRIPT1=$DIR/touchedAndCopied.awk
AllBMs="lambda nperm treejoin lcss sudoku fibheap nqueens gc_bench"
AllBMs="lambda nperm treejoin lcss sudoku fibheap nqueens knightstour gc_bench "
#AllBMs="sudoku"
echo "\\\\newcommand{\\\\rlratio}{$\\\\frac{RGC}{LGC}$}"
cat <<EOF
\begin{tabular}{|@{\ }c@{\ }| @{\ }r@{\ }|@{\ }r@{\ }| @{\ }r@{\ }|@{\ }r@{\ }| r@{\ }|r@{\ }| r@{\ }|r@{\ }|r@{\ }| r@{\ }|r@{\ }|r@{\ }|}
\hline
  &   \multicolumn{2}{@{}c@{}|}{\#Cells collected/GC}
  &   \multicolumn{2}{c@{}|}{\#Cells touched/GC}
  &   \multicolumn{2}{c@{}|}{\#GCs}
  &   \multicolumn{3}{c@{}|}{\#Drag Cells}
  &   \multicolumn{2}{c@{}|}{GC time (sec)} &Speedup \\\\
\cline{2-12}
{Program}&RGC&LGC&RGC&LGC&RGC&LGC&RGC&LGC&\rlratio&RGC&LGC&\rlratio\\\\
\hline
\hline

EOF
OUTPUTDIR=output.GOLD
for BM in $AllBMs
do
    line=""
    FILE=${OUTPUTDIR}/${BM}_gc-plain_output/garbage-dump.txt
    line="$line `awk -f $SCRIPT1 $FILE`"
    FILE=${OUTPUTDIR}/${BM}_gc-live_output/garbage-dump.txt
    line="$line  `awk -f $SCRIPT1 $FILE`"

    FILE=${OUTPUTDIR}/${BM}_gc-plain_output/general_stats.out
    tmpData=`cut -d: -f2 $FILE`
    data1=`echo $tmpData | cut -d' ' -f2`

    factor=100000
    FILE=${OUTPUTDIR}/${BM}_gc-live_output/general_stats.out
    tmpData=`cut -d: -f2 $FILE`
    data2=`echo $tmpData | cut -d' ' -f2`
    
    d1=$data1
    d2=$data2
    dratio=`bc -l << EOF                                                  
scale=2
$data1/$data2
EOF
`
    line="$line  $d1  $d2 $dratio"

    #GC time to be filled separately
    
    FILE=../benchmarks/programs/${BM}/results/${BM}-gc-plain
    r1=`grep "GC Time" $FILE | cut -d= -f3`
    FILE=../benchmarks/programs/${BM}/results/${BM}-gc-live
    r2=`grep "GC Time" $FILE | cut -d= -f3`
    ratio=`bc -l << EOF                                                  
scale=2
$r1/$r2
EOF
`
    if [ "x$ratio" = "x" ]; then
	ratio="--"
    fi
    dfagen=`grep "DFA Gen Time" $FILE | cut -d= -f2`
    dfastate=`grep "Num of DFA states" $FILE | cut -d= -f2`
    line="$line  $r1  $r2  $ratio $dfastate $dfagen"
    line="\\\\verb@$BM@  $line"
    #echo $line
    echo $line | awk '{
    printf "%s & %.1f & %.1f & %.1f & %.1f & %d & %d & %d & %d & %.2f & %.2f & %.2f & %.2f\n", $1, $2, $5, $3, $6, $4, $7, $8, $9, $10, $11, $12, $13}'
    echo "\\\\\\\\ \\hline"
    echo
    
done

echo "\end{tabular}\\\\\\\\"

echo "(b) Comparing RGC with LGC"


