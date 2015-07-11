

# for colored graphs, uncomment next line
set terminal postscript dashed color portrait "Helvetica" 16
# else, for black & white graphs in paper, uncomment next line
#set terminal postscript portrait butt "Helvetica" 16
#set terminal size 1300,600

set size ratio 0 1, .25

set xlabel "sudoku"
set border 3 lw 0

set output "sudoku_win.eps"
set xtics nomirror ("0.80" 0.8e5, "0.84" .84e5, "0.88" .88e5, "0.92" .92e5, \
    "1.00" 1e5)
set ytics nomirror ("1.8" 1.8e3, "2.4" 2.4e3, "3.0" 3.0e3, "3.6" 3.6e3, "4.2" 4.2e3)
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

plot [.8e5:1.0e5] [1.8e3:4.2e3] "output/sudoku_gc-live_output/rch.sort"  w l ls 2 notitle,     "output/sudoku_gc-plain_output/rch.sort" w l ls 1 notitle,     "output/sudoku_gc-freq_output/rch.sort"  w l ls 3 notitle,        "output/sudoku_gc-live_output/use.sort"  w l ls 5 notitle

#    EOF
