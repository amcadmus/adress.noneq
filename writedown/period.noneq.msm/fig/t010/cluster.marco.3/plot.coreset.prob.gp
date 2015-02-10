set term post eps enh color font 16 size 8.5cm,6cm
set out 'fig-coreset-prob.eps'

set style line 11 lc 1 lw 2 pt 2 lt 1
set style line 12 lc 2 lw 2 pt 2 lt 1
set style line 13 lc 3 lw 2 pt 2 lt 1
set style line 14 lc 4 lw 2 pt 2 lt 1
set style line 15 lc 5 lw 2 pt 2 lt 1
set style line 16 lc 6 lw 2 pt 2 lt 1

set style line 21 lc 1 lw 2 pt 2 lt 2
set style line 22 lc 2 lw 2 pt 1 lt 2
set style line 23 lc 3 lw 2 pt 5 lt 2
set style line 24 lc 4 lw 2 pt 8 lt 2
set style line 25 lc 5 lw 2 pt 10 lt 2

set style line 31 lc 1 lw 2 pt 3 lt 3
set style line 32 lc 2 lw 2 pt 3 lt 3
set style line 33 lc 3 lw 2 pt 3 lt 3
set style line 34 lc 4 lw 2 pt 3 lt 3
set style line 35 lc 5 lw 2 pt 3 lt 3

set style line 41 lc 1 lw 2 pt 4 lt 4
set style line 42 lc 2 lw 2 pt 4 lt 4
set style line 43 lc 3 lw 2 pt 4 lt 4
set style line 44 lc 4 lw 2 pt 4 lt 4
set style line 45 lc 5 lw 2 pt 4 lt 4

set grid
set xtics 100
set mxtics 10
set mytics 5
set format y '%.2f'
set xrange [0:800]
set xlabel "t [ ps ]"

set key spacing 1.3
set key at 800, .32
set label '\^' at 620,0.315
set label '\^' at 623,0.239
set label '\^' at 638,0.274

pl\
'coreset.prob.out' u 1:2 w l ls 11 t 'p_{{/Symbol a}_R}',\
'coreset.prob.out' u 1:3 w l ls 12 t 'p_{{/Symbol b}}',\
'coreset.prob.out' u 1:4 w l ls 13 t 'p_{{/Symbol a}_L}',\
'coreset.prob.coreset.msm.out' u 1:2 w l ls 21 not,\
'coreset.prob.coreset.msm.out' u 1:3 w l ls 22 not,\
'coreset.prob.coreset.msm.out' u 1:4 w l ls 23 not

