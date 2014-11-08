pme=-81288
f(x)=abs((x-pme)/pme)

set term post eps enh color solid font 14 size 8.5cm,6cm
set out 'fig-energy-varA.eps'

set style line 111 lt 1 lw 3 pt 7 linecolor rgb "#ffbbbb"
set style line 112 lt 1 lw 3 pt 7 linecolor 1
set style line 121 lt 1 lw 3 pt 7 linecolor rgb "#99ff99"
set style line 122 lt 1 lw 3 pt 7 linecolor 2
set style line 131 lt 1 lw 3 pt 7 linecolor rgb "#bbbbff"
set style line 132 lt 1 lw 3 pt 0 linecolor 3
set style line 141 lt 1 lw 3 pt 7 linecolor rgb "#ffccff"
set style line 142 lt 1 lw 3 pt 0 linecolor 4
#set style line 151 lt 1 lw 3 pt 7 linecolor rgb "#bbffff"
set style line 151 lt 1 lw 3 pt 7 linecolor rgb "#CEF6F5"
set style line 152 lt 1 lw 3 pt 0 linecolor 5
set style line 162 lt 1 lw 3 pt 1 lc 9

set xlabel '{/Symbol a} [ nm^{-1} ]'
set ylabel 'Relative energy error'
set logscale y
set format y '10^{%L}'
set mxtics 5
set mytics 10
set xrange [0:2.1]

pl \
'energy.l1.varAlpha.out' u 1:(f($3)) w lp ls 112 t 'ZM with corr',\
'energy.l1.varAlpha.nocorr.out' u 1:(f($3)) w lp ls 122 t 'ZM without corr'

set out 'fig-energy-varL.eps'
set xrange [1:4]
set xtics 1
set mxtics 1

pl \
'energy.varL.alpha0.00.out' u 1:(f($3)) w lp ls 112 t 'ZM with corr',\
'energy.varL.alpha0.00.nocorr.out' u 1:(f($3)) w lp ls 122 t 'ZM without corr'
