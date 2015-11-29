

# for colored graphs, uncomment next line
set terminal postscript dashed color portrait "Helvetica" 16
# else, for black & white graphs in paper, uncomment next line
#set terminal postscript portrait butt "Helvetica" 16
#set terminal size 1300,600

set size ratio 0 1, .33

set xlabel "gc_bench"
set border 3 lw 0

set output "gc_bench.eps"
set xtics nomirror ("0" 0, "1" 1e5, "2" 2e5, "3" 3e5, "4" 4e5, "5" 5e5, "6" 6e5, "7" 7e5, "8" 8e5, "9" 9e5)
set ytics nomirror ("0" 0, "70" 70e3, "140" 140e3, "210" 210e3)
set autoscale
set grid noxtics

# clolored ... blue, red, black, gray
set style line 1 lt 1 lc  3 lw 1.0 
set style line 2 lt 1 lc  1 lw 1.0
set style line 3 lt 1 lc -1 lw .4
set style line 4 lt 1 lc  9 lw .4

# B/W
# set style line 1 lw 0.90 lc -1  lt 1 
# set style line 2 lw 0.90 lc 9  lt 1
# set style line 3 lw 0.90 lc -1
# set style line 4 lw 0.10 lc -1

plot [:] [0:210e3] "output/gc_bench_gc-live_output/rch.sort"  w l ls 2 notitle,     "output/gc_bench_gc-plain_output/rch.sort" w l ls 1 notitle,     "output/gc_bench_gc-freq_output/rch.sort"  w l ls 3 notitle,           "output/gc_bench_gc-live_output/use.sort"  w l ls 5 notitle

#    EOF
