nnodes=6

gmx_dir=$HOME/local/
source $gmx_dir/bin/GMXRC

zm_gen_dir=$HOME/study/adress.noneq.bk/zm/tools/gen.pot

gmx_grompp_command="grompp"
gmx_tune_command="g_pme_error"
gmx_mdrun_command="mdrun -nt $nnodes -notunepme -v"

