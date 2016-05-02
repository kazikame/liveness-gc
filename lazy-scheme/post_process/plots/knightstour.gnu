

# for colored graphs, uncomment next line
set terminal postscript dashed color portrait "NimbusSanL-Regu" 16
# else, for black & white graphs in paper, uncomment next line
#set terminal postscript portrait butt "NimbusSanL-Regu" 16
#set terminal size 1300,600

set size ratio 0 1, .33

set xlabel "knightstour"
set border 3 lw 0

set output "knightstour.eps"
set xtics nomirror ("0" 0, "85" 85e5, "170" 170e5, "255" 255e5, "340" 340e5, "425" 425e5, "510" 510e5, "595" 595e5, "680" 680e5, "765" 765e5, "850" 850e5, "935" 935e5)
set ytics nomirror ("0" 0, "200" 200e3, "400" 400e3, "600" 600e3, "700" 700e3)
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

plot "output/knightstour_gc-live_output/rch.sort"  w l ls 1 notitle,     "output/knightstour_gc-plain_output/rch.sort" w l ls 2 notitle,     "output/knightstour_gc-freq_output/rch.sort"  w l ls 3 notitle,            "output/knightstour_gc-live_output/use.sort"  w l ls 5 notitle

#    EOF
