#!/bin/bash

grompp -n index.ndx
if [ $? -ne 0 ]; then
    echo "failed at grompp exit"; exit
fi
g_pme_error -tune -self 0.0001 &> run.log
if [ $? -ne 0 ]; then
    echo "failed at g_pme_error exit"; exit
fi
mv -f tuned.tpr topol.tpr
aprun -B mdrun
if [ $? -ne 0 ]; then
    echo "failed at mdrun exit"; exit
fi
