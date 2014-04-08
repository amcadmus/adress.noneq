#!/bin/bash

function do_rdf () {
    folder=$1
    source env.sh
    source parameters.sh
    echo "# find $folder, cal rdf"
    cd $folder
    if test -f rdf.xvg; then
	echo "# find rdf.xvg, do nothing"
	exit
    fi    
    echo "# command: echo 2 2 | g_rdf -rdf mol_com -b $gmx_equi_skip -nice 0 -xvg none"
    echo 2 2 | g_rdf -rdf mol_com -b $gmx_equi_skip -nice 0 -xvg none
    cd ..
}


if test -d npt; then
    do_rdf npt
fi
if test -d nvt; then
    do_rdf nvt
fi

