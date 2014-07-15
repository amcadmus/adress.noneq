#set border 4095 front linetype -1 linewidth 1.000
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
# set palette rgbformulae 22,13,-31
set palette rgbformulae 33,13,10
set size ratio 1.0

#set cbrange [0:2e-4]
set format cb "%.1f"
set xrange [-180:180]
set yrange [-180:180]

set term post eps enh color solid font 18 size 8cm,8cm 
#set term post eps enh color solid font 32 size 16cm,16cm 
#set title 't=0.0 ps'
set cbrange [-3.0e-1 : 3.0e-1]

set title 'SPME 2nd'
set out 'fig-spme-eig-vec-2.eps'
spl 'spme.1mus/eigen.vector.l.02.dist'not
set title 'SPME 3rd'
set out 'fig-spme-eig-vec-3.eps'
spl 'spme.1mus/eigen.vector.l.03.dist' u 1:2:($3) not
set title 'SPME 4th'
set out 'fig-spme-eig-vec-4.eps'
spl 'spme.1mus/eigen.vector.l.04.dist' u 1:2:($3) not
set title 'SPME 5th'
set out 'fig-spme-eig-vec-5.eps'
spl 'spme.1mus/eigen.vector.l.05.dist' not

set title 'RF 2nd'
set out 'fig-rf-eig-vec-2.eps'
spl 'rf.rc1.2/eigen.vector.l.02.dist' u 1:2:(-$3) not
set title 'RF 3rd'
set out 'fig-rf-eig-vec-3.eps'
spl 'rf.rc1.2/eigen.vector.l.03.dist' u 1:2:(-$3) not
set title 'RF 4th'
set out 'fig-rf-eig-vec-4.eps'
spl 'rf.rc1.2/eigen.vector.l.04.dist' not
set title 'RF 5th'
set out 'fig-rf-eig-vec-5.eps'
spl 'rf.rc1.2/eigen.vector.l.05.dist' not

set title 'ZM l=2 2nd'
set out 'fig-zm2-eig-vec-2.eps'
spl 'zm.l2.alpha0.00.rc1.2/eigen.vector.l.02.dist' not
set title 'ZM l=2 3rd'
set out 'fig-zm2-eig-vec-3.eps'
spl 'zm.l2.alpha0.00.rc1.2/eigen.vector.l.03.dist' not
set title 'ZM l=2 4th'
set out 'fig-zm2-eig-vec-4.eps'
spl 'zm.l2.alpha0.00.rc1.2/eigen.vector.l.04.dist' u 1:2:(-$3) not
set title 'ZM l=2 5th'
set out 'fig-zm2-eig-vec-5.eps'
spl 'zm.l2.alpha0.00.rc1.2/eigen.vector.l.05.dist' not

set title 'ZM l=3 2nd'
set out 'fig-zm3-eig-vec-2.eps'
spl 'zm.l3.alpha0.00.rc1.2/eigen.vector.l.02.dist' not
set title 'ZM l=3 3rd'
set out 'fig-zm3-eig-vec-3.eps'
spl 'zm.l3.alpha0.00.rc1.2/eigen.vector.l.03.dist' not
set title 'ZM l=3 4th'
set out 'fig-zm3-eig-vec-4.eps'
spl 'zm.l3.alpha0.00.rc1.2/eigen.vector.l.04.dist' u 1:2:(-$3) not
set title 'ZM l=3 5th'
set out 'fig-zm3-eig-vec-5.eps'
spl 'zm.l3.alpha0.00.rc1.2/eigen.vector.l.05.dist' not


set title 'ZM l=4 2nd'
set out 'fig-zm4-eig-vec-2.eps'
spl 'zm.l4.alpha0.00.rc1.2/eigen.vector.l.02.dist' not
set title 'ZM l=4 3rd'
set out 'fig-zm4-eig-vec-3.eps'
spl 'zm.l4.alpha0.00.rc1.2/eigen.vector.l.03.dist' not
set title 'ZM l=4 4th'
set out 'fig-zm4-eig-vec-4.eps'
spl 'zm.l4.alpha0.00.rc1.2/eigen.vector.l.04.dist' not
set title 'ZM l=4 5th'
set out 'fig-zm4-eig-vec-5.eps'
spl 'zm.l4.alpha0.00.rc1.2/eigen.vector.l.05.dist' not


set cbrange [0: 5e-3]
set format cb "%.e"
set title 'RF 2nd'
set out 'fig-rf-eig-vec-2-comp.eps'
spl 'rf.rc1.2/eigen.vector.l.02.dist.comp' u 1:2:(abs($3-$6)) not
set title 'RF 3rd'
set out 'fig-rf-eig-vec-3-comp.eps'
spl 'rf.rc1.2/eigen.vector.l.03.dist.comp' u 1:2:(abs($3+$6)) not
# set title 'RF 5th'
# set out 'fig-rf-eig-vec-5-comp.eps'
# spl 'rf.rc1.2/eigen.vector.l.05.dist.comp' u 1:2:(abs($3+$6)) not

set title 'ZM l=3 2nd'
set out 'fig-zm3-eig-vec-2-comp.eps'
spl 'zm.l3.alpha0.00.rc1.2/eigen.vector.l.02.dist.comp' u 1:2:(abs($3-$6)) not
set title 'ZM l=3 3rd'
set out 'fig-zm3-eig-vec-3-comp.eps'
spl 'zm.l3.alpha0.00.rc1.2/eigen.vector.l.03.dist.comp' u 1:2:(abs($3-$6)) not
# set title 'ZM l=3 5th'
# set out 'fig-zm3-eig-vec-5-comp.eps'
# spl 'zm.l3.alpha0.00.rc1.2/eigen.vector.l.05.dist.comp' u 1:2:(abs($3-$6)) not

set title 'ZM l=2 2nd'
set out 'fig-zm2-eig-vec-2-comp.eps'
spl 'zm.l2.alpha0.00.rc1.2/eigen.vector.l.02.dist.comp' u 1:2:(abs($3-$6)) not
set title 'ZM l=2 3rd'
set out 'fig-zm2-eig-vec-3-comp.eps'
spl 'zm.l2.alpha0.00.rc1.2/eigen.vector.l.03.dist.comp' u 1:2:(abs($3-$6)) not
# set title 'ZM l=2 5th'
# set out 'fig-zm2-eig-vec-5-comp.eps'
# spl 'zm.l2.alpha0.00.rc1.2/eigen.vector.l.05.dist.comp' u 1:2:(abs($3-$6)) not

set cbrange [0: 1e-2]
set format cb "%.e"
set title 'RF 4th'
set out 'fig-rf-eig-vec-4-comp.eps'
spl 'rf.rc1.2/eigen.vector.l.04.dist.comp' u 1:2:(abs($3-$6)) not
set title 'ZM l=3 4th'
set out 'fig-zm3-eig-vec-4-comp.eps'
spl 'zm.l3.alpha0.00.rc1.2/eigen.vector.l.04.dist.comp' u 1:2:(abs($3+$6)) not
set title 'ZM l=2 4th'
set out 'fig-zm2-eig-vec-4-comp.eps'
spl 'zm.l2.alpha0.00.rc1.2/eigen.vector.l.04.dist.comp' u 1:2:(abs($3+$6)) not


