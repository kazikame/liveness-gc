

# for colored graphs, uncomment next line
set terminal postscript dashed color portrait "Helvetica" 16
# else, for black & white graphs in paper, uncomment next line
#set terminal postscript portrait butt "Helvetica" 16
#set terminal size 1300,600

set size ratio 0 1, .33

set xlabel "fibheap"
set border 3 lw 0

set output "fibheap.eps"
set xtics nomirror ("0" 0, "8" 8e5, "16" 16e5, "24" 24e5, "32" 32e5, "40" 40e5, "48" 48e5)
set ytics nomirror ("0" 0, "10" 10e3, "20" 20e3, "30" 30e3, "40" 40e3)
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

plot [:48e5] [:40e3] "output/fibheap_gc-live_output/rch.sort"  w l ls 1 notitle,     "output/fibheap_gc-plain_output/rch.sort" w l ls 2 notitle,     "output/fibheap_gc-freq_output/rch.sort"  w l ls 3 notitle,      "output/fibheap_gc-live_output/use.sort"  w l ls 5 notitle

#    EOF
