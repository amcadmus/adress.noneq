#!/bin/bash

if [ ! -f parameters.sh ]; then
    echo "# no file parameters.sh, exit"
    exit
fi
source parameters.sh

script_dir=$(cd ${0%/*} && echo $PWD)
base_dir=$script_dir/..
angle_dir=$base_dir/angles
msm_dir=$base_dir/msm

if [ -f msm.parameters.sh ]; then
    echo "# use parameters file: $cwd/msm.parameters.sh"
    source msm.parameters.sh
else
    echo "# use parameters file: $script_dir/msm.parameters.sh"
    source $script_dir/msm.parameters.sh
fi

if test ! -f dir.name; then
    sed -e 's,/gxs.out,/,g' gxs.name > dir.name
fi
if test ! -f largestSet.5sets; then
    echo "0 1 2 3 4" > largestSet.5sets
fi

make -C $angle_dir -j4 &> /dev/null
make -C $msm_dir -j4 &> /dev/null

echo "# make initial prob dist"
head -n 1 metastable.out | awk '{print $2" "$3" "$4" "$5" "$6}' > init.prob.5sets.out
first_dir=`head -n 1 dir.name`
if [ $redo -eq 1 ] || [ ! -f $first_dir/traj.5sets.disc ]; then
    echo "# discretize the traj to 5 sets"
    $angle_dir/assign.metaSet --input-dir dir.name --input angle.dat --output traj.5sets.disc
fi
echo "# calculate the transition matrix"
$msm_dir/calculate.trans.matrix --input traj.5sets.disc --input-dir dir.name --input-largest-set largestSet.5sets --n-data-block 1 --dt $pert_frame_feq --period $pert_warm_time --tau $msm_tau --begin $msm_steady_begin --end $msm_steady_end --output tmatrix.5sets --output-init-prob prob.$msm_steady_begin.out --max-rel-error 2.
echo "# evlove the probability"
$msm_dir/evolve.prob --input tmatrix.5sets --input-prob init.prob.out --dt $pert_frame_feq --period $pert_warm_time --end $msm_steady_end --output cg.prob.5sets.out

