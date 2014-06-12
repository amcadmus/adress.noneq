#!/bin/bash

echo "[ phi ]" > angle.ndx
echo "5 7 9 15" >> angle.ndx
echo "[ psi ]" >> angle.ndx
echo "7 9 15 17" >> angle.ndx

echo 0 | g_angle -type dihedral -xvg none -od angdist.phi.xvg -ov angaver.phi.xvg -nice 0 -f alanine.xtc
echo 1 | g_angle -type dihedral -xvg none -od angdist.psi.xvg -ov angaver.psi.xvg -nice 0 -f alanine.xtc

