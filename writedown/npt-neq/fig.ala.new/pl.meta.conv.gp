set term post eps enh color font 16 size 8.5cm,6cm
set out 'fig-meta-conv.eps'

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

set style line 51 lc 1 lw 3 pt 2 lt 5
set style line 52 lc 2 lw 3 pt 2 lt 5
set style line 53 lc 3 lw 3 pt 2 lt 5
set style line 54 lc 4 lw 3 pt 2 lt 5
set style line 55 lc 5 lw 3 pt 2 lt 5

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
set key at 1000, 0.5
# set label 't_{init}=10 ps' at 10, 0.7
set grid

set xrange [0:100]
pl \
'ext.mode1.010.Ex.01.00.t1000ps/set/metastable.out'	u 1:2 every 4 w l ls 11 not, '' u 1:3 every 4 w l ls 12 not, '' u 1:4 every 4 w l ls 13  not, '' u 1:5 every 4 w l ls 14  not, '' u 1:6 every 4 w l ls 15 not,\
'ext.mode1.010.Ex.01.00.t1000ps.box4.0.r1.0/set/metastable.out'	u 1:2 every 4 w l ls 21 not, '' u 1:3 every 4 w l ls 22 not, '' u 1:4 every 4 w l ls 23  not, '' u 1:5 every 4 w l ls 24  not, '' u 1:6 every 4 w l ls 25 not,\
'ext.mode1.010.Ex.01.00.t1000ps.box4.0.r1.5/set/metastable.out'	u 1:2 every 4 w l ls 41 not, '' u 1:3 every 4 w l ls 42 not, '' u 1:4 every 4 w l ls 43  not, '' u 1:5 every 4 w l ls 44  not, '' u 1:6 every 4 w l ls 45 not

# 'ext.mode1.010.Ex.01.00.t1000ps.fullSD/set/metastable.out'	u 1:2 every 4 w l ls 51 not, '' u 1:3 every 4 w l ls 52 not, '' u 1:4 every 4 w l ls 53  not, '' u 1:5 every 4 w l ls 54  not, '' u 1:6 every 4 w l ls 55 not,\

# 'ext.mode1.010.Ex.01.00.t0600ps/set/metastable.out'	u 1:2 every 4 w l ls 11 not, '' u 1:3 every 4 w l ls 12 not, '' u 1:4 every 4 w l ls 13  not, '' u 1:5 every 4 w l ls 14  not, '' u 1:6 every 4 w l ls 15 not,\

# 'lines' w l ls 20 not,\

# 'ext.mode1.000.Ex.01.00.t1000ps/set/metastable.out'		u 1:2 every 4 w l ls 41 not, '' u 1:3 every 4 w l ls 42 not, '' u 1:4 every 4 w l ls 43  not, '' u 1:5 every 4 w l ls 44  not, '' u 1:6 every 4 w l ls 45 not,\
# 'ext.mode1.050.Ex.01.00.t0600ps/set/metastable.out'		u 1:2 every 4 w l ls 21 not, '' u 1:3 every 4 w l ls 22 not, '' u 1:4 every 4 w l ls 23  not, '' u 1:5 every 4 w l ls 24  not, '' u 1:6 every 4 w l ls 25 not,\


set size 0.6,1
set origin 0.4,0.0
unset ylabel
set lmargin 0
set rmargin 2
set format y ""

set xrange [100:1000]
set xtics 200
set mxtics 2

set label 't [ ps ]' at 200, -0.11
set label 'A_1' at 800,.64
set label 'A_2' at 800,.089
set label 'B_1' at 250,.12
set label 'B_2' at 800,.01
set label 'C'   at 800,.25


pl \
'ext.mode1.010.Ex.01.00.t1000ps/set/metastable.out'	u 1:2 every 48 w l ls 11 t 'L = 2.7 nm, R_{ex} = 1.0 nm', '' u 1:3 every 48 w l ls 12 not, '' u 1:4 every 48 w l ls 13  not, '' u 1:5 every 48 w l ls 14  not, '' u 1:6 every 48 w l ls 15 not,\
'ext.mode1.010.Ex.01.00.t1000ps.box4.0.r1.0/set/metastable.out'	u 1:2 every 48 w l ls 21 t 'L = 4.0 nm, R_{ex} = 1.0 nm', '' u 1:3 every 48 w l ls 22 not, '' u 1:4 every 48 w l ls 23  not, '' u 1:5 every 48 w l ls 24  not, '' u 1:6 every 48 w l ls 25 not,\
'ext.mode1.010.Ex.01.00.t1000ps.box4.0.r1.5/set/metastable.out'	u 1:2 every 48 w l ls 41 t 'L = 4.0 nm, R_{ex} = 1.5 nm', '' u 1:3 every 48 w l ls 42 not, '' u 1:4 every 48 w l ls 43  not, '' u 1:5 every 48 w l ls 44  not, '' u 1:6 every 48 w l ls 45 not

# 'ext.mode1.010.Ex.01.00.t1000ps.fullSD/set/metastable.out'	u 1:2 every 48 w l ls 51 t 'L = 2.7 nm, R_{ex} = 1.0 nm', '' u 1:3 every 48 w l ls 52 not, '' u 1:4 every 48 w l ls 53  not, '' u 1:5 every 48 w l ls 54  not, '' u 1:6 every 48 w l ls 55 not,\

# 'ext.mode1.010.Ex.01.00.t0600ps/set/metastable.out'		u 1:2 every 48 w l ls 11 t 'L = 2.7 nm, R_{ex} = 1.0 nm', '' u 1:3 every 48 w l ls 12 not, '' u 1:4 every 48 w l ls 13  not, '' u 1:5 every 48 w l ls 14  not, '' u 1:6 every 48 w l ls 15 not,\

# 'lines.1' w l ls 20 not,\

# 'ext.mode1.000.Ex.01.00.t1000ps/set/metastable.out'		u 1:2 every 48 w l ls 41 t 't_{init}=10 ps', '' u 1:3 every 48 w l ls 42 not, '' u 1:4 every 48 w l ls 43  not, '' u 1:5 every 48 w l ls 44  not, '' u 1:6 every 48 w l ls 45 not,\
# 'ext.mode1.050.Ex.01.00.t0600ps/set/metastable.out'		u 1:2 every 48 w l ls 21 t 't_{init}=50 ps', '' u 1:3 every 48 w l ls 22 not, '' u 1:4 every 48 w l ls 23  not, '' u 1:5 every 48 w l ls 24  not, '' u 1:6 every 48 w l ls 25 not,\

