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
int    conc_step;                              //
double t_step,x_step;                          //                 
double To,Tg;                                  //
double **T;                                    //                 
double **C,**co2;                              //                
double **Grad,**Grad_co2;                      //              
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

 Nx=int(grid_x*(1/x_step));
 Ny=int(grid_y*(1/x_step));
 Nstep=tot_time/t_step;
 conc_step=int(1/x_step);
 //Temp , Concenteration and Gradiant arrays

 T     = new double *[Nx];
 C     = new double *[Nx];
 co2   = new double *[Nx];
 Grad  = new double *[Nx];
 tc    = new double *[Nx];
 alpha = new int    *[Nx];
 max_temp_pos = new  double [Ny];
 max_temp     = new  double [Ny];
 array_xline  = new  double*[Ny];
 Grad_co2  = new double *[Nx];

 for (i=0;i<Ny;i++){
  array_xline[i] = new double [Nx];}

 for(i=0;i<Nx;i++){
    T[i]    = new double [Ny];
    C[i]    = new double [Ny];
    co2[i]  = new double [Ny];
    Grad[i] = new double [Ny];  
    tc[i]   = new double [Ny]; 
    alpha[i]= new int    [Ny];
    Grad_co2[i] = new double [Ny];  
  }
}

/**********************Initialize each array*******************/

void array_Init(){

 int i,j,k;


  for(i=0;i<Nx;i++){
     for(j=0;j<Ny;j++){
       T[i][j]=To;
       C[i][j]=0.0;
       co2[i][j]=0.0;
       Grad[i][j]=0.0;
       Grad_co2[i][j]=0.0;
       tc[i][j]=0.0;
       alpha[i][j]=0.0;
     }
  }
  
//Kinetic model - Ignite the cells for the first row (with the indice i=1)
/*** For Oxidation model 1.6 should be replaced by Tg ***/

  i=0;
   while(i<Nx){
     j=0;
      while(j<Ny){
        C[i][j]=1.0;
        j=j+conc_step;
      }
    i=i+conc_step;
  }
    
  for(k=0;k<3;k++){
       for(j=0;j<Ny;j++){          
          T[k][j]=Tg;
       }
   }
}

/*******************************/



