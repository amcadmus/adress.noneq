set term post eps enh color font 16 size 8.5cm,6cm
set out 'fig-mode2-dipol.eps'

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

set style line 20 lc 0 lw 3 pt 2 lt 1

set yrange [-8:8]
set mytics 2
set xtics 50
set mxtics 5
set xlabel 't [ ps ]'
set ylabel 'M_x(t) [ Debye ]'
set rmargin 8

set key bottom right
# set label 't_{init}=10 ps' at 10, 0.7
set grid

set multiplot
set origin 0,0
set size 0.4, 1
set lmargin 6
set rmargin 0
set bmargin 3
unset xlabel

set xrange [0:100]
pl \
'ext.mode1.010.Ex.01.00.t1000ps/set/avg.dipole.out'	w l ls 20 not 'const. field t_{init} = 10 ps', \
'ext.mode2.010.Ex.01.00.shift0.t1000ps/set/avg.dipole.out' w l ls 11 not 'T = 10 ps', \
'ext.mode2.040.Ex.01.00.shift0.t1000ps/set/avg.dipole.out' w l ls 12 not 'T = 40 ps', \
'ext.mode2.200.Ex.01.00.shift0.t6400ps/set/avg.dipole.out' w l ls 13 not 'T =100 ps'

set origin 0.4,0
set size 0.6,1
set lmargin 0
set rmargin 4
set xtics 200
unset ylabel
set format y ""
set mxtics 2

set label 't [ ps ]' at 200, -10
set label '(a)' at -580,8 font "Helvetica,30"

set xrange [100:1000]
rep