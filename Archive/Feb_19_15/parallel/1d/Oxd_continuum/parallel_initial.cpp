//** file inclusion **/

#include"Mddef.h"
#include"fundef.h"

//Global Variable Definitions (For SIMULATION):  

//*********************************************//                                                
int    len_x;                                  //  
int    Nx,Nstep;                               //
int    grid_x;                                 //
int    tot_time;                               //
int    *alpha;                                 //
int    conc_step;                              //
double t_step,x_step;                          //                 
double To,Tg;                                  //
double *T;                                     //                 
double *C;                                     //                
double *Grad;                                  //              
double *tc;                                    //              
double temp_pos;                               //
double max_temp,max_temp_pos;                  //
double ave_pos,ave_max_temp;                   //
double *array_xline;
//*********************************************//

/********************/

void Setup(){
  AllocArrays();
  array_Init();
}

/********************/

void AllocArrays(){ 

 int i;

 Nx=int(grid_x*(1/x_step));
 Nstep=tot_time/t_step;
 conc_step=int(1/x_step);
 
 //Temp , Concenteration and Gradiant arrays

 T     = new double [Nx];
 C     = new double [Nx];
 Grad  = new double [Nx];
 tc    = new double [Nx];
 alpha = new int    [Nx];

}

/**********************Initialize each array*******************/

void array_Init(){

 int i,j,k;

  for(i=0;i<Nx;i++){

       T[i]=To;
       C[i]=0.0;
       Grad[i]=0.0;
       tc[i]=0.0;
       alpha[i]=0.0;
     }
  
  //Kinetic model - Ignite the cells for the first row (with the indice i=1)
  /*** For Oxidation model 1.6 should be replaced by Tg ***/
    
  for(k=0;k<=40;k++){
      T[k]=2*Tg;}
}

/*******************************/



