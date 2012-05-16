#!/bin/bash

source parameters.sh
source functions.sh

cwd=`pwd`
# prepare files
if test ! -d $pert_conf_dir ; then
    echo "no dir result.equi/equiConfs/, exit"
    exit
fi

cd $pert_conf_dir
targets=`ls *gro`
cd $cwd

for i in $targets;
do
    count=`echo $i | cut -d '.' -f 2`
    my_dir=pert.$count
    if test -d $my_dir; then
	echo "existing dir $my_dir, remove"
	rm -fr $my_dir
    fi
    cp -a $gro_dir $my_dir
    rm -f $my_dir/$i
    cp $pert_conf_dir/$i $my_dir
    cd $my_dir
    rm -f run.log
    ../$perturbation_command -f $i -o conf.gro &>> run.log
    set_parameters_pert grompp.mdp
    grompp &>> run.log
    mdrun -v &>> run.log
    cd ..
done

