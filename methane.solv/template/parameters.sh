# method of run:
run_method=adress
gro_dir=gromacs.gromos43a1.spce.large

# productive equilibrium run settings
long_equi_warm_time=200		# ps
long_equi_frame_feq=5		# ps
long_equi_num_frame=100		# n
long_equi_dt=0.002		# ps
long_equi_taut=0.5		# ps
long_equi_seed=`date +%s`	# 

# equilibrium run settings
equi_warm_time=200		# ps
equi_frame_feq=5		# ps
equi_num_frame=100		# n
equi_dt=0.002			# ps
equi_taut=0.5			# ps
equi_seed=`date +%s`		# 

# non-equilibrium settings
pert_conf_dir="result.equi/equiConfs/"
pert_num_conf_use=100		# 
pert_strength=2			# nm/ps velocity
pert_rcut0=0.8			# nm
pert_rcut1=1.0			# nm
perturbation_command="tools/perturbations/center.step --strength $pert_strength --rcut0 $pert_rcut0 --rcut1 $pert_rcut1"
pert_time=3			# ps
pert_frame_feq=0.01		# ps
pert_dt=0.002			# ps
pert_taut=1			# ps

grompp_command="grompp"
mdrun_command="mdrun -v"

if echo $run_method | grep adress &> /dev/null; then
    gro_dir=$gro_dir.adress
    grompp_command="grompp -n "
    gromacs_install_dir=~/study/thermo.convection/local.inhomo.thermostat.w.rdfCorr
    source $gromacs_install_dir/bin/GMXRC.bash
fi

