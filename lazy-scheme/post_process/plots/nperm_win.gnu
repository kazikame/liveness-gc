

# for colored graphs, uncomment next line
set terminal postscript dashed color portrait "Helvetica" 16
# else, for black & white graphs in paper, uncomment next line
#set terminal postscript portrait butt "Helvetica" 16
#set terminal size 1300,600

set size ratio 0 1, .33

set xlabel "nperm"
set border 3 lw 0

set output "nperm_win.eps"
set xtics nomirror ("5" 5e5, "6" 6e5, "7" 7e5, "8" 8e5, "9" 9e5)
set ytics nomirror ("12" 12e3, "16" 16e3, "20" 20e3, "24" 24e3, "28" 28e3)
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

plot [5e5:9e5] [12e3:28e3] "output/nperm_gc-live_output/rch.sort"  w l ls 2 notitle,     "output/nperm_gc-plain_output/rch.sort" w l ls 1 notitle,     "output/nperm_gc-freq_output/rch.sort"  w l ls 3 notitle,          "output/nperm_gc-live_output/use.sort"  w l ls 5 notitle

#    EOF
