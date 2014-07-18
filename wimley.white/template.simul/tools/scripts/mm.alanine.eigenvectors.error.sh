#!/bin/bash

function pick_col2 () {
    awk 'function abs(x){return ((x < 0.0) ? -x : x)} {print abs($2)}' $1 > tmp.out; mv -f tmp.out $1
}

emma_dir=$HOME/Softs/emma
script_dir=$(cd ${0%/*} && echo $PWD)
base_dir=$script_dir/..
analyze_dir=$base_dir/analyze
assign_command=$analyze_dir/assign.dihedral
back_assign_command=$analyze_dir/recover.full.dist
eigenvector_command=$script_dir/mm.alanine.eigenvectors.sh
make -j -C $analyze_dir &>/dev/null
cwd=`pwd`

if [ -f mm.alanine.parameters.sh ]; then
    echo "# use parameters file: $cwd/mm.alanine.parameters.sh"
    source mm.alanine.parameters.sh
else
    echo "# use parameters file: $script_dir/mm.alanine.parameters.sh"
    source $script_dir/mm.alanine.parameters.sh
fi

num_samples=$err_num_samples

$eigenvector_command

eigv_l1_phi_list=""
eigv_l2_phi_list=""
eigv_l3_phi_list=""
eigv_l4_phi_list=""
eigv_l1_psi_list=""
eigv_l2_psi_list=""
eigv_l3_psi_list=""
eigv_l4_psi_list=""
for jj in `seq 1 1 $num_samples`;
do
    pjj=`printf %03d $jj`    
    echo "# doing $pjj"
    for ii in $targets;
    do
	test ! -d $ii/$traj_dir/ && continue
	nlines=`wc -l dih.$ii.disctraj | awk '{print $1}'`
	half_nlines=`echo $nlines / 2 | bc`
	start_cut=`echo "$RANDOM/32768.0 * $half_nlines" | bc -l | cut -f 1 -d '.'`
	end_cut=`echo $half_nlines + $start_cut | bc `
	echo "## cut traj file dih.$ii.disctraj.orig from $start_cut to $end_cut"
	mv -f dih.$ii.disctraj dih.$ii.disctraj.orig
	head -n $end_cut dih.$ii.disctraj.orig | tail -n $half_nlines > dih.$ii.disctraj
	# echo "## cut in dir $ii/$traj_dir/ from $start_cut to $end_cut"
	# mv -f $phi_file_name $phi_file_name.orig
	# mv -f $psi_file_name $psi_file_name.orig
	# head -n $end_cut $phi_file_name.orig | tail -n $half_nlines > $phi_file_name
	# head -n $end_cut $psi_file_name.orig | tail -n $half_nlines > $psi_file_name
    done
    $eigenvector_command .$pjj
    pick_col2 eigen.vector.l.01.dist.$pjj.phi
    pick_col2 eigen.vector.l.02.dist.$pjj.phi
    pick_col2 eigen.vector.l.03.dist.$pjj.phi
    pick_col2 eigen.vector.l.04.dist.$pjj.phi
    pick_col2 eigen.vector.l.01.dist.$pjj.psi
    pick_col2 eigen.vector.l.02.dist.$pjj.psi
    pick_col2 eigen.vector.l.03.dist.$pjj.psi
    pick_col2 eigen.vector.l.04.dist.$pjj.psi
    eigv_l1_phi_list="$eigv_l1_phi_list eigen.vector.l.01.dist.$pjj.phi"
    eigv_l2_phi_list="$eigv_l2_phi_list eigen.vector.l.02.dist.$pjj.phi"
    eigv_l3_phi_list="$eigv_l3_phi_list eigen.vector.l.03.dist.$pjj.phi"
    eigv_l4_phi_list="$eigv_l4_phi_list eigen.vector.l.04.dist.$pjj.phi"
    eigv_l1_psi_list="$eigv_l1_psi_list eigen.vector.l.01.dist.$pjj.psi"
    eigv_l2_psi_list="$eigv_l2_psi_list eigen.vector.l.02.dist.$pjj.psi"
    eigv_l3_psi_list="$eigv_l3_psi_list eigen.vector.l.03.dist.$pjj.psi"
    eigv_l4_psi_list="$eigv_l4_psi_list eigen.vector.l.04.dist.$pjj.psi"
    for ii in $targets;
    do
	mv -f dih.$ii.disctraj.orig dih.$ii.disctraj
	# test ! -d $ii/$traj_dir/ && continue
	# cd $ii/$traj_dir/
	# mv -f $phi_file_name.orig $phi_file_name
	# mv -f $psi_file_name.orig $psi_file_name
	# cd $cwd
    done
done

paste $eigv_l1_phi_list > all.tmp
$analyze_dir/avg.line -f all.tmp -c "`seq 1 1 $num_samples`" --output tmp.avg.out
awk '{print ($3)/sqrt(2)}' tmp.avg.out > sd.out
paste eigen.vector.l.01.dist.phi sd.out > tmp.out
mv -f tmp.out eigen.vector.l.01.dist.phi

paste $eigv_l2_phi_list > all.tmp
$analyze_dir/avg.line -f all.tmp -c "`seq 1 1 $num_samples`" --output tmp.avg.out
awk '{print ($3)/sqrt(2)}' tmp.avg.out > sd.out
paste eigen.vector.l.02.dist.phi sd.out > tmp.out
mv -f tmp.out eigen.vector.l.02.dist.phi

paste $eigv_l3_phi_list > all.tmp
$analyze_dir/avg.line -f all.tmp -c "`seq 1 1 $num_samples`" --output tmp.avg.out
awk '{print ($3)/sqrt(2)}' tmp.avg.out > sd.out
paste eigen.vector.l.03.dist.phi sd.out > tmp.out
mv -f tmp.out eigen.vector.l.03.dist.phi

paste $eigv_l4_phi_list > all.tmp
$analyze_dir/avg.line -f all.tmp -c "`seq 1 1 $num_samples`" --output tmp.avg.out
awk '{print ($3)/sqrt(2)}' tmp.avg.out > sd.out
paste eigen.vector.l.04.dist.phi sd.out > tmp.out
mv -f tmp.out eigen.vector.l.04.dist.phi


    
paste $eigv_l1_psi_list > all.tmp
$analyze_dir/avg.line -f all.tmp -c "`seq 1 1 $num_samples`" --output tmp.avg.out
awk '{print ($3)/sqrt(2)}' tmp.avg.out > sd.out
paste eigen.vector.l.01.dist.psi sd.out > tmp.out
mv -f tmp.out eigen.vector.l.01.dist.psi

paste $eigv_l2_psi_list > all.tmp
$analyze_dir/avg.line -f all.tmp -c "`seq 1 1 $num_samples`" --output tmp.avg.out
awk '{print ($3)/sqrt(2)}' tmp.avg.out > sd.out
paste eigen.vector.l.02.dist.psi sd.out > tmp.out
mv -f tmp.out eigen.vector.l.02.dist.psi

paste $eigv_l3_psi_list > all.tmp
$analyze_dir/avg.line -f all.tmp -c "`seq 1 1 $num_samples`" --output tmp.avg.out
awk '{print ($3)/sqrt(2)}' tmp.avg.out > sd.out
paste eigen.vector.l.03.dist.psi sd.out > tmp.out
mv -f tmp.out eigen.vector.l.03.dist.psi

paste $eigv_l4_psi_list > all.tmp
$analyze_dir/avg.line -f all.tmp -c "`seq 1 1 $num_samples`" --output tmp.avg.out
awk '{print ($3)/sqrt(2)}' tmp.avg.out > sd.out
paste eigen.vector.l.04.dist.psi sd.out > tmp.out
mv -f tmp.out eigen.vector.l.04.dist.psi

for jj in `seq 1 1 $num_samples`;
do
    pjj=`printf %03d $jj`    
    for ii in `seq 1 $num_eig`;
    do
	pii=`printf %02d $ii`
	rm -f eigen.vector.l.$pii.dist.$pjj.phi
	rm -f eigen.vector.l.$pii.dist.$pjj.psi
	rm -f eigen.vector.l.$pii.dist.$pjj
	rm -f eigen.vector.r.$pii.dist.$pjj.phi
	rm -f eigen.vector.r.$pii.dist.$pjj.psi
	rm -f eigen.vector.r.$pii.dist.$pjj
    done
done
