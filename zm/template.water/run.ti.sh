#!/bin/bash

if [ $# -ne 1 ] ; then
    echo useage:
    echo run.ti.sh TARGET
    exit
fi
target=$1
if test ! -d $target; then
    echo "No dir $target"
    exit
fi

cwd=`pwd`
source env.sh
source parameters.sh

if echo "$gmx_ele_method" | grep pme &> /dev/null ; then
    echo "# run with ele method pme"
    gmx_ele_method_gromacs=pme
    gmx_ele_method_ind=1
    if echo "$gmx_ele_method" | grep pme-switch &> /dev/null ; then
	echo "# run with ele method pme-switch"
	gmx_ele_method_gromacs=pme-switch
	gmx_ele_method_ind=11
    fi
else if echo "$gmx_ele_method" | grep zm &> /dev/null ; then
    echo "# run with ele method zm"
    gmx_ele_method_gromacs=user
    gmx_ele_method_ind=0
else if echo "$gmx_ele_method" | grep rf &> /dev/null ; then
    echo "# run with ele method rf"
    gmx_ele_method_gromacs=reaction-field
    gmx_ele_method_ind=2
fi
fi
fi

echo "# doing in $target"
echo "# call grompp"
cd $target
$gmx_grompp_command > /dev/null
if [ $gmx_ele_method_ind -eq 1 ] || [ $gmx_ele_method_ind -eq 11 ]; then
    echo "# tune pme parameter"
    gmx_seed=`date +%s`
    $gmx_tune_command -tune yes -self 1e-4 -seed $gmx_seed -nice 0
    mv -f tuned.tpr topol.tpr
fi
cd $cwd

echo "# call mdrun"
echo "## run with `which mdrun`"
echo "## run with $gmx_mdrun_command"
cd $target
$gmx_mdrun_command
cd $cwd


