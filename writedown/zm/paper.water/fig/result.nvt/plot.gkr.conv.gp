set term post eps enh color solid font 14 size 8.5cm,6cm
set out 'fig-gkr-conv.eps'

set multiplot

set style fill  transparent solid 0.50 noborder

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

set xrange [0:3.7]
set yrange [0:5]
set mytics 10
set mxtics 5
set grid
set xlabel 'r [ nm ]'
set ylabel 'G_k(r)'
set key bottom right font "Helvetica, 14"

# set arrow from 1.55, 3.4   to 1.55, 3.1
set arrow from 1.55, 3.4   to 1.55, 2.0
# set label "(a)" at -0.45,5.0 font ",18"

pl \
'spme.tune.nvt.smooth.20ns/nvt/gkr.out' u 1:2 w l ls 112 t 'SPME',\
'zm.l3.alpha0.00.rc1.2.long/nvt/gkr.out' u 1:2 w l ls 122 t 'r_c=1.2 nm',\
'zm.l3.alpha0.00.rc1.5.long/nvt/gkr.out' u 1:2 w l ls 132 t 'r_c=1.5 nm',\
'zm.l3.alpha0.00.rc1.8.long/nvt/gkr.out' u 1:2 w l ls 142 t 'r_c=1.8 nm',\
'zm.l3.alpha0.00.rc2.1.long/nvt/gkr.out' u 1:2 w l ls 152 t 'r_c=2.1 nm',\
'spme.tune.nvt.smooth.20ns/nvt/gkr.out' u 1:2:($2-2*$3):($2+2*$3) every 15 w e ls 112 not,\
'' u 1:2 w l ls 112 not 'SPME',\
'zoom.box' u 1:2 w l lc 0 lw 3 not

# set origin 0.268, 0.28
# set size 0.392,0.4
set origin 0.198, 0.1
set size 0.461,0.4
unset key
unset xlabel
unset ylabel
set mxtics 1
set mytics 1
set xtics 0.1 font "Helvetica, 10"
set mxtics 1
set ytics 0.1 font "Helvetica, 10"
# set format y "" 
set format x ""
unset grid
unset arrow

set xrange [0.9:2.2]
set yrange [3.45:3.9]

rep


# pl \
# 'spme.tune.20ns/nvt/gkr.out' u 1:($2-2*$3):($2+2*$3) w filledcu ls 111 not,\
# '' u 1:2 w l ls 112 not,\
# 'zm.l1.alpha0.00.rc1.2.long/nvt/gkr.out' u 1:($2-2*$3):($2+2*$3) w filledcu ls 121 not,\
# '' u 1:2 w l ls 122 not

