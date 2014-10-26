#!/bin/bash

# simulation result from spme.tune
# nvt_num_density=32.90917546138462644658

gmx_init_conf=./confs/conf.wwa.oplsaa.box004.gro
gmx_init_index=./confs/index.wwa.oplsaa.box004.ndx
gmx_sys_tempalte_dir=./tools/system.wwa.oplsaa
gmx_dt=0.002
gmx_time=1000000		# in ps
gmx_equi_skip=100		# in ps
gmx_conf_feq=1			# in ps
gmx_energy_feq=1		# in ps
gmx_ele_method=pme-switch	# pme or zm or rf
gmx_rlist=1.50			# 
gmx_nstlist=5			# 
gmx_rcut_ele=1.2		# if pme is used, set this value to rlist
gmx_rcut_ele_switch=1.15
gmx_pme_F_spacing=0.06
gmx_pme_order=6
gmx_vdw_type=shift		# using adress will overwrite it with "user"
gmx_rcut_vdw=1.2
gmx_rcut_vdw_switch=1.15
gmx_tab_ext=0.5
gmx_integrator=md		# md or md-vv
gmx_thermostat=nose-hoover	# nose-hoover, sd or sd1
gmx_taut=1.0
gmx_npt=yes
gmx_taup=2.0
gmx_e_rf=80.0
gmx_local_sd_range=0.0

zm_l=3
zm_alpha=0.0

adress_ex_region=1.5
adress_hy_region=1.5
adress_type=sphere
adress_tf_file=tools/adress.tf/tabletf_CMW.charmm27.ex0.6.hy1.0.xvg
