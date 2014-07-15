set term post eps enh color solid font 14 size 8.5cm,6cm
set out 'fig-dih-dist-psi.eps'

set multiplot

set style line 111 lt 1 lw 3 pt 7 linecolor rgb "#ffbbbb"
set style line 112 lt 1 lw 3 pt 0 linecolor rgb "#ff0000"
set style line 121 lt 1 lw 3 pt 7 linecolor rgb "#99ff99"
set style line 122 lt 1 lw 3 pt 0 linecolor 2
set style line 131 lt 1 lw 3 pt 7 linecolor rgb "#bbbbff"
set style line 132 lt 1 lw 3 pt 0 linecolor 3
set style line 141 lt 1 lw 3 pt 7 linecolor rgb "#ffccff"
set style line 142 lt 1 lw 3 pt 0 linecolor 4
#set style line 151 lt 1 lw 3 pt 7 linecolor rgb "#bbffff"
set style line 151 lt 1 lw 3 pt 7 linecolor rgb "#CEF6F5"
set style line 152 lt 1 lw 3 pt 0 linecolor 5
set style line 162 lt 1 lw 3 pt 0 lc 9

set origin 0,0
set size 1,1

set xrange [-180:180]
set yrange [0:0.018]
set xtics 60
set mxtics 6
set mytics 2
set format y "%.3f"
set grid
set xlabel '{/Symbol y} [ deg. ]'
set ylabel 'P ({/Symbol y})'
set key top left font "Helvetica, 14"

set arrow from -45,0.007 to -15,0.006
set arrow from  65,0.0035 to 35,0.0022
set arrow from 110,0.015 to 140,0.016

pl \
'spme.1mus/dist.psi.out'  u 1:2 w l ls 112 t 'SPME',\
'rf.rc1.2/dist.psi.out' u 1:2 w l ls 162 t 'RF',\
'zm.l1.alpha0.00.rc1.2/dist.psi.out' u 1:2 w l ls 122 t 'ZM l = 1',\
'zm.l2.alpha0.00.rc1.2/dist.psi.out' u 1:2 w l ls 132 t 'ZM l = 2',\
'zm.l3.alpha0.00.rc1.2/dist.psi.out' u 1:2 w l ls 142 t 'ZM l = 3',\
'zm.l4.alpha0.00.rc1.2/dist.psi.out' u 1:2 w l ls 152 t 'ZM l = 4',\
'spme.1mus/dist.psi.out' u 1:2:($2-2*$3):($2+2*$3) every 1 w e ls 112 not,\
'' u 1:2 w l ls 112 not 'SPME'
# 'zm.l4.alpha0.00.rc1.2/dist.psi.out' u 1:2:($2-2*$3):($2+2*$3) w e ls 152 not 'ZM l = 4',\

unset key
unset xlabel
unset ylabel
set mxtics 2
set mytics 5
set xtics 10 font "Helvetica, 8"
set ytics 0.001 font "Helvetica, 8"
# set format y "" 
# set format x ""
unset grid
unset arrow

set origin 0.55, 0.6
set size 0.3, 0.35
set xrange [145:165]
set yrange [0.0149:0.0165]
rep

set origin 0.1, 0.24
set size 0.40, 0.35
set xrange [-25:-01]
set yrange [0.0049:0.0061]
rep

set origin 0.51, 0.23
set size 0.35, 0.40
set xrange [15:45]
set yrange [0.0009:0.0031]
rep



