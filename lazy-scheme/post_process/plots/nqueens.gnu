

# for colored graphs, uncomment next line
set terminal postscript dashed color portrait "Helvetica" 16
# else, for black & white graphs in paper, uncomment next line
#set terminal postscript portrait butt "Helvetica" 16
#set terminal size 1300,600

set size ratio 0 1, .25

set xlabel "nqueens"
set border 3 lw 0

set output "nqueens.eps"
set xtics nomirror ("0" 0, "2.5" 2.5e5, "5.0" 5e5, "7.5" 7.5e5, "10.0" 10e5, "12.5" 12.5e5, "15.0" 15e5, "17.5" 17.5e5)
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

plot [:17.5e5] "output/nqueens_gc-live_output/rch.sort"  w l ls 2 notitle,     "output/nqueens_gc-plain_output/rch.sort" w l ls 1 notitle,     "output/nqueens_gc-freq_output/rch.sort"  w l ls 3 notitle,           "output/nqueens_gc-live_output/use.sort"  w l ls 5 notitle

#    EOF
