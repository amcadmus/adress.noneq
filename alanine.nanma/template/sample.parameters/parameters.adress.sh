# gromacs seed dir
gro_dir=gromacs.conf.box.006.00.adress

# equilibrium run settings
equi_warm_time=200		# ps
equi_frame_feq=2		# ps
equi_num_frame=10000		# n
equi_dt=0.002			# ps
equi_taut=0.5			# ps
equi_seed=`date +%s`		# 

# non-equilibrium settings
## non-equilibrium control parameters
pert_equi_result="$HOME/group_storage/run.result.equi.box06.00.adress/gromacs.traj.nvt/"
pert_num_conf_use=1		#
pert_mode=2			# 1: relax, 2: cos wave
pert_strength=1.0		# nm/ps velocity
pert_warm_time=40		# ps 1: warm time, 2: periodicity
pert_shift=0			# unitless
pert_phi=270			# deg. shift of the phase
pert_time=1000			# ps
pert_frame_feq=0.5		# ps

## electrostatic and vdw method
pert_ele_method=rf		# pme or zm or rf
pert_rlist=1.0			# 
pert_nstlist=5			# 
pert_rcut_ele=1.0		# if pme is used, set this value to rlist
pert_rcut_ele_switch=0.95
pert_pme_F_spacing=0.12
pert_pme_order=4
pert_vdw_type=shift		# using adress will overwrite it with "user"
pert_rcut_vdw=1.0
pert_rcut_vdw_switch=0.95
pert_tab_ext=0.5
pert_e_rf=80.0
pert_zm_l=3
pert_zm_alpha=0.0

## timestep and ensemble control
pert_dt=0.002			# ps
pert_integrator=sd1		# now should be sd1
pert_local_sd_range=0.60
pert_taut=0.1			# ps
pert_barostat=no		# Parrinello-Rahman or no
pert_taup=2.0			# ps

## parallel simultion control
pert_parallel_num_pro=1		# n
pert_parallel_my_id=0		# n

## adress stuff
pert_adress=yes
pert_adress_ex_region=0.60
pert_adress_hy_region=1.01
pert_adress_type=sphere
pert_adress_tf_file=tools/adress.tf/tabletf_CMW.charmm27.rf.ex0.6.hy1.0.xvg
