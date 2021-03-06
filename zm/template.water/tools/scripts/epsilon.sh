#!/bin/bash

make -j4 -C /home/wanghan/NO_BACKUP/zm/template.water/tools/analyze/ 
command=/home/wanghan/NO_BACKUP/zm/template.water/tools/analyze/epsilon

function do_epsilon () {
    folder=$1
    source env.sh
    source parameters.sh
    echo "# find $folder, cal epsilon"
    cd $folder
    echo "# command: $command -b $gmx_equi_skip --num-data-block 32"
    $command -b $gmx_equi_skip --num-data-block 256 | head -n 4 > epsilon.out
    cd ..
}


if test -d npt; then
    do_epsilon npt
fi
if test -d nvt; then
    do_epsilon nvt
fi

