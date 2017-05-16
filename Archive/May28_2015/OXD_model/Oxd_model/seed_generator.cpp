

#include<iomanip>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <signal.h>
#include <sys/time.h>
#include <assert.h>
#include <string.h>
#include "in_namelist.h"
#include <unistd.h>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <algorithm>



char filename [BUFSIZ];

unsigned long  Seed_gen(){

    unsigned int  ff=0;

     struct timeval time; 
     gettimeofday(&time,NULL);
     ff=(time.tv_sec * 100) + (time.tv_usec / 100);
     return ff;

     //write seed to file to be used by all systems in ensemble


 }


int main(){

     sprintf(filename, "seed_number.dat");
      FILE * rann = fopen(filename, "w");           
        fprintf(rann, "%10lu \n",Seed_gen());          

     fclose(rann);
return 0;

}
