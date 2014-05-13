#!/bin/bash

if [ $# -ne 1 ] ; then
    echo useage:
    echo auto.allegro.ti.sh TARGET
    exit
fi
target=$1

source env.sh

cwd=`pwd`
tmpname=$cwd/$target
jobname=${tmpname##*/}

rm -f allegro.$jobname.out

echo "#!/bin/bash"				>  submit.$jobname.sh
echo "#PBS -N $jobname"				>> submit.$jobname.sh
echo "#PBS -o allegro.$jobname.out"		>> submit.$jobname.sh
echo "#PBS -e allegro.$jobname.out"		>> submit.$jobname.sh
echo "#PBS -M han.wang@fu-berlin.de"		>> submit.$jobname.sh
echo "#PBS -l walltime=24:00:00"		>> submit.$jobname.sh
echo "#PBS -l nodes=1:ppn=$nnodes"		>> submit.$jobname.sh
echo "#PBS -l pmem=1000mb"			>> submit.$jobname.sh

echo "cd $cwd"					>> submit.$jobname.sh
echo "hostname"					>> submit.$jobname.sh
echo "date"					>> submit.$jobname.sh
echo "pwd"					>> submit.$jobname.sh
#echo "./assign.param.sh"			>> submit.$jobname.sh
echo "./run.ti.sh $target"			>> submit.$jobname.sh

qsub submit.$jobname.sh &> jobid

