#!/bin/bash

grep flags /proc/cpuinfo  | tail -n 1 | grep sse2 &> /dev/null
has_sse2=$?
grep flags /proc/cpuinfo  | tail -n 1 | grep sse4 &> /dev/null
has_sse4=$?
grep flags /proc/cpuinfo  | tail -n 1 | grep avx &> /dev/null
has_avx=$?
grep flags /proc/cpuinfo  | tail -n 1 | grep fma &> /dev/null
has_fma=$?

# echo $has_avx $has_fma  $has_sse4 $has_ssee

if [ $has_avx -eq 0 ] && [ $has_fma -eq 0 ]; then
    echo "acc: avx_128_fma";
    appendix=.avx_128_fma
else if [ $has_avx -eq 0 ]; then    
    echo "acc: avx_256";
    appendix=.avx_256
else if [ $has_sse4 -eq 0 ]; then
    echo "acc: sse4";
    appendix=.sse4    
else if [ $has_sse2 -eq 0 ]; then
    echo "acc: sse2";
    appendix=.sse2
fi
fi
fi
fi  

votca_install_dir=$HOME/local/
source $votca_install_dir/bin/VOTCARC.bash

# gromacs_install_dir=$HOME/local/gromacs/4.6.5/
# source $gromacs_install_dir/bin/GMXRC.bash

gmx_dir=$HOME/local/gromacs/4.6.5.localsd.fcorr$appendix
source $gmx_dir/bin/GMXRC


