#!/bin/bash

function set_parameters_equi () {
    file=$1
    equi_nstep_warm=`echo "$equi_warm_time/$equi_dt" | bc -l | cut -d '.' -f 1`
    equi_nstep_count=`echo "$equi_frame_feq * $equi_num_frame / $equi_dt" | bc -l | cut -d '.' -f 1`
    equi_nstep=`echo "$equi_nstep_warm + $equi_nstep_count" | bc`
    equi_xvout_feq=`echo "$equi_frame_feq / $equi_dt" | bc -l | cut -d '.' -f 1`
    sed -e "/^integrator/s/=.*/= sd/g" $file |\
    sed -e "/^dt/s/=.*/= $equi_dt/g" |\
    sed -e "/^Tcoupl/s/=.*/= no/g" |\
    sed -e "/^nstep/s/=.*/= $equi_nstep/g" |\
    sed -e "/^nstxout/s/=.*/= $equi_xvout_feq/g" |\
    sed -e "/^nstvout/s/=.*/= $equi_xvout_feq/g" |\
    sed -e "/^ld-seed/s/=.*/= $equi_seed/g" |\
    sed -e "/^gen_vel/s/=.*/= no/g" |\
    sed -e "/^tau_t/s/=.*/= $equi_taut/g" |\
    sed -e "/^nstxtcout/s/=.*/= $equi_xvout_feq/g" > tmptmptmp.mdp
    mv -f tmptmptmp.mdp $file
}

# function set_parameters_long_equi () {
#     file=$1
#     long_equi_nstep_warm=`echo "$long_equi_warm_time/$long_equi_dt" | bc -l | cut -d '.' -f 1`
#     long_equi_nstep_count=`echo "$long_equi_frame_feq * $long_equi_num_frame / $long_equi_dt" | bc -l | cut -d '.' -f 1`
#     long_equi_nstep=`echo "$long_equi_nstep_warm + $long_equi_nstep_count" | bc`
#     long_equi_xvout_feq=`echo "$long_equi_frame_feq / $long_equi_dt" | bc -l | cut -d '.' -f 1`
#     sed -e "/^integrator/s/=.*/= sd/g" $file |\
#     sed -e "/^dt/s/=.*/= $long_equi_dt/g" |\
#     sed -e "/^nstep/s/=.*/= $long_equi_nstep/g" |\
#     sed -e "/^nstxout/s/=.*/= 0/g" |\
#     sed -e "/^nstvout/s/=.*/= 0/g" |\
#     sed -e "/^ld-seed/s/=.*/= $long_equi_seed/g" |\
#     sed -e "/^tau_t/s/=.*/= $long_equi_taut/g" |\
#     sed -e "/^nstxtcout/s/=.*/= $long_equi_xvout_feq/g" > tmptmptmp.mdp
#     mv -f tmptmptmp.mdp $file
# }

function set_parameters_pert () {
    file=$1
    grep nstcalcenergy $file &> /dev/null
    if [ $? -eq 1 ]; then
	echo "nstcalcenergy = 1" >> $file
    fi
    pert_nstep=`echo "$pert_time / $pert_dt" | bc -l | cut -d '.' -f 1`
    pert_xtcout_feq=`echo "$pert_frame_feq / $pert_dt" | bc -l | cut -d '.' -f 1`
    pert_xvout_feq=0
    pert_energy_feq=$pert_xtcout_feq
    pert_nstcalce=`echo "$pert_energy_feq" | bc`
    pert_nstcomm=`echo "$pert_energy_feq" | bc`
    if test $pert_ele_method_ind -eq 0; then # zm
	pert_nstlist=$pert_nstlist
	pert_rcut_ele_switch=$pert_rcut_ele
    fi
    if test $pert_ele_method_ind -eq 1; then # pme
	pert_rcut_ele=$pert_rlist
	pert_rcut_ele_switch=$pert_rcut_ele
    fi
    if test $pert_ele_method_ind -eq 2; then # rf
	pert_rcut_ele=$pert_rlist
	pert_rcut_ele_switch=$pert_rcut_ele
    fi
    sed -e "/^dt /s/=.*/= $pert_dt/g" $file |\
    sed -e "/^nsteps /s/=.*/= $pert_nstep/g" |\
    sed -e "/^ld-seed /s/=.*/= `date +%s`/g" |\
    sed -e "/^nstcalcenergy /s/=.*/= $pert_nstcalce/g"|\
    sed -e "/^nstcomm /s/=.*/= $pert_nstcomm/g"|\
    sed -e "/^nstenergy /s/=.*/= $pert_energy_feq/g"|\
    sed -e "/^integrator/s/=.*/= $pert_integrator/g" |\
    sed -e "/^nstxtcout/s/=.*/= $pert_xtcout_feq/g" |\
    sed -e "/^nstxout/s/=.*/= $pert_xvout_feq/g" |\
    sed -e "/^nstvout/s/=.*/= $pert_xvout_feq/g" |\
    sed -e "/^nstfout/s/=.*/= 0/g" |\
    sed -e "/^nstlog /s/=.*/= 0/g"|\
    sed -e "/^nstlist /s/=.*/= $pert_nstlist/g"|\
    sed -e "/^rlist /s/=.*/= $pert_rlist/g"|\
    sed -e "/^coulombtype /s/=.*/= $pert_ele_method_gromacs/g"|\
    sed -e "/^rcoulomb-switch /s/=.*/= $pert_rcut_ele_switch/g"|\
    sed -e "/^rcoulomb /s/=.*/= $pert_rcut_ele/g"|\
    sed -e "/^fourierspacing /s/=.*/= $pert_pme_F_spacing/g"|\
    sed -e "/^pme-order /s/=.*/= $pert_pme_order/g"|\
    sed -e "/^vdwtype /s/=.*/= $pert_vdw_type/g"|\
    sed -e "/^vdw-type /s/=.*/= $pert_vdw_type/g"|\
    sed -e "/^vdw_type /s/=.*/= $pert_vdw_type/g"|\
    sed -e "/^rvdw-switch /s/=.*/= $pert_rcut_vdw_switch/g"|\
    sed -e "/^rvdw /s/=.*/= $pert_rcut_vdw/g"|\
    sed -e "/^DispCorr /s/=.*/= EnerPres/g"|\
    sed -e "/^Tcoupl /s/=.*/= no/g" |\
    sed -e "/^Pcoupl /s/=.*/= $pert_barostat/g" |\
    sed -e "/^tau_t/s/=.*/= $pert_taut/g" |\
    sed -e "/^tau-t/s/=.*/= $pert_taut/g" |\
    sed -e "/^tau_p/s/=.*/= $pert_taup/g" |\
    sed -e "/^tau-p/s/=.*/= $pert_taup/g" |\
    sed -e "/^epsilon-rf /s/=.*/= $pert_e_rf/g"|\
    sed -e "/^userreal1/s/=.*/= $pert_local_sd_range/g" |\
    sed -e "/^gen_vel /s/=.*/= no/g" |\
    sed -e "/^gen-vel /s/=.*/= no/g"|\
    sed -e "/^couple-moltype /s/=.*/= /g"|\
    sed -e "/^E-x /s/=.*/= 1 $pert_strength 0.0/g" |\
    sed -e "/^table-extension /s/=.*/= $pert_tab_ext/g"> tmptmptmp.mdp
    mv -f tmptmptmp.mdp $file
    if test $pert_mode -eq 1; then
	sed -e "/^E-xt /s/=.*/= 1 $pert_warm_time 0.0/g" $file > tmptmptmp.mdp
    else if test $pert_mode -eq 2; then
	pi=3.14159265359
	my_phi=`echo "$pert_phi / 180.0 * $pi" | bc -l`
	my_T=`echo "2. * $pi / $pert_warm_time" | bc -l`
	sed -e "/^E-xt /s/=.*/= 2 $my_T $my_phi $pert_shift 0.0/g" $file > tmptmptmp.mdp
    else
	echo "wrong pert mode: $pert_mode"
	exit
    fi
    fi
    mv -f tmptmptmp.mdp $file
    if echo $pert_adress | grep no &> /dev/null; then
	grep -v adress $file >  tmptmptmp.mdp
	mv -f tmptmptmp.mdp $file
    else
	echo "# run with adress"
	boxx=`tail -n 1 conf.gro | awk '{print $1}'`
	boxy=`tail -n 1 conf.gro | awk '{print $2}'`
	boxz=`tail -n 1 conf.gro | awk '{print $3}'`
	hboxx=`echo "$boxx * 0.5" | bc -l`
	hboxy=`echo "$boxy * 0.5" | bc -l`
	hboxz=`echo "$boxz * 0.5" | bc -l`
	cat $file |\
        sed -e "/^DispCorr /s/=.*/= no/g"|\
        sed -e "/^adress-type /s/=.*/= $pert_adress_type/g" |\
        sed -e "/^adress-interface-correction /s/=.*/= thermoforce/g" |\
        sed -e "/^adress-site /s/=.*/= com/g" |\
        sed -e "/^adress-ex-width /s/=.*/= $pert_adress_ex_region/g" |\
        sed -e "/^adress-hy-width /s/=.*/= $pert_adress_hy_region/g" |\
        sed -e "/^vdwtype /s/=.*/= user/g"|\
        sed -e "/^vdw-type /s/=.*/= user/g"|\
        sed -e "/^vdw_type /s/=.*/= user/g"|\
        sed -e "/^adress-reference-coords /s/=.*/= $hboxx $hboxy $hboxz/g" > tmptmptmp.mdp
	mv -f tmptmptmp.mdp grompp.mdp
    fi
}

function split_trr () {
    echo 2 0 | trjconv -f traj.trr -o out.gro -center
    equi_xvout_feq=`echo "$equi_frame_feq / $equi_dt" | bc -l | cut -d '.' -f 1`
    equi_warm_num_frame=`echo "$equi_warm_time / $equi_frame_feq" | bc -l | cut -d '.' -f 1`
    equi_nline_gro=`wc -l conf.gro | awk '{print $1}'`
    tmp_nline_del=`echo "$equi_nline_gro * ($equi_warm_num_frame+1)" | bc -l`
    sed "1,$tmp_nline_del d" out.gro > tmptmp.gro
    split -l $equi_nline_gro -a 5 -d tmptmp.gro
    mkdir -p equiConfs
    for i in `ls | grep "^x"`
    do
	tmp_number=`echo $i | cut -d 'x' -f 2 | cut -d 'c' -f 1`
	mv -f $i conf.$tmp_number.gro
	mv conf.$tmp_number.gro equiConfs
    done
    rm -f out.gro tmptmp.gro
}

function print_equi_frame_time () {
    rm -f equi.frame
    equi_frame_start_time=`echo "$equi_warm_time + $equi_frame_feq" | bc -l`
    equi_frame_end_time=`  echo "$equi_warm_time + $equi_frame_feq * $equi_num_frame" | bc -l`
    count=1
    for i in `seq $equi_frame_start_time $equi_frame_feq $equi_frame_end_time`
    do
	myprint=`echo "$i - $equi_frame_feq * 0.5" | bc -l`
	mycount=`printf %06d $count`
	echo "$mycount $myprint" >> equi.frame
	count=$(($count+1))
    done
}
