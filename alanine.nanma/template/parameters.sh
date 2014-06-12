# method of run:
run_method=atom.inhomo.sd
gro_dir=gromacs.conf.box.002.70
nthreads=24

# # productive equilibrium run settings
# long_equi_warm_time=200		# ps
# long_equi_frame_feq=5		# ps
# long_equi_num_frame=100		# n
# long_equi_dt=0.002		# ps
# long_equi_taut=0.5		# ps
# long_equi_seed=`date +%s`	# 

# equilibrium run settings
equi_warm_time=200		# ps
equi_frame_feq=2		# ps
equi_num_frame=10000		# n
equi_dt=0.002			# ps
equi_taut=0.5			# ps
equi_seed=`date +%s`		# 

# non-equilibrium settings
pert_equi_result="$HOME/group_storage/run.result.equi.box02.70.more/result.equi/"
pert_num_conf_use=100000	#
pert_mode=2			# 1: relax, 2: cos wave
pert_strength=1.0		# nm/ps velocity
pert_warm_time=40		# ps 1: warm time, 2: periodicity
pert_shift=0			# unitless
pert_phi=270			# deg. shift of the phase
pert_time=1000			# ps
pert_frame_feq=0.5		# ps
pert_dt=0.002			# ps
pert_taut=0.1			# ps
pert_noSdRange=1.0		# nm
pert_barostat=Parrinello-Rahman # Parrinello-Rahman or no
pert_taup=2.0			# ps
pert_parallel_num_pro=1		# n
pert_parallel_my_id=0		# n

grompp_command="grompp -n index.ndx"
mdrun_command="mdrun -nt $nthreads"

if	echo $run_method | grep adress &> /dev/null; then
    pert_integrator=sd
    gro_dir=$gro_dir.adress
    grompp_command="grompp -n "
    gromacs_install_dir=~/study/thermo.convection/local.inhomo.thermostat.w.rdfCorr
    source $gromacs_install_dir/bin/GMXRC.bash
else if echo $run_method | grep "atom.inhomo.sd" | grep -v sd2 &> /dev/null; then
    pert_integrator=sd1
    gromacs_install_dir=~/study/adress.noneq/methane.solv/local.inhomo.sd
    source $gromacs_install_dir/bin/GMXRC.bash
else if echo $run_method | grep "atom.inhomo.sd2" &> /dev/null; then
    pert_integrator=sd
    gromacs_install_dir=~/study/adress.noneq/methane.solv/local.inhomo.sd2
    source $gromacs_install_dir/bin/GMXRC.bash
else if echo $run_method | grep "atom.langevin" &> /dev/null; then
    pert_integrator=sd1
    gromacs_install_dir=~/study/adress.noneq/alanine.nanma/gromacses/local.gromacs.rev.1
    source $gromacs_install_dir/bin/GMXRC.bash
else
    pert_integrator=md
    pert_barostat=no		# NVE run, force to no.
    gromacs_install_dir=~/study/adress.noneq/methane.solv/local.inhomo.sd
    source $gromacs_install_dir/bin/GMXRC.bash
fi
fi
fi
fi

