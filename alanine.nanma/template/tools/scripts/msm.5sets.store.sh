#!/bin/bash

if [ $# -ne 1 ]; then
    echo "usage:"
    echo "$0 target_dir"
    exit
fi

target_dir=$1

test ! -d $target_dir && mkdir -p $target_dir

files="cg.prob.5sets.out `ls | grep ^tmatrix.5sets | grep out$` init.prob.5sets.out floque.B.out largestSet.5sets"

if [ -f msm.parameters.sh ]; then
    files="$files msm.parameters.sh"
fi

for ii in $files;
do
    if [ ! -f $ii ]; then
	echo "no file $ii"
	continue
    fi
    echo "back up $ii"
    if [ -f $target_dir/$ii ]; then
	echo "over write $target_dir/$ii"
	rm -f $target_dir/$ii
    fi
    cp $ii $target_dir/$ii
done

