

# for colored graphs, uncomment next line
set terminal postscript dashed color portrait "Helvetica" 16
# else, for black & white graphs in paper, uncomment next line
#set terminal postscript portrait butt "Helvetica" 16
#set terminal size 1300,600

set size ratio 0 1, .25

set xlabel "(x 100000)"
set ylabel "(x 1000)"
set border 3 lw 0

set output "lambda_win.eps"
set xtics nomirror ("50" 50e5, "51" 51e5, "52" 52e5, "53" 53e5, "54" 54e5, "55" 55e5, "56" 56e5)
set ytics nomirror ("0" 0, "5" 5e3, "10" 10e3, "15" 15e3, "20" 20e3)
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

plot [50e5:56e5] [0:] "output/lambda_gc-live_output/rch.sort"  w l ls 2 notitle,     "output/lambda_gc-plain_output/rch.sort" w l ls 1 notitle,     "output/lambda_gc-freq_output/rch.sort"  w l ls 3 notitle,      "output/lambda_gc-live_output/rgc.sort"  w l ls 5 notitle,      "output/lambda_gc-live_output/use.sort"  w l ls 4 notitle

#    EOF
