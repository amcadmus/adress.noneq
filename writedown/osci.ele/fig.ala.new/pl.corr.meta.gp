set term post eps enh color solid size 8.5cm,6cm
set out 'fig-corr-meta.eps'

set ls 1 lc 1 lw 3 pt 5
set ls 2 lc 2 lw 3 pt 5
set xzeroaxis lt -1
set mxtics 2
set mytics 5
set xrange [0:1000]
set yrange [-0.2:0.2]
set format y "%.2f"
set xlabel 't [ps]'
set ylabel '< j(0) {/Symbol c}_{{/Symbol a}_L}(t) >   [ {/Symbol b}^{-1}ps^{-1} ]'
set label '(a)' at 20, 0.16 font "Helvetica,25"

p 'comb.12.corr.meta.out' u 1:(($10+$20)*0.5):(2*sqrt(($11**2 + $21**2) * 0.5)) every 20 w e ls 1 not, '' u 1:(($10+$20)*0.5) every 20 w l ls 1 not

set out 'fig-corr-meta-c.eps'
set xrange [0:600]
set yrange [0:1.0]
set mytics 2
set mxtics 10
unset label 1
set label '(b)' at 10, 0.9 font "Helvetica,25"

set ylabel 'P_{{/Symbol a}_L}'
set key spacing 1.5

p  'ext.mode1.010.Ex.01.00.t1000ps/set/metastable.out' u 1:6 w l lc 1 lw 3 t'D-NEMD', 'avg.resp.meta.out' u 1:($6-0.) w l lc 2 lw 3 t 'Linear response theory'
