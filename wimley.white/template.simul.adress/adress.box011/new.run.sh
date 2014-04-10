#!/bin/bash

grompp -n index.ndx
if [ $? -ne 0 ]; then
    echo "failed at grompp exit"; exit
fi
aprun -B mdrun
if [ $? -ne 0 ]; then
    echo "failed at mdrun exit"; exit
fi
