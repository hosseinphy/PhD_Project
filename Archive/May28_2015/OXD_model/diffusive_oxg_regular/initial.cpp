#include"Mddef.h"
#include"fundef.h"
/*****************************/

//Global Variable Definitions (For SIMULATION):  
/******************************************///                                                
int       len_x,grid_x,conc_step;           //   
int       *alpha;                           //          
int       Nx,nx,Nstep;                      //
int       tot_time;                         //
double    t_step,x_step;                    //                 
double    To,Tg;                            //
double    *T;                               //                 
double    *C;                               //                
double    *co2;                             //                
double    *Grad,*Grad_co2;                  //
double    ave_pos,*endiff;                  //
double    max_temp;                         //
double    temp_pos;                         //
double    *tc;                              //
double    *strn;                            //
/******************************************///
/******************************************///
void Setup()
{
  AllocArrays();
  Initialize();
}
/******************************************///
/******************************************///
void AllocArrays()                  
{ 

 int i;

 //for making sure if the Nx and Ny are integers
 
//Nx=int(len_x/x_step);
 Nx=int(grid_x*(1.0/x_step));
 Nstep=tot_time/t_step;
 conc_step=int(1.0/x_step);
//Mesh points including ghost points
 nx=Nx+2;
 //Temp , Concenteration and Gradiant arrays

 T= new double [nx];
 C= new double [nx];
 co2= new double [nx];
 Grad = new double [nx];
 Grad_co2 = new double [nx];
 tc = new  double [nx];
 strn = new  double [nx];
 alpha = new int [nx];
}
/******************************************///
/***        Initialize each array        ***/

void Initialize(){

 int i,j,k;

   for(i=0;i<nx;i++){
     T[i]=To;
     C[i]=0.0;
     co2[i]=0.0;
     Grad[i]=0.0;
     Grad_co2[i]=0.0;
   }

 //Ignite the cells for the first row (with the indice i=1)
   for(k=2;k<=10;k++){  
      T[k]=2*Tg;
    }

 //set the surroding parameters
 //Corners - ghost points

/*     T[0]=To;
     T[Nx+1]=To;
     C[0]=0.0;
     C[Nx+1]=0.0;*/
}


/*********          END       ***********///

