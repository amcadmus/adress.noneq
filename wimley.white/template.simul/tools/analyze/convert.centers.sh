#!/bin/bash

if [ $# -ne 1 ]; then
    echo "usage: "
    echo "$0 centers.txt"
    exit
fi

file=$1
if [ ! -f $file ]; then
    echo "cannot find file $file"
    exit
fi

name=`echo $file | sed 's/.txt//g' `

./txt2xtc -f $file -o $name.xtc

echo "[ phi ]" > angle.ndx
echo "1 2 3 4" >> angle.ndx
echo "[ psi ]" >> angle.ndx
echo "2 3 4 5" >> angle.ndx

echo 0 | g_angle -type dihedral -xvg none -od angdist.phi.xvg -ov angaver.phi.xvg -nice 0 -f $name.xtc
echo 1 | g_angle -type dihedral -xvg none -od angdist.psi.xvg -ov angaver.psi.xvg -nice 0 -f $name.xtc

join angaver.phi.xvg angaver.psi.xvg > $name.angles.out

rm -f angdist.phi.xvg angdist.psi.xvg angaver.phi.xvg angaver.psi.xvg


