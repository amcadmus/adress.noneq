#!/bin/bash

source parameters.sh
source functions.sh

make -C tools/angles/ makedir
make -C tools/angles/ -j4

cwd=`pwd`
# prepare files
if test ! -d $pert_conf_dir ; then
    echo "no dir result.equi/equiConfs/, exit"
    exit
fi
if test ! -d $gro_dir; then
    echo "no dir $gro_dir, exit"
    exit
fi
rm -f angle.name

cd $pert_conf_dir
targets=`ls *gro | head -n $pert_num_conf_use`
cd $cwd

pert_main_dir=result.perts
for i in $targets;
do
    count=`echo $i | cut -d '.' -f 2`
    runid=`echo "$count % $pert_parallel_num_pro" | bc`
    test $runid -ne $pert_parallel_my_id && continue
    test ! -d $pert_main_dir && mkdir -p $pert_main_dir
    my_dir=$pert_main_dir/pert.$count
    if test -d $my_dir; then
	echo "existing dir $my_dir, remove"
	rm -fr $my_dir
    fi
    cp -a $gro_dir $my_dir
    rm -f $my_dir/$i
    cp $pert_conf_dir/$i $my_dir
    cd $my_dir
    rm -f run.log
    mv -f $i conf.gro
    set_parameters_pert grompp.mdp
    echo "# run with command `which grompp`" &>> run.log
    $grompp_command &>> run.log
    echo "# run with command `which mdrun`" &>> run.log
    $mdrun_command &>> run.log

    echo 3 14 | trjconv -center -pbc whole
    mv -f trajout.xtc alanine.xtc
    $cwd/tools/angles/evolve -f alanine.xtc -s angle.dat &> angle.log
    rm -f traj.xtc traj.trr state*.cpt topol.tpr conf.gro index.ndx 
    
    cd $cwd
    
    echo "$my_dir/angle.dat" >> angle.name    
done

