#!/bin/bash
clear
echo -n "****** Compiling and executing the code .... *******  "
cd sim_sources
icc -openmp -O3 *.cpp -o ff
qsub script.sh 




