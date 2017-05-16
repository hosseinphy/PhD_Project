#!/bin/bash
#theis file gives a user a chance to copy all date from Simulation_result in /gs/scratch dir \
# to data analysis folder or delete all files in data analysis folder
clear
 echo -n "Enter your choice and press [ENTER]: "kint" for Kinetic  "oxd" for Oxcidation & "arh"for Arhenius "
 echo -n
 read choice
 
 var0=$choice

   if [[ "$choice" == "kint" ]]||[[ "$choice" == "oxd" ]]||[[ "$choice" == "arh" ]]
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

         file1="$var1"_"$var2"_"$var3"_"$var4"_"$var5"

         echo " file "$file1" "
         echo -n
         echo "copying from folder "$var1"_"$var2"_"$var3"_"$var4"_"$var5" "

         echo "Check if there is any files left in the distination ...."

         FILES=/home/hophy/DATA_ANALYSIS/1d_serial/"$var0"_model/*
         DIR=/home/hophy/DATA_ANALYSIS/1d_serial/"$var0"_model

         for f in $FILES
           do
           # if find $DIR -maxdepth 0 -empty | read v; then 
          
           #  echo "Empty dir , is fine to copy"; 
           #  echo "Copying into Continuum folder is in progress ..."
   
              for i in {1..200} 
               do
                i=`echo $i -1 | bc`
                FILES2=/gs/scratch/hophy/Simulation_results/1d_serial_"$var0"/"$var1""_""$var2""_""$var3""_""$var4""_""$var5"/1dserial_"$var0"_"$i"/*

                for g in $FILES2 
                 do
                  if [[ "$g" == *\.txt* ]]
                  then
                   cp  $g   /gs/scratch/hophy/DATA_ANALYSIS/1d_serial/"$var0"_model      
                            # /home/hophy/DATA_ANALYSIS/1d_serial/"$var0"_model          
                 fi
               done
             done

       # else
       #   rm  $f
       # fi
     done

 #After all copy all files to the distination

     #   for i in {1..200} 
     #      do
     #       i=`echo $i -1 | bc`
     #         FILES2=/home/hophy/Simulation_results/continuum_ser_higg/serial_higgins_"$i"/*
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
           FILES=/gs/scratch/hophy/DATA_ANALYSIS/1d_serial/"$var0"_model          
           #FILES=/home/hophy/DATA_ANALYSIS/1d_serial/"$var0"_model/*

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

