//** file inclusion **/

#include"Mddef.h"
#include"fundef.h"

//Global Variable Definitions (For SIMULATION):  

//*********************************************//                                                
int    len_x,len_y;                            //  
int    Nx,Ny,Nstep;                            //
int    grid_x,grid_y;                          //
int    tot_time;                               //
int    **alpha;                                //
double t_step,x_step;                          //                 
double To,Tg;                                  //
double **T;                                    //                 
double **C;                                    //                
double **Grad;                                 //              
double **tc;                                   //              
double *temp_pos;                              //
double *max_temp,*max_temp_pos;                //
double ave_pos,ave_max_temp;                   //
double **array_xline;
//*********************************************//

/********************/

void Setup(){
  AllocArrays();
  array_Init();
}

/********************/

void AllocArrays(){ 

 int i;
 
 //Mesh points W/O ghost points
 // make grid points even along x direction
 /*Nx=int(len_x/x_step);
 Ny=int(len_y/x_step);*/
 Nx=int(grid_x*(1/x_step));
 Ny=int(grid_y*(1/x_step));
 Nstep=tot_time/t_step;

 //Temp , Concenteration and Gradiant arrays

 T     = new double *[Nx];
 C     = new double *[Nx];
 Grad  = new double *[Nx];
 tc    = new double *[Nx];
 alpha = new int    *[Nx];
 max_temp_pos = new  double [Ny];
 max_temp     = new  double [Ny];
 array_xline  = new  double*[Ny];

 for (i=0;i<Ny;i++){
  array_xline[i] = new double [Nx];}

 for(i=0;i<Nx;i++){
    T[i]    = new double [Ny];
    C[i]    = new double [Ny];
    Grad[i] = new double [Ny];  
    tc[i]   = new double [Ny]; 
    alpha[i]= new int    [Ny];
  }
}

/**********************Initialize each array*******************/

void array_Init(){

 int i,j,k;

  for(i=0;i<Nx;i++){
     for(j=0;j<Ny;j++){
       T[i][j]=To;
       C[i][j]=0.0;
       Grad[i][j]=0.0;
       tc[i][j]=0.0;
       alpha[i][j]=0.0;
     }
  }
  
//Kinetic model - Ignite the cells for the first row (with the indice i=1)
/*** For Oxidation model 1.6 should be replaced by Tg ***/
    
  for(k=0;k<20;k++){
       for(j=0;j<Ny;j++){          
          T[k][j]=2*Tg;
       }
   }
}

/*******************************/



