#!/bin/bash

make -C tools/h.bond makedir
make -C tools/h.bond -j8

target=$1
if test ! -d $target; then
    echo "no dir $target"
    exit
fi

cd $target
../tools/h.bond/equi.ch4.1 -t mytop -b 100 -r .52

