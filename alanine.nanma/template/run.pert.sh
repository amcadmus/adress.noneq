#!/bin/bash

source env.sh
source parameters.sh
source functions.sh

make -C tools/angles/ makedir
# make -C tools/angles/ clean
make -C tools/angles/ -j$nthreads

wwd=`pwd`
# prepare files
if test ! -d $pert_equi_result ; then
    echo "no dir $pert_equi_result, exit"
    exit
fi
if test ! -f $pert_equi_result/equi.frame ; then
    echo "no file $pert_equi_result/equi.frame, exit"
    exit
fi
if test ! -d $gro_dir; then
    echo "no dir $gro_dir, exit"
    exit
fi

if echo "$pert_ele_method" | grep pme &> /dev/null ; then
    echo "# run with ele method pme"
    pert_ele_method_gromacs=pme
    pert_ele_method_ind=1
    if echo "$pert_ele_method" | grep pme-switch &> /dev/null ; then
	echo "# run with ele method pme-switch"
	pert_ele_method_gromacs=pme-switch
	pert_ele_method_ind=11
    fi
else if echo "$pert_ele_method" | grep zm &> /dev/null ; then
    echo "# run with ele method zm"
    pert_ele_method_gromacs=user
    pert_ele_method_ind=0
else if echo "$pert_ele_method" | grep rf &> /dev/null ; then
    echo "# run with ele method rf"
    pert_ele_method_gromacs=reaction-field
    pert_ele_method_ind=2
fi
fi
fi

touch angle.name gxs.name success.dir.name

# targets=`awk '{print $1}' $pert_equi_result/equi.frame | head -n $pert_num_conf_use`
targets=`seq 0 $(($pert_num_conf_use-1))`
nlines_equi_frame=`wc  $pert_equi_result/equi.frame | awk '{print $1}'`

pert_main_dir=result.perts
for i in $targets;
do
    count=`printf %06d $i`
    runid=`echo "$count % $pert_parallel_num_pro" | bc`
    test $runid -ne $pert_parallel_my_id && continue
    test ! -d $pert_main_dir && mkdir -p $pert_main_dir
    my_dir=$pert_main_dir/pert.$count
    if test -d $my_dir; then
	if grep $my_dir success.dir.name &> /dev/null; then
	    echo "# existing successful dir $my_dir, continue"
	    continue
	else
	    echo "# existing dir $my_dir, but failed, remrove"
	    rm -fr $my_dir
	fi
    fi
    echo "# doing in dir $my_dir"
    mkdir -p $my_dir
#    cp -a $gro_dir $my_dir
    cd $wwd/$gro_dir
    ln_file_list=`ls | grep -v grompp | grep -v mdp$`
    cd $wwd/$my_dir
    for file in $ln_file_list;
    do
	ln -sf $wwd/$gro_dir/$file .
    done
    rm -f grompp.mdp
    cp $wwd/$gro_dir/grompp.mdp .
    rm -f run.log
    set_parameters_pert grompp.mdp

    if test $pert_ele_method_ind -eq 0; then	# zm
	echo "# gen zm pot"
	zm_xup=`echo $pert_rlist + $pert_tab_ext + .1 | bc -l`
	make -C $zm_gen_dir &> /dev/null
	$zm_gen_dir/zm -l $pert_zm_l --xup $zm_xup --alpha $pert_zm_alpha --rc $pert_rcut_ele --output table.xvg &> /dev/null
	rm -f tablep.xvg
	ln -s $wwd/tools/table6-12.xvg tablep.xvg
    fi
    if echo $pert_adress | grep yes &> /dev/null; then	# vdw tables for adress
	rm -f table.xvg tablep.xvg
	ln -s $wwd/tools/table6-12.xvg tablep.xvg
	ln -s $wwd/tools/table6-12.xvg table.xvg
	if [ -f $wwd/$pert_adress_tf_file ]; then	# tf table for adress
	    if [ -f tabletf_CMW.xvg ]; then
		echo "# rm existing tf table"
		rm -f tabletf_CMW.xvg
	    fi
	    ln -s $wwd/$pert_adress_tf_file ./tabletf_CMW.xvg
	fi
    fi
    
    count_1=`echo "($count % $nlines_equi_frame) + 1" | bc `
    count_1=`printf %06d $count_1`
    start_time=`grep "^$count_1" $pert_equi_result/equi.frame | awk '{print $2}'`
    echo "# run with command `which grompp`"
    $pert_grompp_command -n index.ndx -t $pert_equi_result/traj.trr -time $start_time &> run.log
    if [ $? -ne 0 ]; then
	echo "failed at grompp exit"; exit
    fi
    if [ $pert_ele_method_ind -eq 1 ] || [ $pert_ele_method_ind -eq 11 ]; then
	echo "# tune pme with command `which g_pme_error`"
	$pert_tune_command -tune yes -self 1e-2 -seed `date +%s` -nice 0
	if [ $? -ne 0 ]; then
	    echo "# failed at $pert_tune_command, return"
	    exit
	fi    
	mv -f tuned.tpr topol.tpr
    fi
    echo "# run with command `which mdrun`"
    echo "# run with $pert_mdrun_command"
    $pert_mdrun_command &> run.log
    if [ $? -ne 0 ]; then
	echo "failed at mdrun exit"; exit
    fi

    echo 3 14 | trjconv -center -pbc whole
    mv -f trajout.xtc alanine.xtc
    $wwd/tools/angles/evolve -f alanine.xtc -s angle.dat &> angle.log
    if [ $? -ne 0 ]; then
	echo "failed at evolve exit"; exit
    fi

    tmpid=`echo "$count - $pert_parallel_num_pro" | bc -l`
    echo "tmpid is $tmpid"
#    if [ $tmpid -lt $pert_parallel_num_pro ]; then
    if [ $count -eq 0 ]; then
	cp -a ..//pert.$count ..//backup.pert.$count
    fi
    rm -f traj.xtc traj.trr state*.cpt topol.tpr conf.gro index.ndx angle.log md.log genbox.log mdout.mdp protein.gro run.log confout.gro error.out grompp.mdp topol.top table*xvg
    
    cd $wwd
    echo "$my_dir/angle.dat" >> angle.name
    echo "$my_dir/gxs.out" >> gxs.name
    echo "$my_dir" >> success.dir.name
    sync
done

