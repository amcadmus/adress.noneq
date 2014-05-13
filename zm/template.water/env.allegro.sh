nnodes=8

gmx_dir=$HOME/local/gromacs/4.6.5/
source $gmx_dir/bin/GMXRC

zm_gen_dir=$HOME/NO_BACKUP/zm/tools/gen.pot

gmx_grompp_command="grompp"
gmx_tune_command="g_pme_error"
gmx_mdrun_command="mdrun -nt $nnodes -notunepme"
