set term post eps enh color font 16 size 8.5cm,6cm
set out 'fig-meta-mode2-2000.eps'

set style line 11 lc 1 lw 3 pt 2 lt 1
set style line 12 lc 2 lw 3 pt 2 lt 1
set style line 13 lc 3 lw 3 pt 2 lt 1
set style line 14 lc 4 lw 3 pt 2 lt 1
set style line 15 lc 5 lw 3 pt 2 lt 1

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

set yrange [0:.7]
set mytics 2
# set lmargin 8
# set rmargin 2
set xtics 400
set mxtics 4
# set ylabel "P_I"
set xlabel "t [ ps ]"
set title "T_P = 2000 ps"

set key at 1100, 0.5
# set label 't_{init}=10 ps' at 10, 0.7
set grid

set xrange [0:3200]
pl \
'ext.mode2.2000.Ex.01.00.shift0.t2500ps/set/metastable.out'		u 1:2 every 4 w l ls 11 not, '' u 1:3 every 4 w l ls 12 not, '' u 1:4 every 4 w l ls 13  not, '' u 1:5 every 4 w l ls 14  not, '' u 1:6 every 4 w l ls 15 not

