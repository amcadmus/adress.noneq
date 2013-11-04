set term post eps enh color font 16 size 8.5cm,6cm
set out 'fig-dipol-order.eps'

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

set xrange [0:1000]
set y2tics
set ytics nomirror
set key bottom right
set yrange [0:8]
set y2range [-.2:.6]
unset xlabel 
set ylabel 'Dipole moment [ Debye ]'
# set y2label 'Orientational order parameter'
set lmargin 6
set rmargin 8
set bmargin 3
set mxtics 10
set mytics 2
set my2tics 2
set grid

set multiplot
set origin 0,0
set size 0.3,1
set rmargin 0
unset y2label
set format y2 ""
set xrange [0:100]
set xtics 50
set mxtics 5

pl \
   '../ext.mode1.010.Ex.01.00.t1000ps.recheck/set/avg.dipole.out'	w l ls 11 ax x1y1 not, \
   '../ext.mode1.010.Ex.01.00.t1000ps.recheck/set/two.vectors.out' u 1:7 every 5 w l ls 21 ax x1y2 not

set origin 0.3,0
set size 0.7,1
set rmargin 8
set lmargin 0
unset ylabel
set format y ""
set format y2 "%.1f"
set y2label 'Orientational order parameter'
set y2label 'Orientational order parameter S_{/Symbol q}'

set xrange[100:1000]
set xtics 200
set mxtics 10
set label 't [ ps ]' at 300, -1
set label "Dipole moment" at 400,7.3
set label "Order parameter S_{/Symbol q}" at 400,5.3
set arrow from 380,7.3 to 280,7.3
set arrow from 880,5.3 to 980,5.3
pl \
   '../ext.mode1.010.Ex.01.00.t1000ps.recheck/set/avg.dipole.out'	w l ls 11 ax x1y1 not, \
   '../ext.mode1.010.Ex.01.00.t1000ps.recheck/set/two.vectors.out' u 1:7 every 20 w l ls 31 ax x1y2 not
