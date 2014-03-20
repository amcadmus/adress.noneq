set term post eps enh color solid size 28cm,12cm
set out 'ele.eps'

unset border 
unset tics
set lmargin 10
set format x ""
set format y ""
set label "Electric Field" at -1, -0.9 rotate by 90 font "Helvetica,50" 
set label "t" at 10.3*pi,-.3 font "Helvetica,70" 

set ls 1 lw 10 linecolor rgb "#777777"
set ls 2 lw 20 linecolor rgb "#ffbbbb"
set style arrow 3 head filled size screen 0.03,15,45 ls 1

set arrow from 0,0 to (10.5*pi),0 ls 1 as 3
set arrow from 0,-1.8 to 0,2.0 ls 1 as 3

pl[0:10.5*pi][-2.0:2.0] sin(x) ls 2 not
