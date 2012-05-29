#!/bin/bash

base_conf=tools/gen.conf/spc216.gro
n_base_block="4 2 2"
number_density=33.286      # 0.99577 g/cm^3
cg_pot_file=tools/table_CG_CG.xvg
tgt_rdf_file=tools/rdf.com.1e-2.xvg

# rdf interation
rdf_iterations=50

# ibi parameters
ibi_iterations_max=1

ibi_gmx_nsteps=500000
ibi_gmx_nstenergy=500
ibi_gmx_nstxtcout=500

# tf parameters
tf_iterations_max=1
tf_iterations_init=10

tf_gmx_nsteps=500000
tf_gmx_nstenergy=500
tf_gmx_nstxtcout=500

ex_region_r=0.5
hy_region_r=2.75
tf_extension=0.05
tf_step=0.01
tf_spline_extension=0.1
tf_spline_step=0.42142857142857142857 # 2.95/7
tf_prefactor=0.039


# productive run.
prod_gmx_nsteps=500000
prod_gmx_nstenergy=500
prod_gmx_nstxtcout=500

last_step_num=10
