#!/bin/bash

source parameters.sh
source functions.sh

make -C tools/perturbations/ makedir
make -C tools/perturbations/ -j4
make -C tools/waves/ -j4

cwd=`pwd`
# prepare files
if test ! -d $pert_conf_dir ; then
    echo "no dir result.equi/equiConfs/, exit"
    exit
fi
rm -f h.count.name
rm -f density.wave.name

cd $pert_conf_dir
targets=`ls *gro | head -n $pert_num_conf_use`
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
    rm -f $i
    set_parameters_pert grompp.mdp
    echo "# run with command `which grompp`" &>> run.log
    $grompp_command &>> run.log
    echo "# run with command `which mdrun`" &>> run.log
    $mdrun_command &>> run.log

    if test ! -f mytop; then
	echo "old version, no mytop"
	../tools/h.bond/equi.ch4.1 -o h.count.out &> /dev/null
    else
	../tools/h.bond/equi.ch4.1 -t mytop -o h.count.out &> /dev/null
	../tools/waves/density.wave -t mytop --refh 0.1 -o density.wave.dat &> density.wave.log 
    fi
    rm -f traj.xtc state*.cpt topol.tpr conf.gro confout.gro index.ndx 
    
    cd ..
    
    echo "$my_dir/h.count.out" >> h.count.name    
    echo "$my_dir/density.wave.dat" >> density.wave.name    
done

