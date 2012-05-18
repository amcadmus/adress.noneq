#!/bin/bash

source parameters.sh
source functions.sh

if test -d equi.long; then
    echo "existing dir equi.long, mv and backup"
    mv equi.long equi.long.`date +%s`
fi
cp -a $gro_dir equi.long

cd equi.long
set_parameters_long_equi grompp.mdp

grompp
mdrun -v

cd ..