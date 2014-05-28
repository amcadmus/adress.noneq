#!/bin/bash

emma_dir=$HOME/Softs/emma
script_dir=$(cd ${0%/*} && echo $PWD)
base_dir=$script_dir/..
timescale_command=$script_dir/timescales.sh

analyze_dir=$base_dir/analyze
assign_command=$analyze_dir/assign.dihedral
traj_dir=gromacs.traj.npt
phi_file_name=angaver.phi.xvg
psi_file_name=angaver.psi.xvg
cwd=`pwd`
targets=`ls | grep ^simul | grep [0-9]$`
num_samples=8
num_eig=3

$timescale_command eig.avg.out

eig_test_list=""
for jj in `seq 1 1 $num_samples`;
do
    pjj=`printf %03d $jj`    
    echo "# doing $pjj"
    for ii in $targets;
    do
	cd $ii/$traj_dir/
	nlines=`wc -l $phi_file_name | awk '{print $1}'`
	half_nlines=`echo $nlines / 2 | bc`
	start_cut=`echo "$RANDOM/32768.0 * $half_nlines" | bc -l | cut -f 1 -d '.'`
	end_cut=`echo $half_nlines + $start_cut | bc `
	echo "## cut in dir $ii/$traj_dir/ from $start_cut to $end_cut"
	mv -f $phi_file_name $phi_file_name.orig
	mv -f $psi_file_name $psi_file_name.orig
	head -n $end_cut $phi_file_name.orig | tail -n $half_nlines > $phi_file_name
	head -n $end_cut $psi_file_name.orig | tail -n $half_nlines > $psi_file_name
	cd $cwd
    done
    $timescale_command eig.$pjj.out
    eig_test_list="$eig_test_list eig.$pjj.out"
    for ii in $targets;
    do
	cd $ii/$traj_dir/
	mv -f $phi_file_name.orig $phi_file_name
	mv -f $psi_file_name.orig $psi_file_name
	cd $cwd
    done
done

sd_list=""
for kk in `seq 2 1 $(($num_eig+1))`
do
    echo "# post process with colume $kk"
    file_list=""
    for jj in `seq 1 1 $num_samples`;
    do
	pjj=`printf %03d $jj`
	awk "{print \$$kk}" eig.$pjj.out > tmp.$pjj.out
	file_list="$file_list tmp.$pjj.out"
    done
    paste $file_list > all.$kk.out
    $analyze_dir/avg.line -f all.$kk.out -c "`seq 1 1 $num_samples`" --output tmp.avg.out
    awk '{print ($3)/sqrt(2)}' tmp.avg.out > sd.$kk.out
    sd_list="$sd_list sd.$kk.out"
    rm -f $file_list all.$kk.out tmp.avg.out
done

paste eig.avg.out $sd_list > eig.err.out
mv -f eig.err.out eig.avg.out
rm -f $sd_list
#rm -f $eig_test_list
    

