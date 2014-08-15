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
paste $target_dir/commitor.fw.msm.out $target_dir/commitor.bw.msm.out > $target_dir/commitor.msm.out

mycommand="$msm_dir/calculate.steady.dist --num-bin $msm_dih_nbin --input-traj-dir current.name --input-disc-traj traj.out --output $target_dir/steady.dist.msm.out"
echo "# compute steady dist by $mycommand"
$mycommand

mycommand="$msm_dir/calculate.fht --input-largest-set $target_dir/largestSet.dih --input-cluster-map $target_dir/cluster.map.out  --num-bin $msm_dih_nbin --input-traj-dir current.name --input-disc-traj traj.out --output $target_dir/fht.msm.out"
echo "# calculate first hitting time by command: $mycommand"
$mycommand

if [ -f $target_dir/cluster.map.1.out ]; then
    mycommand="$msm_dir/calculate.fht --input-largest-set $target_dir/largestSet.dih --input-cluster-map $target_dir/cluster.map.1.out  --num-bin $msm_dih_nbin --input-traj-dir current.name --input-disc-traj traj.out --output $target_dir/fht.msm.1.out"
    echo "# calculate first hitting time 1 by command: $mycommand"
    $mycommand
fi
    
if [ -f $target_dir/cluster.map.2.out ]; then
    mycommand="$msm_dir/calculate.fht --input-largest-set $target_dir/largestSet.dih --input-cluster-map $target_dir/cluster.map.2.out  --num-bin $msm_dih_nbin --input-traj-dir current.name --input-disc-traj traj.out --output $target_dir/fht.msm.2.out"
    echo "# calculate first hitting time 2 by command: $mycommand"
    $mycommand
fi
    
if [ -f $target_dir/cluster.map.3.out ]; then
    mycommand="$msm_dir/calculate.fht --input-largest-set $target_dir/largestSet.dih --input-cluster-map $target_dir/cluster.map.3.out  --num-bin $msm_dih_nbin --input-traj-dir current.name --input-disc-traj traj.out --output $target_dir/fht.msm.3.out"
    echo "# calculate first hitting time 3 by command: $mycommand"
    $mycommand
fi
