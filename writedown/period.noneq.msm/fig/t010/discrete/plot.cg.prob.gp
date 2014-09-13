set term post eps enh color font 16 size 8.5cm,6cm
set out 'fig-cg-prob.eps'

set style line 11 lc 1 lw 2 pt 2 lt 1
set style line 12 lc 2 lw 2 pt 2 lt 1
set style line 13 lc 3 lw 2 pt 2 lt 1
set style line 14 lc 4 lw 2 pt 2 lt 1
set style line 15 lc 5 lw 2 pt 2 lt 1
set style line 16 lc 6 lw 2 pt 2 lt 1

set style line 21 lc 1 lw 2 pt 2 lt 2
set style line 22 lc 2 lw 2 pt 2 lt 2
set style line 23 lc 3 lw 2 pt 2 lt 2
set style line 24 lc 4 lw 2 pt 2 lt 2
set style line 25 lc 5 lw 2 pt 2 lt 2

set style line 31 lc 1 lw 2 pt 2 lt 3
set style line 32 lc 2 lw 2 pt 2 lt 3
set style line 33 lc 3 lw 2 pt 2 lt 3
set style line 34 lc 4 lw 2 pt 2 lt 3
set style line 35 lc 5 lw 2 pt 2 lt 3

set style line 41 lc 1 lw 2 pt 2 lt 4
set style line 42 lc 2 lw 2 pt 2 lt 4
set style line 43 lc 3 lw 2 pt 2 lt 4
set style line 44 lc 4 lw 2 pt 2 lt 4
set style line 45 lc 5 lw 2 pt 2 lt 4

set style line 20 lc 0 lw 2 pt 2 lt 2
set style line 111 lt 1 lw 2 pt 7 linecolor rgb "#ffbbbb"

set multiplot

set size 1,1
set origin 0,0
set yrange [0:.17]
set xtics 50
set mxtics 5
set mytics 2
set format y '%.2f'
# set lmargin 8
# set rmargin 2
set xlabel "t [ ps ]"
# set ylabel 'Probability'

# # set key at 1100, 0.5
set key right bottom
set key spacing 1.3
set key font "Helvetica, 14"
# set label 't_{init}=10 ps' at 10, 0.7
set grid

set xrange [0:400]
pl \
'metastable.out' \
u 1:5 every 1 w l ls 11 t 'Brute force', \
'' u 1:($5-$9*2):($5+$9*2) every 1 w filledcu ls 111 not,\
'dis.dih2.tau0.5.b0.e80.5/cg.prob.dih.5sets.out' \
u 1:5 every 1 w l ls 22 t '[0,80] K=2', \
'dis.dih2.tau0.5.b320.e400.5/cg.prob.dih.5sets.out' \
u 1:5 every 1 w l ls 12 t '[320,400] K=2', \
'dis.dih20.tau0.5.b0.e80.5/cg.prob.dih.5sets.out' \
u 1:5 every 1 w l ls 23 t '[0,80] K=20', \
'dis.dih20.tau0.5.b320.e400.5/cg.prob.dih.5sets.out' \
u 1:5 every 1 w l ls 13 t '[320,400] K=20', \
'metastable.out' \
u 1:5 every 1 w l ls 11 not

unset xlabel
unset ylabel
unset grid
unset arrow
unset key
set xtics 10 font "Helvetica, 8"
set ytics 0.01 font "Helvetica, 8"
set mxtics 2
set mytics 1
set size 0.32,0.29
set origin 0.2,0.2
clear
set size 0.42,0.38
set origin 0.13,0.13
set xrange [0:40]
set yrange [0.05: 0.10]
rep

set ytics 0.01 font "Helvetica, 8"
set mytics 2
set xtics 10
set mytics 1
set size 0.20,0.23
set origin 0.72,0.53
clear
set size 0.38,0.38
set origin 0.6,0.46
set xrange [320:340]
set yrange [0.135: 0.165]
rep
