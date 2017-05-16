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

 void shuffle(int *,int);

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
char   filename11[BUFSIZ];                      //
char   filename12[BUFSIZ];                      //
char   filename13[BUFSIZ];                      //
/**********************************************///
//Global Variable Definitions:  
                                                //
double Ctshold,Gamma,q_T,Lambda_1,beta,Diff;    //
double tou_c;                                   //
double Q,cap_Gamma,TC,TL,T_bar,mass_Diff,qo;    //
double recent_pos,recent_time;                  // 
long   seed, *seed_num;                         //
int    datafreq,tempfreq,printdelay;            //
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
long   *seed_number;                            //
int    *p_number;                               //
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
  NameI (printdelay),       

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
 Ran_Conc();
 mpi_setup(); 
 proc_exchange();

 //creat file for storing front position                  
 sprintf(filename2,"frontname%d_%0.2f_%0.3f.txt",f_n,tou_c,Tg); 
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


//No randomness
/**************/

/*void Ran_Conc(){

double number ;
int i,j,num;

  i=1;
   while(i<Nxp-1){
      pconc[i]=1.0;
    i=i+conc_step;
   } */ 
/**************/


//random distribution
/*******************/

void Ran_Conc() {

    int max=grid_x;
    double number ;
    int i,j;
    int *values;
    int counter=0;
    int result[grid_x];  
    int ff;

    struct timeval time; 
    gettimeofday(&time,NULL);

    //ff=(time.tv_sec * 100) + (time.tv_usec / 100);
    //std::cout<<ff<<"\n";
    srand((time.tv_sec * 100) + (time.tv_usec / 100));

    //srand(time(NULL)); 
    // Generate grid_x random numbers within the range 1 to Nx 

    GenerateRandomNumbers(result,1,Nx,grid_x);  

    // Now shuffle the array values randomly.

    shuffle(result,grid_x);

    // fill up the box based on RNG

    for (int i = 0; i < Nx ; i++) {   
         C[i]=0.0;}

    for (int i = 0; i < grid_x; i++) {
        
          C[result[i]]=1;
          counter+=1;     
     }
    //std::cout<<"number of particles : "<<counter<<"\n";

/*Previous steps where aech processor individually run RNG ...
with iys own seed*/

/* int i,j,num;
 double number;
 int counter=0;
 int part_num=grid_x;

  seed_number[myid]=seed+time(NULL)+myid*(nproc)+myid;
  std::cout <<myid<<" \t "<< std::setprecision(10) << seed_number[myid] << '\n';
  srand48(seed_number[myid]);
   i=1;
      while(i<Nxp-1){
         number=drand48();
           if (number<=Ctshold){
               pconc[i]=1.0;
               counter=counter+1;              
           }
           else {
               pconc[i]=0.0;
           }
           
        if(counter>=part_num){
           i=Nxp-1;
           p_number[myid]=counter;           
        }
        else {     
          i=i+1;
        }
    }
 std::cout<<"total number of particles for processor : "<<"\t"<<myid<<"\t"<<"is"<<p_number[myid]<<"\n";
//Write seed number 


        sprintf(filename10, "Seeds.dat");
          for (num = 0; num < nproc; num++) {
             if(myid==num){
                if (myid == 0) {
                  FILE * pseed = fopen(filename10, "w"); //if first process, create the file
                        fprintf(pseed, "%d  %ld  \n", myid,seed_number[myid]);
                   fclose(pseed);
                 } else {
                  FILE * pseed = fopen(filename10, "a"); //if succeeding process, append to file
                         fprintf(pseed, "%d %ld \n",  myid, seed_number[myid]);
                   fclose(pseed);
                }
              }
            MPI_Barrier(MPI_COMM_WORLD);
          }

*/

              sprintf(filename6, "oneproc.dat");
                  FILE * concr = fopen(filename6, "w"); //if first process, create the file              
                   for (i = 0; i < Nx; i++) {
                     fprintf(concr, "%d %f \n", i,C[i]);
                   }
                  fclose(concr);


               sprintf(filename13, "random_dist_parallel.dat");
                  FILE * rann = fopen(filename13, "w"); //if first process, create the file              
                   for (i = 0; i < grid_x; i++) {
                     fprintf(rann, "%d %d \n", i,result[i]);
                   }
                  fclose(rann);
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
  seed_number = new long[nproc];
  p_number = new int[nproc];

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

    //Test to see if particles are distrubuted correctly among processors ...
    //
    if(myid==0){
       for (i=1;i<Nxp-1;i++){
              tot_xline_test[i-1]=pconc[i];                                     
       }                               
    }else if (myid!=0){                              
       for (i=1;i<Nxp-1;i++){
              xline_test[i-1]=pconc[i];                                     
       }
    }

    //  really important to have this for several process 
     if(myid!=0){
        MPI_Send(&(xline_test[0]),nxp,MPI_DOUBLE,0,myid,MPI_COMM_WORLD);                          
     }
     else if(myid==0) {
         for(i=1;i<nproc;i++){                      
            MPI_Recv(&(tot_xline_test[nxp*i]),nxp,MPI_DOUBLE,i,i,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
         }
     }               
      
      MPI_Barrier(MPI_COMM_WORLD);//pasue to make processes to catch up with each other   

      // End of communication between processes


      sprintf(filename11, "whole_parallel.dat");
         if (myid == 0) {
           FILE * concpr = fopen(filename11, "w"); //if first process, create the fil  
             for (j = 0; j < Nx; j++) {
	          fprintf(concpr, "%d %f \n", j, tot_xline_test[j]);
              }
           fclose(concpr);
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

         ptemp[i]=ptemp[i]+(t_step*(pgrad[i]+(1./tou_c)*(1.0/x_step)*alphap[i]*pconc[i]));
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

/****** print  function  *******/

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
               if(t*t_step>=printdelay){
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
           ave_pos=temp_pos*x_step;
  
        if (myid == 0) {
  	   FILE * frontfile = fopen(filename2, "a");
	   fprintf(frontfile, "%f  %f  %f\n", ave_pos,ave_max_temp, t*t_step);
           fclose(frontfile);

     }
   }//End of front pos print
 }//end of front function

/****** End of writing of front position *****/


void shuffle(int * Array, int size)

{
      int i = size - 1;
      int j, temp;

      srand(time(NULL));

      while (i > 0)
      {
           // std::cout<<i<<"\t"<<size<<"\n";
            j = rand() % (i + 1);
            temp = Array[i];
            Array[i] = Array[j];
            Array[j] = temp;
            i = i - 1;
      }
}


/********RANDOM NUMBER GENERATOR ***********/

unsigned int rand_int(){  
    unsigned int result=0;  
    for(unsigned int i=0;i<sizeof(int);++i){  
        result=(result<<8)+rand()%256;  
    }  
    return result;  
}  
  
int rand_range(int lower, int upper){    
    if(lower>upper){ // Swap if lower>upper    
        std::swap(lower,upper);  
    }  
    return rand_int()%(upper-lower+1)+lower;    
}  
  
int GenerateRandomNumbers(int* result, int lower, int upper, int count){  
      
    if(lower>upper){ // Swap if lower>upper  
        std::swap(lower,upper);  
    }  
      
    if(count>upper-lower+1){ // It is impossible to generate the array  
        return 0;  
    }  
      
    int diff=upper-lower;         
      
    for(int i=0;i<=count;++i){ // Initialize the array with the first numbers  
        result[i]=i+lower;  
    }  
      
    std::map<int,int> number_map;  


    for(int i=0;i<=count;++i){  
        int index=rand_range(i,diff);  
        if(index<=count){ // The index is in the array, so swap the items  
            std::swap(result[i],result[index]);  
        }  
        else{  
            std::map<int,int>::iterator it=number_map.find(index+lower);  
            if(it!=number_map.end()){ // Lookup the number_map  
                std::swap(it->second,result[i]);  
            }  
            else{  
                number_map.insert(std::make_pair(index+lower,result[i])); // Add the item into the number_map  
                result[i]=index+lower;  
            }  
        }  
    }  
      
    return count;  
}  


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

