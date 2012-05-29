#!/bin/bash

base_conf=tools/gen.conf/spc216.gro
n_base_block="4 4 4"
number_density=33.286      # 0.99577 g/cm^3
cg_pot_file=tools/table_CG_CG.xvg
tgt_rdf_file=tools/rdf.com.1e-2.xvg

# rdf interation
rdf_iterations=20

# ibi parameters
ibi_iterations_max=50

ibi_gmx_nsteps=50000
ibi_gmx_nstenergy=50
ibi_gmx_nstxtcout=50

# tf parameters
tf_iterations_max=2
tf_iterations_init=10

tf_gmx_nsteps=500000
tf_gmx_nstenergy=200
tf_gmx_nstxtcout=200

ex_region_r=0.54
hy_region_r=2.70
tf_extension=0.05
tf_step=0.01
tf_spline_extension=0.1
tf_spline_step=0.41428571428571428571 # 2.9/7
tf_prefactor=0.039


# productive run.
prod_gmx_nsteps=500000
prod_gmx_nstenergy=500
prod_gmx_nstxtcout=500

last_step_num=10
