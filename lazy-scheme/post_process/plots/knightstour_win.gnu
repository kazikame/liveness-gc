

# for colored graphs, uncomment next line
set terminal postscript dashed color portrait "NimbusSanL-Regu" 16
# else, for black & white graphs in paper, uncomment next line
#set terminal postscript portrait butt "NimbusSanL-Regu" 16
#set terminal size 1300,600

set size ratio 0 1, .33

set xlabel "knightstour (partial)"
set xtics nomirror ("300" 300e5, "320" 320e5, "340" 340e5, "360" 360e5,"380" 380e5,  "400" 400e5)
set ytics nomirror ("400" 400e3, "500" 500e3, "600" 600e3, "7000" 700e3)
set border 3 lw 0

set output "knightstour_win.eps"
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

plot [300e5:400e5] [400e3:] "output/knightstour_gc-live_output/rch.sort"  w l ls 1 notitle,     "output/knightstour_gc-plain_output/rch.sort" w l ls 2 notitle,     "output/knightstour_gc-freq_output/rch.sort"  w l ls 3 notitle,      "output/knightstour_gc-live_output/use.sort"  w l ls 5 notitle

#    EOF
