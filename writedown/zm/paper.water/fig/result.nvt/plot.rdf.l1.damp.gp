set term post eps enh color solid font 14 size 8.5cm,6cm

set out 'fig-rdf-l1-damp.eps'

set multiplot

set style fill  transparent solid 0.50 noborder

set style line 111 lt 1 lw 2 pt 7 linecolor rgb "#ffbbbb"
set style line 112 lt 1 lw 2 pt 0 linecolor 1
set style line 121 lt 1 lw 2 pt 7 linecolor rgb "#99ff99"
set style line 122 lt 1 lw 2 pt 0 linecolor 2
set style line 131 lt 1 lw 2 pt 7 linecolor rgb "#bbbbff"
set style line 132 lt 1 lw 2 pt 0 linecolor 3
set style line 141 lt 1 lw 2 pt 7 linecolor rgb "#ffccff"
set style line 142 lt 1 lw 2 pt 0 linecolor 4
#set style line 151 lt 1 lw 3 pt 7 linecolor rgb "#bbffff"
set style line 151 lt 1 lw 2 pt 7 linecolor rgb "#CEF6F5"
set style line 152 lt 1 lw 2 pt 0 linecolor 5
set style line 162 lt 1 lw 2 pt 1 lc 9

set origin 0,0
set size 1,1

set xrange [0:2.1]
set yrange [0:3]
set mxtics 5
set mytics 5
set format y "%.1f"
set ylabel "g(r)"
set key bottom right font ",12" at 2.1,0.1
set key spacing 0.8
# set y2range [1e-5:10]
# set logscale y2
# set ytics nomirror
# set y2tics
# set my2tics 10
# set format y2 '10^{%L}'

set xlabel 'r [nm]'

set arrow from 0.9, 1.0 to 0.68,1.45
set arrow from 1.5, 1.0 to 1.72,1.45
set arrow from 1.2, 0.7 to 1.2, 1
set label at 1.1, 0.6 "cut-off"

pl 1 lc 0 lw 2 not,\
   'spme.tune.nvt.smooth.20ns/nvt/rdf.xvg' u 1:2 every 1 w l ls 112 t 'SPME',\
   'zm.l1.alpha0.00.rc1.2.long/nvt/rdf.xvg' u 1:2 w l ls 162 t '{/Symbol a} = 0.0 nm^{-1}',\
   'zm.l1.alpha0.50.rc1.2.long/nvt/rdf.0.002.xvg' u 1:2 w l ls 122 t '{/Symbol a} = 0.5 nm^{-1}',\
   'zm.l1.alpha1.00.rc1.2.long/nvt/rdf.0.002.xvg' u 1:2 w l ls 132 t '{/Symbol a} = 1.0 nm^{-1}',\
   'zm.l1.alpha1.50.rc1.2.long/nvt/rdf.0.002.xvg' u 1:2 w l ls 142 t '{/Symbol a} = 1.5 nm^{-1}',\
   'zm.l1.alpha2.00.rc1.2.long/nvt/rdf.0.002.xvg' u 1:2 w l ls 152 t '{/Symbol a} = 2.0 nm^{-1}',\
   'spme.tune.nvt.smooth.20ns/nvt/rdf.xvg' u 1:2 every 1 w l ls 112 not 'SPME',\
   'plot.rdf.box' u 1:2 index 1 w l lc 0 lw 3 not,\
   'plot.rdf.box' u 1:2 index 2 w l lc 0 lw 3 not
   # 'rdf.all.0.01.out' u 1:(abs($2 - $3)) every 5 w l  ls 162 axis x1y2 not,\
   # '' u 1:(abs($4 - $3)) every 5 w l  ls 122 axis x1y2 not,\
   # '' u 1:(abs($5 - $3)) every 5 w l  ls 132 axis x1y2 not,\
   # '' u 1:(abs($6 - $3)) every 5 w l  ls 142 axis x1y2 not,\
   # '' u 1:(abs($7 - $3)) every 5 w l  ls 152 axis x1y2 not

   
set origin .256, .45
set size .60,.5

unset label
unset arrow
unset y2tics
unset my2tics
unset ylabel
set ytics mirror
set xrange [0.9:1.5]
set yrange [0.996:1.004]
set xtics .1
set ytics .002
set mytics 2
set mxtics 1
set xlabel ''
set xtics font "Helvetica,10"
set ytics font "Helvetica,10"
set format y '%.3f'

pl 1 lc 0 lw 2 not,\
   'plot.rdf.box' u 1:2 index 0 w l lc 0 lw 3 not,\
   'spme.tune.nvt.smooth.20ns/nvt/rdf.0.01.xvg' u 1:2 every 1 w l ls 112 not 'SPME',\
   'zm.l1.alpha0.00.rc1.2.long/nvt/rdf.0.01.xvg' u 1:2 w l ls 162 not '{/Symbol a} = 0.0 nm^{-1}',\
   'zm.l1.alpha0.50.rc1.2.long/nvt/rdf.0.01.xvg' u 1:2 w l ls 122 not '{/Symbol a} = 0.0 nm^{-1}',\
   'zm.l1.alpha1.00.rc1.2.long/nvt/rdf.0.01.xvg' u 1:2 w l ls 132 not '{/Symbol a} = 0.0 nm^{-1}',\
   'zm.l1.alpha1.50.rc1.2.long/nvt/rdf.0.01.xvg' u 1:2 w l ls 142 not '{/Symbol a} = 0.0 nm^{-1}',\
   'zm.l1.alpha2.00.rc1.2.long/nvt/rdf.0.01.xvg' u 1:2 w l ls 152 not '{/Symbol a} = 0.0 nm^{-1}',\
   'spme.tune.nvt.smooth.20ns/nvt/rdf.0.01.xvg' u 1:2 every 1 w l ls 112 not 'SPME',\
   'plot.rdf.box' u 1:2 index 2 w l lc 0 lw 3 not
