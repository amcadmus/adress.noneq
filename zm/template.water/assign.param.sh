#!/bin/bash

cwd=`pwd`
dirname=${cwd##*/}

ll=`echo $dirname | cut -f 2 -d '.' | cut -f 2 -d 'l'`
alpha1=`echo $dirname | cut -f 3 -d '.' | cut -f 3 -d 'a'`
alpha2=`echo $dirname | cut -f 4 -d '.'`
alpha=$alpha1.$alpha2
rc1=`echo $dirname | cut -f 5 -d '.' | cut -f 2 -d 'c'`
rc2=`echo $dirname | cut -f 6 -d '.'`
rc=$rc1.$rc2

source parameters.sh
if test `echo "$gmx_rcut_vdw" '>' "$rc" | bc` -eq 1; then
    maxrc=$gmx_rcut_vdw
else
    maxrc=$rc
fi

rlist=`echo "$maxrc+0.3" | bc -l`

cat parameters.sh |\
sed -e "s/zm_l=.*/zm_l=$ll/g" |\
sed -e "s/zm_alpha=.*/zm_alpha=$alpha/g" |\
sed -e "s/gmx_rlist=.*/gmx_rlist=$rlist/g" |\
sed -e "s/gmx_rcut_ele=.*/gmx_rcut_ele=$rc/g" \
    > tmp.param

mv -f tmp.param parameters.sh

