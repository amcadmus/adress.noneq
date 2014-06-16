#!/bin/bash

input_conf=confs/conf.011.gro

gmx_dt=0.001
gmx_nsteps=100000
gmx_nstenergy=100
gmx_nstxtcout=100
gmx_tau_t=0.1
gmx_epsilon_rf=60
gmx_ex_region_r=1.5
gmx_hy_region_r=1.5
gmx_fix_ndx=33

tf_extension=0.05
tf_step=0.01
tf_spline_extension=0.1
tf_spline_step=0.34
tf_iterations_max=20
tf_equi_time_discard=20
tf_init_guess_CMW=./tabletf_CMW.xvg
tf_SOL_prefactor=0.04
tf_K_value=100
tf_CL_value=100

SOL_poten_eps=0.6
SOL_poten_sigma=0.31
K_poten_eps=0.014
K_poten_sigma=0.49
CL_poten_eps=0.49
CL_poten_sigma=0.44


