set term post eps enh color solid font 14 size 8.5cm,6cm

set style line 111 lt 1 lw 3 pt 7 linecolor rgb "#ffbbbb"
set style line 112 lt 1 lw 3 pt 0 linecolor rgb "#ff0000"
set style line 113 lt 1 lw 3 pt 0 linecolor rgb "#ff0000"
set style line 121 lt 1 lw 3 pt 7 linecolor rgb "#99ff99"
set style line 122 lt 1 lw 3 pt 0 linecolor 2
set style line 123 lt 2 lw 3 pt 0 linecolor 2
set style line 131 lt 1 lw 3 pt 7 linecolor rgb "#bbbbff"
set style line 132 lt 1 lw 3 pt 0 linecolor 3
set style line 133 lt 2 lw 3 pt 0 linecolor 3
set style line 141 lt 1 lw 3 pt 7 linecolor rgb "#ffccff"
set style line 142 lt 1 lw 3 pt 0 linecolor 4
set style line 143 lt 2 lw 3 pt 0 linecolor 4
#set style line 151 lt 1 lw 3 pt 7 linecolor rgb "#bbffff"
set style line 151 lt 1 lw 3 pt 7 linecolor rgb "#CEF6F5"
set style line 152 lt 1 lw 3 pt 0 linecolor 5
set style line 153 lt 2 lw 3 pt 0 linecolor 5

set key bottom right
set xlabel 't [ ps ]'
set ylabel '{/Symbol e}'
set grid
set yrange [0:160]

# pl \
# 'spme.tune.20ns/nvt/eps.3000.xvg'   u 1:2 w l ls 112 t 'SPME',\
# 'spme.tune.20ns/nvt/eps.10000.xvg'  u 1:2 w l ls 112 not,\
# 'zm.l1.alpha0.00.rc1.2.long/nvt/eps.3000.xvg'  u 1:2 w l ls 122 t 'ZM l=1',\
# 'zm.l1.alpha0.00.rc1.2.long/nvt/eps.10000.xvg' u 1:2 w l ls 122 not,\
# 'zm.l2.alpha0.00.rc1.2.long/nvt/eps.3000.xvg'  u 1:2 w l ls 132 t 'ZM l=2',\
# 'zm.l2.alpha0.00.rc1.2.long/nvt/eps.10000.xvg' u 1:2 w l ls 132 not,\
# 'zm.l3.alpha0.00.rc1.2.long/nvt/eps.3000.xvg'  u 1:2 w l ls 142 t 'ZM l=3',\
# 'zm.l3.alpha0.00.rc1.2.long/nvt/eps.10000.xvg' u 1:2 w l ls 142 not,\
# 'zm.l4.alpha0.00.rc1.2.long/nvt/eps.3000.xvg'  u 1:2 w l ls 152 t 'ZM l=4',\
# 'zm.l4.alpha0.00.rc1.2.long/nvt/eps.10000.xvg' u 1:2 w l ls 152 not
set out 'fig-eps-t-3000.eps'
pl \
'spme.tune.20ns/nvt/eps.3000.xvg'   u 1:2 w l ls 112 t 'SPME',\
'zm.l1.alpha0.00.rc1.2.long/nvt/eps.3000.xvg'  u 1:2 w l ls 122 t 'ZM l=1',\
'zm.l2.alpha0.00.rc1.2.long/nvt/eps.3000.xvg'  u 1:2 w l ls 132 t 'ZM l=2',\
'zm.l3.alpha0.00.rc1.2.long/nvt/eps.3000.xvg'  u 1:2 w l ls 142 t 'ZM l=3',\
'zm.l4.alpha0.00.rc1.2.long/nvt/eps.3000.xvg'  u 1:2 w l ls 152 t 'ZM l=4'

set out 'fig-eps-t-zm3-3000.eps'
pl \
'spme.tune.20ns/nvt/eps.3000.xvg'   u 1:2 w l ls 112 t 'SPME',\
'zm.l3.alpha0.00.rc1.2.long/nvt/eps.3000.xvg'  u 1:2 w l ls 122 t 'ZM l=3 r_c=1.2 nm',\
'zm.l3.alpha0.00.rc1.5.long/nvt/eps.3000.xvg'  u 1:2 w l ls 132 t 'ZM l=3 r_c=1.5 nm',\
'zm.l3.alpha0.00.rc1.8.long/nvt/eps.3000.xvg'  u 1:2 w l ls 142 t 'ZM l=3 r_c=1.8 nm'

set out 'fig-eps-t-10000.eps'
pl \
'spme.tune.20ns/nvt/eps.10000.xvg'   u 1:2 w l ls 112 t 'SPME',\
'zm.l1.alpha0.00.rc1.2.long/nvt/eps.10000.xvg'  u 1:2 w l ls 122 t 'ZM l=1',\
'zm.l2.alpha0.00.rc1.2.long/nvt/eps.10000.xvg'  u 1:2 w l ls 132 t 'ZM l=2',\
'zm.l3.alpha0.00.rc1.2.long/nvt/eps.10000.xvg'  u 1:2 w l ls 142 t 'ZM l=3',\
'zm.l4.alpha0.00.rc1.2.long/nvt/eps.10000.xvg'  u 1:2 w l ls 152 t 'ZM l=4'

set out 'fig-eps-t-zm3-10000.eps'
pl \
'spme.tune.20ns/nvt/eps.10000.xvg'   u 1:2 w l ls 112 t 'SPME',\
'zm.l3.alpha0.00.rc1.2.long/nvt/eps.10000.xvg'  u 1:2 w l ls 122 t 'ZM l=3 r_c=1.2 nm',\
'zm.l3.alpha0.00.rc1.5.long/nvt/eps.10000.xvg'  u 1:2 w l ls 132 t 'ZM l=3 r_c=1.5 nm',\
'zm.l3.alpha0.00.rc1.8.long/nvt/eps.10000.xvg'  u 1:2 w l ls 142 t 'ZM l=3 r_c=1.8 nm'
