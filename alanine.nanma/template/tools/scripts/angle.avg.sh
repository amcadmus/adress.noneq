#!/bin/bash

make -C ./tools/angles/ -j

./tools/angles/average.traj -f angle.name -o avg.angle

test ! -d dir.avg.angle && mkdir dir.avg.angle

mv -f avg.angle* dir.avg.angle
