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
set cbrange [0:1]
set term post eps enh color solid font 18 size 8cm,8cm 
set format cb "%.1f"

set table 'img.dist.2d.dat'
splot 'commitor.fw.out' u 1:2:3 not
unset table
set out 'fig-commitor-fw-1.eps'
set title 'Commitor fw 1' font "Helvetica,30"
set title 'q@^{+}_{{/Symbol a}_R} by NEMD' font "Helvetica,30"
plot 'img.dist.2d.dat' with image

set table 'img.dist.2d.dat'
splot 'commitor.fw.out' u 1:2:5 not
unset table
set out 'fig-commitor-fw-2.eps'
set title 'Commitor fw 2' font "Helvetica,30"
set title 'q@_{{/Symbol b}}^{+} by NEMD' font "Helvetica,30"
plot 'img.dist.2d.dat' with image

set table 'img.dist.2d.dat'
splot 'commitor.fw.out' u 1:2:7 not
unset table
set out 'fig-commitor-fw-3.eps'
set title 'Commitor fw 3' font "Helvetica,30"
set title 'q@^{+}_{{/Symbol a}_L} by NEMD' font "Helvetica,30"
plot 'img.dist.2d.dat' with image

set table 'img.dist.2d.dat'
splot 'commitor.fw.msm.out' u 1:2:3 not
unset table
set out 'fig-commitor-fw-msm-1.eps'
set title 'Commitor fw 1 MSM' font "Helvetica,30"
set title 'q@^{+}_{{/Symbol a}_R} by discr. dyn.' font "Helvetica,30"
plot 'img.dist.2d.dat' with image

set table 'img.dist.2d.dat'
splot 'commitor.fw.msm.out' u 1:2:5 not
unset table
set out 'fig-commitor-fw-msm-2.eps'
set title 'Commitor fw 2 MSM' font "Helvetica,30"
set title 'q@_{{/Symbol b}}^{+} by discr. dyn.' font "Helvetica,30"
plot 'img.dist.2d.dat' with image

set table 'img.dist.2d.dat'
splot 'commitor.fw.msm.out' u 1:2:7 not
unset table
set out 'fig-commitor-fw-msm-3.eps'
set title 'Commitor fw 3 MSM' font "Helvetica,30"
set title 'q@^{+}_{{/Symbol a}_L} by discr. dyn.' font "Helvetica,30"
plot 'img.dist.2d.dat' with image



set table 'img.dist.2d.dat'
splot 'commitor.bw.out' u 1:2:3 not
unset table
set out 'fig-commitor-bw-1.eps'
set title 'Commitor bw 1' font "Helvetica,30"
set title 'q@^{-}_{{/Symbol a}_R} by NEMD' font "Helvetica,30"
plot 'img.dist.2d.dat' with image

set table 'img.dist.2d.dat'
splot 'commitor.bw.out' u 1:2:5 not
unset table
set out 'fig-commitor-bw-2.eps'
set title 'Commitor bw 2' font "Helvetica,30"
set title 'q@_{{/Symbol b}}^{-} by NEMD' font "Helvetica,30"
plot 'img.dist.2d.dat' with image

set table 'img.dist.2d.dat'
splot 'commitor.bw.out' u 1:2:7 not
unset table
set out 'fig-commitor-bw-3.eps'
set title 'Commitor bw 3' font "Helvetica,30"
set title 'q@^{-}_{{/Symbol a}_L} by NEMD' font "Helvetica,30"
plot 'img.dist.2d.dat' with image

set table 'img.dist.2d.dat'
splot 'commitor.bw.msm.out' u 1:2:3 not
unset table
set out 'fig-commitor-bw-msm-1.eps'
set title 'Commitor bw 1 MSM' font "Helvetica,30"
set title 'q@^{-}_{{/Symbol a}_R} by discr. dyn.' font "Helvetica,30"
plot 'img.dist.2d.dat' with image

set table 'img.dist.2d.dat'
splot 'commitor.bw.msm.out' u 1:2:5 not
unset table
set out 'fig-commitor-bw-msm-2.eps'
set title 'Commitor bw 2 MSM' font "Helvetica,30"
set title 'q@_{{/Symbol b}}^{-} by discr. dyn.' font "Helvetica,30"
plot 'img.dist.2d.dat' with image

set table 'img.dist.2d.dat'
splot 'commitor.bw.msm.out' u 1:2:7 not
unset table
set out 'fig-commitor-bw-msm-3.eps'
set title 'Commitor bw 3 MSM' font "Helvetica,30"
set title 'q@^{-}_{{/Symbol a}_L} by discr. dyn.' font "Helvetica,30"
plot 'img.dist.2d.dat' with image


set palette defined(\
0       0.2314  0.2980  0.7529,\
0.03125 0.2667  0.3529  0.8000,\
0.0625  0.3020  0.4078  0.8431,\
0.09375 0.3412  0.4588  0.8824,\
0.125   0.3843  0.5098  0.9176,\
0.15625 0.4235  0.5569  0.9451,\
0.1875  0.4667  0.6039  0.9686,\
0.21875 0.5098  0.6471  0.9843,\
0.25    0.5529  0.6902  0.9961,\
0.28125 0.5961  0.7255  1.0000,\
0.3125  0.6392  0.7608  1.0000,\
0.34375 0.6824  0.7882  0.9922,\
0.375   0.7216  0.8157  0.9765,\
0.40625 0.7608  0.8353  0.9569,\
0.4375  0.8000  0.8510  0.9333,\
0.46875 0.8353  0.8588  0.9020,\
0.5     0.8667  0.8667  0.8667,\
0.53125 0.8980  0.8471  0.8196,\
0.5625  0.9255  0.8275  0.7725,\
0.59375 0.9451  0.8000  0.7255,\
0.625   0.9608  0.7686  0.6784,\
0.65625 0.9686  0.7333  0.6275,\
0.6875  0.9686  0.6941  0.5804,\
0.71875 0.9686  0.6510  0.5294,\
0.75    0.9569  0.6039  0.4824,\
0.78125 0.9451  0.5529  0.4353,\
0.8125  0.9255  0.4980  0.3882,\
0.84375 0.8980  0.4392  0.3451,\
0.875   0.8706  0.3765  0.3020,\
0.90625 0.8353  0.3137  0.2588,\
0.9375  0.7961  0.2431  0.2196,\
0.96875 0.7529  0.1569  0.1843,\
1       0.7059  0.0157  0.1490\
)

set format cb "%.2f"

set table 'img.dist.2d.dat'
splot 'commitor.msm.out' u 1:2:($3-$11) not
unset table
set cbrange [-2e-1:2e-1]
set out 'fig-commitor-diff-msm-1.eps'
set title 'Commitor diff 1 MSM' font "Helvetica,30"
set title 'q@^{+}_{{/Symbol a}_R} - q@^{-}_{{/Symbol a}_R} by discr. dyn.' font "Helvetica,30"
plot 'img.dist.2d.dat' with image
set table 'img.dist.2d.dat'
splot 'commitor.out' u 1:2:($3-$11) not
unset table
set out 'fig-commitor-diff-1.eps'
set title 'Commitor diff 1' font "Helvetica,30"
set title 'q@^{+}_{{/Symbol a}_R} - q@^{-}_{{/Symbol a}_R} by NEMD' font "Helvetica,30"
plot 'img.dist.2d.dat' with image

set table 'img.dist.2d.dat'
splot 'commitor.msm.out' u 1:2:($5-$13) not
unset table
set cbrange [-2e-1:2e-1]
set out 'fig-commitor-diff-msm-2.eps'
set title 'Commitor diff 2 MSM' font "Helvetica,30"
set title 'q@_{{/Symbol b}}^{+} - q@_{{/Symbol b}}^{-} by discr. dyn.' font "Helvetica,30"
plot 'img.dist.2d.dat' with image
set table 'img.dist.2d.dat'
splot 'commitor.out' u 1:2:($5-$13) not
unset table
set out 'fig-commitor-diff-2.eps'
set title 'Commitor diff 2' font "Helvetica,30"
set title 'q@_{{/Symbol b}}^{+} - q@_{{/Symbol b}}^{-} by NEMD' font "Helvetica,30"
plot 'img.dist.2d.dat' with image

set table 'img.dist.2d.dat'
splot 'commitor.msm.out' u 1:2:($7-$15) not
unset table
set cbrange [-2e-1:2e-1]
set out 'fig-commitor-diff-msm-3.eps'
set title 'Commitor diff 3 MSM' font "Helvetica,30"
set title 'q@^{+}_{{/Symbol a}_L} - q@^{-}_{{/Symbol a}_L} by discr. dyn.' font "Helvetica,30"
plot 'img.dist.2d.dat' with image
set table 'img.dist.2d.dat'
splot 'commitor.out' u 1:2:($7-$15) not
unset table
set out 'fig-commitor-diff-3.eps'
set title 'Commitor diff 3' font "Helvetica,30"
set title 'q@^{+}_{{/Symbol a}_L} - q@^{-}_{{/Symbol a}_L}  by NEMD' font "Helvetica,30"
plot 'img.dist.2d.dat' with image

# set cbrange [0:1]
# set out 'fig-commitor-bw-1.eps'
# set title 'Commitor bw 1' font "Helvetica,30"
# splot 'commitor.bw.out' u 1:2:3 not w p pt 5 palette
# set out 'fig-commitor-bw-2.eps'
# set title 'Commitor bw 2' font "Helvetica,30"
# splot 'commitor.bw.out' u 1:2:5 not w p pt 5 palette
# set out 'fig-commitor-bw-3.eps'
# set title 'Commitor bw 3' font "Helvetica,30"
# splot 'commitor.bw.out' u 1:2:7 not w p pt 5 palette

# set cbrange [0:1]
# set out 'fig-commitor-bw-msm-1.eps'
# set title 'Commitor bw 1 by MSM' font "Helvetica,30"
# splot 'commitor.bw.msm.out' u 1:2:3 not w p pt 5 palette
# set out 'fig-commitor-bw-msm-2.eps'
# set title 'Commitor bw 2 by MSM' font "Helvetica,30"
# splot 'commitor.bw.msm.out' u 1:2:5 not w p pt 5 palette
# set out 'fig-commitor-bw-msm-3.eps'
# set title 'Commitor bw 3 by MSM' font "Helvetica,30"
# splot 'commitor.bw.msm.out' u 1:2:7 not w p pt 5 palette


# set cbrange [-2e-1:2e-1]
# set out 'fig-commitor-diff-msm-1.eps'
# set title 'Commitor diff 1 by MSM' font "Helvetica,30"
# splot 'commitor.msm.out' u 1:2:($3-$11) not w p pt 5 palette

# set cbrange [-2e-1:2e-1]
# set out 'fig-commitor-diff-1.eps'
# set title 'Commitor diff 1 by BF' font "Helvetica,30"
# splot 'commitor.out' u 1:2:($3-$11) not w p pt 5 palette

# set cbrange [-2e-1:2e-1]
# set out 'fig-commitor-diff-msm-2.eps'
# set title 'Commitor diff 2 by MSM' font "Helvetica,30"
# splot 'commitor.msm.out' u 1:2:($5-$13) not w p pt 5 palette

# set cbrange [-2e-1:2e-1]
# set out 'fig-commitor-diff-2.eps'
# set title 'Commitor diff 2 by BF' font "Helvetica,30"
# splot 'commitor.out' u 1:2:($5-$13) not w p pt 5 palette


# set cbrange [-1e-1:1e-1]
# set out 'fig-commitor-diff-msm-3.eps'
# set title 'Commitor diff 3 by MSM' font "Helvetica,30"
# splot 'commitor.msm.out' u 1:2:($7-$15) not w p pt 5 palette

# set cbrange [-1e-1:1e-1]
# set out 'fig-commitor-diff-3.eps'
# Set title 'Commitor diff 3 by BF' font "Helvetica,30"
# splot 'commitor.out' u 1:2:($7-$15) not w p pt 5 palette
