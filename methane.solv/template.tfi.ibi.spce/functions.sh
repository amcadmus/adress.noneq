#!/bin/bash

function ibi_grompp () {
    sed -e "/^nsteps/s/=.*/= $ibi_gmx_nsteps/g" grompp.mdp |\
    sed -e "/^nstenergy/s/=.*/= $ibi_gmx_nstenergy/g" |\
    sed -e "/^nstxtcout/s/=.*/= $ibi_gmx_nstxtcout/g" > tmp.mdp
    mv -f tmp.mdp grompp.mdp
}

function tf_grompp () {
    sed -e "/^nsteps/s/=.*/= $tf_gmx_nsteps/g" grompp.mdp |\
    sed -e "/^nstenergy/s/=.*/= $tf_gmx_nstenergy/g" |\
    sed -e "/^nstxtcout/s/=.*/= $tf_gmx_nstxtcout/g" > tmp.mdp
    mv -f tmp.mdp grompp.mdp
}

function prod_grompp () {
    sed -e "/^nsteps/s/=.*/= $prod_gmx_nsteps/g" grompp.mdp |\
    sed -e "/^nstenergy/s/=.*/= $prod_gmx_nstenergy/g" |\
    sed -e "/^nstxtcout/s/=.*/= $prod_gmx_nstxtcout/g" > tmp.mdp
    mv -f tmp.mdp grompp.mdp
}

function ibi_setting () {
    sed -e "s/<iterations_max>.*<\/iterations_max>/<iterations_max>$old_ibi_iterations_max<\/iterations_max>/g" settings.xml > settings.xml.tmp
    mv -f settings.xml.tmp settings.xml
}

function tf_setting () {
    boxx=`tail conf.gro -n 1 | awk '{print $1}'`
    half_boxx=`echo "$boxx/2.0" | bc -l`
    tf_min=`echo "$ex_region_r - $tf_extension" | bc -l`
    tf_max=`echo "$ex_region_r + $hy_region_r + $tf_extension" | bc -l`
    tf_spline_start=`echo "$ex_region_r - $tf_spline_extension" | bc -l`
    tf_spline_end=`  echo "$ex_region_r + $hy_region_r + $tf_spline_extension" | bc -l`
    half_boxx_1=`echo "$half_boxx + 1." | bc -l`
    sed -e "s/<min>.*<\/min>/<min>$tf_min<\/min>/g" settings.xml |\
    sed -e "s/<max>.*<\/max>/<max>$tf_max<\/max>/g" |\
    sed -e "s/<step>.*<\/step>/<step>$tf_step<\/step>/g" |\
    sed -e "s/<spline_start>.*<\/spline_start>/<spline_start>$tf_spline_start<\/spline_start>/g" |\
    sed -e "s/<spline_end>.*<\/spline_end>/<spline_end>$tf_spline_end<\/spline_end>/g" |\
    sed -e "s/<spline_step>.*<\/spline_step>/<spline_step>$tf_spline_step<\/spline_step>/g" |\
    sed -e "s/<table_end>.*<\/table_end>/<table_end>$half_boxx_1<\/table_end>/g" |\
    sed -e "s/<prefactor>.*<\/prefactor>/<prefactor>$tf_prefactor<\/prefactor>/g" |\
    sed -e "s/<iterations_max>.*<\/iterations_max>/<iterations_max>$old_tf_iterations_max<\/iterations_max>/g" > settings.xml.tmp
    mv -f settings.xml.tmp settings.xml
}

function clean_ibi () {
    for dirName in `ls | grep step_`;
    do
	echo "# clean $dirName"
	cd $dirName
	if test -f traj.xtc; then
	    echo 2 | g_density -b 10 -d X -xvg none -nice 0 &> g_density.log
	    rm -f traj.xtc
	fi
	cd ..
    done
}

function clean_tf () {
    total_num_step=`ls | grep step_ | wc -l`
    count=1
    for dirName in `ls | grep step_`;
    do
	if test $count -eq $total_num_step; then
	    break
	fi
	echo "# clean $dirName"
	cd $dirName
	if test -f traj.xtc; then
	    echo 2 | g_density -b 10 -d X -xvg none -nice 0 &> g_density.log
	    rm -f traj.xtc
	fi
	cd ..
	count=$(($count+1))
    done
    echo "# clean $dirName"
    cd $dirName
    if test -f traj.xtc; then
	echo 2 | g_density -b 10 -d X -xvg none -nice 0 &> g_density.log
	echo 2 2 | g_rdf -b 10 -rdf mol_com -xvg none -nice 0 -bin 0.01 -dt 1 &> g_rdf.log
    fi
    cd ..
}

