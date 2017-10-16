#!/bin/bash
#PBS  -A fyw-423-ad
#PBS  -l walltime=00:70:00:00
#PBS -l nodes=1:ppn=16
#PBS -o out.log
#PBS -e err.log
#PBS -V
#PBS -N zerorder_Amp_discrete
cd $PBS_O_WORKDIR
#module add ifort_icc openmpi
export OMP_NUM_THREADS=16

./ff

