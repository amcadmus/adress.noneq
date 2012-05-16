gro_dir=gromacs.gromos43a1.spce

# equilibrium run settings
equi_warm_time=200		# ps
equi_frame_feq=10		# ps
equi_num_frame=100		# n
equi_dt=0.002			# ps
equi_taut=0.5			# ps
equi_seed=`date +%s`		# 

# non-equilibrium settings
perturbation_command="tools/center/gauss.v"
pert_conf_dir="result.equi/equiConfs/"
pert_time=5			# ps
pert_frame_feq=0.1		# ps
pert_dt=0.002			# ps
pert_taut=1			# ps

