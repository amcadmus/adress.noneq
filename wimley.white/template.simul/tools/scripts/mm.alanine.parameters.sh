
redo=1
nbins=20
lag_times="`seq 1 1 20` `seq 22 2 40`"
lag_times="`seq 1 1 20`"
#lag_times="`seq 1 1 3`"
selected_lag_time=20
num_eig=5
err_num_samples=8

targets=`ls | grep ^simul | grep [0-9]$`
traj_dir=gromacs.traj.npt
phi_file_name=angaver.phi.xvg
psi_file_name=angaver.psi.xvg

