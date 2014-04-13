#!/bin/bash

# simulation result from spme.tune
nvt_num_density=32.90917546138462644658

gmx_init_conf=./tools/confs/conf.013k.gro
gmx_sys_tempalte_dir=./tools/system
gmx_dt=0.001
gmx_time=10000			# in ps
gmx_equi_skip=1000		# in ps
gmx_conf_feq=20			# in ps
gmx_energy_feq=1		# in ps
gmx_ele_method=pme-switch		# pme or zm or rf
gmx_rlist=1.5			# 
gmx_nstlist=5			# 
gmx_rcut_ele=1.2		# if pme is used, set this value to rlist
gmx_rcut_ele_switch=1.15
gmx_pme_F_spacing=0.12
gmx_pme_order=4
gmx_rcut_vdw=1.2
gmx_rcut_vdw_switch=1.15
gmx_tab_ext=0.5
gmx_taut=0.1
gmx_taup=0.5
gmx_e_rf=60.0

zm_l=2
zm_alpha=0.1

