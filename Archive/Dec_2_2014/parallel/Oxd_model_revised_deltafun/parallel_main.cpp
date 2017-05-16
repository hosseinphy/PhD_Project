//  2D-Parallel-code-Autor:H.Azizi           //
/*  This version is updated  Wed Sep 17th,14 */
/*  modif: make the corr. serial parallel&...*/
/*  ...RNG (from uniform to gussian),still   */
/*  there is no heat dissipation             */
/*********************************************/

/***File Inclusion****/
/*********************/
#include"parallel_initial.cpp"
#include"mpi.h"

/**** Mpi status ****/
 
 MPI_Request request;
 MPI_Status status;

/*******************/

/********** Reading & Writing Files *************/
std::ifstream myReadFile;                       //
std::ifstream myReadFile2;                      //
std::ofstream output1("conc1.dat");             //
std::ofstream output2("conc2.dat");             //
char   filename[15];                            //
char   filename2[15];                           //
char   filename3[15];                           //
char   filename4[15];                           //
char   filename5[15];                           //
char   filename6[15];                           //
char   filename7[15];                           //
char   filename8[15];                           //
char   filename9[15];                           //
/**********************************************///
//Global Variable Definitions:  
                                                //
double Ctshold,Gamma,q_T,Lambda_1,beta,Diff;    //
double tou_c;                                   //
double Q,cap_Gamma,TC,TL,T_bar,mass_Diff,qo;    //
double recent_pos,recent_time;                  // 
long   seed;                                    //
int    printfreq, printfrontpos,printconcfreq;  //
int    t,ign_time_counter,ign_time;             //
int    f_n,t_n,filesize,front_size;             //
int    restart_flag,restart_time,t_resume;  	//
char   bound_option;                            //            
/**********************************************///
//Mpi Variable Definitions:                     //
                                                //
//Processor's parameters                        //
int myid,nproc,count;                           //
int nxproc,nyproc,nx,ny;                        //
                                                //
//Processor's arrays                            //
double **ptemp;                                 //
double **pconc;                                 //
double **pgrad;                                 //
double **pstrgh;                                //             
double **tcp;                                   //             
int    **alphap;                                //           
                                                //
//Processor 0 and others,pos array              //
double *xline;                                  //
double *tot_xline;                              //
                                                //
//mesh points /each processor                   //
int nxp,nyp; // W/o ghost points                //
int Nxp,Nyp; //W ghost points                   //
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

/*** End of reading from input file ***/


/**********************************/
/*****       Main loop      *******/
/**********************************/

int main (int argc, char ** argv){
  
 system ("clear");
 mpi_initialize(); 
 GetNameList (argc, argv);
 PrintNameList (stdout);
 Setup();
 mpi_setup(); 
 Ran_Conc();
 //initialize();
 //
 //creat file for storing front position                  
 sprintf(filename2,"frontname%0.5f%0.6f.txt",t_step,To); 
 FILE * frontfile = fopen(filename2, "w");               
 fprintf(frontfile, "");                                 
 fclose(frontfile); 
 /***************************************************************/
   /*if(restart_flag==0){
     initialize();
     set_file(0); 
    }
   else if (restart_flag==1){
     restart(restart_time);
     set_file(1);
    }
   else {
     std::cout<<"the restart flag should be either 0 or 1 "<<"\n";
      return (1);
   }*/
/*****************************************************************/

//** Simulation Step **//
   for(t=0;t<Nstep;t++){ 
      //exchange data between processors
      proc_exchange();
      grad();
      //kinetic();     
      oxidation();
      //update_conc();
      Mixed();
      //temp_write();
      front();
   }

 MPI_Finalize();
 Dealloc();
 return 0;
} /** end of main loop **/


//***  FUNCTIONS  ***//

/**************************************/

void mpi_initialize(){

 MPI_Init(NULL,NULL);                                                                  
 //Rank and Size of procss              
 MPI_Comm_rank(MPI_COMM_WORLD,&myid);   
 MPI_Comm_size(MPI_COMM_WORLD,&nproc);  

 std::cout<<"this is your proc id :"<<myid<<"\n";
 std::cout<<"this is number of process :"<<nproc<<"\n";
}
/**************************************/

/***** Randon number Generator *****/
void Ran_Conc(){

double number ;
int i,j;

  i=1;
   while(i<Nxp-1){
    j=1;
     while(j<Nyp-1){
       pconc[i][j]=1.0;
       j=j+conc_step;
     }
    i=i+conc_step;
   }

  i=1;
   while(i<Nxp-1){
    j=1;
     while(j<Nyp-1){
       if(pconc[i][j]==1.0){
         pstrgh[i][j]=1./(x_step*x_step);
         pstrgh[i][j+1]=1./(x_step*x_step);
         pstrgh[i+1][j]=1./(x_step*x_step);
         pstrgh[i+1][j+1]=1./(x_step*x_step);
         j=j+2;
       }
       else{
        pstrgh[i][j]=0.0;
         j=j+1;
       }
     }
   i=i+2;
  }

/*
if(myid==0){
 for (i=1;i<Nxp-1;i++){
  for (j=1;j<Nyp-1;j++){
  output1<<i<<"\t"<<j<<"\t"<<pstrgh[i][j]<<"\n";
  }
 }
}

else if(myid==1){
 for (i=1;i<Nxp-1;i++){
  for (j=1;j<Nyp-1;j++){
  output2<<i<<"\t"<<j<<"\t"<<pstrgh[i][j]<<"\n";
  }
 }
}
*/

/*srand48(seed);

  for(i=0;i<Nx;i++){ 
      for(j=0;j<Ny;j++){
             number=drand48();
          if (number<=Ctshold){ 
             C[i][j]=1.0;
          }       
          else { 
             C[i][j]=0.0;       
          }
       }
   }*/
}

/************ End RNG  *************/

/******* mpi setup function ********/

int mpi_setup(){

 int i,j;
 int i1g,i2g,j1g,j2g,ig,jg; 

 nxproc=nproc; //Decomposing the x domain into the even number of processors
 nyproc=1;  // y dirction wont be decomposed

 if(Nx%nxproc!=0) {
    std::cout<<"Choose even number of processors in order to run the code"<<"\n\n";
    return (1);}
 else{
    nxp=Nx/nxproc;
    nyp=Ny/nyproc;
    Nxp=nxp+2;//these two added to the number of meshes due to the ghost points
    Nyp=nyp+2;}

//Initialize array for each process

  ptemp=new double *[Nxp];
  pconc=new double *[Nxp];
  pgrad=new double *[Nxp];
  pstrgh=new double *[Nxp];
  tcp = new double *[Nxp];
  alphap = new int *[Nxp];
  xline  = new double[nxp];
  tot_xline = new double[Nx];

 for(i=0;i<Nxp;i++){

   ptemp[i]=new double[Nyp];
   pconc[i]=new double[Nyp];
   pgrad[i]=new double[Nyp];
   pstrgh[i]=new double[Nyp];
   tcp[i]=new double[Nyp];
   alphap[i]=new int[Nyp];
 }


  i1g=(nxp*myid)+1;
  i2g=i1g+nxp-1;
  j1g=1;
  j2g=Ny;

  for(ig=1;ig<Nx+1;ig++){
    for(jg=1;jg<Ny+1;jg++){
      if (ig>=i1g && ig<=i2g){
         i=ig-i1g+1;
         j=jg; 
         ptemp[i][j]=T[ig-1][jg-1];
         pconc[i][j]=C[ig-1][jg-1];
         pgrad[i][j]=Grad[ig-1][jg-1];
         pstrgh[i][j]=0.0;
         tcp[i][j]=0.0;
         alphap[i][j]=0;
      }
   }
 }


 //set the surroding parameters

  for(i=0;i<Nxp;i++){
     ptemp[i][0]=To; //backtemp @ the left y plane
     ptemp[i][Nyp-1]=To; //backtemo @ the right y plane
     pconc[i][0]=0.0; //backtemp @ the left y plane
     pconc[i][Nyp-1]=0.0; //backtemp @ the left y plane
     pgrad[i][0]=0.0; //backtemo @ the right y plane
     pgrad[i][Nyp-1]=0.0; //backtemo @ the right y plane
     pstrgh[i][0]=0.0; //backtemo @ the right y plane
     pstrgh[i][Nyp-1]=0.0; //backtemo @ the right y plane
     tcp[i][Nyp-1]=0.0;
     alphap[i][Nyp-1]=0;
  }

  for(j=0;j<Nyp;j++){

    ptemp[0][j]=To; //backtemp @ the bottom
    ptemp[Nxp-1][j]=To; //backtemo @ the bottom
    pconc[0][j]=0.0; //backtemp @ the left y plane
    pconc[Nxp-1][j]=0.0; //backtemp @ the left y plane
    pgrad[0][j]=0.0; //backtemo @ the right y plane
    pgrad[Nxp-1][j]=0.0; //backtemo @ the right y plane
    pstrgh[0][j]=0.0; //backtemo @ the right y plane
    pstrgh[Nxp-1][j]=0.0; //backtemo @ the right y plane
    tcp[Nxp-1][j]=0.0;
    alphap[Nxp-1][j]=0;
  }

std::ofstream wrt ("in_data.dat");
if(myid==0){
 for(int i=0;i<Nxp;i++){
  wrt<<i<<"\n";
  for(int j=0;j<Nyp;j++){
   wrt<<j<<"\t"<<ptemp[i][j]<<"\t"<<pconc[i][j]<<"\t"<<pgrad[i][j]<<"\t"<<pstrgh[i][j]<<"\n";
  }
    wrt<<"\n\n";
 }
}
}

/******** End of mpi setup  *********/

/******* processor exchange  function ********/
void proc_exchange(){

 int i,j;
  //? should I use MPI_STATUS_IGNORE 
   for(j=0;j<Nyp;j++){
        if(myid>0){
            MPI_Recv(&(ptemp[0][j]),1,MPI_DOUBLE,myid-1,19,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            MPI_Send(&(ptemp[1][j]),1,MPI_DOUBLE,myid-1,17,MPI_COMM_WORLD);//sending the x=1 from the current proc 
             //MPI_Wait( &request, &status );
         }
        if(myid<nproc-1){  
            //sending the x=Nxp-2 of the current processor
            MPI_Send(&(ptemp[Nxp-2][j]),1,MPI_DOUBLE,myid+1,19,MPI_COMM_WORLD); 
            MPI_Recv(&(ptemp[Nxp-1][j]),1,MPI_DOUBLE,myid+1,17,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        }
    }

   MPI_Barrier(MPI_COMM_WORLD);//pasue to make processes to catch up with each other   
 }

/************ End of proc_exchange  *************/

/****** Gradiant Calculation *******/

 void grad(){
  
 int i,j;

 for(i=1;i<Nxp-1;i++){
     for (j=1;j<Nyp-1;j++){    
          pgrad[i][j] = (0.5*(ptemp[i+1][j]+ptemp[i][j+1]+ptemp[i-1][j]+ptemp[i][j-1])
                         +0.25*(ptemp[i+1][j+1]+ptemp[i-1][j+1]+ptemp[i+1][j-1]+ptemp[i-1][j-1])
                            -3.0*(ptemp[i][j]))/((x_step*x_step));
     }
  }
}

/************ End of grad calculation  *************/

/****** Kinetic RR calculation & temp update  *******/

 void  kinetic(){
  
 int i,j;

    for(i=1;i<Nxp-1;i++){    
      for (j=1;j<Nyp-1;j++){ //here I excluded the first (j=1) and last (j=Nyp-1) horizental planes
       q_T= Q*(exp(-(Tg/(ptemp[i][j]))))*(1.0-(exp((-1.0*TL)*((1./ptemp[i][j])-(1./TC)))));
       ptemp[i][j]=ptemp[i][j]+t_step*((Diff*pgrad[i][j])-(cap_Gamma*(ptemp[i][j]-To))+(qo*Gamma*q_T*pconc[i][j]));
        //  printf ("isnan(T[i][j]): %d\n",isnan(T[i][j]));     
      } 
    }  
}
/************ End of Kinetic function  *************/

/****** Oxidation RR calculation & temp update  *******/
void oxidation() {

  int i,j;

      for(i=1;i<Nxp-1;i++){     
          for (j=1;j<Nyp-1;j++){ //here I excluded the first (j=1) and last (j=Nyp-1) horizental planes
              if(tcp[i][j]>=0){                    
                   if(ptemp[i][j]>=Tg){                  
                        if(tcp[i][j]== 0.0){ // reaction time = zero -> possible reactant for buring  
                                tcp[i][j]=(t*t_step)+tou_c;//end time of burning
                                alphap[i][j]=1;}
                        else if(tcp[i][j]>(t*t_step)){
                                tcp[i][j]=tcp[i][j];
                                alphap[i][j]=1;}           
                        else {
                                tcp[i][j] = -1.0;
                                alphap[i][j]=0;}
                    }
                    else if(ptemp[i][j]<Tg){                         
                        if(tcp[i][j]==0.0){      
                                tcp[i][j]=0.0;
                                alphap[i][j]=0;}
                        else if(tcp[i][j]>(t*t_step)){
                                tcp[i][j]=tc[i][j];
                                alphap[i][j]=1;}
                        else {                            
                                tcp[i][j]=-1;
                                alphap[i][j]=0;}
                    } 
               }         
               else {        
                 tcp[i][j]=-1;
                 alphap[i][j]=0;
               }
           }
       }

 ////// Updating the temprature////////

     for(i=1;i<Nxp-1;i++){     
        for (j=1;j<Nyp-1;j++){
             ptemp[i][j]=ptemp[i][j]+(t_step*(pgrad[i][j]+(1./tou_c)*alphap[i][j]*pstrgh[i][j]-cap_Gamma*(ptemp[i][j]-To)));
        }
     }
  }

/************ End of Oxidation function  *************/

/****** update concenteration  *******/

void update_conc(){
 
 int i,j;

 for(i=1;i<Nxp-1;i++){    
    for (j=1;j<Nyp-1;j++){ //here I excluded the first (j=1) and last (j=Nyp-1) horizental planes
          q_T= Q*(exp(-(Tg/(ptemp[i][j]))))*(1.0-(exp((-1.0*TL)*((1./ptemp[i][j])-(1./TC)))));
          pconc[i][j]=pconc[i][j]+t_step*((-1.0*Gamma*q_T*pconc[i][j]));
    } 
  } 
}
/****** End of update concenterations *****/


/***** Mixed boundary conditions ****/
 void Mixed(){
 
  int i,j;

  //zero flux along x axis for each processor

   if(myid==0){   
      for(j=1;j<Nyp-1;j++){    
         ptemp[0][j]=ptemp[2][j];
      }
   }
   else if (myid==nproc-1){
      for(j=1;j<Nyp-1;j++){    
         ptemp[Nxp-1][j]=ptemp[Nxp-3][j];
      }
   }

   for(i=0;i<Nxp;i++){    
      ptemp[i][0]=ptemp[i][Ny];
      ptemp[i][Ny+1]=ptemp[i][1];
  }
}


/****** End of Mixed Boundary conditions *****/

/***** temperature write function ****/

 void temp_write(){

 int num;
 int a,b;

     if (t%printfreq == 0) {			

       // sprintf(filename6, "temp_oxd%d_%d_%0.5f_%0.5f.dat",f_n,t/printfreq,t_step,To);
         sprintf(filename6, "temp_oxd%d.dat",t/printfreq);
          for (num = 0; num < nproc; num++) {
             if(myid==num){					
                if (myid == 0) {
		  FILE * tmpr = fopen(filename6, "w"); //if first process, create the file
		    for (a = 1; a < Nxp-1; a++) {
		       for (b = 1; b < Nyp-1; b++) {
			 fprintf(tmpr, "%f \n", ptemp[a][b]);
		       }
		    }
		   fclose(tmpr);
	         } else {
		  FILE * tmpr = fopen(filename6, "a"); //if succeeding process, append to file
		    for (a = 1; a < Nxp-1; a++) {
		       for (b = 1; b < Nyp-1; b++) {
		         fprintf(tmpr, "%f \n", ptemp[a][b]);
		       }
		    }
		   fclose(tmpr);
		}
              }
	    MPI_Barrier(MPI_COMM_WORLD);
          }
       }
    }

/****** End of temperature write *****/

/****** Front function  *******/

void front() {


    double  MAX1=0.0;
    double  MAX2=0.0;
    double  temp_max=0.0;
    double  temp_pos;
    double  avefrontpos=0.0;
    int i,j;

        if (t%printfrontpos==0){

          for(j=1;j<Nyp-1;j++){  
                
               if(myid==0){
                  for (i=1;i<Nxp-1;i++){
                    tot_xline[i-1]=ptemp[i][j];                                     
                   }                               
                }else if (myid!=0){                              
                  for (i=1;i<Nxp-1;i++){
                    xline[i-1]=ptemp[i][j];                                     
                  }
                }

              //  really important to have this for several process 
                if(myid!=0){
                   MPI_Send(&(xline[0]),nxp,MPI_DOUBLE,0,myid,MPI_COMM_WORLD);                          
                }
                else if(myid==0) {
                  for(i=1;i<nproc;i++){                      
                   MPI_Recv(&(tot_xline[nxp*i]),nxp,MPI_DOUBLE,i,i,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
                 }
                }
               
                MPI_Barrier(MPI_COMM_WORLD);//pasue to make processes to catch up with each other   

                 // End of communication between processes

                if(myid==0){                          
                  i=Nx-1;
                   while (i>=0){                                                   
                     if(i!=Nx-1 && i!=0){
                      if (tot_xline[i]>tot_xline[i-1] && tot_xline[i]>tot_xline[i+1]) {                         
                         MAX1= tot_xline[i];
                         temp_pos=i+1;
                         break;
                      } 
                     }
                    else{
                      if(tot_xline[Nx-1]>MAX1){
                          MAX1 =tot_xline[Nx-1];
                          temp_pos=Nx-1+1;
                       } 
                      if(tot_xline[0]>MAX1){
                          MAX1 =tot_xline[0];
                          temp_pos=0+1;
                       } 
                    }
                    i=i-1;
                   }
                 }
            max_temp[j-1]=MAX1;
            max_temp_pos[j-1]=temp_pos;	     
            avefrontpos+=temp_pos;
            temp_max+=MAX1;
            MAX1=0.0;
          }//end of loop over j's	
                       
          ave_pos=avefrontpos/Ny;
          ave_max_temp=temp_max/Ny;



        if (myid == 0) {
  	   FILE * frontfile = fopen(filename2, "a");
	   fprintf(frontfile, "%f  %f  %f\n", ave_pos,ave_max_temp, t*t_step);
           fclose(frontfile);

        if(t*t_step>=100.0){
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
 }//end of front function

/****** End of writing of front position *****/

/****** Deallocation function  *******/

void Dealloc(){

int j;

 for ( j=0; j<Ny; j++ ) {

        delete[] T[j];
        delete[] C[j];
        delete[] Grad[j];
        delete[] tc[j];
        delete[] alpha[j];
 }
        delete[] T;
        delete[] C;
        delete[] Grad;
        delete[] tc;
        delete[] alpha;

 }


/****** End of Deallocation *****/

#include"in_namelist.c"

