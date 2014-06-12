#!/bin/bash

if [ ! -f parameters.sh ]; then
    echo "# no file parameters.sh, exit"
    exit
fi
source parameters.sh

msm_tau=1.0
msm_steady_begin=300
msm_steady_end=$pert_time

script_dir=$(cd ${0%/*} && echo $PWD)
base_dir=$script_dir/..
angle_dir=$base_dir/angles
msm_dir=$base_dir/msm

if test ! -f dir.name; then
    sed -e 's,/gxs.out,/,g' gxs.name > dir.name
fi
if test ! -f largestSet.5sets; then
    echo "0 1 2 3 4" > largestSet.5sets
fi
if test ! -f init.prob.out ; then
    head -n 1 metastable.out | awk '{print $2" "$3" "$4" "$5" "$6}' > init.prob.out
fi

make -C $angle_dir -j4 &> /dev/null
make -C $msm_dir -j4 &> /dev/null

echo "# discretize the traj to 5 sets"
$angle_dir/assign.metaSet --input-dir dir.name --input angle.dat --output traj.5sets.disc
echo "# calculate the transition matrix"
$msm_dir/calculate.trans.matrix --input traj.5sets.disc --input-dir dir.name --input-largest-set largestSet.5sets --n-data-block 1 --dt $pert_frame_feq --period $pert_warm_time --tau $msm_tau --begin $msm_steady_begin --end $msm_steady_end --output tmatrix.5sets --output-init-prob prob.$msm_steady_begin.out
echo "# evlove the probability"
$msm_dir/evolve.prob --input tmatrix.5sets --input-prob init.prob.out --input-largest-set largestSet.5sets --dt $pert_frame_feq --period $pert_warm_time --end $msm_steady_end > cg.prob.out

