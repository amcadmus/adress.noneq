
gmx_dir=$WORK/local/gromacs/4.6.5.localsd.fcorr
source $gmx_dir/bin/GMXRC.bash

zm_gen_dir=$WORK/study/adress.noneq/zm/tools/gen.pot

gmx_grompp_command="grompp"
gmx_tune_command="aprun -B g_pme_error"
gmx_mdrun_command="aprun -B mdrun -notunepme"

