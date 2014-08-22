set term post eps enh color font 16 size 8.5cm,6cm
set out 'fig-cg-prob-tau.eps'

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
set yrange [0:.18]
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
# set label 't_{init}=10 ps' at 10, 0.7
set grid

set xrange [0:350]
pl \
'metastable.out' \
u 1:5 every 1 w l ls 11 t 'Brute force', \
'' u 1:($5-$9*2):($5+$9*2) every 1 w filledcu ls 111 not,\
'dis.dih20.tau0.5.b320.e1000/cg.prob.dih.5sets.out' \
u 1:5 every 1 w l ls 12 t '{/Symbol t} = 0.5 ps', \
'dis.dih20.tau1.0.b320.e1000/cg.prob.dih.5sets.out' \
u 1:5 every 1 w l ls 13 t '{/Symbol t} = 1.0 ps', \
'dis.dih20.tau2.0.b320.e1000/cg.prob.dih.5sets.out' \
u 1:5 every 1 w l ls 14 t '{/Symbol t} = 2.0 ps', \
'dis.dih20.tau5.0.b320.e1000/cg.prob.dih.5sets.out' \
u 1:5 every 1 w l ls 15 t '{/Symbol t} = 5.0 ps', \
'metastable.out' \
u 1:5 every 1 w l ls 11 not

unset xlabel
unset ylabel
unset grid
unset arrow
unset key
set xtics 10 font "Helvetica, 8"
set ytics 0.02 font "Helvetica, 8"
set mxtics 2
set size 0.38,0.38
set origin 0.13,0.13
set xrange [0:40]
set yrange [0.05: 0.11]
rep

set ytics 0.01 font "Helvetica, 8"
set mytics 2
set xtics 10
set size 0.4,0.38
set origin 0.55,0.41
set xrange [300:320]
set yrange [0.135: 0.165]
rep
