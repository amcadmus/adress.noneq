#!/bin/bash

source env.sh
source parameters.sh

cd nvt

echo 0 | g_msd -nice 0 -b $gmx_equi_skip &> msd.out

result=`grep D msd.out | grep System `

value=`echo $result | cut -d ']' -f 2 | awk '{print $1}'`
error=`echo $result | cut -d ']' -f 2 | awk '{print $3}' | cut -d ')' -f 1`

echo "# value error"> diffusivity.out
echo $value $error >> diffusivity.out

