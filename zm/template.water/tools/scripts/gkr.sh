#!/bin/bash

make -j4 -C /home/wanghan/NO_BACKUP/zm/template.water/tools/analyze/ 
command=/home/wanghan/NO_BACKUP/zm/template.water/tools/analyze/gkr
command=/home/mi/wanghan//study/adress.noneq.bk/zm//template.water/tools/analyze/gkr

function do_gkr () {
    folder=$1
    source env.sh
    source parameters.sh

    cd $folder
    echo "$command -b $gmx_equi_skip --rup 1e3 --num-data-block 32"
    $command -b $gmx_equi_skip --rup 1e3 --num-data-block 32
    if test $? -ne 0; then
	echo "# failed at $command, return"
	exit
    fi    
    cd ..
}


if test -d npt; then
    do_gkr npt
fi
if test -d nvt; then
    do_gkr nvt
fi

