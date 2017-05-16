#!/bin/bash
#theis file gives a user a chance to copy all date from Simulation_result in home dir \
# to data analysis folder or delete all files in data analysis folder
clear

 echo -n "Enter your choice and press [ENTER]: "C" for Continuum  "D" for Disctere & R for roughening "
 echo -n
 read choice


if  [ "$choice" == "C" ]; then

     echo -n "Do you want to COPY into or Delete from folder [ENTER]:"cp" for copy and "del" for delete  "
     echo -n
     read option

     if  [ "$option" == "cp" ]; then

        echo "Check if there is any files left in the distination ...."

        FILES=/home/hophy/data_Analyis/continum_results/*
        DIR=/home/hophy/data_Analyis/continum_results

        for f in $FILES
         do
         if find $DIR -maxdepth 0 -empty | read v; then 
           
             echo "Empty dir , is fine to copy"; 
             echo "Copying into Continuum folder is in progress ..."
    
              for i in {1..20} 
               do
                i=`echo $i -1 | bc`
                FILES2=/home/hophy/Simulation_results/continuum_ser_higg/serial_higgins_"$i"/*
                for g in $FILES2 
                 do
                  if [[ "$g" == *\.txt* ]] ||[[ "$g" == *\.in* ]] 
                  then
                   cp  $g  /home/hophy/data_Analyis/continum_results 
                  fi
               done
             done

        else
          rm  $f
        fi
     done

 #After all copy all files to the distination

        for i in {1..20} 
           do
            i=`echo $i -1 | bc`

              FILES2=/home/hophy/Simulation_results/continuum_ser_higg/serial_higgins_"$i"/*

              for g in $FILES2 
               do
                if [[ "$g" == *\.txt* ]] ||[[ "$g" == *\.in* ]]; 
                then
                  
                  cp  $g  /home/hophy/data_Analyis/continum_results 
                fi
           done
      done


#other choices 
  
     elif  [ "$option" == "del" ]; then

       echo "Delleting from Continuum folder is in progress ..."
                    
           FILES=/home/hophy/data_Analyis/continum_results/*

              for f in $FILES 
               do
                  rm  $f  
           done
     
    
   else 
    exit 1
fi



############################################

# Discrete  MANIPULATION PART
 
  elif  [ "$choice" == "D" ]; then

     echo -n "Do you want to COPY into or Delete from folder [ENTER]:"cp" for copy and "del" for delete  "
     echo -n
     read option

     if  [ "$option" == "cp" ]; then

        echo "Check if there is any files left in the distination ...."

        FILES=/home/hophy/data_Analyis/discrete_results/*
        DIR=/home/hophy/data_Analyis/discrete_results 

        for f in $FILES
         do
         if find $DIR -maxdepth 0 -empty | read v; then 
           
             echo "Empty dir , is fine to copy"; 
             echo "Copying into Continuum folder is in progress ..."
    
              for i in {1..20} 
               do
                i=`echo $i -1 | bc`
                FILES2=/home/hophy/Simulation_results/discrete_ser_higg/serial_higgins_"$i"/*
                for g in $FILES2 
                 do
                  if [[ "$g" == *\.txt* ]] ||[[ "$g" == *\.in* ]] 
                  then
                   cp  $g  /home/hophy/data_Analyis/discrete_results  
                  fi
               done
             done

        else
          rm  $f
        fi
     done

 #After all copy all files to the distination

        for i in {1..20} 
           do
            i=`echo $i -1 | bc`

              FILES2=/home/hophy/Simulation_results/discrete_ser_higg/serial_higgins_"$i"/*

              for g in $FILES2 
               do
                if [[ "$g" == *\.txt* ]] ||[[ "$g" == *\.in* ]]; 
                then
                  
                  cp  $g  /home/hophy/data_Analyis/discrete_results 
                fi
           done
      done


#other choices 
  
     elif  [ "$option" == "del" ]; then

       echo "Delleting from Continuum folder is in progress ..."
                    
           FILES=/home/hophy/data_Analyis/discrete_results/*

              for f in $FILES 
               do
                  rm  $f  
           done 
    else 
      exit 1
   fi


############################################

# Discrete  MANIPULATION PART
 
  elif  [ "$choice" == "R" ]; then

     echo -n "Do you want to COPY into or Delete from folder [ENTER]:"cp" for copy and "del" for delete  "
     echo -n
     read option

     if  [ "$option" == "cp" ]; then

        #ask user to provide parameters

        echo -n "how many systems do you want to process? ?   "
        echo -n 
        read number

      for ((i = 1; i <= number; i++))
        do  
     
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


         file1="$var1"_"$var2"_"$var3"_"$var4"_"$var5"

         array1[$i]=$file1

         echo " file "${array1[$i]}" "

         echo -n "dose this file belong to different conc , differnt Tign of diffrent size ?   "
         echo -n 
         read diff
          
         if  [ "$diff" == "conc" ]; then

         file2=/gs/scratch/hophy/data_Analyis/rough_results/diff_conc/"$var1"_"$var2"_"$var3"_"$var4"_"$var5"            
         folder=diff_conc 
         array2[$i]=$file2 
         array3[$i]=/gs/scratch/hophy/data_Analyis/rough_results/diff_conc
         array4[$i]=$folder

         elif  [ "$diff" == "tign" ]; then

         file2=/gs/scratch/hophy/data_Analyis/rough_results/diff_tign/"$var1"_"$var2"_"$var3"_"$var4"_"$var5"             
         folder=diff_tign 
         array2[$i]=$file2
         array3[$i]=/gs/scratch/hophy/data_Analyis/rough_results/diff_tign
         array4[$i]=$folder

         elif  [ "$diff" == "size" ]; then

         file2=/gs/scratch/hophy/data_Analyis/rough_results/diff_size/"$var1"_"$var2"_"$var3"_"$var4"_"$var5"             
         folder=diff_size 
         array2[$i]=$file2
         array3[$i]=/gs/scratch/hophy/data_Analyis/rough_results/diff_size
         array4[$i]=$folder

        fi 

      done
	
        DIR3=/gs/scratch/hophy/data_Analyis/*
        DIR4="/gs/scratch/hophy/data_Analyis"

        for ((i = 1; i <= number; i++))
         do
           if  [ -d "${array2[$i]}"  ]; then

            echo "folder  already exist ..."        
            if find ${array2[$i]} -maxdepth 0 -empty | read v; then 
            
            echo "Empty dir , is fine to copy"; 
            echo "Copying into Rough rsult folder is in progress ..."
    
             for j in {1..10}
               do
                 j=`echo $j -1 | bc`
                 FILES2=/gs/scratch/hophy/Simulation_results/rough_results/${array1[$i]}/serial_higgins_rough_"$j"/* 
                 echo $FILES2
                  for g in $FILES2 
                   do
                    if [[ "$g" == *\.txt* ]] ||[[ "$g" == *\.in* ]] 
                     then
                     cp  $g   ${array2[$i]}
                    fi
                 done
                  for p in $DIR3
                   do
                    if [[ "$p" == *\.cpp* ]] ||[[ "$p" == *\.h* ]]
                     then
                      cp  $p   ${array2[$i]}
                    fi
                 done
             done
             find "${DIR4}" -name "script*" | while read -r file
                do
                  cp "${file}" "${array2[$i]}"
             done
         else

         FILES=${array2[$i]}/*     

         for f in $FILES
           do
             rm $f
         done

          #After all copy all files to the distination

            for j in {1..10}
              do
                j=`echo $j -1 | bc`
                FILES2=/gs/scratch/hophy/Simulation_results/rough_results/${array1[$i]}/serial_higgins_rough_"$j"/* 
                  for g in $FILES2 
                   do
                    if [[ "$g" == *\.txt* ]] ||[[ "$g" == *\.in* ]] 
                     then
                     cp  $g   ${array2[$i]}
                     fi
                 done
                  for p in $DIR3
                   do
                    if [[ "$p" == *\.cpp* ]] ||[[ "$p" == *\.h* ]]
                     then
                     cp  $p   ${array2[$i]}
                    fi
                 done
             done
            find "${DIR4}" -name "script*" | while read -r file
              do
                cp "${file}" "${array2[$i]}"
            done
         fi
 
     elif [ ! -d "${array2[$i]}" ]; then
 
             echo -n
             echo "There is no such a file in destination"; 
             echo "Making the file first ..."
              
             cd "${array3[$i]}"

             echo -n
             mkdir "${array1[$i]}"
            
             echo "Dir just created , it's fine to copy"; 
             echo "Copying into Rough rsult folder is in progress ..."
    
            for j in {1..10}
             do
              j=`echo $j -1 | bc`
               FILES2=/gs/scratch/hophy/Simulation_results/rough_results/${array1[$i]}/serial_higgins_rough_"$j"/* 
                for g in $FILES2 
                  do
                   if [[ "$g" == *\.txt* ]] ||[[ "$g" == *\.in* ]] 
                   then
                      cp  $g   ${array2[$i]}
                   fi
               done
               for p in $DIR3
                  do
                    if [[ "$p" == *\.cpp* ]] ||[[ "$p" == *\.h* ]]
                     then
                     cp  $p   ${array2[$i]}
                    fi
               done
           done
            find "${DIR4}" -name "script*" | while read -r file
             do
              cp "${file}" "${array2[$i]}"
            done

         else
         echo "odd situation"
         fi


   # now compile and submit the jobs on cluster
 

       FILES=${array2[$i]}/*     

       for f in $FILES 
         do
          if [[ "$f" != *\.* ]] 
            then
            sed -i "s/ddd/${array4[$i]}/g"  $f
            sed -i "s/1_1_1_1/${array1[$i]}/g"  $f
          fi
       done

       cd ${array2[$i]}
	  g++  initial.cpp ;
       	  if [[ $? -eq 0 ]]; then
            pid[$i]=$(echo "sleep 10" | msub -q sw2 $f)
            echo ${pid[$i]}
         fi
    done #end of the first loop 

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

          for ((i = 1; i <= number; i++))
           do
           read -p "Press [Enter] key to kill the jobs one by one ..."
            canceljob ${pid[$i]}
         done
      fi

     else
       echo "Wrong Choice."
       exit 1
     fi


#other choices 
  
    elif  [ "$option" == "del" ]; then

        
        #ask user to provide parameters

        echo -n "how many systems do you want to process? ?   "
        echo -n 
        read number

      for ((i = 1; i <= number; i++))
        do  
     
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
         file1="$var1"_"$var2"_"$var3"_"$var4"_"$var5"
         file2=/gs/scratch/hophy/data_Analyis/rough_results/"$var1"_"$var2"_"$var3"_"$var4"_"$var5"       
         
         array1[$i]=$file1
         array2[$i]=$file2
 
        echo " file "${array1[$i]}" "
        done

         FILES=/gs/scratch/hophy/data_Analyis/rough_results/${array1[$i]}/*     
         for f in $FILES
         do
          rm $f
         done

     else
      echo "no delete no copy so what the keck !!!!"
      exit 1
    fi
############################################

else
echo "you entered the wrong choice"
exit 1
fi

