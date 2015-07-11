

# for colored graphs, uncomment next line
set terminal postscript dashed color portrait "Helvetica" 16
# else, for black & white graphs in paper, uncomment next line
#set terminal postscript portrait butt "Helvetica" 16
#set terminal size 1300,600

set size ratio 0 1, .25

set xlabel "lcss"
set border 3 lw 0

set output "lcss_win.eps"
set xtics nomirror ("1.0" 1e5, "1.1" 1.1e5, "1.2" 1.2e5, "1.3" 1.3e5, "1.4" 1.4e5,  "1.5" 1.5e5, "1.6" 1.6e5, "1.7" 1.7e5)
set ytics nomirror ("0" 0, "8" 8e3, "16" 16e3, "24" 24e3)
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

plot [1e5:1.7e5] [0:24e3] "output/lcss_gc-live_output/rch.sort"  w l ls 2 notitle,     "output/lcss_gc-plain_output/rch.sort" w l ls 1 notitle,     "output/lcss_gc-freq_output/rch.sort"  w l ls 3 notitle,        "output/lcss_gc-live_output/use.sort"  w l ls 5 notitle

#    EOF
