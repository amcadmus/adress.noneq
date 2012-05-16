#!/bin/bash

targets=`ls | grep "^pert"`
rm -f h.count.name

for i in $targets;
do
    cd $i
    ../tools/h.bond/equi.ch4.1 -o h.count.out
    cd ..
    echo "$i/h.count.out" >> h.count.name
done




