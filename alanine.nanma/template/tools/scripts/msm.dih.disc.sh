#!/bin/bash

if [ ! -f parameters.sh ]; then
    echo "# no file parameters.sh, exit"
    exit
fi
source parameters.sh

msm_tau=1.0
msm_steady_begin=300
msm_steady_end=$pert_time

script_dir=$(cd ${0%/*} && echo $PWD)
base_dir=$script_dir/..
angle_dir=$base_dir/angles
msm_dir=$base_dir/msm
cwd=`pwd`

if test ! -f dir.name; then
    sed -e 's,/gxs.out,/,g' gxs.name > dir.name
fi

make -C $angle_dir -j4 &> /dev/null
make -C $msm_dir -j4 &> /dev/null

echo "# build discrete dih traj"
if test [ ! -f largestSet.dih ]; then
    rm -f uniq.disc.traj.dih count.disc.state.dih
    for ii in `cat dir.name`;
    do
	if [ ! -f $ii/traj.dih.disc ]; then
	    echo "# prepare $ii/traj.dih.disc"
	    cd $ii/
	    $script_dir/build.dih.traj.sh &> /dev/null
	    $msm_dir/assign.dihedral --input-dih-list "angaver.phi.xvg angaver.psi.xvg" --column 2 --num-bin 20 --output traj.dih.disc &> /dev/null
	    rm -f angaver.phi.xvg angaver.psi.xvg angdist.phi.xvg angdist.psi.xvg
	    cd $cwd
	fi
	sort -g $ii/traj.dih.disc  | uniq >> uniq.disc.traj.dih
    done
    sort -g uniq.disc.traj.dih | uniq > largestSet.dih
    for i in `seq 0 399`; do count=`grep $i uniq.disc.traj.dih| wc -l`; echo $i $count >> count.disc.state.dih ; done
fi
# rm -f uniq.disc.traj.dih
    
echo "# calculate the transition matrix"
$msm_dir/calculate.trans.matrix --input traj.dih.disc --input-dir dir.name --input-largest-set largestSet.dih --n-data-block 1 --dt $pert_frame_feq --period $pert_warm_time --tau $msm_tau --begin $msm_steady_begin --end $msm_steady_end --output tmatrix.dih --output-init-prob prob.$msm_steady_begin.dih.out --max-rel-error 2.
$msm_dir/calculate.trans.matrix --input traj.dih.disc --input-dir dir.name --input-largest-set largestSet.dih --n-data-block 1 --dt $pert_frame_feq --period $pert_warm_time --tau $msm_tau --begin 0 --end $pert_frame_feq --output trash.tmatrix.dih --output-init-prob prob.init.dih.out --max-rel-error 2.
rm -f trash.tmatrix.dih*
echo "# evlove the probability"
$msm_dir/evolve.prob --input tmatrix.dih --input-prob prob.init.dih.out --input-largest-set largestSet.dih --dt $pert_frame_feq --period $pert_warm_time --end $msm_steady_end > cg.prob.dih.out

