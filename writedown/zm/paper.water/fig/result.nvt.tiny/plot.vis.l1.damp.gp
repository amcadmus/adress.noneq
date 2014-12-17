set term post eps enh color solid font 14 size 8.5cm,6cm
set out 'fig-vis-l1-damp.eps'

set multiplot

set style fill  transparent solid 0.50 noborder

set style line 111 lt 1 lw 3 pt 7 linecolor rgb "#ffbbbb"
set style line 112 lt 1 lw 3 pt 0 linecolor 1
set style line 121 lt 1 lw 3 pt 7 linecolor rgb "#99ff99"
set style line 122 lt 1 lw 3 pt 0 linecolor 2
set style line 131 lt 1 lw 3 pt 7 linecolor rgb "#bbbbff"
set style line 132 lt 1 lw 3 pt 0 linecolor 3
set style line 141 lt 1 lw 3 pt 7 linecolor rgb "#ffccff"
set style line 142 lt 1 lw 3 pt 0 linecolor 4
#set style line 151 lt 1 lw 3 pt 7 linecolor rgb "#bbffff"
set style line 151 lt 1 lw 3 pt 7 linecolor rgb "#CEF6F5"
set style line 152 lt 1 lw 3 pt 0 linecolor 5
set style line 162 lt 1 lw 3 pt 1 lc 9

set origin 0,0
set size 1,1

set xrange [0:8.0]
set yrange [0:.5]
set mytics 5
set mxtics 10
set format y '%.2f'
set grid
set xlabel 't [ ps ]'
set ylabel 'I_{/Symbol h}(t) [ 10^{-3} Pa s ]'
set key bottom right font "Helvetica, 14"


pl \
'spme.40ns/nvt/viscosity.out' u 1:2 w l ls 112 t 'SPME',\
'zm.l1.alpha0.00.rc1.2/nvt/viscosity.out' u 1:2 w l ls 162 t '{/Symbol a} = 0.0 nm^{-1}',\
'zm.l1.alpha0.50.rc1.2/nvt/viscosity.out' u 1:2 w l ls 122 t '{/Symbol a} = 0.5 nm^{-1}',\
'zm.l1.alpha1.00.rc1.2/nvt/viscosity.out' u 1:2 w l ls 132 t '{/Symbol a} = 1.0 nm^{-1}',\
'zm.l1.alpha1.50.rc1.2/nvt/viscosity.out' u 1:2 w l ls 142 t '{/Symbol a} = 1.5 nm^{-1}',\
'zm.l1.alpha2.00.rc1.2/nvt/viscosity.out' u 1:2 w l ls 152 t '{/Symbol a} = 2.0 nm^{-1}',\
'spme.40ns/nvt/viscosity.out' u 1:2:($2-2*$3):($2+2*$3) every 150 w e ls 112 not,\
'' u 1:2 w l ls 112 not 'SPME'



