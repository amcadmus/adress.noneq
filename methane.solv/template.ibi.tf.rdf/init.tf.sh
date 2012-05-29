#!/bin/bash

source env.sh
source parameters.sh

function set_param () {
    targetfile=$1
    sed -e "s,base_conf=.*,base_conf=$base_conf,g" $targetfile |
    sed -e "s/n_base_block=.*/n_base_block=\"$n_base_block\"/g" |
    sed -e "s/number_density=.*/number_density=$number_density/g" |
    sed -e "s,cg_pot_file=.*,cg_pot_file=$base_dir/$cg_pot_file,g" |
    sed -e "s/gmx_nsteps=.*/gmx_nsteps=$tf_gmx_nsteps/g" |
    sed -e "s/gmx_nstenergy=.*/gmx_nstenergy=$tf_gmx_nstenergy/g" |
    sed -e "s/gmx_nstxtcout=.*/gmx_nstxtcout=$tf_gmx_nstxtcout/g" |
    sed -e "s/ex_region_r=.*/ex_region_r=$ex_region_r/g" |
    sed -e "s/hy_region_r=.*/hy_region_r=$hy_region_r/g" |
    sed -e "s/tf_extension=.*/tf_extension=$tf_extension/g" |
    sed -e "s/tf_step=.*/tf_step=$tf_step/g" |
    sed -e "s/tf_spline_extension=.*/tf_spline_extension=$tf_spline_extension/g" |
    sed -e "s/tf_spline_step=.*/tf_spline_step=$tf_spline_step/g" |
    sed -e "s/tf_prefactor=.*/tf_prefactor=$tf_prefactor/g" |
    sed -e "s/tf_iterations_max=.*/tf_iterations_max=$tf_iterations_init/g" > tmp.param
    mv -f tmp.param $targetfile
}

if test -d step.000.tf; then
    echo "dir step.000.tf exits, backup"
    mv step.000.tf step.000.tf.`date +%s`
fi
rm -f step.000.tf

cp -a template.tf.init step.000.tf
rm -f step.000.tf/env.sh
cp env.sh step.000.tf

echo "# running on `uname -n`"
echo "# with mdrun command located at `which mdrun`"
base_dir=`pwd`
cd step.000.tf
set_param parameters.sh
./gen.tf.sh
cd ..

