

# for colored graphs, uncomment next line
set terminal postscript dashed color portrait "Helvetica" 16
# else, for black & white graphs in paper, uncomment next line
#set terminal postscript portrait butt "Helvetica" 16
#set terminal size 1300,600

set size ratio 0 1, .25

set xlabel "nqueens"
set border 3 lw 0

set output "nqueens_win.eps"
set xtics nomirror ("3.0" 3.0e5, "3.1" 3.1e5, "3.2" 3.2e5, "3.3" 3.3e5, "3.4" 3.4e5, "3.5" 3.5e5, "3.6" 3.6e5, "3.7" 3.7e5, "3.8" 3.8e5, "3.9" 3.9e5, "4.0" 4.0e5)
set ytics nomirror ("0" 0, "4.5" 4.5e3, "9.0" 9.0e3, "13.5" 13.5e3, "18.0" 18e3, "22.5" 22.5e3)
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

plot [3e5:4e5] [0:23e3] "output/nqueens_gc-live_output/rch.sort"  w l ls 2 notitle,     "output/nqueens_gc-plain_output/rch.sort" w l ls 1 notitle,     "output/nqueens_gc-freq_output/rch.sort"  w l ls 3 notitle,         "output/nqueens_gc-live_output/use.sort"  w l ls 4 notitle

#    EOF
