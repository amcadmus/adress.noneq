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

cd $target_dir
octave $script_dir/msm.floquet.eig.m

neig=`grep columns floquet.eigvector.out | awk '{print $3}'`
echo "# $neig eigenvectors are printed"

for ii in `seq 1 $neig`;
do
    grep -v \# floquet.eigvector.out | awk "{print \$$ii}"  | grep -v \# > tmp.vec.in
    $msm_dir/vector.set2dih --input-largest-set largestSet.dih --num-bin $msm_dih_nbin --input tmp.vec.in --output floquet.eigvector.$ii.out
done
for ii in `seq 1 $neig`;
do
    grep -v \# floquet.eigvector.l.out | awk "{print \$$ii}"  | grep -v \# > tmp.vec.in
    $msm_dir/vector.set2dih --input-largest-set largestSet.dih --num-bin $msm_dih_nbin --input tmp.vec.in --output floquet.eigvector.l.$ii.out
done
# rm -f tmp.vec.in

cd $cwd

