#!/bin/bash

source parameters.sh
source functions.sh

if test -d result.equi; then
    echo "existing dir result.equi, mv and backup"
    mv result.equi result.equi.`date +%s`
fi
cp -a $gro_dir result.equi

cd result.equi
set_parameters_equi grompp.mdp

echo "# run with command `which grompp`"
$grompp_command
echo "# run with command `which mdrun`"
$mdrun_command

split_trr
cd ..
