

# for colored graphs, uncomment next line
set terminal postscript dashed color portrait "Helvetica" 16
# else, for black & white graphs in paper, uncomment next line
#set terminal postscript portrait butt "Helvetica" 16
#set terminal size 1300,600

set size ratio 0 1, .33

set xlabel "deriv*"
set border 3 lw 0

set output "deriv_win.eps"
set xtics nomirror ("0.05" 5e3, "0.10" 10e3, "0.15" 15e3, "0.20" 20e3, "0.25" 25e3, "0.30" 30e3, "0.35" 35e3, "0.40" 40e3, "0.45" 40.5e3, "0.50" 50e3, "0.55" 55e3, "0.60" 60e3)
set ytics nomirror ("3" 3e3, "6" 6e3, "9" 9e3, "12" 12e3)
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

plot [:] [:] "output/deriv_gc-live_output/rch.sort"  w l ls 1 notitle,     "output/deriv_gc-plain_output/rch.sort" w l ls 2 notitle,     "output/deriv_gc-freq_output/rch.sort"  w l ls 3 notitle,      "output/deriv_gc-live_output/use.sort"  w l ls 5 notitle

#    EOF
