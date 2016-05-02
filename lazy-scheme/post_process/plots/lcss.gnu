

# for colored graphs, uncomment next line
set terminal postscript dashed color portrait "NimbusSanL-Regu" 16
# else, for black & white graphs in paper, uncomment next line
#set terminal postscript portrait butt "NimbusSanL-Regu" 16
#set terminal size 1300,600

set size ratio 0 1, .33

set xlabel "lcss"
set border 3 lw 0

set output "lcss.eps"
set xtics nomirror ("0.0" 0, "0.5" .5e5, "1.0" 1.0e5, "1.5" 1.5e5, "2.0" 2e5, "2.5" 2.5e5)
set ytics nomirror ("0.0" 0, "5.5" 5.5e3, "11.0" 11e3, "16.5" 16.5e3, "22.0" 22e3)
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

plot "output/lcss_gc-live_output/rch.sort"  w l ls 1 notitle,     "output/lcss_gc-plain_output/rch.sort" w l ls 2 notitle,     "output/lcss_gc-freq_output/rch.sort"  w l ls 3 notitle,          "output/lcss_gc-live_output/use.sort"  w l ls 5 notitle

#    EOF
