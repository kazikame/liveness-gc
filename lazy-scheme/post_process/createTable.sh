#!/bin/sh
DIR=`dirname $0`
SCRIPT1=$DIR/touchedAndCopied.awk

#AllBMs="sudoku lcss gc_bench nperm fibheap  knightstour treejoin nqueens "
AllBMs="lambda nperm treejoin lcss sudoku fibheap nqueens gc_bench"
#AllBMs="sudoku"

cat <<EOF
\begin{tabular}{| c | r | r | r | r | r | r  |  r | r | r | r | r |}
\hline
  & \multicolumn{2}{c|}{\# Collected} 
  & \multicolumn{2}{c|}{\# Touched}
  & \multicolumn{2}{c|}{} 
  & \multicolumn{2}{c|}{}
  & \multicolumn{2}{c|}{GC time}
  &  \\\\
  &   \multicolumn{2}{c|}{cells per GC}
  &   \multicolumn{2}{c|}{cells per GC}
  &   \multicolumn{2}{c|}{\#GCs}
  &   \multicolumn{2}{c|}{Drag Reduction (\%)}
  &   \multicolumn{2}{c|}{(sec)} & \\\\
\cline{2-11}
{Program}&RGC&LGC&RGC&LGC&RGC&LGC&\#Cells&Time&LGC&RGC&Speedup \\\\
\hline
\hline

EOF
format="%s & %.2f & %.2f & %.2f & %.2f & %.2f & %.2f & %.2f & %.2f & %.2f & %.2f & %.2f & %.2f"

for BM in $AllBMs
do
    line=""
    FILE=output/${BM}_gc-plain_output/garbage-dump.txt
    line="$line `awk -f $SCRIPT1 $FILE`"
    FILE=output/${BM}_gc-live_output/garbage-dump.txt
    line="$line  `awk -f $SCRIPT1 $FILE`"

    FILE=output/${BM}_gc-plain_output/general_stats.out
    tmpData=`cut -d: -f2 $FILE`
    data1=`echo $tmpData | cut -d' ' -f2`
    data2=`echo $tmpData | cut -d' ' -f3`

    factor=100000
    FILE=output/${BM}_gc-live_output/general_stats.out
    tmpData=`cut -d: -f2 $FILE`
    data3=`echo $tmpData | cut -d' ' -f2`
    data4=`echo $tmpData | cut -d' ' -f3`
    
    d1=`expr $data1 \* 100 / $data3 - 100`
    d2=`expr $data2 \* 100 / $data4 - 100`
    line="$line  $d1  $d2"

    #GC time to be filled separately
    
    FILE=output/${BM}_gc-plain_output/runlog.out
    r1=TBD1
    FILE=output/${BM}_gc-live_output/runlog.out
    r2=TBD2
    line="$line  $r1  $r2  r1/r2"
    line="\\\\verb@$BM@  $line"

    echo $line | awk '{print $1, "&", $2, "&", $5, "&", $3, "&", $6, "&", 
                             $4, "&", $7, "&", $8, "&", $9, "&", $10, "&",
                             $11, "&", $12}'
    echo "\\\\\\\\ \\hline"
done

echo "\end{tabular}"
