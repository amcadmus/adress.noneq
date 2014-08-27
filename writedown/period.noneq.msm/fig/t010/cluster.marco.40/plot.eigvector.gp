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

set cbrange [-0.2:0.2]
set out 'fig-floquet-vec-2-simple.eps'
set title 'Floquet eig. vec. 2'
splot 'floquet.eigvector.2.out' u 1:2:3 not w p pt 5 palette

set out 'fig-floquet-vec-3-simple.eps'
set title 'Floquet eig. vec. 3'
splot 'floquet.eigvector.3.out' u 1:2:3 not w p pt 5 palette

set out 'fig-floquet-vec-4-simple.eps'
set title 'Floquet eig. vec. 4'
splot 'floquet.eigvector.4.out' u 1:2:3 not w p pt 5 palette


reset
set xrange [-180:180]
set yrange [-180:180]
set isosample 72, 72
# set xlabel "{/Symbol f} [deg.]" 
# set ylabel "{/Symbol y} [deg.]"
# set xtics 60
# set ytics 60
# set mxtics 6
# set mytics 6

set table 'img.dist.2d.dat'
splot 'floquet.eigvector.2.out' not
unset table

set contour base 
set cntrparam level incremental -0.2,0.05,0.2
unset surface
set table 'cont.dist.2d.dat'
splot 'floquet.eigvector.2.out' not
unset table

reset
set xrange [-180:180]
set yrange [-180:180]
set xlabel "{/Symbol f} [deg.]" 
set ylabel "{/Symbol y} [deg.]"
set xtics 60
set ytics 60
set mxtics 6
set mytics 6

# set palette gray negative
set palette rgbformulae 22,13,-31
# set palette rgbformulae 33,13,10
# set palette maxcolors 12
set size ratio 1.0

set cbrange [-0.2:0.2]
# set format cb "%.1e"

set term post eps enh color solid font 18 size 9cm,8cm 

set out 'fig-floquet-vec-2.eps'
set title 'log dist MSM'
pl 'img.dist.2d.dat' with image, 'cont.dist.2d.dat' w l lc 0 not 

