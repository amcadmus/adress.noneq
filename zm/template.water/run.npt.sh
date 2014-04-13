#!/bin/bash

source env.sh
source parameters.sh

if echo "$gmx_ele_method" | grep pme &> /dev/null ; then
    echo "# run with ele method pme"
    gmx_ele_method_gromacs=pme
    gmx_ele_method_ind=1
    if echo "$gmx_ele_method" | grep pme-switch &> /dev/null ; then
	echo "# run with ele method pme-switch"
	gmx_ele_method_gromacs=pme-switch
	gmx_ele_method_ind=11
    fi
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
if test -d npt; then
    mv -f npt npt.`date +%s`
fi
cp -a $gmx_sys_tempalte_dir ./npt

echo "# revise grompp"
cd npt
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
sed -e "/^dt /s/=.*/= $gmx_dt/g" grompp.mdp |\
sed -e "/^nsteps /s/=.*/= $gmx_nsteps/g" |\
sed -e "/^ld-seed /s/=.*/= $gmx_seed/g" |\
sed -e "/^nstcalenergy /s/=.*/= $gmx_nstenergy/g"|\
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
sed -e "/^vdwtype /s/=.*/= shift/g"|\
sed -e "/^rvdw-switch /s/=.*/= $gmx_rcut_vdw_switch/g"|\
sed -e "/^rvdw /s/=.*/= $gmx_rcut_vdw/g"|\
sed -e "/^tau_t /s/=.*/= $gmx_taut/g"|\
sed -e "/^tau_p /s/=.*/= $gmx_taup/g"|\
sed -e "/^epsilon_rf /s/=.*/= $gmx_e_rf/g"|\
sed -e "/^gen_vel /s/=.*/= no/g"|\
sed -e "/^table-extension /s/=.*/= $gmx_tab_ext/g"> tmp.mdp
mv -f tmp.mdp grompp.mdp
cd ..

echo "# copy conf"
rm -f npt/conf.gro
cp $gmx_init_conf ./npt/conf.gro

echo "# revise top"
cd npt
natom=`wc -l conf.gro | awk '{print $1}'`
natom=`echo $natom - 3 | bc`
nmol=`echo $natom / 3 | bc`
sed -e "s/SOL.*/SOL $nmol/g" topol.top > tmp.top
mv -f tmp.top topol.top
cd ..

if test $gmx_ele_method_ind -eq 0; then
    echo "# gen pot"
    cd npt
    zm_xup=`echo $gmx_rlist + $gmx_tab_ext + .1 | bc -l`
    make -C $zm_gen_dir &> /dev/null
    $zm_gen_dir/zm -l $zm_l --xup $zm_xup --alpha $zm_alpha --rc $gmx_rcut_ele --output table.xvg &> /dev/null
    cd ..
fi

echo "# call grompp"
cd npt
$gmx_grompp_command > /dev/null
if [ $gmx_ele_method_ind -eq 1 ] || [ $gmx_ele_method_ind -eq 11 ]; then
    echo "# tune pme parameter"
    $gmx_tune_command -tune yes -self 1e-4 -seed $gmx_seed -nice 0
    mv -f tuned.tpr topol.tpr
fi
cd ..

echo "# call mdrun"
echo "## run with `which mdrun`"
echo "## run with $gmx_mdrun_command"
cd npt
$gmx_mdrun_command
cd ..


