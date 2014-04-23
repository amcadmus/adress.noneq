#!/bin/bash

source env.sh
source parameters.sh

items="Temperature Pressure Box-X Box-Y Box-Z Volume Density"
idx_list=""

cd gromacs.traj.npt
echo "" | g_energy &> tmp.out
for ii in `echo $items`;
do
    idx=`grep $ii tmp.out | sed "s/.* \([0-9]\+  $ii\) .*/\1/g" | awk '{print $1}'`
    idx_list="$idx_list $idx"
done
rm -f tmp.out

echo "# command is: echo $idx_list | g_energy -nice 0 -xvg none -b $gmx_equi_skip &> energy.out"
echo $idx_list | g_energy -nice 0 -xvg none -b $gmx_equi_skip &> energy.out
grep -v \# energy.xvg | grep -v  '@' > tmp.out
mv -f tmp.out energy.xvg

cd ..

#grep Temperature tmp.out | sed "s/.* \([0-9]\+  Temperature\) .*/\1/g"
# cd npt
# echo 8 10 11 12 13 14 15 | g_energy -b $gmx_equi_skip &> energy.out
# grep -v \# energy.xvg | grep -v  '@' > tmp.out
# mv -f tmp.out energy.xvg
# cd ..

