#!/bin/bash

rm -f done

source env.sh
source parameters.sh

if echo "$gmx_npt" | grep yes &> /dev/null ; then
    job_dir=gromacs.traj.npt
else
    job_dir=gromacs.traj.nvt
fi

if echo "$gmx_ele_method" | grep pme &> /dev/null ; then
    echo "# run with ele method pme"
    gmx_ele_method_gromacs=pme
    gmx_ele_method_ind=1
else if echo "$gmx_ele_method" | grep zm &> /dev/null ; then
    echo "# run with ele method zm"
    gmx_ele_method_gromacs=user
    gmx_ele_method_ind=0
else if echo "$gmx_ele_method" | grep rf &> /dev/null ; then
    echo "# run with ele method rf"
    gmx_ele_method_gromacs=reaction-field
    gmx_ele_method_ind=2
fi
fi
fi

echo "# copy files"
if test -d $job_dir; then
    mv -f $job_dir $job_dir.`date +%s`
fi
cp -a $gmx_sys_tempalte_dir ./$job_dir

echo "# copy conf"
rm -f $job_dir/conf.gro
cp $gmx_init_conf ./$job_dir/conf.gro

echo "# revise grompp"
cd $job_dir
gmx_nsteps=`echo "($gmx_time + 0.5 * $gmx_dt) / $gmx_dt" | bc`
gmx_nstenergy=`echo "($gmx_energy_feq + 0.5 * $gmx_dt) / $gmx_dt" | bc`
gmx_nstxtcout=`echo "($gmx_conf_feq   + 0.5 * $gmx_dt) / $gmx_dt" | bc`
gmx_seed=`date +%s`
if test $gmx_ele_method_ind -eq 0; then # zm
    gmx_nstlist=$gmx_nstlist
    gmx_rcut_ele_switch=$gmx_rcut_ele
fi
if test $gmx_ele_method_ind -eq 1; then # pme
    gmx_rcut_ele=$gmx_rlist
    gmx_rcut_ele_switch=$gmx_rcut_ele
fi
if test $gmx_ele_method_ind -eq 2; then # rf
    gmx_rcut_ele=$gmx_rlist
    gmx_rcut_ele_switch=$gmx_rcut_ele
fi
gmx_nstcalce=`echo "$gmx_nstenergy" | bc`
gmx_nstcomm=`echo "$gmx_nstenergy" | bc`
sed -e "/^dt /s/=.*/= $gmx_dt/g" grompp.mdp |\
sed -e "/^nsteps /s/=.*/= $gmx_nsteps/g" |\
sed -e "/^ld-seed /s/=.*/= -1/g" |\
sed -e "/^nstcalcenergy /s/=.*/= $gmx_nstcalce/g"|\
sed -e "/^nstcomm /s/=.*/= $gmx_nstcomm/g"|\
sed -e "/^nstenergy /s/=.*/= $gmx_nstenergy/g"|\
sed -e "/^nstxout /s/=.*/= 0/g"|\
sed -e "/^nstvout /s/=.*/= 0/g"|\
sed -e "/^nstfout /s/=.*/= 0/g"|\
sed -e "/^nstlog /s/=.*/= 0/g"|\
sed -e "/^nstxtcout /s/=.*/= $gmx_nstxtcout/g"|\
sed -e "/^nstlist /s/=.*/= $gmx_nstlist/g"|\
sed -e "/^rlist /s/=.*/= $gmx_rlist/g"|\
sed -e "/^coulombtype /s/=.*/= $gmx_ele_method_gromacs/g"|\
sed -e "/^rcoulomb-switch /s/=.*/= $gmx_rcut_ele_switch/g"|\
sed -e "/^rcoulomb /s/=.*/= $gmx_rcut_ele/g"|\
sed -e "/^fourierspacing /s/=.*/= $gmx_pme_F_spacing/g"|\
sed -e "/^pme_order /s/=.*/= $gmx_pme_order/g"|\
sed -e "/^vdwtype /s/=.*/= $gmx_vdw_type/g"|\
sed -e "/^vdw-type /s/=.*/= $gmx_vdw_type/g"|\
sed -e "/^vdw_type /s/=.*/= $gmx_vdw_type/g"|\
sed -e "/^rvdw-switch /s/=.*/= $gmx_rcut_vdw_switch/g"|\
sed -e "/^rvdw_switch /s/=.*/= $gmx_rcut_vdw_switch/g"|\
sed -e "/^rvdw /s/=.*/= $gmx_rcut_vdw/g"|\
sed -e "/^tau_t /s/=.*/= $gmx_taut/g"|\
sed -e "/^tau_p /s/=.*/= $gmx_taup/g"|\
sed -e "/^Pcoupl /s/=.*/= no/g"|\
sed -e "/^epsilon_rf /s/=.*/= $gmx_e_rf/g"|\
sed -e "/^gen_vel /s/=.*/= no/g"|\
sed -e "/^DispCorr /s/=.*/= EnerPres/g"|\
sed -e "/^table-extension /s/=.*/= $gmx_tab_ext/g"> tmp.mdp
mv -f tmp.mdp grompp.mdp

if grep "^adress " grompp.mdp | grep yes &> /dev/null; then
    echo "# run with adress"
    boxx=`tail -n 1 conf.gro | awk '{print $1}'`
    boxy=`tail -n 1 conf.gro | awk '{print $2}'`
    boxz=`tail -n 1 conf.gro | awk '{print $3}'`
    hboxx=`echo "$boxx * 0.5" | bc -l`
    hboxy=`echo "$boxy * 0.5" | bc -l`
    hboxz=`echo "$boxz * 0.5" | bc -l`
    cat grompp.mdp |\
    sed -e "/^DispCorr /s/=.*/= no/g"|\
    sed -e "/^adress_type /s/=.*/= $adress_type/g" |\
    sed -e "/^adress_interface_correction /s/=.*/= thermoforce/g" |\
    sed -e "/^adress_site /s/=.*/= com/g" |\
    sed -e "/^adress_ex_width /s/=.*/= $adress_ex_region/g" |\
    sed -e "/^adress_hy_width /s/=.*/= $adress_hy_region/g" |\
    sed -e "/^vdwtype /s/=.*/= user/g"|\
    sed -e "/^vdw-type /s/=.*/= user/g"|\
    sed -e "/^vdw_type /s/=.*/= user/g"|\
    sed -e "/^adress_reference_coords /s/=.*/= $hboxx $hboxy $hboxz/g" > tmp.mdp
    mv -f tmp.mdp grompp.mdp
fi
if echo "$gmx_npt" | grep yes &> /dev/null ; then
    cat grompp.mdp |\
    sed -e "/^Pcoupl /s/=.*/= parrinello-rahman/g" |\
    sed -e "/^DispCorr /s/=.*/= EnerPres/g" > tmp.mdp
    mv -f tmp.mdp grompp.mdp
fi
cd ..

if test -f $gmx_init_index; then
    echo "# copy index"
    cp $gmx_init_index ./$job_dir/index.ndx
fi

echo "# revise top"
cd $job_dir 
echo "# prepare topol.top"
nSOL=`../tools/gen.conf/nresd -f conf.gro | grep SOL | awk '{print $2}'`
nline_top=`wc topol.top | awk '{print $1}'`
nline_4_top=`echo $nline_top -4 | bc`
head -n $nline_4_top topol.top > new.top
tail -n 4 topol.top | sed "s/^SOL.*/SOL $nSOL/g" >> new.top
mv -f new.top topol.top
cd ..

# cd $job_dir
# nmol=`../tools/gen.conf/nresd -f conf.gro | grep SOL | awk '{print $2}'`
# sed -e "s/SOL.*/SOL $nmol/g" topol.top > tmp.top
# mv -f tmp.top topol.top
# cd ..

echo "# rescale conf"
cd $job_dir
# volumn=`echo "$nmol/$nvt_num_density" | bc -l`
# t_boxsize=`echo "e ( l($volumn) * (1./3.) )" | bc -l`
# r_boxsize=`tail -n 1 conf.gro | awk '{print $1}'`
# box_scale=`echo "$t_boxsize/$r_boxsize" | bc -l`
# editconf -scale $box_scale -f conf.gro -o out.gro
# mv -f out.gro conf.gro
cd ..

if test $gmx_ele_method_ind -eq 0; then
    echo "# gen pot"
    cd $job_dir
    zm_xup=`echo $gmx_rlist + $gmx_tab_ext + .1 | bc -l`
    make -C $zm_gen_dir &> /dev/null
    $zm_gen_dir/zm -l $zm_l --xup $zm_xup --alpha $zm_alpha --rc $gmx_rcut_ele --output table.xvg &> /dev/null
    cd ..
fi

echo "# call grompp"
cd $job_dir
if test -f index.ndx; then
    $gmx_grompp_command -n index.ndx > /dev/null
else
    $gmx_grompp_command
fi
if [ $? -ne 0 ]; then
    echo "# failed at $gmx_grompp_command, return"
    exit
fi

if test $gmx_ele_method_ind -eq 1; then
    echo "# tune pme parameter"
    $gmx_tune_command -tune yes -self 1e-4 -seed $gmx_seed -nice 0
    if [ $? -ne 0 ]; then
	echo "# failed at $gmx_tune_command, return"
	exit
    fi    
    mv -f tuned.tpr topol.tpr
fi
cd ..

echo "# call mdrun"
echo "## run with `which mdrun`"
echo "## run with $gmx_mdrun_command"
cd $job_dir
$gmx_mdrun_command
if [ $? -ne 0 ]; then
    echo "# failed at $gmx_mdrun_command, return"
    exit
fi
cd ..

touch done


