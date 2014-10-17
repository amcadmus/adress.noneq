set term post eps enh color font 16 size 8.5cm,6cm
set out 'fig-field-dipol.eps'

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

set xrange [0:600]
set y2tics
set ytics nomirror
set key bottom right
set yrange [0:1.1]
set y2range [0:9]
set xlabel 't [ ps ]'
set ylabel 'Electric field [ V/nm ]'
set y2label 'Dipole moment [ D ]'
set lmargin 8
set rmargin 6
set mxtics 10
set mytics 2
set my2tics 2
set grid

pl \
   'ext.mode1.010.Ex.01.00.t0600ps/set/field.out'	w l ls 21 ax x1y1 not, \
   'ext.mode1.010.Ex.01.00.t0600ps/set/avg.dipole.out'	w l ls 11 ax x1y2 t't_{init} = 10 ps', \
   'ext.mode1.100.Ex.01.00.t1200ps/set/field.out'	w l ls 22 ax x1y1 not, \
   'ext.mode1.100.Ex.01.00.t1200ps/set/avg.dipole.out'	w l ls 12 ax x1y2 t't_{init} = 100 ps', \
   'ext.mode1.500.Ex.01.00.t1200ps/set/field.out'	w l ls 23 ax x1y1 not, \
   'ext.mode1.500.Ex.01.00.t1200ps/set/avg.dipole.out'	w l ls 13 ax x1y2 t't_{init} = 500 ps'

