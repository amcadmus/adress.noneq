set term post eps enh color font 14 size 4.2cm,3cm

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

set out 'fig-trans-010-flux-2.eps'
set multiplot
set size 0.4,1
set yrange [-1e-2:1e-2]
set mytics 5
# set format y "%.0t*10^{%L}"
set format y "%.3f"
set origin 0.0,0.0
set lmargin 5.6
set tmargin 0.4
set bmargin 2.7
set rmargin 0
set xtics 50
set mxtics 5
set xrange [0:100]
pl 'ext.mode1.010.Ex.01.00.t1000ps.more/set/meta.flux.smooth.out' u 1:7 ls 11 w l not, '' u 1:8 ls 12 w l not, '' u 1:9 ls 13 w l not, '' u 1:10 ls 14 w l not, '' u 1:11 ls 15 w l not
set label "F_{J,{/Symbol a}'_R}"  at 450,0.006
set size 0.6,1
set origin 0.4,0.0
set lmargin 0
set rmargin 2
set format y ""
set xrange [100:600]
set xtics 200
set mxtics 2

set label 't [ ps ]' at 150, -0.015

pl 'ext.mode1.010.Ex.01.00.t1000ps.more/set/meta.flux.smooth.out' u 1:7 ls 11 w l not, '' u 1:8 ls 12 w l not, '' u 1:9 ls 13 w l not, '' u 1:10 ls 14 w l not, '' u 1:11 ls 15 w l not
unset multiplot

