#!/bin/sh
DIR=`dirname $0`
SCRIPT1=$DIR/touchedAndCopied.awk

#AllBMs="sudoku lcss gc_bench nperm fibheap  knightstour treejoin nqueens "
AllBMs="lambda nperm treejoin lcss sudoku fibheap nqueens gc_bench"
#AllBMs="sudoku"

cat <<EOF
\begin{tabular}{|@{\ }c@{\ }|@{\ }r@{\ }|@{\ }r@{\ }|@{\ }r@{\ }|@{\ }r@{\ }|@{\ }r@{\ }|@{\ }r@{\ }|@{\ }r@{\ }|@{\ }r@{\ }@{\ }|@{\ }@{\ }r@{\ }|@{\ }r@{\ }|@{\ }r@{\ }|@{\ }r@{\ }|@{\ }r@{\ }|}
\hline
  & \multicolumn{2}{@{}c@{}|}{DFA}
  & \multicolumn{2}{@{}c@{}|}{\# Collected} 
  & \multicolumn{2}{@{}c@{}|}{\# Touched}
  & \multicolumn{2}{@{}c@{}|}{} 
  & \multicolumn{2}{@{}c@{}|}{}
  & \multicolumn{2}{@{}c@{}|}{GC time}
  &  \\\\ \cline{2-3}
  &   \# & Generation
  &   \multicolumn{2}{@{}c@{}|}{cells per GC}
  &   \multicolumn{2}{@{}c@{}|}{cells per GC}
  &   \multicolumn{2}{@{}c@{}|}{\#GCs}
  &   \multicolumn{2}{@{}c@{}|}{Drag Reduction (\%)}
  &   \multicolumn{2}{@{}c@{}|}{(sec)} & \\\\
\cline{4-13}
{Program}&States&Time (sec) &RGC&LGC&RGC&LGC&RGC&LGC&\#Cells&Time&RGC&LGC&Speedup \\\\
\hline
\hline

EOF
format="%s & %.2f & %.2f & %.2f & %.2f & %.2f & %.2f & %.2f & %.2f & %.2f & %.2f & %.2f & %.2f"
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
    data2=`echo $tmpData | cut -d' ' -f3`

    factor=100000
    FILE=${OUTPUTDIR}/${BM}_gc-live_output/general_stats.out
    tmpData=`cut -d: -f2 $FILE`
    data3=`echo $tmpData | cut -d' ' -f2`
    data4=`echo $tmpData | cut -d' ' -f3`
    
    d1=`expr $data1 \* 100 / $data3 - 100`
    d2=`expr $data2 \* 100 / $data4 - 100`
    line="$line  $d1  $d2"

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
    printf "%s & %d & %.1f & %.1f & %.1f & %.1f & %.1f & %d & %d & %.2f & %.2f & %.2f & %.2f & %.2f\n", $1, $13, $14, $2, $5, $3, $6, $4, $7, $8, $9, $10, $11, $12}'
    echo "\\\\\\\\ \\hline"
    echo
    
done

echo "\end{tabular}"
