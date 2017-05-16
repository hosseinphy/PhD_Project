#include"Mddef.h"
#include"fundef.h"

/*****************************/
//using namespace std;

/********************************/

//Global Variable Definitions (For SIMULATION):  
                                                
int       len_x,len_y;                       // 
int       grid_x,grid_y;                     // 
int       **alpha;                           //          
int       Nx,nx,Ny,ny,Nstep;                 //
int       tot_time;                          //
double    t_step,x_step;                     //                 
double    To,Tg;                             //
double    **T;                               //                 
double    **C;                               //                
double    **Grad,**Grad_conc;                //
double    *max_temp,*max_temp_pos;           //
double    ave_pos,ave_max_temp;              //
double    **tc;                              //

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

 /*Nx=int(len_x/x_step);
 Ny=int(len_y/x_step);*/
 Nx=int(grid_x*(1/x_step));
 Ny=int(grid_y*(1/x_step));
 Nstep=tot_time/t_step;

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

  for(i=0;i<nx;i++){
      T[i]=new double [ny];
      C[i]=new double [ny];
      Grad[i]=new double [ny];
      Grad_conc[i]=new double [ny];
      tc[i]=new double [ny];
      alpha[i]=new int [ny]; 
  }
}

/**********************Initialize each array*******************/

void Initialize(){

 int i,j,k;

   for(i=0;i<nx;i++){
       for(j=0;j<ny;j++){

            T[i][j]=To;
            C[i][j]=0.0;
            Grad[i][j]=0.0;
            Grad_conc[i][j]=0.0;
            tc[i][j]=0.0;
            alpha[i][j]=0;
       }
   }

//Ignite the cells for the first row (with the indice i=1)

   for(k=1;k<=20;k++){
       for(j=1;j<=Ny;j++){           
          T[k][j]=1.6;
       }
   }

//set the surroding parameters
/*
  //Corners

     T[0][0]=To;
     T[0][Ny+1]=To;
     T[Nx+1][0]=To;
     T[Nx+1][Ny+1]=To;

     C[0][0]=0.0;
     C[0][Ny+1]=0.0;
     C[Nx+1][0]=0.0;
     C[Nx+1][Ny+1]=0.0;

  //Bottom-top planes 

   for(j=1;j<=Ny;j++){
 
       T[0][j]=To;
       T[Nx+1][j]=To;

       C[0][j]=0.0;
       C[Nx+1][j]=0.0;
   }

  //Left-Right planes

   for(i=1;i<=Nx;i++){

       T[i][0]=To;
       T[i][Ny+1]=To;

       C[0][j]=0.0;
       C[Nx+1][j]=0.0;
   }*/
}

/*******************************/


