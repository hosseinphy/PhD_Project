/***************************************/

#include"post_param.h"

/***************************************/

using namespace std;

/***************************************/
 //pointer to the files

 ifstream myReadFile;
 ifstream myReadFile2;
 ifstream input("parallel_rough_input0.in");

/**************************************************/
                                                  //
//Global Variable Definitions (For SIMULATION):   // 
                                                  //
char   filename[BUFSIZ];                          //                          
char   filename2[BUFSIZ];                         //    
char   filename3[BUFSIZ];                         //    
char   filename4[BUFSIZ];                         //    
char   filename5[BUFSIZ];                         //    
char   filename6[BUFSIZ];                         //    
char   filename7[BUFSIZ];                         //    
                                                  //          
int    chunk_num=100;                             //
int    chunk_dim;                                 //
int    L=101;//should be odd number               // 
int    rad_corr=350;                              //
int    smth_dim;                                  //
int    Nx,Ny,Nstep;                               //
int    printfreq,printfrontpos,printconcfreq;     //
double t_step,x_step;                             //
double tot_time;                                  //
long    seed;                                     //
                                                  //
//Global Variable Definitions (Post Processing):  

int filenumber;                                   //
int front_data_size;                              //
int temp_data_size;                               //
int input_data_size;                              //
int len_x;                                        //
int len_y;                                        //
int *f_n;                                         //
int *tpf;                                         //
double h_diff;                                    //
double ***h;                                      //
double ***max_temp;                               //
double ***h_corr;                                 //
double ***hdiff_corr;                             // 
double **array;                                   //
double **corr;                                    //
double **hght_corr_diff;                          //
double **vel_array;                               //
double **pos;                                     //
double **h_ave;                                   //
double **Time;                                    //
double **acc;                                     //
double *width;                                    //
double *velocity;                                 // 
double *log_width;                                //
double *log_time;                                 //
double *log_width_drv;                            //
double *log_width_drv_smth;                       //
double *V_ave;                                    //
double *Vel_mean;                                 //
double *Vel_var;                                  //
double *Vel_div;                                  //
double *Tign;                                     //
double *Tbck;                                     // 
double *tou_c;                                    //
double *Ctshold;                                  //

/**************************************************/

/**************/
int main ()
{
system ("clear");
Setup();
dealloc();
return 0;
}

/**************/

void Setup()
{
  data();
  AllocArrays();
  read_data();
}

/*********************************/
void AllocArrays()                  
{ 

 int i,k,j;
//for making sure if the Nx and Ny are integers

 Nx=int(len_x/x_step);
 Ny=int(len_y/x_step);
 Nstep=int(tot_time/t_step);
 temp_data_size = Nx*Ny;
 front_data_size=500;//this is number of profile files (tot_time/t_step)/printfrontpos;

// smth_dim=front_data_size/L;
/********************POST PROCESS DATA ALLOCATION *********/

 h              = new double**[filenumber];
 max_temp       = new double**[filenumber];
 h_corr         = new double**[filenumber];
 hdiff_corr     = new double**[filenumber];
 array          = new double* [filenumber];
 corr           = new double* [front_data_size];
 hght_corr_diff = new double* [front_data_size];
 vel_array      = new double* [filenumber];
 pos            = new double* [filenumber];
 h_ave          = new double* [filenumber];
 Time           = new double* [filenumber];
 acc            = new double  *[filenumber];
 V_ave          = new double  [filenumber];
 Vel_var        = new double  [filenumber];
 Vel_mean       = new double  [filenumber];
 width          = new double  [front_data_size];
 velocity       = new double  [front_data_size];
 log_width      = new double  [front_data_size];
 log_time       = new double  [front_data_size];
 log_width_drv  = new double  [front_data_size];
 log_width_drv_smth=new double [front_data_size];

//Position , time , velocity & acceleration

   for (i = 0; i < filenumber; i++) {

      pos[i]       = new double [front_data_size];
      h_ave[i]     = new double [front_data_size];
      Time[i]      = new double [front_data_size];
      array[i]     = new double [front_data_size];
      vel_array[i] = new double [front_data_size];
   }
   
   for (i = 0; i < front_data_size; i++) {
      corr[i] = new double [rad_corr];
      hght_corr_diff[i] = new double [rad_corr];
    }


   for ( i = 0; i < filenumber; i++) {

      h[i] = new double *[front_data_size];
      max_temp[i] = new double *[front_data_size];
        for ( j = 0; j < front_data_size; j++) {

           h[i][j] = new double[Ny+6];
           max_temp[i][j] = new double[Ny+6];
        }
    }

   for ( i = 0; i < filenumber; i++) {
      h_corr[i] = new double *[front_data_size];
      hdiff_corr[i] = new double *[front_data_size];
        for ( j = 0; j < front_data_size; j++) {
           h_corr[i][j] = new double[rad_corr];
           hdiff_corr[i][j] = new double[rad_corr];
        }
    }
}

/***********Read Data for Allocation***************/
void data(){

    int i;
    double trash;
    int  a;
    string G;
    filenumber=16;
    //input>>G>>filenumber;

    Tign     = new double[filenumber];
    Tbck     = new double[filenumber];
    tou_c    = new double[filenumber];
    Ctshold  = 	new double[filenumber];
    f_n      = new int[filenumber];
    tpf      = new int[filenumber];
   
  //Simulation data reading ....
    
    string PP[BUFSIZ];
    string st1 = "parallel_rough_input";
    string ext = ".in";
    string FileName;


	for (i=0; i<filenumber; i++){

		stringstream ss;
		ss<<i;	
		FileName = st1 + ss.str() + ext;

                myReadFile.open(FileName.c_str());

                    cout<<FileName.c_str()<<endl;
                 if (myReadFile.is_open()) {
                      
                   while (!myReadFile.eof()) {


                     myReadFile >>PP[i]>>a;
                     myReadFile >>PP[i]>>f_n[i];
                     myReadFile >>PP[i]>>Tign[i];                     
                     myReadFile >>PP[i]>>trash;
                     myReadFile >>PP[i]>>trash;
                     myReadFile >>PP[i]>>Tbck[i];
                     myReadFile >>PP[i]>>trash;
                     myReadFile >>PP[i]>>trash;
                     myReadFile >>PP[i]>>tou_c[i];
                     myReadFile >>PP[i]>>trash;
                     myReadFile >>PP[i]>>trash;
                     myReadFile >>PP[i]>>seed;
                     myReadFile >>PP[i]>>Ctshold[i];
                     myReadFile >>PP[i]>>t_step;
                     myReadFile >>PP[i]>>x_step;
                     myReadFile >>PP[i]>>trash;
                     myReadFile >>PP[i]>>trash;
                     myReadFile >>PP[i]>>trash;
                     myReadFile >>PP[i]>>trash;
                     myReadFile >>PP[i]>>len_x;
                     myReadFile >>PP[i]>>len_y;
                     myReadFile >>PP[i]>>tot_time;
                     myReadFile >>PP[i]>>printfreq;
                     myReadFile >>PP[i]>>printfrontpos;
                     myReadFile >>PP[i]>>printconcfreq;

                    }
                 }
      

                //cout<<tou_c[0]<<"\t"<<Tign[0]<<"\t"<<Ctshold[0]<<endl;
            myReadFile.close();

         }
  }

/*************Read Data for Post Process*************/

void read_data(){

     int    i,j,k,l;
     char   trash1;
     double trash2;
     double jj; //only for test
     FILE   * pFile;
     int    **t_domain;
     double *chunk_max;
     int    *chunk_max_time;
     double *chunk_max_vel;
     string capture[BUFSIZ];
     int    cap_num[Ny];
     
     /***********************************/

     int t_counter;

        for(i=0;i<filenumber;i++){
         t_counter=0;
          k=0;
           while(k<front_data_size){
            sprintf(filename2,"profile%d_%d_%0.5f_%0.6f.txt",i,k,t_step,Tbck[i]);
             myReadFile2.open(filename2);

              if (myReadFile2.is_open()) {            
                      
                   myReadFile2>>capture[i]>>trash1>>capture[i];
                   myReadFile2>>h_ave[i][k]>>trash1>>Time[i][k];
                   myReadFile2>>capture[i];

                    for(j=6;j<Ny+6;j++){
                      myReadFile2>>cap_num[j-6]>>h[i][k][j-6]>>max_temp[i][k][j-6];
                    }
               }

              else {
                 t_counter-=1;
              }

              myReadFile2.close();              
              t_counter+=1;
              k=k+1;
          }  

        tpf[i]=t_counter;
      }

  //finding the max number of time files for each file overal
  //This helps when number of files for each conc are not same 


   int max_len=0;

    for (i=0;i<filenumber;i++){
        if(tpf[i]>max_len){
          max_len = tpf[i];
        }
    }

              /*sprintf(filename7,"test.txt");
               FILE * writetest = fopen(filename7, "w");

                for(i=0;i<filenumber;i++){

                  fprintf(writetest, "%d,\n",tpf[i]);
                 for(k=0;k<front_data_size;k++){
                  for(j=0;j<Ny;j++){
                   fprintf(writetest, "%d,%d,%d,%f,%f\n",i,k,cap_num[j],h[i][k][j],max_temp[i][k][j]);
                   fprintf(writetest, "\n\n");
                  }
                 }
               }

              fclose(writetest);*/

 
 /**************************/
 //wdith calculation

   for(i=0; i<filenumber; i++){
      k=0;
        while(k<max_len){
         h_diff=0.0;
          if(k<tpf[i]){
           for(j=0; j<Ny; j++){
            h_diff += Sqr(h[i][k][j]-h_ave[i][k]);
           }
          array[i][k]=sqrt(h_diff/Ny);
         }
        k=k+1;
       }
    }


//Configurational averaging of width

    double sum = 0.0;
    int counter = 0;

    for(k=0;k<max_len;k++){
        i=0;
          while(i<filenumber){
           if(k<tpf[i]){
              sum += array[i][k];
              counter=counter+1;
            }
           i=i+1;
          }
        width[k]= sum/counter;
       sum = 0.0;
      counter = 0;
     }


//******Height-Height Correlation function*******/

 double h_nomer;
 double h_next;
 double h_now;
 double h_denom;

   for (i=0; i<filenumber; i++){
       k=0;
       while(k<max_len){
         if(k<tpf[i]){
           for(l=0;l<rad_corr;l++){
             h_nomer=0.0;
             h_denom=0.0;
             for(j=0; j<Ny/2; j++){
              h_next = (h[i][k][j+l]-h_ave[i][k]);
              h_now  = (h[i][k][j]-h_ave[i][k]);
              h_nomer+=h_next*h_now;
              h_denom+=(Sqr(h_now));
              }
             h_corr[i][k][l]=sqrt(h_nomer/(Ny/2))/(sqrt(h_denom/(Ny/2)));
           }
         }
       k=k+1;
     }
  }

//Configurational averaging of height_height_corr

    double sum_corr = 0.0;
    int counter_corr = 0;

     for(k=0;k<max_len;k++){
        if(k<tpf[i]){
          for(l=0;l<rad_corr;l++){
           i=0;
            while(i<filenumber){
             sum_corr += h_corr[i][k][l];
             counter_corr=counter_corr+1;
             i=i+1;
           }
           corr[k][l]=sum_corr/counter_corr;
           sum_corr = 0.0;
           counter_corr = 0;
          }
        }
      }

/**********************/

//******Height-diffrence Correlation function*******/
/*
 double height_diff;
 double height_sum;

   for (i=0; i<filenumber; i++){
       k=0;
       while(k<max_len){
         if(k<tpf[i]){
           for(l=0;l<rad_corr;l++){
             height_sum=0.0;
             for(j=0; j<Ny/2; j++){

               height_diff  = (h[i][k][j+l]-h[i][k][j]);
               height_sum+=(Sqr(height_diff));
              }
             hdiff_corr[i][k][l]=sqrt(height_sum/(Ny/2));
           }
         } 
       k=k+1;
     }
  }


//Configurational averaging of height_diffrence_corr

    double sum_diffcorr = 0.0;
    int counter_diffcorr = 0;

     for(k=0;k<max_len;k++){
        if(k<tpf[i]){
          for(l=0;l<rad_corr;l++){
           i=0;
            while(i<filenumber){
             sum_diffcorr += hdiff_corr[i][k][l];
             counter_diffcorr=counter_diffcorr+1;
             i=i+1;
           }
           hght_corr_diff[k][l]=sum_diffcorr/counter_diffcorr;
           sum_diffcorr = 0.0;
           counter_diffcorr = 0;
          }
        }
      }  
*/
/**********************/
/**********************/
//Velocity calculation

   for (i=0; i<filenumber; i++){
       k=0;
       while(k<max_len){
         if(k==0){
           vel_array[i][k]=0;
          }
         else{
           vel_array[i][k]=((h_ave[i][k+1]-h_ave[i][k-1])/(2*(Time[i][k+1]-Time[i][k])));
          }
         k=k+1;
        }
      }



//Configurational averaging of velocity

    double sum_vel = 0.0;
    int counter_vel = 0;

    for(k=0;k<max_len;k++){
        i=0;
          while(i<filenumber){
           if(k<tpf[i]){
              sum_vel += vel_array[i][k];
              counter_vel=counter_vel+1;
            }
           i=i+1;
          }
        velocity[k]= sum_vel/counter_vel;
       sum_vel = 0.0;
      counter_vel = 0;
     }

/**************************************************/

//Get rid of the transient part of width & velocity

//Devide time domain
//we intend to devid the whole dome to 100 chunks

chunk_dim = max_len/chunk_num;
t_domain = new int  *[chunk_num];


chunk_max= new double [chunk_num];
chunk_max_time = new int [chunk_num];
chunk_max_vel= new double [chunk_num];


   for (i = 0; i <chunk_num ; i++) {
       t_domain[i] = new int [chunk_dim];

       chunk_max[i] = 0.0;
       chunk_max_vel[i] = 0.0;
       chunk_max_time[i] = 0;}

int chunk_counter=0;
    for(i=0;i<chunk_num;i++){
      j=0;
      while (chunk_counter<max_len && j<chunk_dim){
       t_domain[i][j] = chunk_counter;
       chunk_counter+=1;
       j=j+1;
     }
   }

int G;
   for(i=0;i<10;i++){
     for (j=0;j<chunk_dim;j++){
        G = t_domain[i][j];
        if(width[G]>chunk_max[i]){
          chunk_max[i] = width[G];
          chunk_max_vel[i]=velocity[G];
          //max_time[i]=t_domain[i][j];
        }
      }
/*      cout<<chunk_max[i]<<endl;
     cout<<chunk_max_vel[i]<<endl;
    cout<<"\n"<<endl;*/
  }

double max=0.0;
 int max_time;
  for(i=0;i<10;i++){
    if(chunk_max[i]>max){
       max=chunk_max[i];
       max_time = i;
    }
  }
cout<<max<<"\t"<<max_time<<endl;

//do the same for velocity

 double max_vel=0.0;
 int max_time_vel;
  for(i=0;i<10;i++){
    if(chunk_max_vel[i]>max_vel){
       max_vel=chunk_max_vel[i];
       max_time_vel = i;
    }
  }
//cout<<max_vel<<"\t"<<max_time_vel<<endl;


//Width

   int     min_time=0;
   double  min=0;

  for(i=max_time;i<20;i++){

      if(width[i]<width[i-1] && width[i]<width[i+1]){

         min = width[i];
         min_time = i;
      }
  }

    cout<<min<<"\t"<<min_time<<endl;

//velocity

   int     min_time_vel=0;
   double  min_vel=0;

  for(i=max_time_vel;i<20;i++){

      if(velocity[i]<velocity[i-1] && velocity[i]<velocity[i+1]){

         min_vel = velocity[i];
         min_time_vel = i;
      }
  }

//    cout<<min_vel<<"\t"<<min_time_vel<<endl;
                                             
//more process for widrh & velocity
//
double log_t_drv;
double log_w_drv;

//log definition
//NOTIVE !!!! we have to start from i=1 not i=0

      for(i=min_time;i<max_len;i++){
        if(i>1){
           log_width[i]=log(width[i]);
           log_time[i]=log(i);
         }
        else if(i==1 || i==0) {
           log_width[i]=0;
        }
      }

      for(i=min_time;i<max_len;i++){
        if(i!=0 && i!=1){
         log_w_drv=log(width[i])-log(width[i-1])/(i-(i-1));
         log_t_drv=1./((log_time[i]-log_time[i-1])/(i-(i-1)));
         log_width_drv[i]=log_w_drv*log_t_drv;
       }
       else{
         log_width_drv[i]=0;
       }
     }



///////////////////////
////Smoothing the log_width_drv eq dlog(W(t))/dlog(t)
//
//for(int p=0; p<SIZE;p++){
//    
//     V[p]=0;//Velocity*time
//     T[p]=0;//Time_sum}

  //window filtering lenght     

  j=0;
  int win_index=(L-1)/2;
  int win_count;
    while(j<max_len){
       double smth_sum=0.0;

      if(j>=(win_index) && j<=(max_len-win_index)){
        for(k=j-win_index;k<j+win_index;k++){
           smth_sum+=log_width_drv[k];
         }
       log_width_drv_smth[j]=smth_sum/L;
      }
      else if (j>(max_len-win_index)){
       win_count=0;
        for (k=j-win_index;k<max_len;k++){
            smth_sum+=log_width_drv[k];
            win_count+=1;
         }
       log_width_drv_smth[j]=smth_sum/(win_count);
      }                                                                                                                                       
     else if(j<win_index){
       win_count=0;
        for(k=0;k<j+win_index;k++){
           smth_sum+=log_width_drv[k];
            win_count+=1;
        }
       log_width_drv_smth[j]=smth_sum/(win_count);
     }

    j+=1;
  }


/**************************************************/


//Writing height_height correlation function

  sprintf(filename6,"hght_diff%d%s%d%s%.2f%s%.2f%s%.2f.txt",len_x,"_",len_y,"_",tou_c[0],"_",Tign[0],"_",Ctshold[0]);

  FILE * crr_diff = fopen(filename6, "w");

      for(k=min_time;k<max_len;k++){
        for(l=0;l<rad_corr;l++){
          fprintf(crr_diff, "%f,%d\n", (hght_corr_diff[k][l]), k);
        }
      }

  fclose(crr_diff);


//Writing height_height correlation function

  sprintf(filename5,"hght_hght%d%s%d%s%.2f%s%.2f%s%.2f.txt",len_x,"_",len_y,"_",tou_c[0],"_",Tign[0],"_",Ctshold[0]);

  FILE * crr = fopen(filename5, "w");

      for(k=min_time;k<max_len;k++){
        for(l=0;l<rad_corr;l++){
          fprintf(crr, "%f,%d\n", (corr[k][l]), k);
        }
      }

  fclose(crr);


//Writing dlog(W(t))/dt

  sprintf(filename3,"log_width%d%s%d%s%.2f%s%.2f%s%.2f.txt",len_x,"_",len_y,"_",tou_c[0],"_",Tign[0],"_",Ctshold[0]);

  FILE * drv = fopen(filename3, "w");

      for(i=min_time;i<max_len;i++){

       fprintf(drv, "%f,%d\n", (log_width_drv_smth[i]), i);

      }

  fclose(drv);



//Writing width of the interface

  sprintf(filename,"width%d%s%d%s%.2f%s%.2f%s%.2f.txt",len_x,"_",len_y,"_",tou_c[0],"_",Tign[0],"_",Ctshold[0]);

  FILE * frontfile = fopen(filename, "w");

      for(i=min_time;i<max_len;i++){

       fprintf(frontfile, "%f,%d\n", (width[i]-min), i);

      }

  fclose(frontfile);

//Writing velocity of the interface

  sprintf(filename4,"velocity%d%s%d%s%.2f%s%.2f%s%.2f.txt",len_x,"_",len_y,"_",tou_c[0],"_",Tign[0],"_",Ctshold[0]);

  FILE * frontvel = fopen(filename4, "w");

      for(i=0/*min_time_vel*/;i<max_len;i++){

       fprintf(frontvel, "%f,%d\n", (velocity[i]/*-min_vel*/), i);

      }

  fclose(frontvel);
                     

}//end of read file



//%%%%%%% Allocation function %%%%%%%%%%%%%%%%%%%%%%%%%%

void dealloc(){

 int i,j;

 for (i = 0; i <filenumber; ++i){
    for(j = 0; j< front_data_size; ++j){
        delete[] h[i][j];
        delete[] h_corr[i][j];
        delete[] hdiff_corr[i][j];
        delete[] max_temp[i][j];
     }
     delete[] h[i];
     delete[] h_corr[i];
     delete[] hdiff_corr[i];
     delete[] max_temp[i];
  } 
/*
    for (i=0; i <filenumber; ++i ) {

        delete[] array[i];
        delete[] vel_array[i];
        delete[] pos[i];
        delete[] h_ave[i];
        delete[] Time[i];
        delete[] acc[i];
      }

     delete[] array;
     delete[] vel_array;
     delete[] pos;
     delete[] h_ave;
     delete[] Time;
     delete[] acc;*/
     delete[] h;
     delete[] h_corr;
     delete[] hdiff_corr;
     delete[] max_temp;
 }
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//



    





