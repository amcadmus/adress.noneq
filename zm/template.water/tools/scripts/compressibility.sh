#!/bin/bash

source env.sh
source parameters.sh

script_dir=$(cd ${0%/*} && echo $PWD)
base_dir=$script_dir/..
analyze_dir=$base_dir/analyze
flu_command=$analyze_dir/fluctuation

items="Volume"
idx_list=""

cd npt
echo "" | g_energy &> tmp.out
for ii in `echo $items`;
do
    idx=`grep $ii tmp.out | sed "s/.* \([0-9]\+  $ii\) .*/\1/g" | awk '{print $1}'`
    idx_list="$idx_list $idx"
done
rm -f tmp.out

echo "# command is: echo $idx_list | g_energy -nice 0 -xvg none -b $gmx_equi_skip -o volume.xvg &> volume.out"
echo $idx_list | g_energy -nice 0 -xvg none -b $gmx_equi_skip -o volume.xvg&> volume.out
grep -v \# volume.xvg | grep -v  '@' > tmp.out
mv -f tmp.out volume.xvg


result=`$flu_command -f volume.xvg -c 2 --num-block 256 | grep -v \#`

avgv=`echo $result | awk '{print $1}'`
avgv=`printf %.16f $avgv`
fluc=`echo $result | awk '{print $3}'`
fluc=`printf %f $fluc`
fluc_err=`echo $result | awk '{print $4}'`
fluc_err=`printf %f $fluc_err`

temperature=300
scale=`echo "1000000/1.3806488/$temperature/$avgv" | bc -l`

fluc=`echo "$fluc * $scale" | bc -l`
fluc_err=`echo "$fluc_err * $scale" | bc -l`
fluc=`printf %f $fluc`
fluc_err=`printf %f $fluc_err`

echo "$fluc $fluc_err" > kappa.out

