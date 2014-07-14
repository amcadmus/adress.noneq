#!/bin/bash

gmx_dir=$WORK/local/gromacs/4.6.5.localsd.fcorr
source $gmx_dir/bin/GMXRC.bash

zm_gen_dir=$WORK/study/adress.noneq/zm/tools/gen.pot

pert_grompp_command="grompp"
pert_tune_command="aprun -B g_pme_error"
pert_mdrun_command="aprun -B mdrun -notunepme"
