#!/bin/bash

if [ $# -ne 1 ]; then
    echo "usage:"
    echo "$0 DIR_STORING_MSM"
    exit
fi

target_dir=$1
script_dir=$(cd ${0%/*} && echo $PWD)
base_dir=$script_dir/..
angle_dir=$base_dir/angles
msm_dir=$base_dir/msm
cwd=`pwd`

if [ ! -d $target_dir ]; then
    echo "no dir $target_dir"
    exit
fi

if [ ! -f $target_dir/msm.parameters.sh ]; then
    echo "no file $target_dir/msm.parameters.sh"
    exit
else
    source $target_dir/msm.parameters.sh
fi

mycommand="$msm_dir/evolve.markov --input-largest-set $target_dir/largestSet.dih --input-m $target_dir/floque.B.out --num-step 1e8 --output traj.out"
echo "# evolve markov model by: $mycommand"
$mycommand

rm -f current.name
echo "." > current.name
mycommand="$msm_dir/calculate.commitor --num-bin $msm_dih_nbin --input-cluster-map $target_dir/cluster.map.out --input-traj-dir current.name --input-disc-traj traj.out --output-fw $target_dir/commitor.fw.msm.out --output-bw $target_dir/commitor.bw.msm.out"
echo "# compute commitor by $mycommand"
$mycommand
