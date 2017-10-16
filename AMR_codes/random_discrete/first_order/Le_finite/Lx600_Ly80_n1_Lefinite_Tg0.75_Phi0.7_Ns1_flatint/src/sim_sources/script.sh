#!/bin/bash
#PBS -l nodes=1:ppn=16
#PBS -l walltime=06:00:00:00
#PBS -o out.log
#PBS -e err.log
#PBS -V
#PBS -N curv_effect
#PBS -A fyw-423-ad

cd $PBS_O_WORKDIR

export OMP_NUM_THREADS=16

./ff

