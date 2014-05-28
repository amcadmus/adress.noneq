#!/bin/bash

source env.sh
source parameters.sh

acc_command=/home/wanghan/NO_BACKUP/zm/template.water/tools/analyze/autoCorrelation
int_command=/home/wanghan/NO_BACKUP/zm/template.water/tools/analyze/integrate
avl_command=/home/wanghan/NO_BACKUP/zm/template.water/tools/analyze/avg.line

#	2	3	4	5	6	7
items="Pres-XX Pres-XY Pres-XZ Pres-YY Pres-YZ Pres-ZZ"
idx_list=""

cd nvt
echo "" | g_energy &> tmp.out
for ii in `echo $items`;
do
    idx=`grep $ii tmp.out | sed "s/.* \([0-9]\+  $ii\) .*/\1/g" | awk '{print $1}'`
    idx_list="$idx_list $idx"
done
rm -f tmp.out

echo "# command is: echo $idx_list | g_energy -nice 0 -xvg none -b $gmx_equi_skip -o pressure.xvg &> pressure.out"
echo $idx_list | g_energy -nice 0 -xvg none -b $gmx_equi_skip -o pressure.xvg&> pressure.out
grep -v \# pressure.xvg | grep -v  '@' > tmp.out
mv -f tmp.out pressure.xvg

awk '{print $1" "$3" "$4" "$6" "($2-$5)*0.5" "($5-$7)*0.5}' pressure.xvg > tmp.out

box=`tail -n 1 conf.gro | awk '{print $1}'`
volume=`echo "$box * $box * $box" | bc -l`
temperature=300
scale=`echo "$volume / 1.38 / $temperature * 0.0001 * 10" | bc -l`

echo "# auto corr and integrate for xy"
$acc_command -f tmp.out -u 20 --column-data 2 --num-data-block 5000
$int_command --scale $scale -f autoCorrelation.out -d 2 -e 3 -i 20 > integrate.xy.out
echo "# auto corr and integrate for xz"
$acc_command -f tmp.out -u 20 --column-data 3 --num-data-block 5000
$int_command --scale $scale -f autoCorrelation.out -d 2 -e 3 -i 20 > integrate.xz.out
echo "# auto corr and integrate for yz"
$acc_command -f tmp.out -u 20 --column-data 4 --num-data-block 5000
$int_command --scale $scale -f autoCorrelation.out -d 2 -e 3 -i 20 > integrate.yz.out
echo "# auto corr and integrate for (x - y)/2"
$acc_command -f tmp.out -u 20 --column-data 5 --num-data-block 5000
$int_command --scale $scale -f autoCorrelation.out -d 2 -e 3 -i 20 > integrate.xmy.out
echo "# auto corr and integrate for (y - z)/2"
$acc_command -f tmp.out -u 20 --column-data 6 --num-data-block 5000
$int_command --scale $scale -f autoCorrelation.out -d 2 -e 3 -i 20 > integrate.ymz.out

echo "# join results"
cp integrate.xy.out integrate.all.out
join integrate.all.out integrate.xz.out > tmp.out; mv -f tmp.out integrate.all.out
join integrate.all.out integrate.yz.out > tmp.out; mv -f tmp.out integrate.all.out
join integrate.all.out integrate.xmy.out > tmp.out; mv -f tmp.out integrate.all.out
join integrate.all.out integrate.ymz.out > tmp.out; mv -f tmp.out integrate.all.out

$avl_command -f integrate.all.out  -c "2 4 6 8 10" -o avg.out
awk '{print $1}' integrate.all.out > tmp.out
paste tmp.out avg.out > viscosity.out
rm -f avg.out tmp.out

#awk '{print $1" "($2+$4+$6+$8+$10)/5.0" "sqrt(($3*$3+$5*$5+$7*$7+$9*$9+$11*$11)/(5.0-1)/5.0)}' integrate.all.out > viscosity.out

cd ..
