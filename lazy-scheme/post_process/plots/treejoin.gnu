

# for colored graphs, uncomment next line
set terminal postscript dashed color portrait "NimbusSanL-Regu" 16
# else, for black & white graphs in paper, uncomment next line
#set terminal postscript portrait butt "NimbusSanL-Regu" 16
#set terminal size 1300,600

set size ratio 0 1, .33

set xlabel "treejoin"
set border 3 lw 0

set output "treejoin.eps"
set xtics nomirror ("0" 0, "7" 7e5, "14" 14e5, "21" 21e5, "28" 28e5, "35" 35e5, "42" 42e5, "49" 49e5, "56" 56e5, "63" 63e5)
set ytics nomirror ("0" 0, "330" 330e3, "660" 660e3, "990" 990e3, "1320" 1320e3, "1650" 1650e3)
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

plot [:63e5] "output/treejoin_gc-live_output/rch.sort"  w l ls 1 notitle,     "output/treejoin_gc-plain_output/rch.sort" w l ls 2 notitle,     "output/treejoin_gc-freq_output/rch.sort"  w l ls 3 notitle,          "output/treejoin_gc-live_output/use.sort"  w l ls 5 notitle

#    EOF
