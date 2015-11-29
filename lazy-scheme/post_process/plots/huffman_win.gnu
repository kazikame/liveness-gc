

# for colored graphs, uncomment next line
set terminal postscript dashed color portrait "Helvetica" 16
# else, for black & white graphs in paper, uncomment next line
#set terminal postscript portrait butt "Helvetica" 16
#set terminal size 1300,600

set size ratio 0 1, .33

set xlabel "huffman*"
set border 3 lw 0

set output "huffman_win.eps"
set xtics nomirror ("0" 0, "7" 7e5, "14" 14e5, "21" 21e5, "28" 28e5, "35" 35e5, "39" 39e5)
set ytics nomirror ("0" 0, "85" 85e3, "170" 170e3, "255" 255e3, "340" 340e3, "425" 425e3)

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

plot [:] [:] "output/huffman_gc-live_output/rch.sort"  w l ls 1 notitle,     "output/huffman_gc-plain_output/rch.sort" w l ls 2 notitle,     "output/huffman_gc-freq_output/rch.sort"  w l ls 3 notitle,      "output/huffman_gc-live_output/rgc.sort"  w l ls 5 notitle,      "output/huffman_gc-live_output/use.sort"  w l ls 4 notitle

#    EOF
