#!/bin/bash

input_conf=confs/conf.amber99sb.gro

gmx_dt=0.002
gmx_nsteps=100000
gmx_nstenergy=100
gmx_nstxtcout=100
gmx_tau_t=0.1
gmx_epsilon_rf=80
gmx_ex_region_r=1.00
gmx_hy_region_r=1.01
gmx_fix_ndx=9

tf_template=tools/tf.template.amber99sb
tf_extension=0.05
tf_step=0.01
tf_spline_extension=0.1
tf_spline_step=0.30250
tf_iterations_max=20
tf_equi_time_discard=20
tf_init_guess_CMW=./SOL.pot.new
tf_SOL_prefactor=0.04

SOL_poten_eps=0.6
SOL_poten_sigma=0.31

