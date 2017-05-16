#!/bin/bash

####################################################################################
#this code will copy the main roughening code into as many as the user prefer and
#then manipulate them in input .in and script file. complie them and give you a chance 
#to kill the submitted ones if you like , also it combined with the cleaning file.
####################################################################################
clear


  
 echo -n "Enter your choice and press [ENTER]: "copy" for copying..compiling & "del" for delete all the submitted jobs "
 echo -n
 read choice

  if  [ "$choice" == "copy" ]; then

        echo "Copying into /home/hophy/Simulation_results/rough_results is in progress ..."

      
     #  echo -n "How many copies do you want to creat?  "
     #  echo -n
     #  read number

        echo -n "lenght of your box ?   "
        echo -n 
        read lenx
        var1=$lenx

        echo -n "width of your box ?    "
        echo -n 
        read leny
        var2=$leny

        echo -n "tou_c ?   "
        echo -n 
        read tou
        var3=$tou
        echo -n "T_ign ?   "
        echo -n 
        read Tign
        var4=$Tign

        echo -n "thershold concenteration ?   "
        echo -n 
        read C
        var5=$C

        echo "copying to file "$var1"_"$var2"_"$var3"_"$var4"_"$var5" "


        DR="/scratch/fyw-423-ad/hophy/Simulation_results/2dprov"
        cd "$DR"

        mkdir "$var1"_"$var2"_"$var3"_"$var4"_"$var5"

        for i in {1..40} 
           do
             i=`echo $i -1 | bc`

               cp  -r /home/hophy/serial/provatas/2d_code \
                     -- /scratch/fyw-423-ad/hophy/Simulation_results/2dprov/"$var1""_""$var2""_""$var3""_""$var4""_""$var5"/serial_2dprov_"$i"
         done
 
        echo "it is done"
        echo " "
        echo "Editing files..."

  # number1=`echo $number -1 |bc`
   for i in {1..39} 
     do
     mv  /scratch/fyw-423-ad/hophy/Simulation_results/2dprov/"$var1""_""$var2""_""$var3""_""$var4""_""$var5"/serial_2dprov_"$i"/input0.in \
                -- /scratch/fyw-423-ad/hophy/Simulation_results/2dprov/"$var1""_""$var2""_""$var3""_""$var4""_""$var5"/serial_2dprov_"$i"/input"$i".in
   done


  #Generate RN for seed

#  for r in {1..999};
#   do 
#     n=$RANDOM
#
#      if [ $r  -gt  989 ]; then
#       r=`echo $r -990 | bc`
#       ran[$r]=$(( n%1000+1000 ))
#       echo ${ran[$r]}
#      fi
#   done



 #Change in input files 

  k=0.33016
  incl=0.00001

   for i in {1..40} 
    do
      i=`echo $i -1 | bc`

      FILES=/scratch/fyw-423-ad/hophy/Simulation_results/2dprov/"$var1""_""$var2""_""$var3""_""$var4""_""$var5"/serial_2dprov_"$i"/*

      for f in $FILES 
         do
          if [[ "$f" == *\.in* ]] 
           then          
           sed -i  "2s/.*/f_n             "$i"/"  $f
           k=`echo $k + $incl | bc`
           sed -i  "8s/.*/To              "$k"/"  $f
 #          sed -i  "6s/.*/seed            "${ran[$i]}"/"  $f
           
      	   fi
      done
  done


# now compile and submit the jobs on cluster
 
  for i in {1..40} 
   do
    i=`echo $i -1 | bc`
    FILES=/scratch/fyw-423-ad/hophy/Simulation_results/2dprov/"$var1""_""$var2""_""$var3""_""$var4""_""$var5"/serial_2dprov_"$i"/*
      for f in $FILES 
        do
         if [[ "$f" != *\.* ]] 
           then            
           sed -i "s/1_1_1_1/"$var1""_""$var2""_""$var3""_""$var4""_""$var5"/g"  $f
           sed -i "s/serial_2dprov/serial_2dprov_"$i"/g"  $f
           sed -i "s/input0/input"$i"/g"  $f         
         fi
      done

      cd /scratch/fyw-423-ad/hophy/Simulation_results/2dprov/"$var1""_""$var2""_""$var3""_""$var4""_""$var5"/serial_2dprov_"$i"

           icc -o  input"$i" -O2 main.cpp ;
        	     if [[ $? -eq 0 ]]; then

              pid[$i]=$(echo "sleep 10" | msub $f)
              echo ${pid[$i]}
             fi
 
   done 


# This is a program that asks you if you want to kill the job or not.

echo -n "Enter your choice and press [ENTER]: q for quite , k for kill the job "
echo -n
read name


if  [ "$name" == "q" ]; then
  echo "Quitting."
  exit 1

elif [ "$name" == "k" ]; then

  echo -n "!! WARNING ..."
  echo -n "You are about to cancel the jobs."
  echo
  echo -n "If you want to continue press y otherwise press n "
  
  read option
   
    if  [ "$option" == "n" ]; then
     echo -n "Quitting."
     exit 1

    elif [ "$option" == "y" ]; then
      
      echo -n "Killing jobs ...."
      echo 
       
       for i in {1..40}
         do
          i=`echo $i -1 | bc`
          read -p "Press [Enter] key to kill the jobs one by one ..."
           canceljob ${pid[$i]}
       done
    fi

    else
      echo "Wrong Choice."
      exit 1
    fi



################################
 
    elif [ "$choice" == "del" ]; then


        echo "Deleting from  /home/hophy/Simulation_results/rough_results is in progress ..."


       # echo -n "How many of files do you want to delete ? "
       # echo -n
       # read num
       # num=${num}

        echo -n "lenght of your box ?   "
        echo -n 
        read lenx
        var1=$lenx

        echo -n "width of your box ?    "
        echo -n 
        read leny
        var2=$leny

        echo -n "tou_c ?   "
        echo -n 
        read tou
        var3=$tou
        echo -n "T_ign ?   "
        echo -n 
        read Tign
        var4=$Tign

        echo -n "thershold concenteration ?   "
        echo -n 
        read C
        var5=$C

        echo "Deleting from file "$var1"_"$var2"_"$var3"_"$var4"_"$var5" "


         for i in {1..40} 
           do
            i=`echo $i -1 | bc`

            rm -r  /scratch/fyw-423-ad/hophy/Simulation_results/2dprov/"$var1""_""$var2""_""$var3""_""$var4""_""$var5"/serial_2dprov_"$i"
         done
      

        DR="/scratch/fyw-423-ad/hophy/Simulation_results/2dprov"
        cd "$DR"
        rm -r "$var1"_"$var2"_"$var3"_"$var4"_"$var5"

 
   else
      echo "Wrong Choice."
      exit 1
   fi
