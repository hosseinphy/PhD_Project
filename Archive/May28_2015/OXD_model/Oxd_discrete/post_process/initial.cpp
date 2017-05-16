#include"Mddef.h"

/***************************************/

using namespace std;

/**************************************/

 //pointer to the files

 ifstream myReadFile;
 ifstream myReadFile2;
 ifstream input("input0.in");

/*************************************/


//Global Variable Definitions (For SIMULATION):  

double t_step,x_step;                                           
int    Nx,Ny,Nstep;
int    tot_time,printfreq,printfrontpos;

//Global Variable Definitions (For Post Processing):  

int    filenumber;
int    front_data_size;
int    temp_data_zize;
int    input_data_size;

double **pos;

double **Time;
double **Temp;
double **velocity; 
double **acc; 
double *V_ave;
double *Vel_mean;
double *Vel_var;
double *Vel_div;
double *Acc_ave;
double *Acc_mean;
double *Acc_var;
double *Acc_div;
double *Tign; 
double *Tbck; 
double *tou_c;
int    grid_x;
int    len_y;


/********************************/
/*int main ()
{
system ("clear");
Setup();

}*/
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

 int i;
//for making sure if the Nx and Ny are integers

 Nx=int(grid_x/x_step);
 Nstep=int(tot_time/t_step);
 temp_data_zize = Nx;
 front_data_size=6000;//(tot_time/t_step)/printfrontpos;

/********************POST PROCESS DATA ALLOCATION *********/

pos      = new double*[filenumber];
Time     = new double*[filenumber];
Temp     = new double*[filenumber];
velocity = new double*[filenumber];
acc      = new double*[filenumber];

V_ave    = new double[filenumber];
Vel_var  = new double[filenumber];
Vel_mean = new double[filenumber];
Vel_div    = new double[filenumber];

Acc_ave    = new double[filenumber];
Acc_var  = new double[filenumber];
Acc_mean = new double[filenumber];
Acc_div    = new double[filenumber];
    
  for (int i =0; i < filenumber; i++) {

      pos[i]      = new double[front_data_size];
      Time[i]     = new double[front_data_size];
      Temp[i]     = new double[front_data_size];
      velocity[i] = new double[front_data_size];
      acc[i]      = new double[front_data_size];

    }
}


/***********Read Data for Allocation***************/
void data(){

    int i;
    double  trash;
    int  a,b,c,d,e,f,g,h,l,m,n,o;
    string G;


    input>>G>>a;
    //filenumber=a;
    filenumber=20;

    Tign     = new double[filenumber];
    Tbck     = new double[filenumber];
    tou_c    = new double[filenumber];

   //Simulation data reading ....
    

    string PP[BUFSIZ];
    string st1 = "input";
    string ext = ".in";
    string filename;


	for (i=0; i<filenumber; i++){

		stringstream ss;
		ss<<i;	
		filename = st1 + ss.str() + ext;
                cout<<filename<<endl;

                myReadFile.open(filename.c_str());

                 if (myReadFile.is_open()) {
                      
                   while (!myReadFile.eof()) {                         

                     myReadFile >>PP[i]>>trash;
                     myReadFile >>PP[i]>>trash;
                     myReadFile >>PP[i]>>Tign[i];
                     myReadFile >>PP[i]>>trash;
                     myReadFile >>PP[i]>>trash;
                     myReadFile >>PP[i]>>Tbck[i];
                     myReadFile >>PP[i]>>trash;
                     myReadFile >>PP[i]>>trash;
                     myReadFile >>PP[i]>>tou_c[i];
                     myReadFile >>PP[i]>>trash;
                     myReadFile >>PP[i]>>trash;
                     myReadFile >>PP[i]>>trash; 
                     myReadFile >>PP[i]>>trash; 
                     myReadFile >>PP[i]>>t_step;
                     myReadFile >>PP[i]>>x_step;
                     myReadFile >>PP[i]>>trash;
                     myReadFile >>PP[i]>>trash;                     
                     myReadFile >>PP[i]>>trash;
                     myReadFile >>PP[i]>>grid_x;
                     myReadFile >>PP[i]>>tot_time;
                     myReadFile >>PP[i]>>printfreq;
                     myReadFile >>PP[i]>>printfrontpos;
                     myReadFile >>PP[i]>>trash;



                    }
                 }

            myReadFile.close();

         }
  }

/*************Read Data for Post Process*************/

void read_data(){

     int i,j;
     char trash1,trash2;
     string PPP[BUFSIZ];

    string st2 = "frontoxd";
    string ext2 = ".txt";
    string filename2;

    j=0;

     for(i=0;i<filenumber;i++){

		stringstream dd;
		dd<<i;
		filename2 = st2 + dd.str() + ext2;

                myReadFile2.open(filename2.c_str());
/*
                 if (myReadFile2.is_open()) {
                      
                   while (!myReadFile2.eof()) {
*/
                    for(j=0;j<front_data_size;j++){
                     myReadFile2>>pos[i][j]>>trash1>>Temp[i][j]>>trash2>>Time[i][j];

               //      ++j;
                 
                   }
             //   }


            myReadFile2.close();

            j=0;

      }

/*for(int i=0;i<filenumber;i++){

  for(int k=0;k<SIZE;k++){

 OUTPUT<<pos[i][k]<<"\t"<<Time[i][k]<<endl;


  }

 OUTPUT<<"\n\n"<<endl;
 }*/
}

