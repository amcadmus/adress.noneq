set label 'Q_{J,C}, T_P = 40 ps' at 50,0.4
set lmargin 4.5
set rmargin 2
unset tmargin
unset bmargin
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

set out 'fig-trans-mode2-0040-iflux-5.eps'

set xrange [0:1000]
set xtics 200
set mxtics 2
set yrange [-0.2:0.5]
set ytics 0.1
set mytics 2
set format y "%.1f"
set grid
set tmargin 1.0
set bmargin 2.5
set label 't [ ps ]' at 400,-0.35

pl 'ext.mode2.040.Ex.01.00.shift0.t1000ps/set/meta.flux.inte.out' u 1:22 ls 11 w l not, '' u 1:23 ls 12 w l not, '' u 1:24 ls 13 w l not, '' u 1:25 ls 14 w l not, '' u 1:26 ls 15 w l not

