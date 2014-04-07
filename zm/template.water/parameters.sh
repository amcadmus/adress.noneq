#!/bin/bash

# cal volumn
gmx_init_conf=./tools/confs/conf.013k.gro
gmx_sys_tempalte_dir=./tools/system
gmx_dt=0.001
gmx_time=10000			# in ps
gmx_equi_skip=1000		# in ps
gmx_conf_feq=20			# in ps
gmx_energy_feq=1		# in ps
gmx_ele_method=pme		# pme or zm
gmx_rlist=1.5			# 
gmx_nstlist=5			# if pme is used, set this value, otherwise, set -1
gmx_rcut_ele=1.2		# if pme is used, set this value to rlist
gmx_rcut_ele_switch=1.15
gmx_rcut_vdw=1.2
gmx_rcut_vdw_switch=1.15
gmx_tab_ext=0.5
gmx_taut=0.1
gmx_taup=0.5

zm_l=2
zm_alpha=0.1

gmx_grompp_command="grompp"
gmx_mdrun_command="mdrun -nt $nnodes"
