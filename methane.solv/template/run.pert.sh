#!/bin/bash

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
    cp $gro_dir $my_dir
    rm -f $my_dir/$i
    cp $pert_conf_dir/$i $my_dir
    cd $my_dir
    mv $i conf.gro
    set_parameters_pert grompp.mdp
    grompp
    mdrun -v &> md.log
    cd ..
done

