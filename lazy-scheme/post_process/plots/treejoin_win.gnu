

# for colored graphs, uncomment next line
set terminal postscript dashed color portrait "NimbusSanL-Regu" 16
# else, for black & white graphs in paper, uncomment next line
#set terminal postscript portrait butt "NimbusSanL-Regu" 16
#set terminal size 1300,600

set size ratio 0 1, .33

set xlabel "treejoin (partial)"
set border 3 lw 0

set output "treejoin_win.eps"
set xtics nomirror ("39" 39e5, "40" 40e5, "41" 41e5, "42" 42e5, "43" 43e5, "44" 44e5, "45" 45e5)
set ytics nomirror ("1430" 1430e3, "1480" 1480e3, "1530" 1530e3, "1580" 1580e3, "1630" 1630e3)
set autoscale
set grid noxtics

# colored ... blue, red, black, gray
set style line 1 lt 1 lc  3 lw 1.0 
set style line 2 lt 1 lc  1 lw 1.0
set style line 3 lt 1 lc -1 lw .4
set style line 4 lt 1 lc  9 lw .4

# B/W
# set style line 1 lw 0.90 lc -1  lt 1 
# set style line 2 lw 0.90 lc 9  lt 1
# set style line 3 lw 0.90 lc -1
# set style line 4 lw 0.10 lc -1

plot [3.9e6:4.5e6] [1430e3:1630e3] "output/treejoin_gc-live_output/rch.sort"  w l ls 1 notitle,     "output/treejoin_gc-plain_output/rch.sort" w l ls 2 notitle,     "output/treejoin_gc-freq_output/rch.sort"  w l ls 3 notitle,           "output/treejoin_gc-live_output/use.sort"  w l ls 5 notitle

#    EOF
