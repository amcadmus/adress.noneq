#!/bin/bash

make -C ./tools/average/ -j8

./tools/average/average.wave -f density.wave.name -o avg.density.wave

test ! -d dir.density.wave && mkdir dir.density.wave

mv -f avg.density.wave* dir.density.wave


