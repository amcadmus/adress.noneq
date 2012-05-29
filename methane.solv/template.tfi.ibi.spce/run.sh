#!/bin/bash

source env.sh
source parameters.sh
source functions.sh
base_dir=`pwd`

echo running on `uname -n`
echo with gromacs command `which mdrun`

if test -f done; then
    echo "all work is done, do nothing."
    exit
fi

echo "# prepare working files"
if test ! -f step.000.tf/tf/done; then
    echo "should finish initilize tf step first! do nothing"
    exit
fi
cd step.000.tf/tf
init_laststep=`ls | grep step_ | tail -n 1`
init_lastdir=step.000.tf/tf/$init_laststep
cd ../..

echo "## enter template.ibi"
cd template.ibi
rm -f env.sh
cp ../env.sh .
cp ../$init_lastdir/confout.gro ./conf.gro
sed -e 's/COM/ CG/g' conf.gro > tmp.gro
mv -f tmp.gro conf.gro
rm -f grompp.mdp
cp ../$init_lastdir/grompp.mdp .
ibi_grompp
rm -f index.ndx
cp ../$init_lastdir/index.ndx .
rm -f tabletf.xvg SOL.pot.new
cp ../$init_lastdir/tabletf.xvg .
cp ../$init_lastdir/SOL.pot.new .
rm -f topol.top
cp ../$init_lastdir/topol.top .
sed -e 's/COM/CG/g' topol.top > tmp.top
mv -f tmp.top topol.top
rm -f table_CG_CG.xvg rdf.com.xvg
cp $base_dir/$cg_pot_file ./table_CG_CG.xvg
cp $base_dir/$tgt_rdf_file ./rdf.com.xvg
cd ..

echo "## enter template.tf"
cd template.tf
rm -f env.sh
cp ../env.sh .
rm -f conf.gro dens.SOL.xvg index.ndx topol.top grompp.mdp
cp ../$init_lastdir/confout.gro ./conf.gro
cp ../$init_lastdir/grompp.mdp .
tf_grompp
cp ../step.000.tf/tf/dens.SOL.xvg .
cp ../$init_lastdir/index.ndx .
cp ../$init_lastdir/topol.top .
rm -f table_CG_CG.xvg
cp $base_dir/$cg_pot_file ./table_CG_CG.xvg
cd ..

old_ibi_iterations_max=$ibi_iterations_max
old_tf_iterations_max=$tf_iterations_max
ibi_iterations_max=`printf "%03d" $ibi_iterations_max`
tf_iterations_max=`printf "%03d" $tf_iterations_max`
ibi_resultDir=step_$ibi_iterations_max
tf_resultDir=step_$tf_iterations_max

for i in `seq 1 $rdf_iterations`
do
    thisNum=`printf "%03d" $i`
    ibiDir=step.${thisNum}.ibi
    tfDir=step.${thisNum}.tf
    if test -d $ibiDir; then
	echo "# dir $ibiDir exists, quit!"
	mv -f $ibiDir $ibiDir.`date +%s`
    fi
    if test -d $tfDir; then
	echo "# dir $tfDir exists, quit!"
	mv -f $tfDir $tfDir.`date +%s`
    fi

    cp -a template.ibi $ibiDir
    echo "# rdf iter: doing $ibiDir"
    cd $ibiDir
    ibi_setting
    if test $i -eq 1; then
	./run.sh
    else
	rm -f tabletf.xvg tablerdf.xvg
	cp ../$last_tfDir/tabletf.xvg .
#	cp ../$last_ibiDir/$ibi_resultDir/tablerdf.xvg .
	cp ../$last_ibiDir/CG-CG.pot.new ./CG-CG.pot.in
	cp ../$last_ibiDir/confout.gro ./conf.gro
	./run.sh
    fi
    ibi_last_step=`ls | grep step_ | tail -n 1`
    cp $ibi_last_step/CG-CG.pot.new .
    cp $ibi_last_step/confout.gro .
    csg_call --ia-type C12 --options settings.xml convert_potential gromacs CG-CG.pot.new tablerdf.xvg &>> inverse.log
    clean_ibi
    cd ..

    cp -a template.tf $tfDir
    echo "# tf iter: doing $tfDir"
    cd $tfDir
    tf_setting
    rm -f tabletf.xvg tablerdf.xvg
    cp ../$ibiDir/tablerdf.xvg .
    if test $i -eq 1; then
	cp ../$ibiDir/SOL.pot.new ./SOL.pot.in
	./run.sh
    else
#	cp ../$last_tfDir/$tf_resultDir/tabletf.xvg .
	cp ../$last_tfDir/SOL.pot.new ./SOL.pot.in
	cp ../$last_tfDir/confout.gro ./conf.gro
	./run.sh
    fi
    tf_last_step=`ls | grep step_ | tail -n 1`
    cp $tf_last_step/SOL.pot.new .
    cp $tf_last_step/confout.gro .
    csg_call --ia-type bonded --options settings.xml convert_potential gromacs SOL.pot.new tabletf.xvg &>> inverse.log
    clean_tf
    cd ..

    last_tfDir=$tfDir
    last_ibiDir=$ibiDir
done

touch done
