

# for colored graphs, uncomment next line
set terminal postscript dashed color portrait "Helvetica" 16
# else, for black & white graphs in paper, uncomment next line
#set terminal postscript portrait butt "Helvetica" 16
#set terminal size 1300,600

set size ratio 0 1, .33

set xlabel "nperm"
set border 3 lw 0

set output "nperm.eps"
set xtics nomirror ("0" 0, "8" 8e5, "16" 16e5, "24" 24e5, "32" 32e5)
set ytics nomirror ("7" 7e3, "14" 14e3, "21" 21e3, "28" 28e3)
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

plot [:34e5] [:28e3] "output/nperm_gc-live_output/rch.sort"  w l ls 1 notitle,     "output/nperm_gc-plain_output/rch.sort" w l ls 2 notitle,     "output/nperm_gc-freq_output/rch.sort"  w l ls 3 notitle,           "output/nperm_gc-live_output/use.sort"  w l ls 5 notitle

#    EOF
