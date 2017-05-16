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
double tou_c;                                   //
double Q,cap_Gamma,TC,TL,T_bar,mass_Diff,qo;    //
double recent_pos,recent_time;                  // 
long   seed, *seed_num;                         //
int    datafreq,tempfreq;                       //
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
double *ptemp;                                  //
double *pconc;                                  //
double *pgrad;                                  //
double *tcp;                                    //             
int    *alphap;                                 //           
                                                //
//Processor 0 and others,pos array              //
double *xline,*xline_test;                      //
double *tot_xline,*tot_xline_test;              //
                                                //
//mesh points /each processor                   //
int nxp; // W/o ghost points                    //
int Nxp; //W ghost points                       //
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
  NameI (tot_time),
  NameI (datafreq),       
  NameI (tempfreq),       

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
 proc_exchange();

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
      proc_exchange();
      grad();
      oxidation();
      Mixed();
      data_write();
   }

 MPI_Finalize();
 Dealloc();
 return 0;
} /** end of main loop **/


//***  FUNCTIONS  ***//

/**************************************/

void mpi_initialize(){

 MPI_Request request;

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
int i,j,num;

  i=1;
   while(i<Nxp-1){
      pconc[i]=1.0;
    i=i+1;
   }       
}

/************ End RNG  *************/

/******* mpi setup function ********/

int mpi_setup(){

 int i,j,num,a;
 int i1g,i2g,j1g,j2g,ig,jg; 

 nxproc=nproc; //Decomposing the x domain into the even number of processors

 if(Nx%nxproc!=0) {
    std::cout<<"Choose even number of processors in order to run the code"<<"\n\n";
    return (1);}
 else{
    nxp=Nx/nxproc;
    Nxp=nxp+2;//these two added to the number of meshes due to the ghost points
 }

//Initialize array for each process

  ptemp=new double [Nxp];
  pconc=new double [Nxp];
  pgrad=new double [Nxp];
  tcp = new double [Nxp];
  alphap = new int [Nxp];
  xline  = new double[nxp];
  tot_xline = new double[Nx];
  xline_test  = new double[nxp];
  tot_xline_test = new double[Nx];

   i1g=(nxp*myid)+1;
   i2g=i1g+nxp-1;
     for(ig=1;ig<Nx+1;ig++){
          if (ig>=i1g && ig<=i2g){
            i=ig-i1g+1;
            ptemp[i]=T[ig-1];
            pconc[i]=C[ig-1];
            pgrad[i]=Grad[ig-1];
            tcp[i]=0.0;
            alphap[i]=0;
         }
   }
      


 //set the surroding parameters
   if(myid==0){
    ptemp[0]=To; //backtemp @ the bottom
    pconc[0]=0.0; //backtemp @ the left y plane
    pgrad[0]=0.0; //backtemo @ the right y plane
    tcp[0]=0.0;
    alphap[0]=0;
    pconc[Nxp-1]=0.0; //backtemp @ the left y plane
    pgrad[Nxp-1]=0.0; //backtemo @ the right y plane
    tcp[Nxp-1]=0.0;
    alphap[Nxp-1]=0;
   }
   else if(myid==nxproc-1){

    ptemp[Nxp-1]=To; //backtemo @ the bottom
    pconc[0]=0.0; //backtemp @ the left y plane
    pgrad[0]=0.0; //backtemo @ the right y plane
    tcp[0]=0.0;
    alphap[0]=0;
    pconc[Nxp-1]=0.0; //backtemp @ the left y plane
    pgrad[Nxp-1]=0.0; //backtemo @ the right y plane
    tcp[Nxp-1]=0.0;
    alphap[Nxp-1]=0;
   }
   else{

    ptemp[0]=0.0; //backtemo @ the bottom
    ptemp[Nxp-1]=0.0; //backtemo @ the bottom
    pconc[0]=0.0; //backtemp @ the left y plane
    pgrad[0]=0.0; //backtemo @ the right y plane
    tcp[0]=0.0;
    alphap[0]=0;
    pconc[Nxp-1]=0.0; //backtemp @ the left y plane
    pgrad[Nxp-1]=0.0; //backtemo @ the right y plane
    tcp[Nxp-1]=0.0;
    alphap[Nxp-1]=0;
   }


/*
//Only for test-print out arrays
    if(myid==0){
       for (i=1;i<Nxp-1;i++){
         tot_xline_test[i-1]=ptemp[i];                                     
       }                              
    }else if (myid!=0){                              
       for (i=1;i<Nxp-1;i++){
         xline_test[i-1]=ptemp[i];                                     
       }
    }


 //  really important to have this for several process 
  
    if(myid!=0){

      MPI_Send(&(xline_test[0]),nxp,MPI_DOUBLE,0,myid,MPI_COMM_WORLD);                          
    }
    else if(myid==0){
       for(i=1;i<nproc;i++){                      
          MPI_Recv(&(tot_xline_test[nxp*i]),nxp,MPI_DOUBLE,i,i,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
       }
     }
               
    MPI_Barrier(MPI_COMM_WORLD);//pasue to make processes to catch up with each other   

   // End of communication between processes


    sprintf(filename10, "data_oxd.dat");
      if(myid==0){
        FILE * test = fopen(filename10, "w"); //if first process, create the file
          for (j = 1; j < Nx; j++) {
	    fprintf(test, "%d %f \n", j, tot_xline_test[j]);
          }
        fclose(test);
      }*/
   }

/******** End of mpi setup  *********/

/******* processor exchange  function ********/
void proc_exchange(){

 int i,j;
  //? should I use MPI_STATUS_IGNORE 

        if(myid>0){

            MPI_Irecv(&(ptemp[0]),1,MPI_DOUBLE,myid-1,19,MPI_COMM_WORLD,&request);
            MPI_Send(&(ptemp[1]),1,MPI_DOUBLE,myid-1,17,MPI_COMM_WORLD);//sending the x=1 from the current proc 
            MPI_Wait( &request, &status );
         }
        if(myid<nproc-1){  
            //sending the x=Nxp-2 of the current processor
            MPI_Send(&(ptemp[Nxp-2]),1,MPI_DOUBLE,myid+1,19,MPI_COMM_WORLD);             
            MPI_Recv(&(ptemp[Nxp-1]),1,MPI_DOUBLE,myid+1,17,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        }

   MPI_Barrier(MPI_COMM_WORLD);//pasue to make processes to catch up with each other   
 }

/************ End of proc_exchange  *************/

/****** Gradiant Calculation *******/

 void grad(){
  
 int i,j,a,num;

 for(i=1;i<Nxp-1;i++){

          pgrad[i]= ((ptemp[i+1]+ptemp[i-1])-2.0*(ptemp[i]))/(x_step*x_step);
  }
/*
//print out for each processor 
//
 
         sprintf(filename6, "init_data%d.dat",myid);
          for (num = 0; num < nproc; num++) {
             if(myid==num){					
                if (myid == 0) {
		  FILE * tmpr = fopen(filename6, "w"); //if first process, create the file
		    for (a = 0; a <= Nxp-1; a++) {
			 fprintf(tmpr, "%f \n", ptemp[a]);
		    }
		   fclose(tmpr);
	         } else {
		  FILE * tmpr = fopen(filename6, "w"); //if succeeding process, append to file
		    for (a = 0; a <= Nxp-1; a++) {
		         fprintf(tmpr, "%f \n", ptemp[a]);
		    }
		   fclose(tmpr);
		}
              }
            }

	 MPI_Barrier(MPI_COMM_WORLD);
*/
}

/************ End of grad calculation  *************/

/****** Oxidation RR calculation & temp update  *******/
void oxidation() {

  int i,j;

      for(i=1;i<Nxp-1;i++){     
              if(tcp[i]>=0){                    
                   if(ptemp[i]>=Tg){                  
                        if(tcp[i]== 0.0){ // reaction time = zero -> possible reactant for buring  
                                tcp[i]=(t*t_step)+tou_c;//end time of burning
                                alphap[i]=1;}
                        else if(tcp[i]>(t*t_step)){
                                tcp[i]=tcp[i];
                                alphap[i]=1;}           
                        else {
                                tcp[i] = -1.0;
                                alphap[i]=0;}
                    }
                    else if(ptemp[i]<Tg){                         
                        if(tcp[i]==0.0){      
                                tcp[i]=0.0;
                                alphap[i]=0;}
                        else if(tcp[i]>(t*t_step)){
                                tcp[i]=tc[i];
                                alphap[i]=1;}
                        else {                            
                                tcp[i]=-1;
                                alphap[i]=0;}
                    } 
               }         
               else {        
                 tcp[i]=-1;
                 alphap[i]=0;
               }

       }

 ////// Updating the temprature////////

     for(i=1;i<Nxp-1;i++){     

         ptemp[i]=ptemp[i]+(t_step*(pgrad[i]+(1./tou_c)*alphap[i]*pconc[i]));
     }
  }

/************ End of Oxidation function  *************/

/***** Mixed boundary conditions ****/
 void Mixed(){
 
  int i,j;

  //zero flux along x axis for each processor

  if(myid==0){   
     ptemp[0]=ptemp[2];
  }
  else if (myid==nproc-1){
     ptemp[Nxp-1]=ptemp[Nxp-3];
  }
}

/****** End of Mixed Boundary conditions *****/

/***** temperature write function ****/
/*
 void temp_write(){

 int num;
 int a,b;

     if (t%printfreq == 0) {			

       // sprintf(filename6, "temp_oxd%d_%d_%0.5f_%0.5f.dat",f_n,t/printfreq,t_step,To);
       if(myid==0){
         for (i=1;i<Nxp-1;i++){
           tot_xline[i-1]=ptemp[i];                                     
          }                               
       }else if (myid!=0){                              
         for (i=1;i<Nxp-1;i++){
           xline[i-1]=ptemp[i];                                     
         }
       }

         sprintf(filename6, "temp_oxd%d.dat",t/printfreq);
          for (num = 0; num < nproc; num++) {
             if(myid==num){					
                if (myid == 0) {
		  FILE * tmpr = fopen(filename6, "w"); //if first process, create the file
		    for (a = 1; a < Nxp-1; a++) {
			 fprintf(tmpr, "%f \n", ptemp[a]);
		    }
		   fclose(tmpr);
	         } else {
		  FILE * tmpr = fopen(filename6, "a"); //if succeeding process, append to file
		    for (a = 1; a < Nxp-1; a++) {
		         fprintf(tmpr, "%f \n", ptemp[a]);
		    }
		   fclose(tmpr);
		}
              }
            }

	 MPI_Barrier(MPI_COMM_WORLD);
       }
    }
*/
/****** End of temperature write *****/

/****** Front function  *******/

void data_write() {


    double  MAX1=0.0;
    double  MAX2=0.0;
    double  temp_max=0.0;
    double  temp_pos;
    double  avefrontpos=0.0;
    int i,j;

        if (t%datafreq==0){
               if(myid==0){
                  for (i=1;i<Nxp-1;i++){
                    tot_xline[i-1]=ptemp[i];                                     
                   }                               
                }else if (myid!=0){                              
                  for (i=1;i<Nxp-1;i++){
                    xline[i-1]=ptemp[i];                                     
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

               //Temperature write
               if(t%tempfreq==0){
                 sprintf(filename6, "temp_oxd%d.dat",t/datafreq);
                  if (myid == 0) {
		    FILE * tmpr = fopen(filename6, "w"); //if first process, create the file
		      for (j = 0; j < Nx; j++) {
		 	  fprintf(tmpr, "%f \n", tot_xline[j]);
		      }
		    fclose(tmpr);
                  }
                }
                //data process for front position
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

           ave_max_temp=MAX1;
           ave_pos=temp_pos;
  
        if (myid == 0) {
  	   FILE * frontfile = fopen(filename2, "a");
	   fprintf(frontfile, "%f  %f  %f\n", ave_pos,ave_max_temp, t*t_step);
           fclose(frontfile);

     }
   }//End of front pos print
 }//end of front function

/****** End of writing of front position *****/

/****** Deallocation function  *******/

void Dealloc(){

int j;

        delete[] T;
        delete[] C;
        delete[] Grad;
        delete[] tc;
        delete[] alpha;

 }


/****** End of Deallocation *****/

#include"in_namelist.c"

