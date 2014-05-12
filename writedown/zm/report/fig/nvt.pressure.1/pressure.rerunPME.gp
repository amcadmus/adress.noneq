set term post eps enh color solid

set ls 100 lc 0 lw 3 pt 5
set ls 11 lc 1 lw 3 pt 4
set ls 12 lc 1 lw 3 pt 7
set ls 21 lc 2 lw 3 pt 4
set ls 22 lc 2 lw 3 pt 7
set ls 31 lc 3 lw 3 pt 4
set ls 32 lc 3 lw 3 pt 7
set ls 41 lc 4 lw 3 pt 4
set ls 42 lc 4 lw 3 pt 7
set ls 51 lc 5 lw 3 pt 4
set ls 61 lc 6 lw 3 pt 4

set xrange [0.8:2.8]
set yrange [-70:40]
set mxtics 2
set ytics 10
set mytics 2
set grid
set xlabel 'r_c [ nm ]'
set ylabel 'Pressure [ Bar ]'

set out 'pressure.l1.rerunPME.eps'

pl \
1 w l ls 100,\
'pressure.l1.alpha0.00.rerunPME.out' w l ls 11 not 'order=1, {/Symbol a}=1.2', '' u 1:2:($3*2)  w e ls 11 t 'order=1, {/Symbol a}=0.0',\
'pressure.l1.alpha0.50.rerunPME.out' w l ls 21 not 'order=1, {/Symbol a}=1.2', '' u 1:2:($3*2)  w e ls 21 t 'order=1, {/Symbol a}=0.5',\
'pressure.l1.alpha1.00.rerunPME.out' w l ls 31 not 'order=1, {/Symbol a}=1.2', '' u 1:2:($3*2)  w e ls 31 t 'order=1, {/Symbol a}=1.0',\
'pressure.l1.alpha1.50.rerunPME.out' w l ls 41 not 'order=1, {/Symbol a}=1.2', '' u 1:2:($3*2)  w e ls 41 t 'order=1, {/Symbol a}=1.5',\
'pressure.l1.alpha2.00.rerunPME.out' w l ls 51 not 'order=1, {/Symbol a}=1.2', '' u 1:2:($3*2)  w e ls 51 t 'order=1, {/Symbol a}=2.0',\
'pressure.rf.rerunPME.out' w l ls 61 not 'RF', '' u 1:2:($3*2)  w e ls 61 t 'RF'

set out 'pressure.l2.rerunPME.eps'

pl \
1 w l ls 100,\
'pressure.l2.alpha0.00.rerunPME.out' w l ls 11 not 'order=1, {/Symbol a}=1.2', '' u 1:2:($3*2)  w e ls 11 t 'order=2, {/Symbol a}=0.0',\
'pressure.l2.alpha0.50.rerunPME.out' w l ls 21 not 'order=1, {/Symbol a}=1.2', '' u 1:2:($3*2)  w e ls 21 t 'order=2, {/Symbol a}=0.5',\
'pressure.l2.alpha1.00.rerunPME.out' w l ls 31 not 'order=1, {/Symbol a}=1.2', '' u 1:2:($3*2)  w e ls 31 t 'order=2, {/Symbol a}=1.0',\
'pressure.l2.alpha1.50.rerunPME.out' w l ls 41 not 'order=1, {/Symbol a}=1.2', '' u 1:2:($3*2)  w e ls 41 t 'order=2, {/Symbol a}=1.5',\
'pressure.l2.alpha2.00.rerunPME.out' w l ls 51 not 'order=1, {/Symbol a}=1.2', '' u 1:2:($3*2)  w e ls 51 t 'order=2, {/Symbol a}=2.0'

set out 'pressure.l3.rerunPME.eps'

pl \
1 w l ls 100,\
'pressure.l3.alpha0.00.rerunPME.out' w l ls 11 not 'order=1, {/Symbol a}=1.2', '' u 1:2:($3*2)  w e ls 11 t 'order=3, {/Symbol a}=0.0',\
'pressure.l3.alpha0.50.rerunPME.out' w l ls 21 not 'order=1, {/Symbol a}=1.2', '' u 1:2:($3*2)  w e ls 21 t 'order=3, {/Symbol a}=0.5',\
'pressure.l3.alpha1.00.rerunPME.out' w l ls 31 not 'order=1, {/Symbol a}=1.2', '' u 1:2:($3*2)  w e ls 31 t 'order=3, {/Symbol a}=1.0',\
'pressure.l3.alpha1.50.rerunPME.out' w l ls 41 not 'order=1, {/Symbol a}=1.2', '' u 1:2:($3*2)  w e ls 41 t 'order=3, {/Symbol a}=1.5',\
'pressure.l3.alpha2.00.rerunPME.out' w l ls 51 not 'order=1, {/Symbol a}=1.2', '' u 1:2:($3*2)  w e ls 51 t 'order=3, {/Symbol a}=2.0'


set out 'pressure.l4.rerunPME.eps'

pl \
1 w l ls 100,\
'pressure.l4.alpha0.00.rerunPME.out' w l ls 11 not 'order=1, {/Symbol a}=1.2', '' u 1:2:($3*2)  w e ls 11 t 'order=4, {/Symbol a}=0.0',\
'pressure.l4.alpha0.50.rerunPME.out' w l ls 21 not 'order=1, {/Symbol a}=1.2', '' u 1:2:($3*2)  w e ls 21 t 'order=4, {/Symbol a}=0.5',\
'pressure.l4.alpha1.00.rerunPME.out' w l ls 31 not 'order=1, {/Symbol a}=1.2', '' u 1:2:($3*2)  w e ls 31 t 'order=4, {/Symbol a}=1.0',\
'pressure.l4.alpha1.50.rerunPME.out' w l ls 41 not 'order=1, {/Symbol a}=1.2', '' u 1:2:($3*2)  w e ls 41 t 'order=4, {/Symbol a}=1.5',\
'pressure.l4.alpha2.00.rerunPME.out' w l ls 51 not 'order=1, {/Symbol a}=1.2', '' u 1:2:($3*2)  w e ls 51 t 'order=4, {/Symbol a}=2.0'


# 'pressure.l3.out' w l ls 31 t 'order=3', '' u 1:2:($3*2)  w e ls 31 not,\

set out 'pressure.rf.rerunPME.eps'

pl \
1 w l ls 100,\
'pressure.rf.rerunPME.out' w l ls 11 not 'RF', '' u 1:2:($3*2)  w e ls 11 t 'RF'

set out 'pressure.methods.rerunPME.eps'

pl \
1 w l ls 100 t '1 Bar',\
'pressure.l1.alpha0.00.rerunPME.out' w l ls 11 not, '' u 1:2:($3*2)  w e ls 11 t 'order=1, {/Symbol a}=0.0',\
'pressure.l2.alpha0.00.rerunPME.out' w l ls 21 not, '' u 1:2:($3*2)  w e ls 21 t 'order=2, {/Symbol a}=0.0',\
'pressure.l3.alpha0.00.rerunPME.out' w l ls 31 not, '' u 1:2:($3*2)  w e ls 31 t 'order=3, {/Symbol a}=0.0',\
'pressure.l4.alpha0.00.rerunPME.out' w l ls 41 not, '' u 1:2:($3*2)  w e ls 41 t 'order=4, {/Symbol a}=0.0',\
'pressure.rf.rerunPME.out' w l ls 51 not, '' u 1:2:($3*2)  w e ls 51 t 'Reaction-Field {/Symbol e}_{rf} = 80'

