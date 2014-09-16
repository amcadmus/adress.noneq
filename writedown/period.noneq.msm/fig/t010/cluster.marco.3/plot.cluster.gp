# reset
# set view map
# set samples 100, 100
# set isosamples 100, 100
# unset surface
# set style data pm3d
# set style function pm3d
# set ticslevel 0
# set xlabel "{/Symbol f} [deg.]" 
# set ylabel "{/Symbol y} [deg.]"
# set xtics 60
# set ytics 60
# set mxtics 6
# set mytics 6

# set pm3d implicit at b
# # set palette gray negative
# set palette maxcolors 5
# set palette rgbformulae 22,13,-31
# # set palette rgbformulae 33,13,10
# set size ratio 1.0

# #set cbrange [0:2e-4]
# # set format cb "%.0f"
# set xrange [-180:180]
# set yrange [-180:180]

# set term post eps enh color solid font 18 size 8cm,8cm 
# #set term post eps enh color solid font 32 size 16cm,16cm 
# set title 'Clustering'
# set cbrange [0:4]

# set out 'fig-cluster-p.eps'
# splot 'cluster.out' u 1:2:3   w p not
# set out 'fig-cluster-all.eps'
# splot 'cluster.out' u 1:2:4  not


reset

set term post eps enh color solid font 18 size 8cm,8cm 

set view map
set xlabel "{/Symbol f} [deg.]" 
set ylabel "{/Symbol y} [deg.]"
set xtics 60
set ytics 60
set mxtics 6
set mytics 6

# set palette gray negative
set palette rgbformulae 22,13,-31
# set palette rgbformulae 33,13,10
set size ratio 1.0

#set cbrange [0:2e-4]
# set format cb "%.0f"
set xrange [-180:180]
set yrange [-180:180]

#set term post eps enh color solid font 32 size 16cm,16cm 
# set cbrange [-0.5:4.5]
# set format cb "%d"

# set cbrange [-0.5:3.5]
set palette maxcolors 4
unset colorbox
set tics out
set out 'fig-cluster.eps'
set title 'Core sets'
splot 'cluster.out' u 1:2:3 not w p pt 5 ps 2.0 palette
