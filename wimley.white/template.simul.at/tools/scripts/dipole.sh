#!/bin/bash

function do_dipole () {
    folder=$1
    source env.sh
    source parameters.sh
    echo "# find $folder, cal dipole"
    cd $folder
    if test -d dipole; then
	echo "# find existing result, do nothing"
	exit
    fi
    echo "command: echo 2 | g_dipoles -b $gmx_equi_skip -nice 0 -g -xvg none -dt 100 &> g_dipole.out"
    echo 2 | g_dipoles -b $gmx_equi_skip -nice 0 -g -xvg none &> g_dipole.out
    if test $? -ne 0; then
	echo "# failed at g_dipoles, return"
	exit
    fi
    mkdir dipole
    mv Mtot.xvg epsilon.xvg aver.xvg dipdist.xvg gkr.xvg cmap.xpm g_dipole.out dipole/
    cd ..
}


if test -d npt; then
    do_dipole npt
fi
if test -d nvt; then
    do_dipole nvt
fi

