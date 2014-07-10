set term post eps enh color font 16 size 8.5cm,6cm
set out 'fig-cg-prob.eps'

set style line 11 lc 1 lw 3 pt 2 lt 1
set style line 12 lc 2 lw 3 pt 2 lt 1
set style line 13 lc 3 lw 3 pt 2 lt 1
set style line 14 lc 4 lw 3 pt 2 lt 1
set style line 15 lc 5 lw 3 pt 2 lt 1
set style line 16 lc 6 lw 3 pt 2 lt 1

set style line 21 lc 1 lw 3 pt 2 lt 2
set style line 22 lc 2 lw 3 pt 2 lt 2
set style line 23 lc 3 lw 3 pt 2 lt 2
set style line 24 lc 4 lw 3 pt 2 lt 2
set style line 25 lc 5 lw 3 pt 2 lt 2

set style line 31 lc 1 lw 3 pt 2 lt 3
set style line 32 lc 2 lw 3 pt 2 lt 3
set style line 33 lc 3 lw 3 pt 2 lt 3
set style line 34 lc 4 lw 3 pt 2 lt 3
set style line 35 lc 5 lw 3 pt 2 lt 3

set style line 41 lc 1 lw 3 pt 2 lt 4
set style line 42 lc 2 lw 3 pt 2 lt 4
set style line 43 lc 3 lw 3 pt 2 lt 4
set style line 44 lc 4 lw 3 pt 2 lt 4
set style line 45 lc 5 lw 3 pt 2 lt 4

set style line 20 lc 0 lw 3 pt 2 lt 2

set yrange [0:.4]
set mytics 5
set format y '%.2f'
# set lmargin 8
# set rmargin 2
set xtics 200
set mxtics 2
set xlabel "t [ ps ]"
set ylabel 'P_{{/Symbol a}_L} (t)'

# # set key at 1100, 0.5
set key right bottom
# set label 't_{init}=10 ps' at 10, 0.7
set grid

set xrange [0:1000]
pl \
'metastable.out' \
u 1:6 every 4 w l ls 11 t 'Brute force', \
'dis.5sets.tau0.5.b300.e1000/cg.prob.5sets.out' \
u 1:6 every 4 w l ls 12 t '5 sets', \
'dis.dih05.tau0.5.b300.e1000/cg.prob.dih.5sets.out' \
u 1:6 every 4 w l ls 13 t 'Uni. M=05', \
'dis.dih10.tau0.5.b300.e1000/cg.prob.dih.5sets.out' \
u 1:6 every 4 w l ls 14 t 'Uni. M=10', \
'dis.dih20.tau0.5.b300.e1000/cg.prob.dih.5sets.out' \
u 1:6 every 4 w l ls 15 t 'Uni. M=20', \
'dis.dih30.tau0.5.b300.e1000/cg.prob.dih.5sets.out' \
u 1:6 every 4 w l ls 16 t 'Uni. M=30', \
'metastable.out' \
u 1:6 every 4 w l ls 11 not
