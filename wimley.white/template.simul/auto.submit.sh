#!/bin/bash

new_submit_command=./auto.hlrn.run.de
con_submit_command=./auto.hlrn.con.de
siml_dir=gromacs.traj.nvt

while [ 1 -eq 1 ];
do
    if test ! -f jobid; then
	echo "new submission"
	$new_submit_command
    else
	checkjob `cat jobid ` &> checkjob.out
	if [ $? -eq 1 ]; then
	    echo "failed at checkjob, no job id, something goes wrong, quit"
	    exit
	fi
	status=`grep State checkjob.out | head -n 1`
	# Idle Running Completed
	if echo $status | grep Completed &> /dev/null; then
	    echo "current job completed"
	    if test -f $siml_dir/confout.gro; then
		echo "finished simulation, exit!"
		exit
	    else
		echo "continue simulation, after sleeping 2s"
		if test ! -d $siml_dir ; then
		    echo "no simulation dir $siml_dir, something goes wrong, quit"
		    exit
		fi
		sleep 2
		cd $siml_dir/
		if test ! -f state.cpt; then
		    echo "no state.cpt, no way to continue, exit"
		    exit
		fi
		ln -sf ../$con_submit_command .
		$con_submit_command
		mv -f jobid ..
		cd ..
	    fi
	else
	    mydate=`date`
	    echo "checked at $mydate, job is either Idel or Running, should wait, sleep 240s"
	    sleep 240
	fi
    fi
    njobs=`qstat  | grep wangh | wc -l | awk '{print $1}'`
    if [ $njobs -gt 10 ]; then
	echo "$njobs, more than 10, something may be wrong, exit"
	exit
    fi
done
