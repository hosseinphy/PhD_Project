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
char   filename[BUFSIZ];                        //
char   filename2[BUFSIZ];                       //
char   filename3[BUFSIZ];                       //
char   filename4[BUFSIZ];                       //
char   filename5[BUFSIZ];                       //
char   filename6[BUFSIZ];                       //
char   filename7[BUFSIZ];                       //
char   filename8[BUFSIZ];                       //
char   filename9[BUFSIZ];                       //
char   filename10[BUFSIZ];                      //
/**********************************************///
//Global Variable Definitions:  
                                                //
double Ctshold,Gamma,q_T,Lambda_1,beta,Diff;    //
double tou_c,lambda;                            //
double Q,cap_Gamma,TC,TL,T_bar,mass_Diff,qo;    //
double recent_pos,recent_time;                  // 
long   seed, *seed_num;                         //
int    printfreq, printfrontpos,profile_delay;  //
int    printconc;                               //
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
double **pconc,**pco2;                          //
double **pgrad,**pgrad_co2;                     //
double **pstrgh;                                //             
double **tcp;                                   //             
int    **alphap;                                //           
                                                //
//Processor 0 and others,pos array              //
double *xline,*xline_test;                      //
double *tot_xline,*tot_xline_test;              //
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
  NameR (lambda),
  NameI (grid_x),        
  NameI (grid_y),        
  NameI (tot_time),
  NameI (printfreq),       
  NameI (printfrontpos),
  NameI (printconc),
  NameI (profile_delay),

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

 //creat file for storing front position                  

 sprintf(filename2,"frontoxd_regular_2D_%d_%0.2f_%0.3f.txt",f_n,tou_c,Tg);
 FILE * frontfile = fopen(filename2, "w");               
 fprintf(frontfile, "");                                 
 fclose(frontfile); 

/*****************************************************************/

//** Simulation Step **//
   for(t=0;t<Nstep;t++){ 

      grad();
      oxidation();
      Mixed();
      Mixed_oxg();
      proc_exchange();
      front();
      //temp_write();
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
int i,j,k,l;

//In parallel one  has to be done
//in initial code
/*
 // Delta spikes
  i=1;
   while(i<Nxp-1){
    j=1;
     while(j<Nyp-1){
       pconc[i][j]=1.0;
       j=j+conc_step;
     }
    i=i+conc_step;
   }
*/

 // oxygen distribution
  k=1;
   while(k<Nxp-1){
    l=1;
     while(l<Nyp-1){
       pco2[k][l]=1.0;
       l=l+1;
     }
    k=k+1;
   }
 //Exchange O2 concentertaion and temperature acrross processors
 proc_exchange();
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
  pco2 =new double *[Nxp];
  pgrad=new double *[Nxp];
  pgrad_co2=new double *[Nxp];
  pstrgh=new double *[Nxp];
  tcp = new double *[Nxp];
  alphap = new int *[Nxp];
  xline  = new double[nxp];
  tot_xline = new double[Nx];

  xline_test  = new double[nxp];
  tot_xline_test = new double[Nx];


 for(i=0;i<Nxp;i++){

   ptemp[i]=new double[Nyp];
   pconc[i]=new double[Nyp];
   pco2[i]=new double[Nyp];
   pgrad[i]=new double[Nyp];
   pstrgh[i]=new double[Nyp];
   tcp[i]=new double[Nyp];
   alphap[i]=new int[Nyp];
   pgrad_co2[i]=new double[Nyp];
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
         pco2[i][j]=co2[ig-1][jg-1];
         pgrad[i][j]=Grad[ig-1][jg-1];
         pgrad_co2[i][j]=Grad_co2[ig-1][jg-1];
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
     pco2[i][0]=0.0; //backtemp @ the left y plane
     pco2[i][Nyp-1]=0.0; //backtemp @ the left y plane
     pgrad[i][0]=0.0; //backtemo @ the right y plane
     pgrad[i][Nyp-1]=0.0; //backtemo @ the right y plane     
     pgrad_co2[i][0]=0.0; //backtemo @ the right y plane
     pgrad_co2[i][Nyp-1]=0.0; //backtemo @ the right y plane
     pstrgh[i][0]=0.0; //backtemo @ the right y plane
     pstrgh[i][Nyp-1]=0.0; //backtemo @ the right y plane
     tcp[i][0]=0.0;
     tcp[i][Nyp-1]=0.0;
     alphap[i][0]=0;
     alphap[i][Nyp-1]=0;
  }

  for(j=0;j<Nyp;j++){

    ptemp[0][j]=To; //backtemp @ the bottom
    ptemp[Nxp-1][j]=To; //backtemo @ the bottom
    pconc[0][j]=0.0; //backtemp @ the left y plane
    pconc[Nxp-1][j]=0.0; //backtemp @ the left y plane 
    pco2[0][j]=0.0; //backtemp @ the left y plane
    pco2[Nxp-1][j]=0.0; //backtemp @ the left y plane
    pgrad[0][j]=0.0; //backtemo @ the right y plane
    pgrad[Nxp-1][j]=0.0; //backtemo @ the right y plane   
    pgrad_co2[0][j]=0.0; //backtemo @ the right y plane
    pgrad_co2[Nxp-1][j]=0.0; //backtemo @ the right y plane
    pstrgh[0][j]=0.0; //backtemo @ the right y plane
    pstrgh[Nxp-1][j]=0.0; //backtemo @ the right y plane
    tcp[0][j]=0.0;
    tcp[Nxp-1][j]=0.0;
    alphap[0][j]=0;
    alphap[Nxp-1][j]=0;
  }
}

/******** End of mpi setup  *********/

/******* processor exchange  function ********/

/**** Exchange data between processors ******/

void proc_exchange(){

 int i,j;

   for(j=0;j<Nyp;j++){

        if(myid>0){


            MPI_Irecv(&(pco2[0][j]),1,MPI_DOUBLE,myid-1,23,MPI_COMM_WORLD,&request);
            MPI_Send(&(pco2[1][j]),1,MPI_DOUBLE,myid-1,21,MPI_COMM_WORLD);//sending the x=1 from the current proc 

            MPI_Wait( &request, &status );
         }
        if(myid<nproc-1){  

            MPI_Send(&(pco2[Nxp-2][j]),1,MPI_DOUBLE,myid+1,23,MPI_COMM_WORLD); 
            MPI_Recv(&(pco2[Nxp-1][j]),1,MPI_DOUBLE,myid+1,21,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

        }
    }


   for(j=0;j<Nyp;j++){

        if(myid>0){

            MPI_Irecv(&(ptemp[0][j]),1,MPI_DOUBLE,myid-1,19,MPI_COMM_WORLD,&request);
            MPI_Send(&(ptemp[1][j]),1,MPI_DOUBLE,myid-1,17,MPI_COMM_WORLD);//sending the x=1 from the current proc 

            MPI_Wait( &request, &status );
         }
        if(myid<nproc-1){  

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


 //Temp laplacian
 for(i=1;i<Nxp-1;i++){
     for (j=1;j<Nyp-1;j++){    
          pgrad[i][j] = (0.5*(ptemp[i+1][j]+ptemp[i][j+1]+ptemp[i-1][j]+ptemp[i][j-1])
                         +0.25*(ptemp[i+1][j+1]+ptemp[i-1][j+1]+ptemp[i+1][j-1]+ptemp[i-1][j-1])
                            -3.0*(ptemp[i][j]))/((x_step*x_step));
     }
  }

 //oxygen laplacian
 for(i=1;i<Nxp-1;i++){
     for (j=1;j<Nyp-1;j++){    
          pgrad_co2[i][j] = (0.5*(pco2[i+1][j]+pco2[i][j+1]+pco2[i-1][j]+pco2[i][j-1])
                         +0.25*(pco2[i+1][j+1]+pco2[i-1][j+1]+pco2[i+1][j-1]+pco2[i-1][j-1])
                            -3.0*(pco2[i][j]))/((x_step*x_step));
     }
  }
}

/************ End of grad calculation  *************/

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
      ptemp[i][j]=ptemp[i][j]+(t_step*(pgrad[i][j]+((1./tou_c)*((1/x_step)*(1/x_step))*Gamma*lambda*pco2[i][j]*alphap[i][j]*(pconc[i][j]))));
    }
  }

 ////// Updatingoxygen concenteration////////


  for(i=1;i<Nxp-1;i++){     
    for (j=1;j<Nyp-1;j++){
      pco2[i][j]=pco2[i][j]+(t_step*(mass_Diff*pgrad_co2[i][j]-((1./tou_c)*((1/x_step)*(1/x_step))*pco2[i][j]*alphap[i][j]*(pconc[i][j]))));
    }
  }
}

/************ End of Oxidation function  *************/

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

/****** Mixed boundary conditions for O2  ****/
 
 void Mixed_oxg(){
 
  int i,j;

  //zero flux along x axis for each processor

   if(myid==0){   
      for(j=1;j<Nyp-1;j++){    
         pco2[0][j]=pco2[2][j];
      }
   }
   else if (myid==nproc-1){
      for(j=1;j<Nyp-1;j++){    
         pco2[Nxp-1][j]=pco2[Nxp-3][j];
      }
   }

   for(i=0;i<Nxp;i++){    
      pco2[i][0]=pco2[i][Ny];
      pco2[i][Ny+1]=pco2[i][1];
  }
}


/****** End of Mixed Boundary conditions *****/

/***** temperature write function ****/
/*
 void temp_write(){

 int num;
 int a,b;

     if (t%printfreq == 0) {			

         sprintf(filename10,"data_%d.dat",t/printfreq);
          for (num = 0; num < nproc; num++) {
             if(myid==num){					
                if (myid == 0) {
		  FILE * tmpr = fopen(filename10, "w"); //if first process, create the file
		    for (a = 0; a < Nxp; a++) {
		       for (b = 0; b < Nyp; b++) {

                         fprintf(tmpr, "%d   %d   %0.6f   %0.6f  %0.6f  %0.6f  %0.6f \n", a,b,ptemp[a][b],pconc[a][b],pco2[a][b],pgrad[a][b],pgrad_co2[a][b]);
		       }
		    }
		   fclose(tmpr);
	         } else {
		  FILE * tmpr = fopen(filename10, "a"); //if succeeding process, append to file
		    for (a = 0; a < Nxp; a++) {
		       for (b = 0; b < Nyp; b++) {

                         fprintf(tmpr, "%d   %d   %0.6f   %0.6f  %0.6f  %0.6f  %0.6f \n", a,b,ptemp[a][b],pconc[a][b],pco2[a][b],pgrad[a][b],pgrad_co2[a][b]);

		       }
		    }
		   fclose(tmpr);
		}
              }
	    MPI_Barrier(MPI_COMM_WORLD);
          }
       }
    }
*/
/****** End of temperature write *****/

/***** temperature write function ****/

 void conc_write(){

 int num;
 int a,b;

     if (t%printconc == 0) {			

         sprintf(filename7,"conc_regular_2D_%d_%0.2f_%0.3f_%d.txt",f_n,tou_c,Tg,t/printconc);
          for (num = 0; num < nproc; num++) {
             if(myid==num){					
                if (myid == 0) {
		  FILE * concr = fopen(filename7, "w"); //if first process, create the file
		    for (a = 1; a < Nxp-1; a++) {
		       for (b = 1; b < Nyp-1; b++) {
			 fprintf(concr, "%f \n", pco2[a][b]);
		       }
		    }
		   fclose(concr);
	         } else {
		  FILE * concr = fopen(filename7, "a"); //if succeeding process, append to file
		    for (a = 1; a < Nxp-1; a++) {
		       for (b = 1; b < Nyp-1; b++) {
		         fprintf(concr, "%f \n", pco2[a][b]);
		       }
		    }
		   fclose(concr);
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
                      if(tot_xline[Nx-1]>=MAX1){
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
            max_temp_pos[j-1]=temp_pos*x_step;	     
            avefrontpos+=temp_pos;
            temp_max+=MAX1;
            MAX1=0.0;
          }//end of loop over j's	
                       
          ave_pos=(avefrontpos/Ny);
          ave_max_temp=temp_max/Ny;

        if (myid == 0) {
  	   FILE * frontfile = fopen(filename2, "a");
	   fprintf(frontfile, "%f  %f  %f\n", ave_pos,ave_max_temp, t*t_step);
           fclose(frontfile);

        if(t*t_step>=profile_delay){
           //Writing y-x  profile
           sprintf(filename4,"profile_regular_oxd_2D_%d_%d_%0.2f_%0.3f.txt",f_n,t/printfrontpos,tou_c,Tg);
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
        delete[] co2[j];
        delete[] Grad[j];
        delete[] Grad_co2[j];
        delete[] tc[j];
        delete[] alpha[j];
 }
        delete[] T;
        delete[] C;
        delete[] co2;
        delete[] Grad;
        delete[] Grad_co2;
        delete[] tc;
        delete[] alpha;

 }


/****** End of Deallocation *****/

#include"in_namelist.c"


/********************************/
/****** Printing file **********/

/*         sprintf(filename10,"conc.dat");
          for (num = 0; num < nproc; num++) {
             if(myid==num){					
                if (myid == 0) {
		  FILE * tmpr = fopen(filename10, "w"); //if first process, create the file
		    for (a = 0; a < Nxp; a++) {
		       for (b = 0; b < Nyp; b++) {

                         fprintf(tmpr, "%d   %d   %0.6f   %0.6f  %0.6f \n", a,b,ptemp[a][b],pconc[a][b],pco2[a][b]);
		       }
		    }
		   fclose(tmpr);
	         } else {
		  FILE * tmpr = fopen(filename10, "a"); //if succeeding process, append to file
		    for (a = 0; a < Nxp; a++) {
		       for (b = 0; b < Nyp; b++) {

                         fprintf(tmpr, "%d   %d   %0.6f   %0.6f  %0.6f \n", a,b,ptemp[a][b],pconc[a][b],pco2[a][b]);

		       }
		    }
		   fclose(tmpr);
		}
              }
	    MPI_Barrier(MPI_COMM_WORLD);
          }*/
