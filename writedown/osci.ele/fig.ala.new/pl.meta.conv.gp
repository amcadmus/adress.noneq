set term post eps enh color font 16 size 8.5cm,6cm
set out 'fig-meta-conv.eps'

set style line 11 lc 1 lw 3 pt 2 lt 1
set style line 12 lc 2 lw 3 pt 2 lt 1
set style line 13 lc 3 lw 3 pt 2 lt 1
set style line 14 lc 4 lw 3 pt 2 lt 1
set style line 15 lc 5 lw 3 pt 2 lt 1

set style line 21 lc 1 lw 3 pt 2 lt 2
set style line 22 lc 2 lw 3 pt 2 lt 2
set style line 23 lc 3 lw 3 pt 2 lt 2
set style line 24 lc 4 lw 3 pt 2 lt 2
set style line 25 lc 5 lw 3 pt 2 lt 2

set style line 31 lc 1 lw 3 pt 2 lt 3
set style line 32 lc 2 lw 3 pt 2 lt 3
set style line 33 lc 3 lw 3 pt 2 lt 3
set style line 34 lc 4 lw 3 pt 2 lt 3
set style line 35 lc 5 lw 3 pt 2 lt 3

set style line 41 lc 1 lw 3 pt 2 lt 4
set style line 42 lc 2 lw 3 pt 2 lt 4
set style line 43 lc 3 lw 3 pt 2 lt 4
set style line 44 lc 4 lw 3 pt 2 lt 4
set style line 45 lc 5 lw 3 pt 2 lt 4

set style line 51 lc 1 lw 3 pt 2 lt 5
set style line 52 lc 2 lw 3 pt 2 lt 5
set style line 53 lc 3 lw 3 pt 2 lt 5
set style line 54 lc 4 lw 3 pt 2 lt 5
set style line 55 lc 5 lw 3 pt 2 lt 5

set style line 20 lc 0 lw 3 pt 2 lt 2

set style line 111 lt 1 lw 3 pt 7 linecolor rgb "#ffbbbb"
set style line 112 lt 1 lw 3 pt 7 linecolor rgb "#ff0000"
set style line 121 lt 1 lw 3 pt 7 linecolor rgb "#99ff99"
set style line 122 lt 1 lw 3 pt 7 linecolor rgb "#33cc33"
set style line 131 lt 1 lw 3 pt 7 linecolor rgb "#bbbbff"
set style line 132 lt 1 lw 3 pt 7 linecolor rgb "#0000ff"
set style line 141 lt 1 lw 3 pt 7 linecolor rgb "#ffccff"
set style line 142 lt 1 lw 3 pt 7 linecolor rgb "#cc00ff"
#set style line 151 lt 1 lw 3 pt 7 linecolor rgb "#bbffff"
set style line 151 lt 1 lw 3 pt 7 linecolor rgb "#CEF6F5"
set style line 152 lt 1 lw 3 pt 7 linecolor rgb "#00ffff"

set multiplot
set size 0.4,1

set yrange [0:.75]
set mytics 2
set origin 0.0,0.0
set lmargin 7.8
set rmargin 0
set bmargin 3
set xtics 50
set mxtics 5

set ylabel 'P@_I'
set key at 1000, 0.5
# set label 't_{init}=10 ps' at 10, 0.7
set grid

set xrange [0:100]
pl \
   'ext.mode1.010.Ex.01.00.t1000ps/set/metastable.out' u 1:($2-$7*2):($2+$7*2) every 4 w filledcu ls 111 not, \
   '' u 1:($3-$8 *2):($3+$8 *2) every 4 w filledcu ls 121 not,\
   '' u 1:($4-$9 *2):($4+$9 *2) every 4 w filledcu ls 131 not,\
   '' u 1:($5-$10*2):($5+$10*2) every 4 w filledcu ls 141 not,\
   '' u 1:($6-$11*2):($6+$11*2) every 4 w filledcu ls 151 not,\
'ext.mode1.010.Ex.01.00.t1000ps/set/metastable.out'	u 1:2 every 4 w l ls 11 not, '' u 1:3 every 4 w l ls 12 not, '' u 1:4 every 4 w l ls 13  not, '' u 1:5 every 4 w l ls 14  not, '' u 1:6 every 4 w l ls 15 not,\
'ext.mode1.010.Ex.01.00.t1000ps.box4.0.r1.0/set/metastable.out'	u 1:2 every 4 w l ls 21 not, '' u 1:3 every 4 w l ls 22 not, '' u 1:4 every 4 w l ls 23  not, '' u 1:5 every 4 w l ls 24  not, '' u 1:6 every 4 w l ls 25 not,\
'ext.mode1.010.Ex.01.00.t1000ps.box4.0.r1.5/set/metastable.out'	u 1:2 every 4 w l ls 41 not, '' u 1:3 every 4 w l ls 42 not, '' u 1:4 every 4 w l ls 43  not, '' u 1:5 every 4 w l ls 44  not, '' u 1:6 every 4 w l ls 45 not

# 'ext.mode1.010.Ex.01.00.t1000ps.fullSD/set/metastable.out'	u 1:2 every 4 w l ls 51 not, '' u 1:3 every 4 w l ls 52 not, '' u 1:4 every 4 w l ls 53  not, '' u 1:5 every 4 w l ls 54  not, '' u 1:6 every 4 w l ls 55 not,\

# 'ext.mode1.010.Ex.01.00.t0600ps/set/metastable.out'	u 1:2 every 4 w l ls 11 not, '' u 1:3 every 4 w l ls 12 not, '' u 1:4 every 4 w l ls 13  not, '' u 1:5 every 4 w l ls 14  not, '' u 1:6 every 4 w l ls 15 not,\

# 'lines' w l ls 20 not,\

# 'ext.mode1.000.Ex.01.00.t1000ps/set/metastable.out'		u 1:2 every 4 w l ls 41 not, '' u 1:3 every 4 w l ls 42 not, '' u 1:4 every 4 w l ls 43  not, '' u 1:5 every 4 w l ls 44  not, '' u 1:6 every 4 w l ls 45 not,\
# 'ext.mode1.050.Ex.01.00.t0600ps/set/metastable.out'		u 1:2 every 4 w l ls 21 not, '' u 1:3 every 4 w l ls 22 not, '' u 1:4 every 4 w l ls 23  not, '' u 1:5 every 4 w l ls 24  not, '' u 1:6 every 4 w l ls 25 not,\


set size 0.6,1
set origin 0.4,0.0
unset ylabel
set lmargin 0
set rmargin 2
set format y ""

set xrange [100:1000]
set xtics 200
set mxtics 2

set label 't [ ps ]' at 200, -0.11
set label '{/Symbol a}_R'	at 400,.59	font ",16" front
set label "{/Symbol a}'_R"	at 100,.08	font ",16" front
set label 'C7_{eq}'		at 180,.15	font ",16" front
set label 'C5'			at 180,.025	font ",16" front
set label '{/Symbol a}_L'	at 400,.22	font ",16" front


pl \
   'ext.mode1.010.Ex.01.00.t1000ps/set/metastable.out' u 1:($2-$7*2):($2+$7*2) every 48 w filledcu ls 111 not, \
   '' u 1:($3-$8 *2):($3+$8 *2) every 48 w filledcu ls 121 not,\
   '' u 1:($4-$9 *2):($4+$9 *2) every 48 w filledcu ls 131 not,\
   '' u 1:($5-$10*2):($5+$10*2) every 48 w filledcu ls 141 not,\
   '' u 1:($6-$11*2):($6+$11*2) every 48 w filledcu ls 151 not,\
'ext.mode1.010.Ex.01.00.t1000ps/set/metastable.out'	u 1:2 every 48 w l ls 11 t 'L = 2.7 nm, R_{ex} = 1.0 nm', '' u 1:3 every 48 w l ls 12 not, '' u 1:4 every 48 w l ls 13  not, '' u 1:5 every 48 w l ls 14  not, '' u 1:6 every 48 w l ls 15 not,\
'ext.mode1.010.Ex.01.00.t1000ps.box4.0.r1.0/set/metastable.out'	u 1:2 every 48 w l ls 21 t 'L = 4.0 nm, R_{ex} = 1.0 nm', '' u 1:3 every 48 w l ls 22 not, '' u 1:4 every 48 w l ls 23  not, '' u 1:5 every 48 w l ls 24  not, '' u 1:6 every 48 w l ls 25 not,\
'ext.mode1.010.Ex.01.00.t1000ps.box4.0.r1.5/set/metastable.out'	u 1:2 every 48 w l ls 41 t 'L = 4.0 nm, R_{ex} = 1.5 nm', '' u 1:3 every 48 w l ls 42 not, '' u 1:4 every 48 w l ls 43  not, '' u 1:5 every 48 w l ls 44  not, '' u 1:6 every 48 w l ls 45 not

# 'ext.mode1.010.Ex.01.00.t1000ps.fullSD/set/metastable.out'	u 1:2 every 48 w l ls 51 t 'L = 2.7 nm, R_{ex} = 1.0 nm', '' u 1:3 every 48 w l ls 52 not, '' u 1:4 every 48 w l ls 53  not, '' u 1:5 every 48 w l ls 54  not, '' u 1:6 every 48 w l ls 55 not,\

# 'ext.mode1.010.Ex.01.00.t0600ps/set/metastable.out'		u 1:2 every 48 w l ls 11 t 'L = 2.7 nm, R_{ex} = 1.0 nm', '' u 1:3 every 48 w l ls 12 not, '' u 1:4 every 48 w l ls 13  not, '' u 1:5 every 48 w l ls 14  not, '' u 1:6 every 48 w l ls 15 not,\

# 'lines.1' w l ls 20 not,\

# 'ext.mode1.000.Ex.01.00.t1000ps/set/metastable.out'		u 1:2 every 48 w l ls 41 t 't_{init}=10 ps', '' u 1:3 every 48 w l ls 42 not, '' u 1:4 every 48 w l ls 43  not, '' u 1:5 every 48 w l ls 44  not, '' u 1:6 every 48 w l ls 45 not,\
# 'ext.mode1.050.Ex.01.00.t0600ps/set/metastable.out'		u 1:2 every 48 w l ls 21 t 't_{init}=50 ps', '' u 1:3 every 48 w l ls 22 not, '' u 1:4 every 48 w l ls 23  not, '' u 1:5 every 48 w l ls 24  not, '' u 1:6 every 48 w l ls 25 not,\

