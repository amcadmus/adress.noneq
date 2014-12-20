#!/bin/bash

if [ $# -ne 1 ]; then
    echo "usage:"
    echo "$0 target_dir"
    exit
fi

target_dir=$1

test ! -d $target_dir && mkdir -p $target_dir

echo "pack tansition matrixes"
tar cjvf tmatrix.tar.bz2 tmatrix.dih.*.out

files="cg.prob.dih.out bf.prob.dih.out cg.prob.dih.5sets.out tmatrix.tar.bz2 init.prob.dih.out floque.B.out direct.floquet.out uniq.disc.traj.dih largestSet.dih count.disc.state.dih diff.prob.dih.out"

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

