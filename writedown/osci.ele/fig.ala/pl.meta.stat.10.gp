set term post eps enh color font 16 size 8.5cm,6cm
set out 'fig-meta-stat-10.eps'

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

set multiplot
set size 0.3,1

set yrange [0:.75]
set mytics 2
set origin 0.0,0.0
set lmargin 4
set rmargin 0
set xtics 50
set mxtics 5

set key at 1000, 0.5
# set label 't_{init}=10 ps' at 10, 0.7

set xrange [0:100]
pl \
'lines' w l ls 20 not,\
'ext.mode1.010.Ex.01.00.t1000ps.nve/set/metastable.out'		u 1:2 every 4 w l ls 11 not, '' u 1:3 every 4 w l ls 12 not, '' u 1:4 every 4 w l ls 13  not, '' u 1:5 every 4 w l ls 14  not, '' u 1:6 every 4 w l ls 15 not,\
'ext.mode1.010.Ex.01.00.t0600ps/set/metastable.out'		u 1:2 every 4 w l ls 21 not, '' u 1:3 every 4 w l ls 22 not, '' u 1:4 every 4 w l ls 23  not, '' u 1:5 every 4 w l ls 24  not, '' u 1:6 every 4 w l ls 25 not


set size 0.7,1
set origin 0.3,0.0
set lmargin 0
set rmargin 2
set format y ""

set xrange [100:1000]
set xtics 200
set mxtics 2

set label 'A_1' at 700,.67
set label 'A_2' at 700,.075
set label 'B_1' at 800,.045
set label 'B_2' at 700,.01
set label 'C' at 700,.23


pl \
'lines.1' w l ls 20 not,\
'ext.mode1.010.Ex.01.00.t1000ps.nve/set/metastable.out'		u 1:2 every 48 w l ls 11 t 'without thermostat', '' u 1:3 every 48 w l ls 12 not, '' u 1:4 every 48 w l ls 13  not, '' u 1:5 every 48 w l ls 14  not, '' u 1:6 every 48 w l ls 15 not,\
'ext.mode1.010.Ex.01.00.t0600ps/set/metastable.out'		u 1:2 every 48 w l ls 21 t 'with thermostat \& barostat', '' u 1:3 every 48 w l ls 22 not, '' u 1:4 every 48 w l ls 23  not, '' u 1:5 every 48 w l ls 24  not, '' u 1:6 every 48 w l ls 25 not

