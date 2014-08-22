reset
set view map
# set samples 50, 50
# set isosamples 50, 50
unset surface
set style data pm3d
set style function pm3d
set ticslevel 0
# set xlabel "{/Symbol f} [deg.]" 
# set ylabel "{/Symbol y} [deg.]"
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

set cbrange [0:1]
set out 'fig-commitor-fw-1.eps'
set title 'Commitor fw 1'
splot 'commitor.fw.out' u 1:2:3 not w p pt 5 palette
set out 'fig-commitor-fw-2.eps'
set title 'Commitor fw 2'
splot 'commitor.fw.out' u 1:2:5 not w p pt 5 palette
set out 'fig-commitor-fw-3.eps'
set title 'Commitor fw 3'
splot 'commitor.fw.out' u 1:2:7 not w p pt 5 palette

set cbrange [0:1]
set out 'fig-commitor-fw-msm-1.eps'
set title 'Commitor fw 1 by MSM'
splot 'commitor.fw.msm.out' u 1:2:3 not w p pt 5 palette
set out 'fig-commitor-fw-msm-2.eps'
set title 'Commitor fw 2 by MSM'
splot 'commitor.fw.msm.out' u 1:2:5 not w p pt 5 palette
set out 'fig-commitor-fw-msm-3.eps'
set title 'Commitor fw 3 by MSM'
splot 'commitor.fw.msm.out' u 1:2:7 not w p pt 5 palette

set cbrange [0:1]
set out 'fig-commitor-bw-1.eps'
set title 'Commitor bw 1'
splot 'commitor.bw.out' u 1:2:3 not w p pt 5 palette
set out 'fig-commitor-bw-2.eps'
set title 'Commitor bw 2'
splot 'commitor.bw.out' u 1:2:5 not w p pt 5 palette
set out 'fig-commitor-bw-3.eps'
set title 'Commitor bw 3'
splot 'commitor.bw.out' u 1:2:7 not w p pt 5 palette

set cbrange [0:1]
set out 'fig-commitor-bw-msm-1.eps'
set title 'Commitor bw 1 by MSM'
splot 'commitor.bw.msm.out' u 1:2:3 not w p pt 5 palette
set out 'fig-commitor-bw-msm-2.eps'
set title 'Commitor bw 2 by MSM'
splot 'commitor.bw.msm.out' u 1:2:5 not w p pt 5 palette
set out 'fig-commitor-bw-msm-3.eps'
set title 'Commitor bw 3 by MSM'
splot 'commitor.bw.msm.out' u 1:2:7 not w p pt 5 palette


set cbrange [-2e-1:2e-1]
set out 'fig-commitor-diff-msm-1.eps'
set title 'Commitor diff 1 by MSM'
splot 'commitor.msm.out' u 1:2:($3-$11) not w p pt 5 palette

set cbrange [-2e-1:2e-1]
set out 'fig-commitor-diff-1.eps'
set title 'Commitor diff 1 by BF'
splot 'commitor.out' u 1:2:($3-$11) not w p pt 5 palette

set cbrange [-2e-1:2e-1]
set out 'fig-commitor-diff-msm-2.eps'
set title 'Commitor diff 2 by MSM'
splot 'commitor.msm.out' u 1:2:($5-$13) not w p pt 5 palette

set cbrange [-2e-1:2e-1]
set out 'fig-commitor-diff-2.eps'
set title 'Commitor diff 2 by BF'
splot 'commitor.out' u 1:2:($5-$13) not w p pt 5 palette


set cbrange [-1e-1:1e-1]
set out 'fig-commitor-diff-msm-3.eps'
set title 'Commitor diff 3 by MSM'
splot 'commitor.msm.out' u 1:2:($7-$15) not w p pt 5 palette

set cbrange [-1e-1:1e-1]
set out 'fig-commitor-diff-3.eps'
set title 'Commitor diff 3 by BF'
splot 'commitor.out' u 1:2:($7-$15) not w p pt 5 palette
