#!/bin/bash

script_dir=$(cd ${0%/*} && echo $PWD)
base_dir=$script_dir/..
analyze_dir=$base_dir/analyze
dist_command=$analyze_dir/dist
traj_dir=gromacs.traj.npt
phi_file_name=angaver.phi.xvg
psi_file_name=angaver.psi.xvg

cwd=`pwd`
targets=`ls | grep ^simul | grep [0-9]$`

phi_list=""
psi_list=""

for ii in $targets;
do
    test ! -d $ii/$traj_dir/ && continue
    cd $ii/$traj_dir/
    if [ ! -f $phi_file_name ] || [ ! -f $psi_file_name ]; then
	echo "# no angle file, make them"
	$script_dir/build.dih.traj.sh &> /dev/null
    fi
    cd $cwd
    phi_list="$phi_list $ii/$traj_dir/$phi_file_name"
    psi_list="$psi_list $ii/$traj_dir/$psi_file_name"
done

echo "# compute the distribution of phi by $dist_command"
$dist_command --input-list-files "$phi_list" --column 2 --num-data-block 320 --num-bins 72 --output dist.phi.out
echo "# compute the distribution of psi by $dist_command"
$dist_command --input-list-files "$psi_list" --column 2 --num-data-block 320 --num-bins 72 --output dist.psi.out

