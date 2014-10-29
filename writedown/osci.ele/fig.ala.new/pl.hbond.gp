set term post eps enh color font 16 size 8.5cm,6cm
set out 'fig-hbond.eps'

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

set yrange [0.5:3.0]
set mytics 5
set origin 0.0,0.0
set lmargin 7.8
set rmargin 0
set bmargin 3
set xtics 50
set mxtics 5
set format y "%.1f"

set ylabel '# of H-bonds'
unset key
# set key at 1100, 0.5
# set label 't_{init}=10 ps' at 10, 0.7
set grid

set xrange [0:100]
pl \
'ext.mode1.010.Ex.01.00.t1000ps.recheck.record/set/avg.hbond.ala.out'	u 1:($2+$6)  w l ls 11 t 'alanine-solvent',\
'ext.mode1.010.Ex.01.00.t1000ps.recheck.record/set/avg.hbond.sol.out'	u 1:($2/641) w l ls 12 t 'solvent-solvent'

set size 0.6,1
set origin 0.4,0.0
set lmargin 0
set rmargin 2
set format y ""
unset ylabel

set xrange [100:1000]
set xtics 200
set mxtics 2
set key at 950, 1.5

set label 't [ ps ]' at 200, 0.2
set label '(b)' at -550, 3.0 font "Helvetica,30"

pl \
'ext.mode1.010.Ex.01.00.t1000ps.recheck.record/set/avg.hbond.ala.out'	u 1:($2+$6)  w l ls 11 t 'alanine-water',\
'ext.mode1.010.Ex.01.00.t1000ps.recheck.record/set/avg.hbond.sol.out'	u 1:($2/641) w l ls 12 t 'water-water'

