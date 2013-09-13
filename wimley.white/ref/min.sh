#! /bin/sh

file=$1
out=$2
GROMPP=$3
MDRUN=$4

cat > MIN.mdp <<EOF
constraints         =  none
integrator          =  steep
nsteps              =  10000  
nstlist             =  10    
coulombtype         =  PME
fourierspacing      =  0.12
ns_type             =  grid  
rlist               =  1.4
rcoulomb            =  1.4 
vdw-type            =  switch
rvdw-switch         =  0.9
rvdw                =  1.1   
pme_order           =  4     
ewald_rtol          =  1e-6  
optimize_fft        = yes    
ewald_geometry      = 3d     
;Energy minimizing stuff     
;                            
emtol               =  20.0  
emstep              =  0.01  
pbc                 =  xyz   
EOF
np=8
$GROMPP -f MIN.mdp -p ${file}.top  -po MINout.mdp -c ${file}.gro -o $out.tpr 
$MDRUN -deffnm $out  -cpi $out.cpt


