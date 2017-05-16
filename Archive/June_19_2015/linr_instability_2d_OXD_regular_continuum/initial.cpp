#include"Mddef.h"
#include"fundef.h"

/*****************************/
//using namespace std;

/********************************/

//Global Variable Definitions (For SIMULATION):  
                                                
int       len_x,len_y,conc_step;             // 
int       grid_x,grid_y;                     // 
int       **alpha;                           //          
int       Nx,nx,Ny,ny,Nstep;                 //
int       tot_time;                          //
double    t_step,x_step;                     //                 
double    To,Tg,Amp;                         //
double    **T;                               //                 
double    **C;                               //                
double    **Grad,**Grad_conc;                //
double    *max_temp,*max_temp_pos;           //
double    ave_pos,ave_max_temp;              //
double    **tc,**strgh,**delta,lambda;       //
//*******************************************

/****************/
void Setup()
{
  AllocArrays();
  Initialize();
}
/*********************************/

void AllocArrays()                  
{ 

 int i,j;

//for making sure if the Nx and Ny are integers

 Nx=int(grid_x*(1/x_step));
 Ny=int(grid_y*(1/x_step));
 Nstep=tot_time/t_step;
 conc_step=int(1/x_step);
 lambda = Ny*4.0;
//Mesh points including ghost points

 nx=Nx+2;
 ny=Ny+2;

//Temp , Concenteration and Gradiant arrays

 T= new double *[nx];
 C= new double *[nx];
 Grad = new double *[nx];
 Grad_conc = new double *[nx];
 max_temp_pos = new  double [Ny];
 max_temp = new  double [Ny];
 tc = new  double *[nx];
 alpha = new int *[nx];
 delta = new  double *[nx];
 strgh = new double *[nx];

  for(i=0;i<nx;i++){
      T[i]=new double [ny];
      C[i]=new double [ny];
      Grad[i]=new double [ny];
      Grad_conc[i]=new double [ny];
      tc[i]=new double [ny];
      alpha[i]=new int [ny]; 
      delta[i]=new double [ny];
      strgh[i]=new double [ny]; 
  }
}

/**********************Initialize each array*******************/

void Initialize(){

 int i,j,k,l,h,cntr;

   for(i=0;i<nx;i++){
       for(j=0;j<ny;j++){

            T[i][j]=To;
            C[i][j]=0.0;
            Grad[i][j]=0.0;
            Grad_conc[i][j]=0.0;
            tc[i][j]=0.0;
            alpha[i][j]=0;
            delta[i][j]=0.0;
            strgh[i][j]=0;
       }
   }
/*
//Ignite the cells for the first row (with the indice i=1)

   j=1;
    while(j<=Ny+1){
         l=Amp*cos(((2*M_PI*(j-1))/lambda));
         for(k=1;k<=l+1;k++){
          T[k][j]=4*Tg;
         } 
     j=j+1;
    }*/
}

/*******************************/


