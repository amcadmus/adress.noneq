#!/bin/bash

frame_begin=100
refh=2.00
nblock=10

make -C tools/analyze.number/ -j4

dir=$1
method=$2
if test ! -d $dir; then
    echo "no dir $dir"
    exit
fi
if echo $method | grep adress &> /dev/null; then
    echo "# method is $method"
else if echo $method | grep atom &> /dev/null; then
    echo "# method is $method"
else if echo $method | grep cg &> /dev/null; then
    echo "# method is $method"
else
    echo no method $method
    exit
fi
fi
fi

cwd=`pwd`
cd $dir
$cwd/tools/analyze.number/count.distrib -b $frame_begin --refh $refh -m $method -f traj.xtc -o number.out

nbin=`head -n 1 number.out | cut -f 3 -d ':'`
centers=`head -n 3 number.out | tail -n 1 | sed -e 's/#//g'`

i=1
for center in $centers
do
    echo "# process bin $i center: $center"
    result=`$cwd/tools/analyze.number/avg.var -n $nblock -c $(($i+1)) -f number.out | grep -v \#`
    i=$(($i+1))
    echo $center $result
done

cd $cwd
    