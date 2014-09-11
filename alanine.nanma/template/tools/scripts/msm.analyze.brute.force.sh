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

echo "# use running parameter: parameters.sh"
source parameters.sh

if [ ! -d $target_dir ]; then
    echo "no dir $target_dir"
    exit
fi

if [ ! -f $target_dir/msm.parameters.sh ]; then
    echo "no file $target_dir/msm.parameters.sh"
    exit
else
    echo "# use parameter: $target_dir/msm.parameters.sh"
    source $target_dir/msm.parameters.sh
fi

for ii in `cat dir.name`;
do
    echo "# prepare $ii/traj.dih.disc.periodT"
    period_frame=`echo "$pert_warm_time / $pert_frame_feq" | bc -l |  cut -d "." -f 1`
    n_head_line=`echo "$msm_steady_end / $pert_warm_time + 1" | bc -l | cut -d "." -f 1`
    n_tail_line=`echo "$n_head_line - $msm_steady_begin / $pert_warm_time" | bc -l | cut -d "." -f 1`
    sed -n "1~${period_frame}p" $ii/traj.dih.disc | head -n $n_head_line | tail -n $n_tail_line > $ii/traj.dih.disc.periodT
done

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

mycommand="$msm_dir/calculate.coreset.msm.nemd --num-bin $msm_dih_nbin --input traj.dih.disc.periodT --input-dir dir.name --n-data-block 100 --input-cluster-map $target_dir/cluster.map.out --output-matrix-t $target_dir/coreset.t.out --output-matrix-m $target_dir/coreset.m.out"
echo "# calculate coreset t and m matrices by command: $mycommand"
$mycommand

if [ -f $target_dir/bf.prob.dih.out ]; then
    sed -n "1~${period_frame}p" $target_dir/bf.prob.dih.out > $target_dir/bf.prob.dih.periodT.out
    mycommand="$msm_dir/calculate.coreset.proj --input $target_dir/bf.prob.dih.periodT.out --input-largest-set $target_dir/largestSet.dih --input-bw $target_dir/commitor.bw.out.orig --output $target_dir/coreset.prob.out"
    echo "# calculate the projected time-dep prob. by command: $mycommand"
    $mycommand

    mycommand="$msm_dir/calculate.coreset.evlove --dt $pert_warm_time --end $pert_time --input-init-prob $target_dir/coreset.prob.out --input-tmatrix $target_dir/coreset.tmatrix.out --output $target_dir/coreset.prob.coreset.out"
    echo "# calculate the prob from coreset msm by $mycommand"
    $mycommand
fi


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

