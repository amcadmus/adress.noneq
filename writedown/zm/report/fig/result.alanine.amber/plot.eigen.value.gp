set term post eps enh color solid font 14 size 8.5cm,6cm

set style line 10 lt 1 lw 3 lc 1 pt 0
set style line 11 lt 1 lw 3 lc 1 pt 1
set style line 12 lt 1 lw 3 lc 1 pt 2
set style line 13 lt 1 lw 3 lc 1 pt 3
set style line 20 lt 1 lw 3 lc 2 pt 0
set style line 21 lt 1 lw 3 lc 2 pt 2
set style line 22 lt 1 lw 3 lc 2 pt 2
set style line 23 lt 1 lw 3 lc 2 pt 3
set style line 30 lt 1 lw 3 lc 3 pt 0
set style line 31 lt 1 lw 3 lc 3 pt 3
set style line 32 lt 1 lw 3 lc 3 pt 2
set style line 33 lt 1 lw 3 lc 3 pt 3
set style line 40 lt 1 lw 3 lc 4 pt 0
set style line 41 lt 1 lw 3 lc 4 pt 4
set style line 42 lt 1 lw 3 lc 4 pt 2
set style line 43 lt 1 lw 3 lc 4 pt 3
set style line 50 lt 1 lw 3 lc 5 pt 0
set style line 51 lt 1 lw 3 lc 5 pt 5
set style line 52 lt 1 lw 3 lc 5 pt 2
set style line 53 lt 1 lw 3 lc 5 pt 3
set style line 60 lt 1 lw 3 lc 9 pt 0
set style line 61 lt 1 lw 3 lc 9 pt 6
set style line 62 lt 1 lw 3 lc 9 pt 2
set style line 63 lt 1 lw 3 lc 9 pt 3

set key bottom right
set xlabel '{/Symbol t} [ ps ]'
set ylabel 'Time scale t [ ps ]'
set grid
# set yrange [0:160]
set xrange [0:20]
set mxtics 5
set mytics 5

# pl \
# 'spme.tune.20ns/nvt/eps.3000.xvg'   u 1:2 w l ls 112 t 'SPME',\
# 'spme.tune.20ns/nvt/eps.10000.xvg'  u 1:2 w l ls 112 not,\
# 'zm.l1.alpha0.00.rc1.2.long/nvt/eps.3000.xvg'  u 1:2 w l ls 122 t 'ZM l=1',\
# 'zm.l1.alpha0.00.rc1.2.long/nvt/eps.10000.xvg' u 1:2 w l ls 122 not,\
# 'zm.l2.alpha0.00.rc1.2.long/nvt/eps.3000.xvg'  u 1:2 w l ls 132 t 'ZM l=2',\
# 'zm.l2.alpha0.00.rc1.2.long/nvt/eps.10000.xvg' u 1:2 w l ls 132 not,\
# 'zm.l3.alpha0.00.rc1.2.long/nvt/eps.3000.xvg'  u 1:2 w l ls 142 t 'ZM l=3',\
# 'zm.l3.alpha0.00.rc1.2.long/nvt/eps.10000.xvg' u 1:2 w l ls 142 not,\
# 'zm.l4.alpha0.00.rc1.2.long/nvt/eps.3000.xvg'  u 1:2 w l ls 152 t 'ZM l=4',\
# 'zm.l4.alpha0.00.rc1.2.long/nvt/eps.10000.xvg' u 1:2 w l ls 152 not
set yrange [1000:2200]
set key top left
set out 'fig-eig-val-1.eps'
pl \
'spme.1mus/eig.avg.out'			u 1:2:($6*2) every 5 w e ls 10 not 'SPME', '' u 1:2 w l ls 11 t'SPME',\
'rf.rc1.2.1/eig.avg.out'			u 1:2:($6*2) every 5 w dot ls 60 not 'RF', '' u 1:2 w l ls 61 t 'RF',\
'zm.l1.alpha0.00.rc1.2/eig.avg.out'	u 1:2:($6*2) every 5 w dot ls 20 not 'ZM l=1', '' u 1:2 w l ls 21 t 'ZM l=1',\
'zm.l2.alpha0.00.rc1.2/eig.avg.out'	u 1:2 w dot ls 31 not 'ZM l=2', '' u 1:2 w l ls 31 t 'ZM l=2',\
'zm.l3.alpha0.00.rc1.2/eig.avg.out'	u 1:2 w dot ls 41 not 'ZM l=3', '' u 1:2 w l ls 41 t 'ZM l=3',\
'zm.l4.alpha0.00.rc1.2/eig.avg.out'	u 1:2:($6*2) every 5 w e ls 50 not 'ZM l=4', '' u 1:2 w l ls 51 t 'ZM l=4',\
'spme.1mus/eig.avg.out'			u 1:2:($6*2) every 5 w e ls 10 not 'SPME', '' u 1:2 w l ls 11 not'SPME'

set yrange [20:80]
set key at 14,44 bottom left
set out 'fig-eig-val-2.eps'
pl \
'spme.1mus/eig.avg.out'			u 1:3:($7*2) every 5 w e ls 10 not 'SPME', '' u 1:3 w l ls 11 t'SPME',\
'rf.rc1.2.1/eig.avg.out'			u 1:3:($7*2) every 5 w e ls 60 not 'RF', '' u 1:3 w l ls 61 t 'RF',\
'zm.l1.alpha0.00.rc1.2/eig.avg.out'	u 1:3:($7*2) every 5 w e ls 20 not 'ZM l=1', '' u 1:3 w l ls 21 t 'ZM l=1',\
'zm.l2.alpha0.00.rc1.2/eig.avg.out'	u 1:3 w dot ls 31 not 'ZM l=2', '' u 1:3 w l ls 31 t 'ZM l=2',\
'zm.l3.alpha0.00.rc1.2/eig.avg.out'	u 1:3 w dot ls 41 not 'ZM l=3', '' u 1:3 w l ls 41 t 'ZM l=3',\
'zm.l4.alpha0.00.rc1.2/eig.avg.out'	u 1:3 w dot ls 50 not 'ZM l=4', '' u 1:3 w l ls 51 t 'ZM l=4',\
'spme.1mus/eig.avg.out'			u 1:4:($8*2) every 5 w e ls 10 not 'SPME', '' u 1:4 w l ls 11 not'SPME',\
'rf.rc1.2.1/eig.avg.out'			u 1:4 w dot ls 61 not 'RF', '' u 1:4 w l ls 61 not 'RF',\
'zm.l1.alpha0.00.rc1.2/eig.avg.out'	u 1:4 w dot ls 21 not 'ZM l=1', '' u 1:4 w l ls 21 not 'ZM l=1',\
'zm.l2.alpha0.00.rc1.2/eig.avg.out'	u 1:4 w dot ls 31 not 'ZM l=2', '' u 1:4 w l ls 31 not 'ZM l=2',\
'zm.l3.alpha0.00.rc1.2/eig.avg.out'	u 1:4 w dot ls 41 not 'ZM l=3', '' u 1:4 w l ls 41 not 'ZM l=3',\
'zm.l4.alpha0.00.rc1.2/eig.avg.out'	u 1:4 w dot ls 51 not 'ZM l=4', '' u 1:4 w l ls 51 not 'ZM l=4',\
'spme.1mus/eig.avg.out'			u 1:3:($7*2) every 5 w e ls 10 not 'SPME', '' u 1:3 w l ls 11 not'SPME',\
'spme.1mus/eig.avg.out'			u 1:4:($8*2) every 5 w e ls 10 not 'SPME', '' u 1:4 w l ls 11 not'SPME'




