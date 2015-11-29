

# for colored graphs, uncomment next line
set terminal postscript dashed color portrait "Helvetica" 16
# else, for black & white graphs in paper, uncomment next line
#set terminal postscript portrait butt "Helvetica" 16
#set terminal size 1300,600

set size ratio 0 1, .33

set xlabel "paraffins"
set border 3 lw 0

set output "paraffins.eps"
set xtics nomirror (".03" 3e3, ".06" 6e3, ".09" 9e3, ".12" 12e3, ".15" 15e3)
set ytics nomirror ("2" 2e3, "4" 4e3, "6" 6e3)
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

plot "output/paraffins_gc-live_output/rch.sort"  w l ls 1 notitle,     "output/paraffins_gc-plain_output/rch.sort" w l ls 2 notitle,     "output/paraffins_gc-freq_output/rch.sort"  w l ls 3 notitle,      "output/paraffins_gc-live_output/rgc.sort"  w l ls 5 notitle,      "output/paraffins_gc-live_output/use.sort"  w l ls 4 notitle

#    EOF
