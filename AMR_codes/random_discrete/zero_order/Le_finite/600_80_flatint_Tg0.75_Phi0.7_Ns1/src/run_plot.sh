#!/bin/bash
clear
echo -n "****** Plotting stuff .... *******  "

cd ../sim_sources
if find . -maxdepth 1 -name "aaa*" -print | grep -q . 
then
  echo "You found the file"
  echo "running the plotting script file ..."
  qsub plotad.sh 
else 
  echo "You haven't found the file"
  exit -1
fi



