#!/bin/bash
DIR=`dirname $0`
AllBMs="fibheap sudoku nperm paraffins lcss huffman knightstour nqueens deriv treejoin lambda gc_bench "
OUTDIR=output.DFA

declare -A bmdata

bmdata["HEADER"]="\\multicolumn{1}{|r||}{Program};\#CFG Nonterminals;\#CFG Rules;\#DFA States;\#DFA Transitions;DFA Gen Time (sec)"

echo -n "\begin{tabular}{|l|"
for bm in $AllBMs
do 
  echo -n "|c@{\\ }"
done
echo "|} \\\\ \hline"

for bm in $AllBMs
do 
   bmname=`echo $bm | sed -e 's/_/\\\\_/'`
   cfgnt=`wc -l $OUTDIR/$bm/program-cfg.txt | cut -d' ' -f1`
   cfgrl=`fgrep -o '|' $OUTDIR/$bm/program-cfg.txt | wc -l`
   cfgrl=`expr $cfgrl + $cfgnt`
   dfast=`wc -l $OUTDIR/$bm/fsmdump-$bm-state-map | cut -d' ' -f1`
   dfatr=`wc -l $OUTDIR/$bm/fsmdump-$bm-state-transition-table | cut -d' ' -f1`
   gentm=`grep "Program.Exec" ../benchmarks/programs/$bm/results/$bm-gc-live | cut -d'=' -f2 | cut -d' ' -f1`
   gentm=`printf "%.2f\n" $gentm`
   bmdata[$bm]="\\rot{\\tt $bmname};$cfgnt;$cfgrl;$dfast;$dfatr;$gentm" 
done

for i in 1 2 3 4 5 6 
do
  for bm in "HEADER" $AllBMs
  do
    if [ $bm = "HEADER" ]; then sep=""; else sep="&"; fi
    col=`echo ${bmdata[$bm]} | cut -d';' -f$i`
    echo -n "$sep $col "
  done
  echo "\\\\ \\hline"
done
echo "\end{tabular}\\\\"
echo " (a) Data for Liveness Analysis "

