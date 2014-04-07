#!/bin/bash

make -C ~/study/adress.noneq/alanine.nanma/template/tools/angles/ -j

~/study/adress.noneq/alanine.nanma/template/tools/angles/average.traj.corr -c 1
~/study/adress.noneq/alanine.nanma/template/tools/average/integrate.file -f meta.flux.out -o meta.flux.inte.out
