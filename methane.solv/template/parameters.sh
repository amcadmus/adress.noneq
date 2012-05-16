gro_dir=gromacs.gromos43a1.spce

# equilibrium run settings
equi_warm_time=200		# ps
equi_frame_feq=5		# ps
equi_num_frame=100		# n
equi_dt=0.002			# ps
equi_taut=0.5			# ps
equi_seed=`date +%s`		# 

# non-equilibrium settings
pert_conf_dir="result.equi/equiConfs/"
pert_strength=2			# nm/ps velocity
pert_rcut0=0.8			# nm
pert_rcut1=1.0			# nm
perturbation_command="tools/perturbations/center.step --strength $pert_strength --rcut0 $pert_rcut0 --rcut1 $pert_rcut1"
pert_time=5			# ps
pert_frame_feq=0.1		# ps
pert_dt=0.002			# ps
pert_taut=1			# ps
