#!/bin/bash

source env.sh
source parameters.sh

cd nvt

echo 2 2 | g_hbond -xvg none -nice 0 -b $gmx_equi_skip &> hbond.log

result=`avg_jk -v col=2 hbnum.xvg | grep -v \#`

value=`echo $result | grep -v \# | awk '{print $1}'`
error=`echo $result | grep -v \# | awk '{print $2}'`

nmol=`grep SOL topol.top | awk '{print $2}'`
value=`echo "$value / $nmol" | bc -l`
error=`echo "$error / $nmol" | bc -l`

echo $value $error > hbond.out
