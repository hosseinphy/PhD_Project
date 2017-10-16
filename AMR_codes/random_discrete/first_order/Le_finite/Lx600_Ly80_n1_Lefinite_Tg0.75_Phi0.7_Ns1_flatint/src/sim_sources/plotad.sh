#!/bin/bash
#PBS -l nodes=1:ppn=16 -l pmem=2700m -l walltime=00:70:00:00
#PBS -A fyw-423-ad
#PBS -N plotad
#PBS -o plotad.log
#PBS -e plotad_err.log
cd $PBS_O_WORKDIR
python  plotad.py 

