#!/bin/sh
temp=$1
start=$2
stop=$3
file=$4
GROMPP=$5
TPBCONV=$6
MPIRUN=$7
MDRUN=$8


fact=50000000
let nsteps=$fact*$start
cat > SIMXX.mdp <<EOF
title               =  protein with solv          ; a string
integrator          =  md
dt                  =  0.002                    ; time step
nsteps              =  $nsteps                      ; number of steps
nstcomm             =  1                        ; reset c.o.m. motion
comm_mode           =  linear
nstxout             =  $nsteps                       ; write coords
nstxtcout           =  1000
nstvout             =  $nsteps                    ; write velocities
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
constraints          =  all-bonds
constraint_algorithm =  lincs
lincs_order         = 4
lincs_iter	    = 1
rlist               =  1.2                      ; cut-off for ns
rvdw                =  1.2                      ; cut-off for vdw
rcoulomb            =  1.2                      ; cut-off for coulomb
Tcoupl              =  V-rescale                ; temperature coupling
ref_t               =  $temp
tc-grps             =  system
tau_t               =  1
Pcoupl              = Parrinello-Rahman                ; pressure bath
Pcoupltype          =  isotropic                ; pressure geometry
tau_p               =  2.5                      ; p-coupoling time
compressibility     =  4.5e-5
ref_p               =  1
gen_vel             =  no                      ; generate initial velocities
unconstrained_start =  yes
;gen_temp            =  $temp                      ; initial temperature
;gen_seed            =  1993                     ; random seed
pbc                 =   xyz

EOF


np=24

out="sim"
while [ $start -le $stop ]
do
    new=${out}${start}
    if [ $start -eq  1 ]
    then
        echo " generating tpr for first time: filename is $out"
        $GROMPP -f SIMXX.mdp -p ${file}.top -po ${file}out.mdp -c EQUI1.gro -o $out.tpr  -maxwarn 2
        #$GROMPP -f SIMXX.mdp -p ${file}.top -po ${file}out.mdp -c MIN1.gro -o $out.tpr -n $file.ndx -maxwarn 2
    else
        let prev=$start-1
        old=${out}${prev}
        if [ -e $old.cpt ]
        then
            cp -f $old.cpt $out.cpt
        else
    	    cp -f sim_prev.cpt $out.cpt
        fi
        let nsteps=$fact*$start
        echo "start-> $start running tpbconv with $nsteps using tprfile $old"
        $TPBCONV -s $old.tpr -nsteps $nsteps -o $out.tpr
    fi
    $MPIRUN -np $np $MDRUN -np $np -deffnm $out  -cpi $out.cpt  -noappend
    mv -f $out.cpt $new.cpt
    mv -f $out.tpr $new.tpr
    let start=$start+1
done





