#!/bin/bash

emma_dir=$HOME/Softs/emma
script_dir=$(cd ${0%/*} && echo $PWD)
base_dir=$script_dir/..
analyze_dir=$base_dir/analyze
assign_command=$analyze_dir/assign.dihedral
back_assign_command=$analyze_dir/recover.full.dist
make -j -C $analyze_dir &>/dev/null
cwd=`pwd`

if [ -f mm.alanine.parameters.sh ]; then
    echo "# use parameters file: $cwd/mm.alanine.parameters.sh"
    source mm.alanine.parameters.sh
else
    echo "# use parameters file: $script_dir/mm.alanine.parameters.sh"
    source $script_dir/mm.alanine.parameters.sh
fi

if [ $# -ge 1 ]; then
    file_surfix=$1
fi

#traj_dir=gromacs.traj.npt
#phi_file_name=angaver.phi.xvg
#psi_file_name=angaver.psi.xvg
#nbins=20
#targets=`ls | grep ^simul | grep [0-9]$`
# lag_times="`seq 1 1 9` `seq 10 10 90` `seq 100 50 1000`"
# lag_times="`seq 100 100 1000`"

my_lag_time=$selected_lag_time
out_list=""

for ii in $targets;
do
    test ! -d $ii/$traj_dir/ && continue
    cd $ii/$traj_dir/
    if [ ! -f $phi_file_name ] || [ ! -f $psi_file_name ]; then
	echo "# no angle file, make them"
	$script_dir/build.dih.traj.sh &> /dev/null
    fi
    cd $cwd
    if [ ! -f dih.$ii.disctraj ]; then
	echo "# assign angle in dir $ii/$traj_dir"
	$assign_command -f "$ii/$traj_dir/$phi_file_name $ii/$traj_dir/$psi_file_name" -n $nbins --column 2 --output dih.$ii.disctraj
    fi
    out_list="$out_list dih.$ii.disctraj"
done

echo "# emma calculates the connectivity"
$emma_dir/bin/mm_connectivity	-i $out_list -o largestSet
echo "# emma calculates the count matrix"
$emma_dir/bin/mm_estimate    -i dih.simul.00*disctraj -restrictToStates largestSet -prior 0.01 -lagtime $my_lag_time -outputtransitionmatrix tmatrix.failed.out -outputcountmatrix cmatrix.out -sampling slidingwindow
echo "# convert count matrix to transition matrix by using on Ben's scirpts"
$script_dir/convert_cmatrix.py cmatrix.out tmatrix.out
echo "# emma calculates the eigen vectors"
$emma_dir/bin/mm_transitionmatrixAnalysis -inputtransitionmatrix tmatrix.out -nev $num_eig -eigenvalues eigen.values.out -lefteigenvectors eigen.vector.l.out -righteigenvectors eigen.vector.r.out
echo "# convert to readable dists"

for ii in `seq 1 $num_eig`;
do
    pii=`printf %02d $ii`
    $back_assign_command --input eigen.vector.r.out --input-largest-set largestSet --column $ii --num-bin $nbins --output eigen.vector.r.$pii.dist$file_surfix
    $back_assign_command --input eigen.vector.l.out --input-largest-set largestSet --column $ii --num-bin $nbins --output eigen.vector.l.$pii.dist$file_surfix
done


# echo "# emma calculates the leading timescales"
# $emma_dir/bin/mm_timescales	-i $out_list -restrictToStates largestSet -prior 0.01 -timestep 1.0 -neig 4 -lagtimes $lag_times -o $eig_file_name

# ~/Softs/emma/bin/mm_timescales  -i dih.simul.00*disctraj -restrictToStates largestSet -prior 0.01 -timestep 1.0 -neig 4 -lagtimes 100
# ~/Softs/emma/bin/mm_estimate    -i dih.simul.00*disctraj -restrictToStates largestSet -prior 0.01 -lagtime 100 -outputtransitionmatrix tmatrix.out -sampling slidingwindow
# ~/Softs/emma/bin/mm_chapman -i dih.simul.00*disctraj -inputtransitionmatrix tmatrix.out -dtT 10  -dtTraj 1 -kmax 10 -randomsets 5 -restrictToStates largestSet
# ~/Softs/emma/bin/mm_transitionmatrixAnalysis -inputtransitionmatrix tmatrix.out -nev 4 -eigenvalues

