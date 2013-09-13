#!/bin/sh
##PBS -o run.log
#PBS -q qwork@mp2
#PBS -l walltime=120:00:00,nodes=1:ppn=1,mem=10gb
#PBS -m n
#PBS -V
#PBS -N xa_xa
#PBS -e xa_xa_e.out
#PBS -o xa_xa_o.out

## The simulations were carried out on 
## These modules were loaded to run gromacs
#module load openmpi_intel64/1.4.3 openmpi_intel64/1.4.3_ofed  lapack64/3.1.1  mkl64/10.1.3.027 

## This is my gromacs-4.5.5 installation compiled with above mentioned module
## I used single precision compilation
## The hardware on which these simulation were performed https://rqchp.ca/?mod=cms&pageId=566&lang=FR&navrev=off&version=-1&
# source $HOME/sft/gmx455/bin/GMXRC
# RUNDIR=$PBS_O_WORKDIR
# cd $RUNDIR
GROMPP=`which grompp`
TPBCONV=`which tpbconv`
MDRUN=`which mdrun`

echo "USING $MDRUN On $HOSTNAME" 
./min.sh xa MIN0 $GROMPP $MDRUN 
./gmx_equi.sh 300 xa $GROMPP $MDRUN $TPBCONV
./gmx_sim.sh 300 1 6 xa $GROMPP $TPBCONV $MDRUN
