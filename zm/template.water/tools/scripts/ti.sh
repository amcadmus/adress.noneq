#!/bin/bash

source env.sh
source parameters.sh

targets=`ls | grep "^ti"`

cwd=`pwd`

sum_value=0.0
sum_error=0.0

for ii in $targets;
do
    test ! -d $ii && continue
    echo "# doing in $ii"
    cd $ii
    value1=0.0
    error1=0.0
    value2=0.0
    error2=0.0
    if ls | grep stage.1 &> /dev/null; then
	g_bar -f stage.1*/dhdl.xvg -b $gmx_equi_skip &> bar.stage.1.out
	range=`cat bar.stage.1.out  | grep total | cut -d ',' -f 1 | cut -d 'l' -f 2`
	echo "# find stage 1, lambda range $range"
	value1=`cat bar.stage.1.out  | grep total | cut -d 'G' -f 2 | cut -d '+' -f 1`
	error1=`cat bar.stage.1.out  | grep total | cut -d '/' -f 2 | cut -d '-' -f 2`
	echo stage1: $value1 $error1
    fi
    if ls | grep stage.2 &> /dev/null; then
	g_bar -f stage.2*/dhdl.xvg -b $gmx_equi_skip &> bar.stage.2.out
	range=`cat bar.stage.2.out  | grep total | cut -d ',' -f 1 | cut -d 'l' -f 2`
	echo "# find stage 2, lambda range $range"
	value2=`cat bar.stage.2.out  | grep total | cut -d 'G' -f 2 | cut -d '+' -f 1`
	error2=`cat bar.stage.2.out  | grep total | cut -d '/' -f 2 | cut -d '-' -f 2`
	echo stage2: $value2 $error2
    fi
    sum_value=`echo "$sum_value + $value1" | bc -l`
    sum_value=`echo "$sum_value + $value2" | bc -l`
    sum_error=`echo "$sum_error + $error1 * $error1" | bc -l`
    sum_error=`echo "$sum_error + $error2 * $error2" | bc -l`
    cd $cwd
done

if echo $gmx_ele_method | grep zm &> /dev/null; then    
    energy_corr=`$zm_gen_dir/energy.corr -l $zm_l --alpha $zm_alpha --rc $gmx_rcut_ele `
else
    energy_corr=0.0
fi

energy_corr=0

# energy should be minus
sum_value=`echo "- $sum_value + $energy_corr" | bc -l`
sum_value=`printf %.2f $sum_value`
sum_error=`echo "sqrt($sum_error)" | bc -l`
sum_error=`printf %.2f $sum_error`

echo energy_corr: $energy_corr
echo total: $sum_value $sum_error

