#!/bin/bash

script_dir=$(cd ${0%/*} && echo $PWD)
base_dir=$script_dir/..
analyze_dir=$base_dir/analyze
dist_command=$analyze_dir/dist
dist2_command=$analyze_dir/dist.2d
cwd=`pwd`

if [ -f mm.alanine.parameters.sh ]; then
    echo "# use parameters file: $cwd/mm.alanine.parameters.sh"
    source mm.alanine.parameters.sh
else
    echo "# use parameters file: $script_dir/mm.alanine.parameters.sh"
    source $script_dir/mm.alanine.parameters.sh
fi

phi_list=""
psi_list=""

for ii in $targets;
do
    test ! -d $ii/$traj_dir/ && continue
    echo "# doing $ii/$traj_dir/"
    cd $ii/$traj_dir/
    if [ ! -f $phi_file_name ] || [ ! -f $psi_file_name ]; then
	echo "# no angle file, make them"
	rm -f angdist*xvg
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
echo "# compute the distribution of phi and psi by $dist2_command"
$dist2_command --input-list-phi-files "$phi_list" --input-list-psi-files "$psi_list" --column 2 --num-data-block 320 --num-bins 72 --output dist.2d.out

