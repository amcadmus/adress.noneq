reset
set view map
unset surface
set style data pm3d
set style function pm3d
set ticslevel 0
set xlabel "{/Symbol f} [deg.]" 
set ylabel "{/Symbol y} [deg.]"
set xtics 60
set ytics 60
set mxtics 6
set mytics 6

set pm3d implicit at b
# set palette gray negative
set palette rgbformulae 22,13,-31
# set palette rgbformulae 33,13,10
set size ratio 1.0

#set cbrange [0:2e-4]
# set format cb "%.0f"
set xrange [-180:180]
set yrange [-180:180]

set term post eps enh color solid font 18 size 8cm,8cm 
#set term post eps enh color solid font 32 size 16cm,16cm 
set cbrange [-0.5:4.5]
# set format cb "%d"

set cbrange [0:3]
set out 'fig-fht-1.eps'
set title 'FHT 1'
splot 'fht.1.out' u 1:2:3 not w p pt 5 palette
set out 'fig-fht-2.eps'
set title 'FHT 2'
splot 'fht.2.out' u 1:2:5 not w p pt 5 palette
set out 'fig-fht-3.eps'
set title 'FHT 3'
splot 'fht.3.out' u 1:2:7 not w p pt 5 palette

set out 'fig-fht-msm-1.eps'
set title 'FHT MSM 1'
splot 'fht.msm.1.out' u 1:2:3 not w p pt 5 palette
set out 'fig-fht-msm-2.eps'
set title 'FHT MSM 2'
splot 'fht.msm.2.out' u 1:2:5 not w p pt 5 palette
set out 'fig-fht-msm-3.eps'
set title 'FHT MSM 3'
splot 'fht.msm.3.out' u 1:2:7 not w p pt 5 palette

