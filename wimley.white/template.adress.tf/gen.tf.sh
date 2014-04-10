#!/bin/bash

source env.sh
source parameters.sh

mylog=`pwd`/gen.tf.log
makelog=`pwd`/make.log
rm -f $mylog
make -C ./tools/gen.pot/ clean
make -C ./tools/gen.pot/ -j8 &> /dev/null
make -C ./tools/gen.conf/ clean
make -C ./tools/gen.conf/ -j8 &> /dev/null
make -C ./tools/parse.top/ clean
make -C ./tools/parse.top/ -j8 &> /dev/null

# prepare conf.gro
echo "# prepare conf.gro"
rm -f conf.gro
if test ! -f $input_conf; then
    echo "cannot find file $input_conf, exit"
    exit
fi
cp $input_conf ./conf.gro
boxx=`tail conf.gro -n 1 | awk '{print $1}'`
boxy=`tail conf.gro -n 1 | awk '{print $2}'`
boxz=`tail conf.gro -n 1 | awk '{print $3}'`
half_boxx=`echo "$boxx/2.0" | bc -l`
half_boxy=`echo "$boxy/2.0" | bc -l`
half_boxz=`echo "$boxz/2.0" | bc -l`

# prepare potentials
echo "# prepare potentials"
cd ./tools/gen.pot
./gen.wca --sigma $SOL_poten_sigma --epsilon $SOL_poten_eps -o table_CMW_CMW.xvg
./gen.wca --sigma $K_poten_sigma --epsilon $K_poten_eps -o table_CMK_CMK.xvg
./gen.wca --sigma $CL_poten_sigma --epsilon $CL_poten_eps -o table_CML_CML.xvg
tmp_sigma=`echo "0.5 * ($SOL_poten_sigma + $K_poten_sigma)" | bc -l`
tmp_eps=`echo "sqrt($SOL_poten_eps * $K_poten_eps)" | bc -l`
./gen.wca --sigma $tmp_sigma --epsilon $tmp_eps -o table_CMW_CMK.xvg
tmp_sigma=`echo "0.5 * ($SOL_poten_sigma + $CL_poten_sigma)" | bc -l`
tmp_eps=`echo "sqrt($SOL_poten_eps * $CL_poten_eps)" | bc -l`
./gen.wca --sigma $tmp_sigma --epsilon $tmp_eps -o table_CMW_CML.xvg
tmp_sigma=`echo "0.5 * ($K_poten_sigma + $CL_poten_sigma)" | bc -l`
tmp_eps=`echo "sqrt($K_poten_eps * $CL_poten_eps)" | bc -l`
./gen.wca --sigma $tmp_sigma --epsilon $tmp_eps -o table_CMK_CML.xvg

./gen.simp.tf --xex $gmx_ex_region_r --xhy $gmx_hy_region_r --xup $boxx --pot0 $tf_K_value --output tabletf_CMK.xvg
./gen.simp.tf --xex $gmx_ex_region_r --xhy $gmx_hy_region_r --xup $boxx --pot0 $tf_CL_value --output tabletf_CML.xvg
mv -f table_*_*.xvg tabletf_*.xvg ../../
cd ../..

# prepare dens.SOL.xvg
echo "# prepare dens.SOL.xvg"
rm -f dens.SOL.xvg
for i in `seq 0 0.05 $boxx`;
do
    echo "$i 0 0" >> dens.SOL.xvg
done

# copy dir
echo "# copy dir"
if test -d tf; then
    mv tf tf.`date +%s`
fi
cp -a tools/tf.template ./tf

# prepare grompp.mdp
echo "# prepare grompp.mdp"
rm -fr grompp.mdp
cp tf/grompp.mdp .
sed -e "/^adress_ex_width/s/=.*/= $gmx_ex_region_r/g" grompp.mdp |\
sed -e "/^adress_hy_width/s/=.*/= $gmx_hy_region_r/g" |\
sed -e "/^adress_type/s/=.*/= sphere/g" |\
sed -e "/^adress /s/=.*/= yes/g" |\
sed -e "/^dt/s/=.*/= $gmx_dt/g" |\
sed -e "/^tau_t/s/=.*/= $gmx_tau_t/g" |\
sed -e "/^nsteps/s/=.*/= $gmx_nsteps/g" |\
sed -e "/^nstenergy/s/=.*/= $gmx_nstenergy/g" |\
sed -e "/^nstcomm/s/=.*/= $gmx_nstenergy/g" |\
sed -e "/^nstxtcout/s/=.*/= $gmx_nstxtcout/g" |\
sed -e "/^epsilon_rf/s/=.*/= $gmx_epsilon_rf/g" |\
sed -e "/^adress_reference_coords/s/=.*/= $half_boxx $half_boxy $half_boxz/g" > grompp.mdp.tmp
mv -f grompp.mdp.tmp grompp.mdp

# prepare settings.xml
echo "# prepare settings.xml"
rm -fr settings.xml
cp tf/settings.xml .
tf_min=`echo "$gmx_ex_region_r - $tf_extension" | bc -l`
tf_max=`echo "$gmx_ex_region_r + $gmx_hy_region_r + $tf_extension" | bc -l`
tf_spline_start=`echo "$gmx_ex_region_r - $tf_spline_extension" | bc -l`
tf_spline_end=`  echo "$gmx_ex_region_r + $gmx_hy_region_r + $tf_spline_extension" | bc -l`
half_boxx_1=`echo "$half_boxx + 1." | bc -l`
prefactor_l2=`grep -n prefactor settings.xml | tail -n 1 | cut -f 1 -d ":"`
sed -e "s/<min>.*<\/min>/<min>$tf_min<\/min>/g" settings.xml |\
sed -e "s/<max>.*<\/max>/<max>$tf_max<\/max>/g" |\
sed -e "s/<step>.*<\/step>/<step>$tf_step<\/step>/g" |\
sed -e "s/<spline_start>.*<\/spline_start>/<spline_start>$tf_spline_start<\/spline_start>/g" |\
sed -e "s/<spline_end>.*<\/spline_end>/<spline_end>$tf_spline_end<\/spline_end>/g" |\
sed -e "s/<spline_step>.*<\/spline_step>/<spline_step>$tf_spline_step<\/spline_step>/g" |\
sed -e "s/<table_end>.*<\/table_end>/<table_end>$half_boxx_1<\/table_end>/g" |\
sed -e "${prefactor_l2}s/<prefactor>.*<\/prefactor>/<prefactor>$tf_SOL_prefactor<\/prefactor>/g" |\
sed -e "s/<equi_time>.*<\/equi_time>/<equi_time>$tf_equi_time_discard<\/equi_time>/g" |\
sed -e "s/<iterations_max>.*<\/iterations_max>/<iterations_max>$tf_iterations_max<\/iterations_max>/g" > settings.xml.tmp
mv -f settings.xml.tmp settings.xml

# prepare topol.top
echo "# prepare topol.top"
nSOL=`./tools/gen.conf/nresd -f conf.gro | grep SOL | awk '{print $2}'`
rm -fr topol.top 
cp tf/topol.top .
nline_top=`wc topol.top | awk '{print $1}'`
nline_4_top=`echo $nline_top -4 | bc`
head -n $nline_4_top topol.top > new.top
tail -n 4 topol.top | sed "s/^SOL.*/SOL $nSOL/g" >> new.top
mv -f new.top topol.top

# prepare initial guess
echo "# prepare initial guess"
if test -f $tf_init_guess_CMW; then
    cp $tf_init_guess_CMW ./tf/SOL.pot.in
fi

# copy all file to tf
echo "# copy files to tf"
mv -f conf.gro dens.SOL.xvg grompp.mdp settings.xml topol.top table_*_*.xvg tabletf_*.xvg tf/

# prepare index file
cd tf/
echo 'q' | make_ndx -f conf.gro
echo "[ FIX ]" >> index.ndx
echo "$gmx_fix_ndx" >> index.ndx
../tools/parse.top/make.idx --cg-key CM --ex-key EXW -o add.ndx
cat add.ndx >> index.ndx
rm -f add.ndx
cd ..

# calculate tf
echo "# calculate tf"
cd tf
sync
csg_inverse --options settings.xml
cd ..

# rm trajs
echo "# rm trajs"
rm -f tf/step_*/traj.xtc


