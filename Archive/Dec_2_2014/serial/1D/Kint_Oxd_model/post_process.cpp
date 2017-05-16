#include <iostream>
#include "fundef.h"
#include <iomanip>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <signal.h>
#include <sys/time.h>
#include <string>
#include <sstream>
#include <assert.h>
#include <unistd.h>
#include <limits>
#include "in_namelist.h"


/*****************************/
using namespace std;
/*****************************/

 //pointer to the files

     ifstream input   ("frontdata.txt");
     ofstream OUTPUT  ("position.txt");
     ofstream OUTPUT_2("velocity.txt");
     ofstream OUTPUT_3("max_temp.txt");

     //Reading from  file pointers
     ifstream myReadFile;
     ifstream myReadFile2;
     ifstream myReadFile3;

     //Writing into file pointers
     ofstream myWriteFile;
     ofstream myWriteFile2;
     ofstream myWriteFile3;
     ofstream myWriteFile4;

 //////////////////////////


/**********************************************///
//Global Variable Definitions:  
                                                //
int    printfreq, printfrontpos,printconcfreq;  //
int    t;                                       //
int    f_n,t_n,filesize,front_size;             //
int    len_x;                                   // 
int    Nx,nx,Nstep;                             //
int    tot_time;                                //
int    data_size;                               //
double Ctshold,Gamma,q_T,Lambda_1,beta,Diff;    //
double tou_c;                                   //
double Q,cap_Gamma,TC,TL,T_bar,mass_Diff,qo;    //
long   seed;                                    //
double t_step,x_step;                           //                 
double To,Tg;                                   //
double /**output*,*/*T_max,*T_min;/*position*/  //                 
double *ave_pos,*ave_time,*maxt;                //                
char   filename1[15];                           //
char   filename2[15];                           //
char   filename3[15];                           //
char   filename4[15];                           //
char   filename5[15];                           //

//**********************************************


/**********************************************/// 

     //each array file size 

      int fun_num=2;
     
     //Velocity fun operation id:
      int ID=1;

     #define Sqr(x)  ((x)*(x))

/****Parameter Reading from the input file*******/

NameList nameList[] = {

  NameI (t_n),
  NameI (f_n),
  NameI (filesize),
  NameI (front_size),
  NameR (Tg),
  NameR (TL),
  NameR (TC),
  NameR (To),
  NameR (Q),
  NameR (qo),
  NameR (tou_c),
  NameR (Gamma),
  NameR (cap_Gamma),
  NameI (seed),
  NameR (Ctshold),
  NameR (t_step),
  NameR (x_step),
  NameR (beta),
  NameR (Diff),
  NameR (mass_Diff),
  NameI (len_x),
  NameI (tot_time),
  NameI (printfreq),
  NameI (printfrontpos),
  NameI (printconcfreq),

};

/***************************/

/***************************/

      //Main Function                 
                             
/***************************/

int main (int argc, char ** argv)
{

 system("clear");             
 GetNameList (argc, argv);
 PrintNameList (stdout);
 alloc();
 read_data();                
 Vel_calc();
 Temp_deri_calc();
 dealloc();
 return 0;
}

/***************************/


/********  FUNCTIONS ***********/
//Allocating Arrays

void alloc(){

 /*Temperature \ Concenteration 2D array Initialization*/

 int i,j;

//for making sure if the Nx and Ny are integers

 Nx=int(len_x/x_step);
 Nstep=tot_time/t_step;

//Each array.txt file number of mesh points 
 
 data_size=Nx;

/*//Temp , Concenteration and Gradiant arrays

 T= new double [Nx];
 C= new double [Nx];
*/
/***************************/
//array holding numner of files and each content

//output= new double*[filesize];
//this array 'll contain all positions for
//all files at which max temp is occued
//position= new double[filesize];
//time and position from frontdata.txt

ave_pos=new double[front_size];
ave_time=new double [front_size];
maxt=new double [front_size];
//min and max temperature at each frontdata.txt file
T_max = new double[front_size];
T_min = new double[front_size];

/*    for (i =0; i < filesize; i++) {
   
        output[i] = new double[data_size];
    }*/
}

/******  END  *******/

/**********************/
//Read data from files

void read_data(){
   
   int i,j;
   double pos[front_size],time[front_size];
   char   c[front_size];

/**********************************/
/*                                */
/*   Reading from array.txt files */  
/*                                */
/**********************************/

//Temperature profile files from sim
    
/*   string st1 = "array";
   string ext = ".txt";
   string filename;

    int k=0;
    char trash;

            for (int i=0; i<filesize; i++){

		stringstream ss;
		ss<<i;
		filename = st1 + ss.str() +ext;
                
                myReadFile.open(filename.c_str());

                 if (myReadFile.is_open()) {
                      
                   while (!myReadFile.eof()) {
                         
                     myReadFile >>output[i][k];

                       ++k;
                    }
                 }

            myReadFile.close();

            k=0;
         }*/
//////////////////////////////////

   /***** Reading from Frontdata.txt *****/

   char s3[front_size],s4[front_size]; 

   sprintf(filename3,"front_zeroflux%0.5f.txt",To);
   //cout<<filename3<<endl;
   myReadFile3.open(filename3);

   if (myReadFile3.is_open()) {
      for(i=0;i<front_size;i++){
         myReadFile3>>ave_pos[i]>>s3[i]>>maxt[i]>>s4[i]>>ave_time[i];

      }  
   }
  
    myReadFile3.close();

 }//end of reading files


/*************************   END  ****************************/

/**********************/
//Velocity calculation

void Vel_calc(){

 char s[front_size]; 
 long double  velocity[front_size],V[front_size],T[front_size];
 int i,k;

   for(i=0;i<front_size;i++){

      velocity[i]=((ave_pos[i+1]-ave_pos[i])/(ave_time[i+1]-ave_time[i]));
    }

//window filltering

 int L=1; // Average Interval
 int j=0; // counter

//Smoothing Procedure

for(i=0; i<front_size; i++){
    
    V[i]=0;//Velocity*time
    T[i]=0;//Time_sum
  }

    while(j<front_size){
    
       if(j>=(L) & j<=(front_size-L)){

          for(k=j-L;k<j+L;k++){

             V[j]+=velocity[k];

          }

       }

    else if (j>(front_size-L)){

        for (k=j-L;k<front_size;k++){
            
             V[j]+=velocity[k];
            
            }
     }
        
      else {

         for(k=0;k<j+L;k++){

           V[j]+=velocity[k];
      
         }
     }
       
         OUTPUT_2<<" \t "<<V[j]/L<< "\t" <<ave_time[j]<<endl;

         j++;
   }

}

/*************************   END  ****************************/


/**********************/
//1st and 2nd time derivative of Temperature

void Temp_deri_calc(){

 char s_t[front_size]; 
 long double  fst_dri[front_size],scd_dri[front_size];
 int i,k;

  //first derivative calculation

   for(i=0;i<front_size;i++){

       if(i==0){

        fst_dri[i]=-100;
       }
       else {

       fst_dri[i]=((maxt[i+1]-maxt[i])/(ave_time[i+1]-ave_time[i]));
       scd_dri[i]=((maxt[i+1]+maxt[i-1]-2*maxt[i])/(Sqr(ave_time[i+1]-ave_time[i])));
      }
    }

  //output the max and min values of tempreture (Extreme tempretures)

   for(i=0;i<front_size;i++){
       if(fst_dri[i]==0.0 && scd_dri[i]<0){
          T_max[i]=maxt[i];
       }
       else if (fst_dri[i]==0.0 && scd_dri[i]>0){
          T_min[i]=maxt[i];
       }
       else{
          T_max[i]=-100;
          T_min[i]=-100;
       }
    }


   for(i=0;i<front_size;i++){

        if(T_max[i]!=-100 && T_min[i]!=-100){   

         //OUTPUT_3<<" \t "<<i<<" \t "<<fst_dri[i]<< "\t" <<scd_dri[i]<<endl;    
         OUTPUT_3<<" \t "<<i<<" \t "<<T_max[i]<< "\t" <<T_min[i]<<endl;    
        }
       /* else { 
         continue;
        }*/
    }
}
/*************************   END  ****************************/

//Deallocating Arrays
/****************************************/

void dealloc(){


int j;

/*  for (int j=0;j<filesize;j++) {

    delete[] output[j];
  }
   delete[] output;
*/
   delete[] T_min;
   delete[] T_max;
   delete[] ave_pos;
   delete[] ave_time;
   delete[] maxt;

 }

/*******************************************/

#include"in_namelist.c"


