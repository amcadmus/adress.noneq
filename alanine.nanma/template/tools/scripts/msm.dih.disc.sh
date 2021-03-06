#!/bin/bash

if [ ! -f parameters.sh ]; then
    echo "# no file parameters.sh, exit"
    exit
fi
source parameters.sh

script_dir=$(cd ${0%/*} && echo $PWD)
base_dir=$script_dir/..
angle_dir=$base_dir/angles
msm_dir=$base_dir/msm
cwd=`pwd`

if [ -f msm.parameters.sh ]; then
    echo "# use parameters file: $cwd/msm.parameters.sh"
    source msm.parameters.sh
else
    echo "# use parameters file: $script_dir/msm.parameters.sh"
    source $script_dir/msm.parameters.sh
fi

if test ! -f dir.name; then
    sed -e 's,/gxs.out,/,g' gxs.name > dir.name
fi

echo "# compile binary"
make -C $angle_dir -j4 &> /dev/null
make -C $msm_dir -j4 &> /dev/null

echo "# build discrete dih traj"
if [ $redo -eq 1 ] || [ ! -f largestSet.dih ]; then
    rm -f uniq.disc.traj.dih count.disc.state.dih
    for ii in `cat dir.name`;
    do
	if [ $redo -eq 1 ] || [ ! -f $ii/traj.dih.disc ]; then
	    echo "# prepare $ii/traj.dih.disc"
	    cd $ii/
	    if [ ! -f angaver.phi.xvg ] || [ ! -f angaver.psi.xvg ]; then
		$script_dir/build.dih.traj.sh &> /dev/null
		rm -f angdist.phi.xvg angdist.psi.xvg
	    fi
	    $msm_dir/assign.dihedral --input-dih-list "angaver.phi.xvg angaver.psi.xvg" --column 2 --num-bin $msm_dih_nbin --output traj.dih.disc &> /dev/null
	    cd $cwd
	fi
	sort -g $ii/traj.dih.disc  | uniq >> uniq.disc.traj.dih
    done
    sort -g uniq.disc.traj.dih | uniq > largestSet.dih
    loop_end_tmp=`echo "$msm_dih_nbin * $msm_dih_nbin - 1" | bc`
    for i in `seq 0 $loop_end_tmp`; do count=`grep $i uniq.disc.traj.dih| wc -l`; echo $i $count >> count.disc.state.dih ; done
fi
# rm -f uniq.disc.traj.dih
    
echo "# calculate the transition matrix with command:"
echo "# $msm_dir/calculate.trans.matrix --input traj.dih.disc --input-dir dir.name --input-largest-set largestSet.dih --n-data-block 1 --dt $pert_frame_feq --period $pert_warm_time --tau $msm_tau --begin $msm_steady_begin --end $msm_steady_end --output tmatrix.dih --output-init-prob prob.$msm_steady_begin.dih.out --max-rel-error 2."
$msm_dir/calculate.trans.matrix --input traj.dih.disc --input-dir dir.name --input-largest-set largestSet.dih --n-data-block 1 --dt $pert_frame_feq --period $pert_warm_time --tau $msm_tau --begin $msm_steady_begin --end $msm_steady_end --output tmatrix.dih --output-init-prob prob.$msm_steady_begin.dih.out --max-rel-error 2.
$msm_dir/calculate.trans.matrix --input traj.dih.disc --input-dir dir.name --input-largest-set largestSet.dih --n-data-block 1 --dt $pert_frame_feq --period $pert_warm_time --tau $msm_tau --begin 0 --end $pert_frame_feq --output trash.tmatrix.dih --output-init-prob init.prob.dih.out --max-rel-error 2.
rm -f trash.tmatrix.dih*
rm -f prob.$msm_steady_begin.dih.out
echo "# evlove the probability with command: "
echo "# $msm_dir/evolve.prob --input tmatrix.dih --input-prob init.prob.dih.out --dt $pert_frame_feq --period $pert_warm_time --end $msm_steady_end --output cg.prob.dih.out"
$msm_dir/evolve.prob --input tmatrix.dih --input-prob init.prob.dih.out --dt $pert_frame_feq --period $pert_warm_time --end $pert_time --output cg.prob.dih.out
echo "# recover 5 sets cg traj with command:"
echo "# $msm_dir/recover.set.prob --input-prob cg.prob.dih.out --input-largest-set largestSet.dih --num-bin $msm_dih_nbin --num-sample 1000 --output cg.prob.dih.5sets.out"
$msm_dir/recover.set.prob --input-prob cg.prob.dih.out --input-largest-set largestSet.dih --num-bin $msm_dih_nbin --num-sample 1000 --output cg.prob.dih.5sets.out

mycommand="$msm_dir/calculate.trans.matrix --input traj.dih.disc --input-dir dir.name --input-largest-set largestSet.dih --n-data-block 1 --dt $pert_frame_feq --period $pert_warm_time --begin $msm_steady_begin --end $msm_steady_end --output direct.floquet.out"
echo "# direct compute floquet matrix P by command : $mycommand"
$mycommand

mycommand="$msm_dir/calculate.time.dist --input-traj-dir dir.name --input-disc-traj traj.dih.disc --dt $pert_frame_feq --num-data-block 1 --num-bin $msm_dih_nbin --output bf.prob.dih.out"
echo "# compute the bf probability with command : $mycommand"
$mycommand

mycommand="$msm_dir/diff.time.dist --input-dist-bf bf.prob.dih.out --input-dist-cg cg.prob.dih.out --input-largest-set largestSet.dih --output diff.prob.dih.out"
echo "# compare bf with cg by command: $mycommand"
$mycommand

echo "# calculate the floque matrix"
rm -f floque.B.out
nstate=`wc init.prob.dih.out | awk '{print $1}'`
for ii in `seq 1 $nstate`
do
    echo "## calcuate for $ii the base"
    rm -f tmp.init.out
    for jj in `seq 1 $nstate`
    do
	if [ $jj -eq $ii ]; then
	    echo 1 >> tmp.init.out
	else
	    echo 0 >> tmp.init.out
	fi
    done
    $msm_dir/evolve.prob --input tmatrix.dih --input-prob tmp.init.out --dt-store $pert_frame_feq --dt $msm_tau --period $pert_warm_time --end $pert_warm_time --output tmp.perid.out
    tail -n 1 tmp.perid.out | awk '{$1=""; print}' >> floque.B.out
    rm -f tmp.init.out tmp.perid.out
done

store_dir_name="dis.dih$msm_dih_nbin.tau$msm_tau.b$msm_steady_begin.e$msm_steady_end"
$script_dir/msm.dih.store.sh $store_dir_name
