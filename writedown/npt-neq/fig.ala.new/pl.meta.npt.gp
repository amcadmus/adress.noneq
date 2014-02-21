set term post eps enh color font 16 size 8.5cm,6cm
set out 'fig-meta-npt.eps'

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
set size 0.4,1

set yrange [0:.75]
set mytics 2
set origin 0.0,0.0
set lmargin 7.8
set rmargin 0
set bmargin 3
set xtics 50
set mxtics 5

set ylabel 'P@_I'
unset key
# set key at 1100, 0.5
# set label 't_{init}=10 ps' at 10, 0.7
set grid

set xrange [0:100]
pl \
'ext.mode1.010.Ex.01.00.t1000ps/set/metastable.out'		u 1:2 every 4 w l ls 11 not, '' u 1:3 every 4 w l ls 12 not, '' u 1:4 every 4 w l ls 13  not, '' u 1:5 every 4 w l ls 14  not, '' u 1:6 every 4 w l ls 15 not

# 'ext.mode1.100.Ex.01.00.t1200ps/set/metastable.out'		u 1:2 every 4 w l ls 21 not, '' u 1:3 every 4 w l ls 22 not, '' u 1:4 every 4 w l ls 23  not, '' u 1:5 every 4 w l ls 24  not, '' u 1:6 every 4 w l ls 25 not,\
# 'ext.mode1.500.Ex.01.00.t1200ps/set/metastable.out'		u 1:2 every 4 w l ls 41 not, '' u 1:3 every 4 w l ls 42 not, '' u 1:4 every 4 w l ls 43  not, '' u 1:5 every 4 w l ls 44  not, '' u 1:6 every 4 w l ls 45 not

# 'lines' w l ls 20 not,\

# 'ext.mode1.000.Ex.01.00.t1000ps/set/metastable.out'		u 1:2 every 4 w l ls 41 not, '' u 1:3 every 4 w l ls 42 not, '' u 1:4 every 4 w l ls 43  not, '' u 1:5 every 4 w l ls 44  not, '' u 1:6 every 4 w l ls 45 not,\
# 'ext.mode1.050.Ex.01.00.t0600ps/set/metastable.out'		u 1:2 every 4 w l ls 21 not, '' u 1:3 every 4 w l ls 22 not, '' u 1:4 every 4 w l ls 23  not, '' u 1:5 every 4 w l ls 24  not, '' u 1:6 every 4 w l ls 25 not,\


set size 0.6,1
set origin 0.4,0.0
set lmargin 0
set rmargin 2
set format y ""
unset ylabel

set xrange [100:1000]
set xtics 200
set mxtics 2

set label 't [ ps ]' at 200, -0.11
set label '{/Symbol a}_R' at 800,.60
set label 'A_2' at 800,.085
set label 'B_1' at 900,.045
set label 'B_2' at 800,.01
set label '{/Symbol a}_L' at 800,.28


pl \
'ext.mode1.010.Ex.01.00.t1000ps/set/metastable.out'		u 1:2 every 48 w l ls 11 t 't_{init}=10 ps', '' u 1:3 every 48 w l ls 12 not, '' u 1:4 every 48 w l ls 13  not, '' u 1:5 every 48 w l ls 14  not, '' u 1:6 every 48 w l ls 15 not

# 'ext.mode1.100.Ex.01.00.t1200ps/set/metastable.out'		u 1:2 every 48 w l ls 21 t 't_{init}=100 ps', '' u 1:3 every 48 w l ls 22 not, '' u 1:4 every 48 w l ls 23  not, '' u 1:5 every 48 w l ls 24  not, '' u 1:6 every 48 w l ls 25 not,\
# 'ext.mode1.500.Ex.01.00.t1200ps/set/metastable.out'		u 1:2 every 48 w l ls 41 t 't_{init}=500 ps', '' u 1:3 every 48 w l ls 42 not, '' u 1:4 every 48 w l ls 43  not, '' u 1:5 every 48 w l ls 44  not, '' u 1:6 every 48 w l ls 45 not

# 'lines.1' w l ls 20 not,\

# 'ext.mode1.000.Ex.01.00.t1000ps/set/metastable.out'		u 1:2 every 48 w l ls 41 t 't_{init}=10 ps', '' u 1:3 every 48 w l ls 42 not, '' u 1:4 every 48 w l ls 43  not, '' u 1:5 every 48 w l ls 44  not, '' u 1:6 every 48 w l ls 45 not,\
# 'ext.mode1.050.Ex.01.00.t0600ps/set/metastable.out'		u 1:2 every 48 w l ls 21 t 't_{init}=50 ps', '' u 1:3 every 48 w l ls 22 not, '' u 1:4 every 48 w l ls 23  not, '' u 1:5 every 48 w l ls 24  not, '' u 1:6 every 48 w l ls 25 not,\
