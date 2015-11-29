
cat > /tmp/run.gnu <<EOF


# for colored graphs, uncomment next line
set terminal postscript dashed color portrait "Helvetica" 16
# else, for black & white graphs in paper, uncomment next line
#set terminal postscript portrait butt "Helvetica" 16
#set terminal size 1300,600

set size ratio 0 1, .33

set xlabel "$1"
set border 3 lw 0

set output "$1.eps"
set xtics nomirror $2
set ytics nomirror $3
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

plot "output/$1_gc-live_output/rch.sort"  w l ls 2 notitle, \
    "output/$1_gc-plain_output/rch.sort" w l ls 1 notitle, \
    "output/$1_gc-freq_output/rch.sort"  w l ls 3 notitle,  \
    "output/$1_gc-live_output/use.sort"  w l ls 4 notitle

#    EOF
EOF

gnuplot /tmp/run.gnu
