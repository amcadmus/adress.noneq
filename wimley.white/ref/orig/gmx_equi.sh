#!/bin/sh
temp=$1
file=$2
GROMPP=$3
MPIRUN=$4
MDRUN=$5
TPBCONV=$6
fact=100000
start=1
stop=1
let nsteps=$fact*$start

cat > SIMXX.mdp <<EOF
title               =  protein with solv          ; a string
cpp                 =  /lib/cpp                 ; c-preprocessor
dt                  =  0.002                    ; time step
nsteps              =  1000000                      ; number of steps
nstcomm             =  1                        ; reset c.o.m. motion
comm_mode           =  linear
nstxout             =  500000                       ; write coords
nstxtcout           =  1000
nstvout             =  500000                    ; write velocities
nstlog              =  1000                     ; print to logfile
nstenergy           =  1000                      ; print energies
nstlist             =  10                        ; update pairlist
ns_type             =  grid                     ; pairlist method
coulombtype         =  pme
fourierspacing      =  0.12
pmeorder            =  4
optimize_fft        =  yes
ewald_rtol          =  1.0e-6
ewald_geometry      =  3d
DispCorr            =  EnerPres
constraints          =  all-bonds
constraint_algorithm =  lincs
unconstrained_start  = no
lincs_order         = 4
lincs_iter	    = 1
rlist               =  1.2                      ; cut-off for ns
rvdw                =  1.2                      ; cut-off for vdw
rcoulomb            =  1.2                      ; cut-off for coulomb
Tcoupl              =  berendsen                ; temperature coupling
ref_t               =  $temp 
tc-grps             =  system 
tau_t               =  1.0   
Pcoupl              = berendsen                 ; pressure bath
Pcoupltype          =  isotropic               ; pressure geometry
tau_p               =  1.0                     ; p-coupoling time
compressibility     =  4.5e-5
ref_p               =  1
gen_vel             =  yes                      ; generate initial velocities
gen_temp            =  $temp                      ; initial temperature
gen_seed            =  -1                     ; random seed
pbc                 =   xyz

EOF


np=24
out=EQUI1
while [ $start -le $stop ]
do
    new=${out}${start}
    if [ $start -eq  1 ]
    then
        echo " generating tpr for first time: filename is $out"
        $GROMPP -f SIMXX.mdp -p ${file}.top -po ${file}out.mdp -c MIN0.gro -o $out.tpr  -maxwarn 2
    else
        let prev=$start-1
        old=${out}${prev}
        if [ -e $old.cpt ]
        then
            cp -f $old.cpt $out.cpt
        fi
        let nsteps=$fact*$start
        echo "start-> $start running tpbconv with $nsteps using tprfile $old"
        $TPBCONV -s $old.tpr -nsteps $nsteps -o $out.tpr
    fi
    $MPIRUN -np $np $MDRUN -np $np -deffnm $out  -cpi $out.cpt 
    mv -f $out.cpt $new.cpt
    mv -f $out.tpr $new.tpr
    let start=$start+1
done







