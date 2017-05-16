/*  2D version of the code 
    Modified at Sep 23 , 2014
    In this version I used the 
    dimensionless form of equations                 
    there is an option for 

  - heat dissipation  
  - Temperature Update
  - Concenteration Update
  - Temperature Boundary Condition
  - Concenteration Boundary Condition 
   
 Updated on Wed , July 9th , 2014   */    

/********************************************/

//File Inclusion
/*********************/
 #include"initial.cpp"
/********************/


/**********************************************///
//Global Variable Definitions:                  //
                                                //
//pointer to the files                          //
std::ifstream myReadFile;                       //
std::ifstream myReadFile2;                      //
                                                //
double Ctshold,Gamma,q_T,Lambda_1,beta,Diff;    //
double tou_c;                                   //
double Q,cap_Gamma,TC,TL,T_bar,mass_Diff,qo;    //
double resit_pos,resit_temp,resit_time;         // 
long   seed;                                    //
int    printfreq, printfrontpos,printconcfreq;  //
int    t,ign_time_counter,ign_time;             //
int    f_n,t_n,filesize,front_size;             //
int    t_resume;                                //
char   bound_option;                            //
char   filename[15];                            //
char   filename2[15];                           //
char   filename3[15];                           //
char   filename4[15];                           //
char   filename5[15];                           //
char   filename6[15];                           //
char   filename7[15];                           //
char   filename8[15];                           //
/**********************************************/// 


/****Parameter Reading from the input file*******/

NameList nameList[] = {

  NameI (t_n),
  NameI (f_n),
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
  NameI (grid_x),
  NameI (grid_y),
  NameI (tot_time),
  NameI (printfreq),
  NameI (printfrontpos),
  NameI (printconcfreq),

};


////////////////////////////////////
/*****        Main loop      ******/
///////////////////////////////////

int main (int argc, char ** argv)
{
  
 system ("clear");
 GetNameList (argc, argv);
 PrintNameList (stdout);
 Setup();
 Ran_Conc();

 sprintf(filename2,"frontname%0.5f%0.6f.txt",t_step,To);
 FILE * frontfile = fopen(filename2, "w");
 fprintf(frontfile, "");
 fclose(frontfile);

   //** Simulation Step **//

         for(t=0;t<Nstep;t++){ 

          /******* laplacian calculation ******/      
          grad();
          /*************************************/
         
          /*******   update temperature   ******/      
          Calc_higgins();
          //Calc_prov_1(); // RR  (T_bat)^(3/2)*exp(-1/T_bar)      
          //Calc_prov_2(); // Nik's -explosive crystalisation       
          //Calc_prov_3();   // Nik's -explosive crystalisation   
          /*************************************/


          /*******  update concenteration ******/      
          // update_conc_1();////reaction rate  (T_bat)^(3/2)*exp(-1/T_bar)
          // update_conc_2();// Nik's -explosive crystalisation
          //update_conc_3();// Nik's -explosive crystalisation                        
          /*************************************/

          /******   tempreture boundary   ******/      
          Mixed();
          //Dissipation();
          //periodic();
          //zero_flux_boundary();  
          /*************************************/

          /******  concenteration boundary *****/      
          //conc_boundary();
          /*************************************/

          /**********   printing step  *********/      
          //Temp_write();
          front();
          //Conc_write();
          /*************************************/

        }//end of simulation step

     Dealloc();
     return 0;
 }//end of main loop

  

//-----FUNCTIONS-------//

/*******Randon number Generator***/

void Ran_Conc(){

double number ;
 int i,j;

  i=1;
   while(i<=Nx){
    j=1;
     while(j<Ny){
       C[i][j]=1.0;
       j=j+conc_step;
     }
    i=i+conc_step;
   }

  i=1;
   while(i<=Nx){
    j=1;
     while(j<=Ny){
       if(C[i][j]==1.0){
         strgh[i][j]=1./(x_step*x_step);
         strgh[i][j+1]=1./(x_step*x_step);
         strgh[i+1][j]=1./(x_step*x_step);
         strgh[i+1][j+1]=1./(x_step*x_step);
         j=j+2;
       }
       else{
         strgh[i][j]=0.0;
         j=j+1;
       }
     }
   i=i+2;
  }
/*
std::ofstream output("conc.dat");

   for(i=1;i<=Nx;i++){  
     for(j=1;j<=Ny;j++){  
      output<<i<<"\t"<<j<<"\t"<<strgh[i][j]<<"\n";
     }
   }
*/
   /*for(i=1;i<=Nx;i++){  
     for(j=1;j<=Ny;j++){  
         C[i][j]=1.0;
      } 
    }*/

   
/* std::ofstream wrt ("int_data_serial.dat");

 for (i=0;i<nx;i++){
   wrt<<i<<"\n";
   for (j=0;j<ny;j++){
    wrt<<j<<"\t"<<T[i][j]<<"\t"<<C[i][j]<<"\t"<<Grad[i][j]<<"\n";
   }
   wrt<<"\n\n";
 }

  
 srand48(seed);

  for(i=1;i<=Nx;i++){  
    for(j=1;j<=Ny;j++){  
      number=drand48();
        if (number<=Ctshold){ 
         C[i][j]=1.0;}       
        else { 
         C[i][j]=0.0;       
       }  
     }
   }*/  
 }
/************** END ***************/

/********Grdaient Function*********/

void grad(){
 
int i,j;

 //this is 9 point stencil (including next nearest neghibours)
    for(i=1;i<=Nx;i++){
         for (j=1;j<=Ny;j++){ //here I excluded the first (j=1) and last (j=Nyp-1) horizental planes
           Grad[i][j] = (0.5*(T[i+1][j]+T[i][j+1]+T[i-1][j]+T[i][j-1])
                           +0.25*(T[i+1][j+1]+T[i-1][j+1]+T[i+1][j-1]+T[i-1][j-1])
                                -3*(T[i][j]))/(Sqr(x_step));
         }
     }

    for(i=1;i<=Nx;i++){  
         for (j=1;j<=Ny;j++){ //here I excluded the first (j=1) and last (j=Nyp-1) horizental planes
              Grad_conc[i][j] = 0.0;// ((C[ip][j]+C[i][jp])+(C[im][j]+C[i][jm])-4*(C[i][j]))/(Sqr(x_step));
         }
     }
}

/************** END ***************/

/***** Main Function *****/

void Calc_prov_1(){

 int i,j;

     for(i=1;i<=Nx;i++){
         for(j=1;j<=Ny;j++){
           T_bar=T[i][j]/Tg;
           q_T= pow((T_bar),beta)*(exp(-1./T_bar));
           T[i][j]=T[i][j]+(t_step*(Diff*Grad[i][j]+Lambda_1*q_T*C[i][j]-Gamma*(T[i][j]-To)));
 
         } 
     }
 }

/////////////////////

void Calc_prov_2(){

 int i,j;
 double g_1;
 double roo_1;

    roo_1=mass_Diff/Diff;
    g_1=Gamma/cap_Gamma;

     for(i=1;i<=Nx;i++){        
          for(j=1;j<=Ny;j++){

             q_T= Q*(exp(-(Tg/(T[i][j]))))*(1.0-(exp((-1.0*TL)*((1./T[i][j])-(1./TC)))));
             T[i][j]=T[i][j]+t_step*(Grad[i][j]-(T[i][j]-To)+(g_1*q_T*C[i][j])-(roo_1*Grad_conc[i][j]));
             //printf ("isnan(T[i]): %d\n",isnan(T[i][j]));
          }
      }   
  }

////////////////////

void Calc_prov_3(){

 int i,j;

 for(i=1;i<=Nx;i++){
   for(j=1;j<=Ny;j++){
     q_T= Q*(exp(-(Tg/(T[i][j]))))*(1.0-(exp((-1.0*TL)*((1./T[i][j])-(1./TC)))));
     T[i][j]=T[i][j]+t_step*((Diff*Grad[i][j])-(cap_Gamma*(T[i][j]-To))+(qo*Gamma*q_T*C[i][j])-(qo*mass_Diff*Grad_conc[i][j]));
     //printf ("isnan(T[i][j]): %d\n",isnan(T[i][j]));     
    } 
  }  
}

/*********** HIGGINS *************/

/***** Main Function *****/

void Calc_higgins(){

int i,j;
	  
       for(i=1;i<=Nx;i++){
     
           for (j=1;j<=Ny;j++){ //here I excluded the first (j=1) and last (j=Nyp-1) horizental planes
               if(tc[i][j]>=0){                    
                    if(T[i][j]>=Tg){                  
                            if(tc[i][j]== 0.0){ // those reactants which have a time equall zero are possible candidate  for buring                       
                                  tc[i][j]=(t*t_step)+tou_c; // set the burning time
                                  alpha[i][j]=1;             // turn on the source
                            }
                            else if(tc[i][j]>(t*t_step)){    //particle is still burning
                                   tc[i][j]=tc[i][j];
                                   alpha[i][j]=1;                              
                             }           
                            else {
                                  tc[i][j] = -1.0;           //particle has been depleted
                                  alpha[i][j]=0;             //turn off the source
                               }
                          }

                    else if(T[i][j]<Tg){                         
                              if(tc[i][j]==0.0){      
                                 tc[i][j]=0.0;
                                 alpha[i][j]=0;
                              }
                              else if(tc[i][j]>(t*t_step)){  //source is still combusting even if its temperature is less that T_ign 
                                 tc[i][j]=tc[i][j];
                                 alpha[i][j]=1;
                              }
                              else {                            
                                tc[i][j]=-1;
                                alpha[i][j]=0;
                             }
                     }
               }
         
              else {
        
              tc[i][j]=-1;
              alpha[i][j]=0;
           }
        
    }
}

 //Updating tempreture afterwards

        for(i=1;i<=Nx;i++){
     
            for (j=1;j<=Ny;j++){ //here I excluded the first (j=1) and last (j=Nyp-1) horizental planes    

                  T[i][j]=T[i][j]+(t_step*(Grad[i][j]+(1/tou_c)*alpha[i][j]*strgh[i][j]/*C[i][j]*/));

            }
       }
 }

/************** END ***************/

/*********Boundary Function********/

void zero_flux_boundary(){

 int i,j;

     for(j=1;j<=Ny;j++){
        T[0][j]=T[2][j];
        T[Nx+1][j]=T[Nx-1][j];
     }

     for(i=1;i<=Nx;i++){
        T[i][0]=T[i][2];
        T[i][Ny+1]=T[i][Ny-1];
     }
 }

/************** END ***************/

 void Dissipation(){
 
  int i,j;

     for(j=1;j<=Ny;j++){

        T[0][j]=(To+(T[1][j]/(cap_Gamma*x_step)))/(1+1/(cap_Gamma*x_step));
        T[Nx+1][j]=(To+(T[Nx][j]/(cap_Gamma*x_step)))/(1+1/(cap_Gamma*x_step));
     }

     for(i=1;i<=Nx;i++){

        T[i][0]=(To+(T[i][1]/(cap_Gamma*x_step)))/(1+1/(cap_Gamma*x_step));
        T[i][Ny+1]=(To+(T[i][Ny]/(cap_Gamma*x_step)))/(1+1/(cap_Gamma*x_step));
     }
}
/************** END ***************/

/*****Periodic Boundary Function******/

void periodic(){

 int i,j;
 
   //Periodic Boundary at 2 ends
    
      for(j=1;j<=Ny;j++){

        T[0][j]=T[Nx][j];
        T[Nx+1][j]=T[1][j];
      }

      for(i=0;i<=Nx+1;i++){
     
        T[i][0]=T[i][Ny];
        T[i][Ny+1]=T[i][1];
      }
 }

/************** END ***************/

/*****Mixed Boundary Function******/

void Mixed(){

 int i,j;
 
   //Periodic @ 2side walls and zeroflux @ 2 end walls 

    for(j=1;j<=Ny;j++){
     T[0][j]=T[2][j];
     T[Nx+1][j]=T[Nx-1][j];
    }

    for(i=0;i<=Nx+1;i++){     
     T[i][0]=T[i][Ny];
     T[i][Ny+1]=T[i][1];
   }
 }

/************** END ***************/

 void update_conc_1(){
  int i,j;
   
   for(i=1;i<=Nx;i++){           
      for(j=1;j<=Ny;j++){  
         
        T_bar=T[i][j]/Tg; 
        q_T= pow((T_bar),beta)*(exp(-1./T_bar));
        C[i][j]=C[i][j]-t_step*(q_T*C[i][j]);
      }
   }
}

////////////////////////

 void update_conc_2(){

  int i,j;
  double g_2;
  double roo_2;

    roo_2=mass_Diff/Diff;
    g_2=Gamma/cap_Gamma;

    for(i=1;i<=Nx;i++){            
       for(j=1;j<=Ny;j++){            
      
          q_T= Q*(exp(-(Tg/(T[i][j]))))*(1.0-(exp((-1.0*TL)*((1./T[i][j])-(1./TC)))));
          C[i][j]=C[i][j]+t_step*((roo_2*Grad_conc[i][j])+(-1.0*g_2*q_T*C[i][j]));
       }
    }
 }

////////////////////////

 void update_conc_3(){

 int i,j;

   for(i=1;i<=Nx;i++){
       for(j=1;j<=Ny;j++){
           q_T= Q*(exp(-(Tg/(T[i][j]))))*(1.0-(exp((-1.0*TL)*((1./T[i][j])-(1./TC)))));
           C[i][j]=C[i][j]+t_step*((mass_Diff*Grad_conc[i][j])+(-1.0*Gamma*q_T*C[i][j]));
       }
    }
 }
/********* Concentertaion Boundary condition ***********/

void conc_boundary(){

 int i,j;

     for(j=1;j<=Ny;j++){
        C[0][j]=C[2][j];
        C[Nx+1][j]=C[Nx-1][j];
     }

     for(i=1;i<=Nx;i++){
        C[i][0]=C[i][2];
        C[i][Ny+1]=C[i][Ny-1];
     }
 }

/********** END ***********/

/*********Writing Functions************/

void Temp_write(){

   int num;
   int a,b;

     if (t%printfreq == 0) {			
        sprintf(filename, "array%d.txt", t/printfreq);
          FILE * file = fopen(filename, "w"); 						
            for (a = 1; a <=Nx; a++) {
	       for (b = 1; b <=Ny; b++) {			
                 fprintf(file, "%f \n", T[a][b]);
	       }
            }	         
       fclose(file);
      }
}

/************** END ***************/

/******Front position Calculation**********/

void front(){

    double  MAX1=0.0;
    double  temp_max=0.0;
    double  temp_pos;
    double  avefrontpos=0.0;
    int i,j;

        for(j=1;j<=Ny;j++){                  
          i=Nx;
           while (i>=1){
            if(i!=Nx && i!=1){
              if(T[i][j]>T[i-1][j] && T[i][j]>T[i+1][j]){
                 MAX1=T[i][j];
                 temp_pos=i;
                 break;
               }
            }
            else {                       
              if(T[Nx][j]>=MAX1){
                 MAX1 =T[Nx][j];
              } 
              if(T[1][j]>MAX1){
                 MAX1=T[1][j];
              }
            }
           i=i-1;
          }

          max_temp[j-1]=MAX1;
          max_temp_pos[j-1]=temp_pos;	     
          avefrontpos+=temp_pos;
          temp_max+=MAX1;
          MAX1=0.0;
         } // end of loop over j's

          ave_pos=avefrontpos/Ny;
          ave_max_temp=temp_max/Ny;

       //Writing Front position into a File

       if (t%printfrontpos==0){

  	   FILE * frontfile = fopen(filename2, "a");
	   fprintf(frontfile, "%f  %f  %f\n", ave_pos,ave_max_temp, t*t_step);
           fclose(frontfile);

        if(t*t_step>=500.0){
           //Writing y-x  profile
           sprintf(filename4,"profile%d_%d_%0.5f_%0.6f.txt",f_n,t/printfrontpos,t_step,To);
           FILE * fronmaxtemp = fopen(filename4, "w");

           fprintf(fronmaxtemp, "Average_position , Time\n");
           fprintf(fronmaxtemp, "\n%f %f\n",ave_pos,t*t_step);
           fprintf(fronmaxtemp, "\n Positions\n\n");

           for (j=1;j<=Ny;j++){                 
           fprintf(fronmaxtemp, "%d  %f %f\n", j, max_temp_pos[j-1], max_temp[j-1]); }
           fclose(fronmaxtemp); 
       }
     }
   }//End of front pos print


//}

/****************************************/

/********Deakkocation Function*********/
void Dealloc(){

int j;

 for ( j=0; j<ny; j++ ) {

        delete[] T[j];
        delete[] C[j];
        delete[] Grad[j];
        delete[] Grad_conc[j];
        delete[] tc[j];
        delete[] alpha[j];
        delete[] delta[j];
        delete[] strgh[j];
 }


        delete[] T;
        delete[] C;
        delete[] Grad;
        delete[] Grad_conc;
        delete[] tc;
        delete[] alpha;
        delete[] delta;
        delete[] strgh;
}

//--------------------------------------/

#include"in_namelist.c"


                             /********************************END OF THE CODE ************************************/
