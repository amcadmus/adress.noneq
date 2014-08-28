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

mycommand="$msm_dir/calculate.commitor --input-largest-set $target_dir/largestSet.dih --num-bin $msm_dih_nbin --input-cluster-map $target_dir/cluster.map.out --input-traj-dir dir.name --input-disc-traj traj.dih.disc.periodT --output-fw $target_dir/commitor.fw.out --output-bw $target_dir/commitor.bw.out"
echo "# calculate commitor by command: $mycommand"
$mycommand
paste $target_dir/commitor.fw.out $target_dir/commitor.bw.out > $target_dir/commitor.out

mycommand="$msm_dir/calculate.steady.dist --input-largest-set $target_dir/largestSet.dih --num-bin $msm_dih_nbin --input-traj-dir dir.name --input-disc-traj traj.dih.disc.periodT --output $target_dir/steady.dist.out"
echo "# calculate steady dist by command: $mycommand"
$mycommand

mycommand="$msm_dir/calculate.fht --input-largest-set $target_dir/largestSet.dih --input-cluster-map $target_dir/cluster.map.out  --num-bin $msm_dih_nbin --input-traj-dir dir.name --input-disc-traj traj.dih.disc.periodT --output $target_dir/fht.out"
echo "# calculate first hitting time by command: $mycommand"
$mycommand

mycommand="$msm_dir/calculate.coreset.msm --num-bin $msm_dih_nbin --input-largest-set $target_dir/largestSet.dih  --input-floquet $target_dir/floque.B.out --input-cluster-map $target_dir/cluster.map.out --input-steady $target_dir/steady.dist.out.orig --input-fw $target_dir/commitor.fw.out.orig --input-bw $target_dir/commitor.bw.out.orig --output-tmatrix $target_dir/coreset.tmatrix.out"
echo "# calculate coreset msm tmatrix by command: $mycommand"
$mycommand

if [ -f $target_dir/cluster.map.1.out ]; then
    mycommand="$msm_dir/calculate.fht --input-largest-set $target_dir/largestSet.dih --input-cluster-map $target_dir/cluster.map.1.out  --num-bin $msm_dih_nbin --input-traj-dir dir.name --input-disc-traj traj.dih.disc.periodT --output $target_dir/fht.1.out"
    echo "# calculate first hitting time 1 by command: $mycommand"
    $mycommand
fi

if [ -f $target_dir/cluster.map.2.out ]; then
    mycommand="$msm_dir/calculate.fht --input-largest-set $target_dir/largestSet.dih --input-cluster-map $target_dir/cluster.map.2.out  --num-bin $msm_dih_nbin --input-traj-dir dir.name --input-disc-traj traj.dih.disc.periodT --output $target_dir/fht.2.out"
    echo "# calculate first hitting time 2 by command: $mycommand"
    $mycommand
fi

if [ -f $target_dir/cluster.map.3.out ]; then
    mycommand="$msm_dir/calculate.fht --input-largest-set $target_dir/largestSet.dih --input-cluster-map $target_dir/cluster.map.3.out  --num-bin $msm_dih_nbin --input-traj-dir dir.name --input-disc-traj traj.dih.disc.periodT --output $target_dir/fht.3.out"
    echo "# calculate first hitting time 3 by command: $mycommand"
    $mycommand
fi

