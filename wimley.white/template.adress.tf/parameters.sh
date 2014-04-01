#!/bin/bash

start_conf=confs/conf.011.gro

gmx_dt=0.002
gmx_nsteps=50000
gmx_nstenergy=50
gmx_nstxtcout=50
gmx_tau_t=0.1
gmx_epsilon_rf=60
gmx_ex_region_r=1.5
gmx_hy_region_r=1.5

tf_extension=0.05
tf_step=0.01
tf_spline_extension=0.1
tf_spline_step=0.34
tf_iterations_max=20
tf_equi_time_discard=20
tf_init_guess_CMW=./tabletf_CMW.xvg
tf_K_value=500
tf_CL_value=500

SOL_tf_prefactor=0.04
SOL_poten_eps=0.6
SOL_poten_sigma=0.3
K_poten_eps=0.6
CL_poten_eps=0.6


