#!/bin/bash

# simulation result from spme.tune
# nvt_num_density=32.90917546138462644658

gmx_init_conf=./confs/conf.alanine.amber99sb-ildn.gro
gmx_init_index=
gmx_sys_tempalte_dir=./tools/system.alanine.amber99sb-ildn
gmx_dt=0.001
gmx_time=10			# in ps
gmx_equi_skip=100		# in ps
gmx_conf_feq=1			# in ps
gmx_energy_feq=1		# in ps
gmx_ele_method=pme-switch	# pme or zm or rf
gmx_rlist=1.45			# 
gmx_nstlist=10			# 
gmx_rcut_ele=1.2		# if pme is used, set this value to rlist
gmx_rcut_ele_switch=1.15
gmx_pme_F_spacing=0.12
gmx_pme_order=4
gmx_vdw_type=switch		# using adress will overwrite it with "user"
gmx_rcut_vdw=1.2
gmx_rcut_vdw_switch=1.15
gmx_tab_ext=0.5
gmx_thermostat=nose-hoover	# hose-hoover or sd
gmx_taut=1.0
gmx_npt=yes
gmx_taup=2.0
gmx_e_rf=80.0

zm_l=2
zm_alpha=0.1

adress_ex_region=1.5
adress_hy_region=1.5
adress_type=sphere

