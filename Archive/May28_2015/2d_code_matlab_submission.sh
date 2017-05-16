#!/bin/bash
#this is a shell code to submit as many matlab file as you want

clear


 echo -n "Enter your choice and press [ENTER]: "kint" for Kinetic  "oxd" for Oxcidation & "arh"for Arhenius "
 echo -n
 read choice
 
 var0=$choice

   if [[ "$choice" == "kint" ]] ||[[ "$choice" == "oxd" ]]||[["$choice" == "arh"]]
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


         echo -n "Filename ?   "
         echo -n 
         read fname
         var6=$fname


         echo -n "start time ?   "
         echo -n 
         read st
         var7=$st


         echo -n "end time ?   "
         echo -n 
         read ft
         var8=$ft



         file1="$var1"_"$var2"_"$var3"_"$var4"_"$var5"

         echo " file "$file1" "
         echo -n
         echo "copying to file "$var1"_"$var2"_"$var3"_"$var4"_"$var5" "

         DR="/gs/scratch/hophy/Simulation_results/matlab_data/2d_model_"$var0"/"
     
         cd "$DR"

         mkdir "$var1"_"$var2"_"$var3"_"$var4"_"$var5"

        for i in {15..22} 
           do
             i=`echo $i -1 | bc`

               cp  -r /home/hophy/FINAL_CODE/data_process/new \
                     -- /gs/scratch/hophy/Simulation_results/matlab_data/2d_model_"$var0"/"$var1""_""$var2""_""$var3""_""$var4""_""$var5"/2dparallel_"$var0"_"$i"
         
        done
 
        echo "it is done"
        echo " "
        echo "Editing files..."

   for i in {14..21} 
     do

     mv  /gs/scratch/hophy/Simulation_results/matlab_data/2d_model_"$var0"/"$var1""_""$var2""_""$var3""_""$var4""_""$var5"/2dparallel_"$var0"_"$i"/main_temp_0.m \
                -- /gs/scratch/hophy/Simulation_results/matlab_data/2d_model_"$var0"/"$var1""_""$var2""_""$var3""_""$var4""_""$var5"/2dparallel_"$var0"_"$i"/main_temp_"$i".m 
   

     mv  /gs/scratch/hophy/Simulation_results/matlab_data/2d_model_"$var0"/"$var1""_""$var2""_""$var3""_""$var4""_""$var5"/2dparallel_"$var0"_"$i"/ps_2D_0.m \
                -- /gs/scratch/hophy/Simulation_results/matlab_data/2d_model_"$var0"/"$var1""_""$var2""_""$var3""_""$var4""_""$var5"/2dparallel_"$var0"_"$i"/ps_2D_"$i".m 


    done

####################################################################################################################

 #Change in input files 

  k=0.334181
  incl=0.00008
  mult=4000000

  echo -n "deltat?   "
  echo -n 
  read dllt
  vardelt=$dllt

   for i in {15..22} 
    do
      i=`echo $i -1 | bc`
      t=0
      FILES=/gs/scratch/hophy/Simulation_results/matlab_data/2d_model_"$var0"/"$var1""_""$var2""_""$var3""_""$var4""_""$var5"/2dparallel_"$var0"_"$i"/*

      for f in $FILES 
         do

       #################
       #Now changing in main_temp.m

            FULLFILENAME=$f
            DIRNAME=$(dirname ${FULLFILENAME})   #-- /home/user/video
            BASENAME=$(basename ${FULLFILENAME}) #-- myvideo-release.xvid.128kbps.avi
            FILENAME=${BASENAME%.*}              #-- myvideo-release.xvid.128kbps
            EXTENSION=${BASENAME##*.}


              if [[ "$FILENAME" == *main_temp* ]]
                then
                 sed -i  "3s/.*/N=         "$var2"/"  $f
                 sed -i  "4s/.*/filename=  ""$var6""/" $f
                 sed -i  "5s/.*/filnumber= "$i"/"   $f
                 sed -i  "6s/.*/deltat=    "$vardelt"/" $f             
                 k=`echo $k + $incl | bc`
                 sed -i  "7s/.*/Tsub=       "$k"/" $f
                 sed -i  "8s/.*/file_start_time= "$var7"/"   $f
                 sed -i  "9s/.*/file_end_time=    "$var8"/" $f             
                 #sed -i "s/output/output"_""$i"/g"  $f

              fi

      done
  done

###########################################

# now compile and submit the jobs on cluster
 
  for i in {15..22} 
   do
    i=`echo $i -1 | bc`
    FILES=/gs/scratch/hophy/Simulation_results/matlab_data/2d_model_"$var0"/"$var1""_""$var2""_""$var3""_""$var4""_""$var5"/2dparallel_"$var0"_"$i"/*

    for f in $FILES 
        do

            FULLFILENAME=$f
            DIRNAME=$(dirname ${FULLFILENAME})   #-- /home/user/video
            BASENAME=$(basename ${FULLFILENAME}) #-- myvideo-release.xvid.128kbps.avi
            FILENAME=${BASENAME%.*}              #-- myvideo-release.xvid.128kbps
            EXTENSION=${BASENAME##*.}

           if [[ "$FILENAME" == *mat_script* ]]
            then                 

              sed -i "s/main_temp_0.m/main_temp"_""$i".m/g"  $f
              sed -i "s/output/output"_""$i"/g"  $f
              sed -i "s/home/hophy/FINAL_CODE/data_process/gs/scratch/hophy/Simulation_results/matlab_data/2d_model_"$var0"/"$var1""_""$var2""_""$var3""_""$var4""_""$var5"/2dparallel_"$var0"_"$i"/g"  $f


           fi

    done

      cd /gs/scratch/hophy/Simulation_results/matlab_data/2d_model_"$var0"/"$var1""_""$var2""_""$var3""_""$var4""_""$var5"/2dparallel_"$var0"_"$i"

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
       
        for i in {1..5}
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


          for i in {1..5} 
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

