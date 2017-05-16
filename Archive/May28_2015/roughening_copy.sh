#!/bin/bash
#theis file gives a user a chance to copy all date from Simulation_result in home dir \
# to data analysis folder or delete all files in data analysis folder
#2d parallel code manipulation
                                               ########## NOTE !!!!! #############
##for this version of compiling shell  I am using modified version of main file (consider each processor random distiribution)
#############################################################################################################################
clear

 echo -n "Enter your choice and press [ENTER]: "kint" for Kinetic  "oxd" for Oxcidation & "arh"for Arhenius "
 echo -n
 read choice
 
 var0=$choice

   if [[ "$choice" == "kint" ]] ||[[ "$choice" == "oxd" ]] || [[ "$choice" == "rough" ]]
    then
     echo -n "Do you want to COPY into / Delete from folder /gs/Simulation/Kinetic_model [ENTER]:"cp" for copy and "del" for delete  "
     echo -n
     read option
       
     if  [ "$option" == "cp" ]; then

        #ask user to provide parameters

         echo -n "lenght of your box ?   "
         echo -n 
         read lenx
         var1=$lenx

         echo -n "width of your box ?    "
         echo -n 
         read leny
         var2=$leny

         echo -n "deltat ?   "
         echo -n 
         read deltat
         var3=$deltat

         echo -n "deltax ?   "
         echo -n 
         read deltax
         var4=$deltax


         echo -n "thershold concenteration ?   "
         echo -n 
         read C
         var5=$C

         file1="$var1"_"$var2"_"$var3"_"$var4"_"$var5"

         echo " file "$file1" "
         echo -n
         echo "copying to file "$var1"_"$var2"_"$var3"_"$var4"_"$var5" "

         DR="/gs/scratch/hophy/Simulation_results/2d_parallel_"$var0"/"
     
         cd "$DR"

         mkdir "$var1"_"$var2"_"$var3"_"$var4"_"$var5"

        for i in {1..50} 
           do
             i=`echo $i -1 | bc`

               cp  -r /home/hophy/FINAL_CODE/parallel/2d/roughening \
                     -- /gs/scratch/hophy/Simulation_results/2d_parallel_"$var0"/"$var1""_""$var2""_""$var3""_""$var4""_""$var5"/2dparallel_"$var0"_"$i"
         
        done
 
        echo "it is done"
        echo " "
        echo "Editing files..."


   for i in {1..49} 
     do
     mv  /gs/scratch/hophy/Simulation_results/2d_parallel_"$var0"/"$var1""_""$var2""_""$var3""_""$var4""_""$var5"/2dparallel_"$var0"_"$i"/parallel_rough_input0.in \
                -- /gs/scratch/hophy/Simulation_results/2d_parallel_"$var0"/"$var1""_""$var2""_""$var3""_""$var4""_""$var5"/2dparallel_"$var0"_"$i"/parallel_rough_input"$i".in
   done

####################################################################################################################

 #Change in input files 

  k=0.336621
  p=0.998
  incl=0.00002
  mult=60000000
  concenteration=0.001

#Generate RN for seed

  for r in {1..999};
   do
     n=$RANDOM

      if [ $r  -gt  949 ]; then
       r=`echo $r -950 | bc`
       ran[$r]=$(( n%1000+1582458428+n ))
       echo ${ran[$r]}
      fi
   done



   for i in {1..50} 
    do
      i=`echo $i -1 | bc`
      t=0
      FILES=/gs/scratch/hophy/Simulation_results/2d_parallel_"$var0"/"$var1""_""$var2""_""$var3""_""$var4""_""$var5"/2dparallel_"$var0"_"$i"/*

      for f in $FILES 
         do
          if [[ "$f" == *\.in* ]] 
            then          
             sed -i  "2s/.*/f_n            "$i"/"  $f
             #sed -i  "6s/.*/To             "$k"/"  $f
             #k=`echo $k + $incl | bc`
             sed -i  "12s/.*/seed          "${ran[$i]}"/"  $f
            # p=`echo $p + $concenteration | bc`
            # sed -i  "13s/.*/Ctshold       "$var5"/"  $f
             sed -i  "14s/.*/t_step        "$var3"/"  $f
             t=`echo "$var3 * $mult" | bc`
             sed -i  "22s/.*/tot_time      "$t"/"  $f
      	  fi  

#################
#Now changing in main.cpp

            FULLFILENAME=$f
            DIRNAME=$(dirname ${FULLFILENAME})   #-- /home/user/video
            BASENAME=$(basename ${FULLFILENAME}) #-- myvideo-release.xvid.128kbps.avi
            FILENAME=${BASENAME%.*}              #-- myvideo-release.xvid.128kbps
            EXTENSION=${BASENAME##*.}

              if [[ "$FILENAME" == *parallel_rough_main* ]]
                then
                 sed -i "s/kint/"$var0"/g"  $f
             fi
      done
  done

###########################################

# now compile and submit the jobs on cluster
 
  for i in {1..50} 
   do
    i=`echo $i -1 | bc`
    FILES=/gs/scratch/hophy/Simulation_results/2d_parallel_"$var0"/"$var1""_""$var2""_""$var3""_""$var4""_""$var5"/2dparallel_"$var0"_"$i"/*

    for f in $FILES 
        do

            FULLFILENAME=$f
            DIRNAME=$(dirname ${FULLFILENAME})   #-- /home/user/video
            BASENAME=$(basename ${FULLFILENAME}) #-- myvideo-release.xvid.128kbps.avi
            FILENAME=${BASENAME%.*}              #-- myvideo-release.xvid.128kbps
            EXTENSION=${BASENAME##*.}

           if [[ "$FILENAME" == *script* ]]
            then

             sed -i "s/1_1_1_1_1/"$var1""_""$var2""_""$var3""_""$var4""_""$var5"/g"  $f
             sed -i "s/2d_parallel_kint/2d_parallel_"$var0"/g"  $f
             sed -i "s/2dparallel_kint/2dparallel_"$var0"_"$i"/g"  $f
             sed -i "s/parallel_rough_input0/parallel_rough_input"$i"/g"  $f         
           fi
    done

      cd /gs/scratch/hophy/Simulation_results/2d_parallel_"$var0"/"$var1""_""$var2""_""$var3""_""$var4""_""$var5"/2dparallel_"$var0"_"$i"

          mpicc -o  parallel_rough_input"$i" -O2 parallel_rough_main.cpp ;
             if [[ $? -eq 0 ]]; then
              pid[$i]=$(echo "sleep 10" | qsub $FILENAME)
              echo ${pid[$i]}
             fi
   done 

###########################################

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
       
        for i in {1..50}
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


###########################################

    elif [ "$option" == "del" ]; then


         echo "Deleting from  /gs/scratch/Simulation_results/2dparallel_choice is in progress ..."

          echo -n "lenght of your box ?   "
          echo -n 
          read lenx
          var1=$lenx

          echo -n "width of your box ?    "
          echo -n 
          read leny
          var2=$leny

          echo -n "deltat ?   "
          echo -n 
          read deltat
          var3=$deltat

          echo -n "deltax ?   "
          echo -n 
          read deltax
          var4=$deltax

          echo -n "thershold concenteration ?   "
          echo -n 
          read C
          var5=$C

         echo "Deleting from file "$var1"_"$var2"_"$var3"_"$var4"_"$var5" "


          for i in {1..20} 
            do
             i=`echo $i -1 | bc`
              
             rm -r /gs/scratch/hophy/Simulation_results/2d_parallel_"$var0"/"$var1""_""$var2""_""$var3""_""$var4""_""$var5"/2dparallel_"$var0"_"$i"

          done
      
         DR="/gs/scratch/hophy/Simulation_results/2d_parallel_"$var0""
         cd "$DR"
         rm -r "$var1"_"$var2"_"$var3"_"$var4"_"$var5"

else
echo "you entered the wrong choice"
exit 1
fi
############################################


else
echo "zaaaaap"
exit 1
fi

