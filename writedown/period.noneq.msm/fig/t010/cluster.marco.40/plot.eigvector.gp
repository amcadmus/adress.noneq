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
# set cbrange [0:1]
set term post eps enh color solid font 18 size 8cm,8cm 

set table 'img.dist.2d.dat'
splot 'floquet.eigvector.l.2.out' u 1:2:3 not
unset table
set out 'fig-eig-vec-2.eps'
set title 'Eigenfunction 2'
plot 'img.dist.2d.dat' with image

set table 'img.dist.2d.dat'
splot 'floquet.eigvector.l.3.out' u 1:2:3 not
unset table
set out 'fig-eig-vec-3.eps'
set title 'Eigenfunction 3'
plot 'img.dist.2d.dat' with image

set table 'img.dist.2d.dat'
splot 'floquet.eigvector.l.4.out' u 1:2:3 not
unset table
set out 'fig-eig-vec-4.eps'
set title 'Eigenfunction 4'
plot 'img.dist.2d.dat' with image

