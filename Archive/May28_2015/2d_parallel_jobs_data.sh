#!/bin/bash
#theis file gives a user a chance to copy all date from Simulation_result in /gs/scratch dir \
# to data analysis folder or delete all files in data analysis folder
# 2D code for copying from parallel Simulation
clear
 echo -n "Enter your choice and press [ENTER]: "kint" for Kinetic  "oxd" for Oxcidation & "arh"for Arhenius "
 echo -n
 read choice
 
 var0=$choice

   if [[ "$choice" == "kint" ]]||[[ "$choice" == "oxd" ]]||[[ "$choice" == "rough" ]]
    then
     echo -n "Do you want to COPY from /gs/Simulation/Kinetic_model or delte from ~/DATA_ANALYSIS folder  [ENTER]:"cp" for copy and "del" for delete  "
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

         echo -n " what is filename ?   "
         echo -n 
         read filename
         var6=$filename

         echo -n " what is filename number ?   "
         echo -n 
         read fn
         var7=$fn

         echo -n "what is file extention ?   "
         echo -n 
         read fext
         var9=$fext

         file1="$var1"_"$var2"_"$var3"_"$var4"_"$var5"
         file2="$var6""$var7"_"$var3"_"$var8""$var9"

         echo " file "$file1" "
         echo -n
         echo "copying from folder "$var1"_"$var2"_"$var3"_"$var4"_"$var5" "
         echo "Check if there is any files left in the distination ...."

         FILES=/gs/scratch/hophy/DATA_ANALYSIS/2d_parallel/"$var0"_model/*
         DIR=/gs/scratch/hophy/DATA_ANALYSIS/2d_parallel/"$var0"_model

         incl=0.00002;
         tnut=0.010000;
       #  for f in $FILES
       #    do          
            echo -n "deltat?   "
            echo -n 
            read dllt
            vardelt=$dllt

          #  echo -n "background temperature ?   "
          #  echo -n 
          #  read tnut
          #  vartnot=$tnut	

             for i in {1..20} 
              do
               i=`echo $i -1 | bc`
 #              tnut=`echo $tnut + $incl | bc` #turn it of for different concenteration, should be turn on for ordinary files
                
                for v in {0..700} 
                 do
                  v=`echo $v | bc`

#                   cp /gs/scratch/hophy/Simulation_results/2d_parallel_"$var0"/"$var1""_""$var2""_""$var3""_""$var4""_""$var5"/2dparallel_"$var0"_"$i"/"$var6""$i""_""$v""_""$vardelt""_""0""$tnut""$var9"    /gs/scratch/hophy/DATA_ANALYSIS/refined_results_9600_512_diff_realzation0.998/ 


                   cp /gs/scratch/hophy/Simulation_results/2d_parallel_"$var0"/"$var1""_""$var2""_""$var3""_""$var4""_""$var5"/2dparallel_"$var0"_"$i"/"$var6""$i""_""$v""_""$vardelt""_""$tnut""$var9"    /gs/scratch/hophy/DATA_ANALYSIS/roughening/4800_700_0.01_1_0.5/ 


              done

             #sed -i  "14s/.*/seed         "${ran[$i]}"/"  $f
             #sed -i  "15s/.*/Ctshold      "$var5"/"  $f
              
           done
           #  FILES2=/gs/scratch/hophy/Simulation_results/2d_parallel_"$var0"/"$var1""_""$var2""_""$var3""_""$var4""_""$var5"/2dparallel_"$var0"_"$i"/*
           #   for g in $FILES2 
           #    do
           #       if [[ "$g" == *\.txt* ]]
           #       
           #       then
     
           #         cp  $g  /gs/scratch/hophy/DATA_ANALYSIS/2d_parallel/"$var0"_model
           #       fi
           #    done
           #  done




       # else
       #   rm  $f
       # fi
  #   done

 #After all copy all files to the distination

     #   for i in {1..42} 
     #      do
     #       i=`echo $i -1 | bc`
     #         FILES2=/home/hophy/Simulation_results/continuum_ser_higg/parallel_higgins_"$i"/*
     #         for g in $FILES2 
     #          do
     #           if [[ "$g" == *\.txt* ]] ||[[ "$g" == *\.in* ]]; 
     #           then                  
     #             cp  $g  /home/hophy/data_Analyis/continum_results 
     #           fi
     #      done
     # done


#other choices 
  
     elif  [ "$option" == "del" ]; then

       echo "Delleting from Continuum folder is in progress ..."
                    
           FILES=/gs/scratch/hophy/DATA_ANALYSIS/2d_parallel/"$var0"_model/*

              for f in $FILES 
               do
                  rm  $f  
           done
     
    
   else 
    exit 1
fi

############################################

else
echo "Boooom , wrong choice"
exit 1
fi

