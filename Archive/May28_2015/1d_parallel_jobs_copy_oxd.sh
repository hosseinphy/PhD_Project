#!/bin/bash
#theis file gives a user a chance to copy all date from Simulation_result in home dir \
# to data analysis folder or delete all files in data analysis folder
clear

 echo -n "Enter your choice and press [ENTER]: "kint" for Kinetic  "oxd" for Oxcidation & "arh"for Arhenius "
 echo -n
 read choice
 
 var0=$choice

   if [[ "$choice" == "kint" ]] ||[[ "$choice" == "oxd_discrete" ]]||[[ "$choice" == "oxd_continuum" ]]
    then
     echo -n "Do you want to COPY into / Delete from folder /gs/project/fyw-423-aa/hophy/Simulation/1d_parallel/oxidation_discrete [ENTER]:"cp" for copy and "del" for delete  "
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

         echo -n "combustion time ?   "
         echo -n 
         read ct
         var6=$ct

         file1="$var1"_"$var2"_"$var3"_"$var4"_"$var5"_"$var6"

         echo " file "$file1" "
         echo -n
         echo "copying to file "$var1"_"$var2"_"$var3"_"$var4"_"$var5"_"$var6" "

         DR="/gs/project/fyw-423-aa/hophy/Simulation/1d_parallel/1d_parallel_"$var0"/"
     
         cd "$DR"

         mkdir "$var1"_"$var2"_"$var3"_"$var4"_"$var5"_"$var6"

        for i in {1..40} 
           do
             i=`echo $i -1 | bc`
              
# for discrete parallel



# for continuum parallel

                 # cp  -r /home/hophy/FINAL_CODE/parallel/1d/Oxd_continuum \

                  cp  -r /home/hophy/FINAL_CODE/parallel/1d/Oxd_discrete \
                     -- /gs/project/fyw-423-aa/hophy/Simulation/1d_parallel/1d_parallel_"$var0"/"$var1""_""$var2""_""$var3""_""$var4""_""$var5""_""$var6"/1dparallel_"$var0"_"$i"
         
        done
 
        echo "it is done"
        echo " "
        echo "Editing files..."


   for i in {1..39} 
     do
     mv  /gs/project/fyw-423-aa/hophy/Simulation/1d_parallel/1d_parallel_"$var0"/"$var1""_""$var2""_""$var3""_""$var4""_""$var5""_""$var6"/1dparallel_"$var0"_"$i"/parallel_input0.in \
                -- /gs/project/fyw-423-aa/hophy/Simulation/1d_parallel/1d_parallel_"$var0"/"$var1""_""$var2""_""$var3""_""$var4""_""$var5""_""$var6"/1dparallel_"$var0"_"$i"/parallel_input"$i".in
   done

####################################################################################################################

 #Change in input files 

  k=0.330101
  to=0.01;
  to_incr=1.0;
  tg=0.6800;
  tg_incr=0.004;
  d=0.0015
  decl=0.00003
  incl=0.00006
  mult=40000000


   for i in {1..40} 
    do
      i=`echo $i -1 | bc`
      #t=0
      FILES=/gs/project/fyw-423-aa/hophy/Simulation/1d_parallel/1d_parallel_"$var0"/"$var1""_""$var2""_""$var3""_""$var4""_""$var5""_""$var6"/1dparallel_"$var0"_"$i"/*

      for f in $FILES 
         do

          if [[ "$f" == *\.in* ]] 
            then          
             sed -i  "2s/.*/f_n            "$i"/"  $f
             sed -i  "3s/.*/Tg             "$tg"/"  $f
             tg=`echo $tg + $tg_incr | bc`              
             #sed -i  "9s/.*/tou_c             "$to"/"  $f
             #to=`echo $to + $to_incr | bc`
             #sed -i  "14s/.*/t_step        "$var3"/"  $f
             #t=`echo "$var3 * $mult" | bc`
             #t=`echo "$d * $mult" | bc`
             #sed -i  "20s/.*/tot_time      "$t"/"  $f
      	  fi  


#################
#Now changing in main.cpp

            FULLFILENAME=$f
            DIRNAME=$(dirname ${FULLFILENAME})   #-- /home/user/video
            BASENAME=$(basename ${FULLFILENAME}) #-- myvideo-release.xvid.128kbps.avi
            FILENAME=${BASENAME%.*}              #-- myvideo-release.xvid.128kbps
            EXTENSION=${BASENAME##*.}

              if [[ "$FILENAME" == *parallel_main* ]]
                then
                 sed -i "s/kint/"$var0"/g"  $f
                 sed -i "s/pr2/"$var2"/g"  $f
             fi
      done
  done

###########################################

# now compile and submit the jobs on cluster
 
  for i in {1..40} 
   do
    i=`echo $i -1 | bc`
    FILES=/gs/project/fyw-423-aa/hophy/Simulation/1d_parallel/1d_parallel_"$var0"/"$var1""_""$var2""_""$var3""_""$var4""_""$var5""_""$var6"/1dparallel_"$var0"_"$i"/*

    for f in $FILES 
        do

            FULLFILENAME=$f
            DIRNAME=$(dirname ${FULLFILENAME})   #-- /home/user/video
            BASENAME=$(basename ${FULLFILENAME}) #-- myvideo-release.xvid.128kbps.avi
            FILENAME=${BASENAME%.*}              #-- myvideo-release.xvid.128kbps
            EXTENSION=${BASENAME##*.}

           if [[ "$FILENAME" == *script* ]]
            then
             sed -i "s/1_1_1_1_1_1/"$var1""_""$var2""_""$var3""_""$var4""_""$var5""_""$var6"/g"  $f
             sed -i "s/1d_parallel_oxd/1d_parallel_"$var0"/g"  $f
             sed -i "s/2dparallel_kint/1dparallel_"$var0"_"$i"/g"  $f
             sed -i "s/parallel_input0/parallel_input"$i"/g"  $f         
           fi
    done


      cd /gs/project/fyw-423-aa/hophy/Simulation/1d_parallel/1d_parallel_"$var0"/"$var1""_""$var2""_""$var3""_""$var4""_""$var5""_""$var6"/1dparallel_"$var0"_"$i"


          mpicc -o  parallel_input"$i" -O2 parallel_main.cpp ;
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


###########################################

    elif [ "$option" == "del" ]; then


         echo "Deleting from  /gs/scratch/Simulation_results/1dserial_choice is in progress ..."

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

         echo -n "combustion time ?   "
         echo -n 
         read ct
         var6=$ct

         echo "Deleting from file "$var1"_"$var2"_"$var3"_"$var4"_"$var5"_"$var6" "


          for i in {1..40} 
            do
             i=`echo $i -1 | bc`
              
             rm -r /gs/project/fyw-423-aa/hophy/Simulation/1d_parallel/1d_parallel_"$var0"/"$var1""_""$var2""_""$var3""_""$var4""_""$var5""_""$var6"/1dparallel_"$var0"_"$i"

          done
      
         DR="/gs/scratch/hophy/Simulation_results/1d_parallel_"$var0""
         cd "$DR"
         rm -r "$var1"_"$var2"_"$var3"_"$var4"_"$var5"_"$var6"

else
echo "you entered the wrong choice"
exit 1
fi
############################################


else
echo "zaaaaap"
exit 1
fi

