set label 'Q_{J,B_1}, T = 200 ps' at 400,0.5
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

set out 'fig-trans-mode2-0200-iflux-3.eps'

unset xrange
set xtics 1600
set mxtics 4
set yrange [-0.8:0.8]
set mytics 2
set format y "%.1f"
set grid
set tmargin 1.0
set bmargin 2.5
set label 't [ ps ]' at 2400,-1.18

pl 'ext.mode2.200.Ex.01.00.shift0.t6400ps/set/meta.flux.inte.out' u 1:12 ls 11 w l not, '' u 1:13 ls 12 w l not, '' u 1:14 ls 13 w l not, '' u 1:15 ls 14 w l not, '' u 1:16 ls 15 w l not

