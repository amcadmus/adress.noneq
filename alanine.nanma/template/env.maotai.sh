#!/bin/bash

gmx_dir=$HOME/local/gromacs/4.6.5.localsd.fcorr
source $gmx_dir/bin/GMXRC.bash

zm_gen_dir=$WORK/study/adress.noneq/zm/tools/gen.pot

pert_grompp_command="grompp"
pert_tune_command="g_pme_error"
pert_mdrun_command="mdrun -notunepme"
