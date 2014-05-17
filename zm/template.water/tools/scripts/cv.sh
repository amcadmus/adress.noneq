#!/bin/bash

source env.sh
source parameters.sh

flu_command=/home/wanghan/NO_BACKUP/zm/template.water/tools/analyze/fluctuation

items="Total-Energy"
idx_list=""

cd nvt
echo "" | g_energy &> tmp.out
for ii in `echo $items`;
do
    idx=`grep $ii tmp.out | sed "s/.* \([0-9]\+  $ii\) .*/\1/g" | awk '{print $1}'`
    idx_list="$idx_list $idx"
done
rm -f tmp.out

echo "# command is: echo $idx_list | g_energy -nice 0 -xvg none -b $gmx_equi_skip -o hamiltonian.xvg &> hamiltonian.out"
echo $idx_list | g_energy -nice 0 -xvg none -b $gmx_equi_skip -o hamiltonian.xvg&> hamiltonian.out
grep -v \# hamiltonian.xvg | grep -v  '@' > tmp.out
mv -f tmp.out hamiltonian.xvg

temperature=300
scale=`echo "1./1.3806488/6.0221413/$temperature/$temperature*1000000" | bc -l`
nmol=`grep SOL topol.top | awk '{print $2}'`

result=`$flu_command -f hamiltonian.xvg -c 2 --num-block 1024 | grep -v \#`

fluc=`echo $result | awk '{print $3}'`
fluc=`printf %f $fluc`
fluc_err=`echo $result | awk '{print $4}'`
fluc_err=`printf %f $fluc_err`

fluc=`echo "$fluc * $scale / $nmol" | bc -l`
fluc_err=`echo "$fluc_err * $scale / $nmol" | bc -l`
fluc=`printf %f $fluc`
fluc_err=`printf %f $fluc_err`





