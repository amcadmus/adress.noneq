#!/bin/bash

eig_file_name=eig.out
if [ $# -eq 1 ]; then
    eig_file_name=$1
fi

emma_dir=$HOME/Softs/emma
script_dir=$(cd ${0%/*} && echo $PWD)
base_dir=$script_dir/..
analyze_dir=$base_dir/analyze
assign_command=$analyze_dir/assign.dihedral
traj_dir=gromacs.traj.npt
phi_file_name=angaver.phi.xvg
psi_file_name=angaver.psi.xvg
nbins=20
cwd=`pwd`
targets=`ls | grep ^simul | grep [0-9]$`
lag_times="`seq 1 1 9` `seq 10 10 90` `seq 100 50 1000`"
#lag_times="`seq 100 100 1000`"
out_list=""

for ii in $targets;
do
    cd $ii/$traj_dir/
    if [ ! -f $phi_file_name ] || [ ! -f $psi_file_name ]; then
	echo "# no angle file, make them"
	$script_dir/build.dih.traj.sh &> /dev/null
    fi
    cd $cwd
    echo "# assign angle in dir $ii/$traj_dir"
    $assign_command -f "$ii/$traj_dir/$phi_file_name $ii/$traj_dir/$psi_file_name" -n $nbins --column 2 --output dih.$ii.disctraj
    out_list="$out_list dih.$ii.disctraj"
done

echo "# emma calculates the connectivity"
$emma_dir/bin/mm_connectivity	-i $out_list -o largestSet
echo "# emma calculates the leading timescales"
$emma_dir/bin/mm_timescales	-i $out_list -restrictToStates largestSet -prior 0.01 -timestep 1.0 -neig 4 -lagtimes $lag_times -o $eig_file_name

