#!/bin/bash

function set_parameters_equi () {
    file=$1
    nstep_warm=`echo "$equi_warm_time/$equi_dt" | bc -l | cut -d '.' -f 1`
    nstep_count=`echo "$equi_frame_feq * $equi_num_frame / $equi_dt" | bc -l | cut -d '.' -f 1`
    nstep=`echo "$nstep_warm + $nstep_count" | bc`
    xvout_feq=`echo "$equi_frame_feq / $equi_dt" | bc -l | cut -d '.' -f 1`
    sed -e "/^dt/s/=.*/= $equi_dt/g" $file |\
    sed -e "/^nstep/s/=.*/= $nstep/g" |\
    sed -e "/^nstxout/s/=.*/= $xvout_feq/g" |\
    sed -e "/^nstvout/s/=.*/= $xvout_feq/g" |\
    sed -e "/^ld-seed/s/=.*/= $equi_seed/g" |\
    sed -e "/^nstxtcout/s/=.*/= 0/g" > tmptmptmp.mdp
    mv -f tmptmptmp.mdp $file
}
