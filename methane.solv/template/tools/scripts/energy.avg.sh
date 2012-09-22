#!/bin/bash

make -C tools/average -j8

targets=`ls | grep ^pert`

count=0
for i in $targets;
do
    count=$(($count+1))
done
count=$(($count-1))

rm -f energy.name
count1=0
for i in $targets;
do
    echo doing $i
    cd $i
    echo 7 8 10 | g_energy -xvg none &> /dev/null
    cd ..
    echo $i/energy.xvg >> energy.name
    
    count1=$(($count1+1))
    if test $count1 -eq $count; then
	break;
    fi
done

echo doing average
tools/average/average.file -f energy.name -o avg.energy.out
