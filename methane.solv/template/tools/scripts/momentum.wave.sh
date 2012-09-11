#!/bin/bash

make -C ./tools/average/ -j8

./tools/average/average.wave -f momentum.wave.name -o avg.momentum.wave

test ! -d dir.momentum.wave && mkdir dir.momentum.wave

mv -f avg.momentum.wave* dir.momentum.wave


