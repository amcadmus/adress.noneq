#set border 4095 front linetype -1 linewidth 1.000
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
splot 'spme.1mus/dist.phi.psi.out' u 1:2:($3>0 ? (-2.5 * log($3) - 20) : 50)  not
unset table

set contour base 
set cntrparam level incremental 0,5,25
unset surface
set table 'cont.dist.2d.dat'
splot 'spme.1mus/dist.phi.psi.out' u 1:2:($3>0 ? (-2.5 * log($3) - 20) : 50)  not
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

set cbrange [0:25]
# set format cb "%.1e"

set term post eps enh color solid font 18 size 9cm,8cm 

set out 'fig-dih-dist-2d.eps'
pl 'img.dist.2d.dat' with image, 'cont.dist.2d.dat' w l lc 0 not 



reset
set view map
set samples 50, 50
set isosamples 50, 50
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
set format cb "%.0f"
set xrange [-180:180]
set yrange [-180:180]

set term post eps enh color solid font 18 size 8cm,8cm 
#set term post eps enh color solid font 32 size 16cm,16cm 
set title 'Free energy landscape'
set cbrange [0:25]

set label "[kJ/mol]" at 185,215
set out 'fig-dih-dist-2d-simple.eps'
splot 'spme.1mus/dist.phi.psi.out' u 1:2:(-2.5 * log($3) - 20)  not
