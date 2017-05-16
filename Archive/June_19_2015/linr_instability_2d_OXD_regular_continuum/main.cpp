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
int    forced_ign_time=1000;                    //
int    start_amp=0;                             //
long   seed;                                    //
int    forced_cntr;                             //
int    printfreq, printfrontpos,printconcfreq;  //
int    t,ign_time_counter,ign_time;             //
int    f_n,t_n,filesize,front_size;             //
int    t_resume;                                //
char   bound_option;                            //
char   filename[BUFSIZ];                        //
char   filename2[BUFSIZ];                       //
char   filename3[BUFSIZ];                       //
char   filename4[BUFSIZ];                       //
char   filename5[BUFSIZ];                       //
char   filename6[BUFSIZ];                       //
char   filename7[BUFSIZ];                       //
char   filename8[BUFSIZ];                       //
char   filename9[BUFSIZ];                       //
/**********************************************/// 


/****Parameter Reading from the input file*******/

NameList nameList[] = {

  NameI (t_n),
  NameI (f_n),
  NameR (Amp),
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

 sprintf(filename2,"front_OXD_continuum_2D%0.5f%0.6f.txt",t_step,To);
 FILE * frontfile = fopen(filename2, "w");
 fprintf(frontfile, "");
 fclose(frontfile);

 sprintf(filename9,"Amp_OXD_continuum_2D%0.5f%0.6f.txt",t_step,To);
 FILE * frontamp = fopen(filename9, "w");
 fprintf(frontamp, "");
 fclose(frontamp);

   //** Simulation Step **//
     forced_cntr=1;
         for(t=0;t<Nstep;t++){ 

          /******* laplacian calculation ******/     
 
          linr_stab();

          grad();

          /*************************************/
         
          /*******   update temperature   ******/      

          Calc_higgins();

          /*************************************/

          /******   tempreture boundary   ******/      
          //Mixed();
          //Dissipation();
          //periodic();
          zero_flux_boundary();  
          /*************************************/

          /**********   printing step  *********/      

          front();
          //Temp_write();
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
     while(j<=Ny){
       C[i][j]=1.0;
       j=j+conc_step;
     }
    i=i+conc_step;
   }
 }
/************** END ***************/

void linr_stab(){

 int i,j,l,k,h;

   if(forced_cntr<10000){
    if(t<=forced_ign_time){
        for (j=1;j<=Ny;j++){ //here I excluded the first (j=1) and last (j=Nyp-1) horizental planes    
         T[forced_cntr][j]=Tg;
        }
      forced_cntr=forced_cntr+1;     
     }
     else if (t>forced_ign_time) {

     h=1;
     std::cout<<t<<" \t "<<forced_cntr<<"\n";
     start_amp=forced_cntr;
     while(h<=Ny+1){
        l=Amp*cos(((2*M_PI*(h-1))/lambda));
         for(k=forced_cntr;k<=forced_cntr+l+1;k++){
           T[k][h]=2*Tg;
         } 
       h=h+1;
     }
    forced_cntr=20000;
    std::ofstream output("conc.dat");
     for(int b =1;b<=Ny;b++){  
      for(int a =1;a<=Nx;a++){  
       output<<b<<"\t"<<a<<"\t"<<T[a][b]<<"\n";
      }
    }
  }
 }
}
   
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
   /*
    //mighte applicable to O_2 concenteration
    for(i=1;i<=Nx;i++){  
         for (j=1;j<=Ny;j++){ //here I excluded the first (j=1) and last (j=Nyp-1) horizental planes
              Grad_conc[i][j] = 0.0;// ((C[ip][j]+C[i][jp])+(C[im][j]+C[i][jm])-4*(C[i][j]))/(Sqr(x_step));
         }
     }
   */
}

/************** END ***************/


/*********** OXIDATION MODEL  *************/

/***** Main Function *****/

void Calc_higgins(){

int i,j;


   //Normal step wise source
	  
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
                  T[i][j]=T[i][j]+(t_step*(Grad[i][j]+(1/tou_c)*alpha[i][j]*C[i][j]));
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
    double max_amp;
    double min_amp;
    double amplitude=0.0;
    int i,j;
 
   //changing the condition for find front
   //when temperature hit the T_ign. 

        for(j=1;j<=Ny;j++){                  
          i=Nx;
           while (i>=1){
            if(i!=Nx && i!=1){
             if(T[i][j]>=Tg){
              //if(T[i][j]>T[i-1][j] && T[i][j]>T[i+1][j]){
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

          ave_pos=(avefrontpos/Ny)*x_step;
          ave_max_temp=temp_max/Ny;

    /*************************************/
    //Finding Amplitude of perturbation
   

  /*  double max_amp = max_temp_pos[0];
    double min_amp = max_temp_pos[0];
    double amplitude=0.0;

    for (j=1; j <= Ny; j++){
       if (max_temp_pos[j-1] > max_amp){
           max_amp = max_temp_pos[j-1];
         }
      else if (max_temp_pos[j-1] < min_amp){
          min_amp = max_temp_pos[j-1];
        }
     }*/
    
         //Writing amplitudes
         if(t>forced_ign_time){

           max_amp=max_temp_pos[0];
           min_amp=max_temp_pos[Ny-1];
           amplitude=max_amp-min_amp;
 
  	   FILE * frontamp = fopen(filename9, "a");
	   fprintf(frontamp, "%f  %f  %f %f\n", min_amp,max_amp,amplitude,t*t_step);
           fclose(frontamp);
         }

       /***********************************/

       //Writing Front position into a File

       if (t%printfrontpos==0){

  	   FILE * frontfile = fopen(filename2, "a");
	   fprintf(frontfile, "%f  %f  %f\n", ave_pos,ave_max_temp, t*t_step);
           fclose(frontfile);

        ///////////////////////////////////////////////////////////////////////////////////

        if(t*t_step>=0){
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
