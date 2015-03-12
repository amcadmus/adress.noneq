reset
set view map
set xtics 60
set ytics 60
set mxtics 6
set mytics 6
set tics out

set pm3d implicit at b
set palette rgbformulae 22,13,-31

set size ratio 1.0
set xrange [-180:180]
set yrange [-180:180]
set term post eps enh color solid font 18 size 8cm,8cm 

set cbrange [0:250]
set table 'img.dist.2d.dat'
splot 'fht.1.out' u 1:2:($3*10) not
unset table
set out 'fig-fht-1.eps'
set title 'FMHT of {{/Symbol a}_R} by NEMD' font "Helvetica,28"
set label "[ps]" font "Helvetica,24" at 250,225
plot 'img.dist.2d.dat' with image

set table 'img.dist.2d.dat'
splot 'fht.msm.1.out' u 1:2:($3*10) not
unset table
set out 'fig-fht-msm-1.eps'
set title 'FMHT of {{/Symbol a}_R} by discr. dyn.' font "Helvetica,28"
plot 'img.dist.2d.dat' with image

set cbrange [0:250]
set table 'img.dist.2d.dat'
splot 'fht.2.out' u 1:2:($5*10) not
unset table
set out 'fig-fht-2.eps'
set title 'FMHT of {{/Symbol b}} by NEMD' font "Helvetica,28"
plot 'img.dist.2d.dat' with image

set table 'img.dist.2d.dat'
splot 'fht.msm.2.out' u 1:2:($5*10) not
unset table
set out 'fig-fht-msm-2.eps'
set title 'FMHT of {{/Symbol b}} by discr. dyn.' font "Helvetica,28"
plot 'img.dist.2d.dat' with image


set cbrange [0:700]
set table 'img.dist.2d.dat'
splot 'fht.3.out' u 1:2:($7*10) not
unset table
set out 'fig-fht-3.eps'
set title 'FMHT of {{/Symbol a}_L} by NEMD' font "Helvetica,28"
plot 'img.dist.2d.dat' with image

set table 'img.dist.2d.dat'
splot 'fht.msm.3.out' u 1:2:($7*10) not
unset table
set out 'fig-fht-msm-3.eps'
set title 'FMHT of {{/Symbol a}_L} by discr. dyn.' font "Helvetica,28"
plot 'img.dist.2d.dat' with image

