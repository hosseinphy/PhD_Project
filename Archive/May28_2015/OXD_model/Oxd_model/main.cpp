/*  1D version of the code Oxidation code
    modified at May 28 , 15 , 5:00 pm
    this code now equipied with RNG
    next step will be using more suitable
    uniform RNG, other than rand() 
    
                  
    there is an option for 

  - heat dissipation  
  - Temperature Update
  - Concenteration Update
  - Temperature Boundary Condition
  - Concenteration Boundary Condition */ 
   

/********************************************/

 //File Inclusion
 /************************/

 #include"initial.cpp"

 /************************/

 void shuffle(int *,int);
 
/**********************************************///
 //Global Variable Definitions:                  //
                                                 //
 double Ctshold,Gamma,q_T,Lambda_1,beta,Diff;    //
 double tou_c;                                   //
 double Q,cap_Gamma,TC,TL,T_bar,mass_Diff,qo;    //
 unsigned long  seed;                            //
 int    printfreq, printfrontpos,printconcfreq;  //
 int    t;                                       //
 int    f_n,t_n;                                 //
 char   filename[BUFSIZ];                        //
 char   filename2[BUFSIZ];                       //
 char   filename3[BUFSIZ];                       //
 char   filename4[BUFSIZ];                       //
 char   filename5[BUFSIZ];                       //
 char   filename6[BUFSIZ];                       //
 char   filename7[BUFSIZ];                       //
 char   filename8[BUFSIZ];                       //
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
  NameI (printfreq),
  NameI (printfrontpos),
  NameI (printconcfreq),

 };

/****** END OF INPUT FILE *******/


/**********************************/
/*****        Main loop      ******/
/**********************************/

 int main (int argc, char ** argv){
  
  system ("clear");
  GetNameList (argc, argv);
  PrintNameList (stdout);
  Setup();
  Ran_Conc();
 
  //creat file for storing front position
   sprintf(filename2,"frontoxd%d_%0.2f_%0.3f.txt",f_n,tou_c,Tg);
   FILE * frontfile = fopen(filename2, "w");
   fprintf(frontfile, "");
   fclose(frontfile);

        //** Simulation Step **//

            for(t=0;t<Nstep;t++){  

               
                grad();              //Laplacian term calculation

                /********* Reaction Rate options and temperature update *******/

                Calc_higgins();    //Oxidation model reaction rate
                //Calc_prov_1();     //reaction rate  (T_bat)^(3/2)*exp(-1/T_bar)      
                //Calc_prov_2();     // Nik's -explosive crystalisation       
                //Calc_prov_3();       // Nik's -explosive crystalisation       
                
                /********* update concenteration **********/
                // update_conc_1();  //reaction rate  (T_bat)^(3/2)*exp(-1/T_bar)
                // update_conc_2();  // Nik's -explosive crystalisation
                //update_conc_3();     // Nik's -explosive crystalisation

                /********* Boundary conditions **********/
                zero_flux_boundary();

                //Dissipation();
                //periodic();
                //conc_boundary();   //implicable only if there is mass diffusion
          
                /********* printing out data **********/

                //Temp_write();
                //Conc_write();
                front();

         } //** end of simulation step **/

     Dealloc();
     return 0;

 } //** end of main loop **/

//#########################################/ 

//-----FUNCTIONS-------//

/*******Randon number Generator***/

void Ran_Conc(){

    int min=1;
    int max=grid_x;
    double number ;
    int i,j;
    int *values;
    int counter=0;
    int result[grid_x];  
    unsigned long ff;


    //Deterministic distribution
    /***************************/
    /* i=1;                    //
       while(i<=Nx){           //
         C[i]=1.0;             //
         i=i+conc_step;}       */
    /***************************/  
    
    //Random distribution of particles
    //Constrain!!! (# of particle is constant)

    // Read already generated seed from file
    
    sprintf(filename8, "seed_number.dat");
    FILE * seedfile = fopen(filename8, "r");
         fscanf(seedfile, "%10lu \n", &ff);
         fclose(seedfile);


     //Seeding RNG
    
     srand(ff);
    
     // Generate grid_x random numbers within the range 1 to Nx 
     
     GenerateRandomNumbers(result,1,Nx,grid_x);  

     // Now shuffle the array values randomly.

     shuffle(result,grid_x);

     // fill up the box based on RNG

     for (int i = 1; i <=Nx ; i++) {   
          C[i]=0.0;}
     for (int i = 0; i < grid_x; i++) {        
          C[result[i]]=1;
          counter+=1;     
     }
   // std::cout<<"number of particles : "<<counter<<"\n";
   

    sprintf(filename6, "random_dist.dat");
     FILE * rann = fopen(filename6, "w"); //if first process, create the file              

         for (i = 0; i < grid_x; i++) {
           fprintf(rann, "%d %d \n", i,result[i]);
         }
    fclose(rann);


    sprintf(filename7, "Conc_dist.dat");
     FILE * concr = fopen(filename7, "w"); //if first process, create the file              

         for (i = 1; i <= Nx; i++) {
           fprintf(concr, "%d %f \n", i,C[i]);
         }
    fclose(concr);

  }

  /*****************************************/

/********Grdaient Function*********/

void grad(){

 int i,j;
 double X_2;
 X_2=(x_step*x_step);
   
   for(i=1;i<=Nx;i++){ 
      Grad[i] = ((T[i+1]+T[i-1])-2.0*(T[i]))/(X_2);
   }
/*
   for(i=1;i<=Nx;i++){
   
    if(i==1){     
      Grad_conc[i]= -1.0*(-T[i+2]+4*T[i+1]-3*T[i])/(2.0*x_step);
     }
    else if (i==Nx){
   

      Grad_conc[i]= -1.0*(T[i-2]-4*T[i-1]+3*T[i])/(2.0*x_step);
   } 
    else {

      Grad_conc[i]= -1.0*(T[i+1]-T[i-1])/(2.0*x_step);
  }
 }*/
}

/************** END OF GRAD FUNCTION  ***************/

/***** Main Functions *****/

void Calc_prov_1(){

 int i,j;

   for(i=1;i<=Nx;i++){
      T_bar=T[i]/Tg;
      q_T= pow((T_bar),beta)*(exp(-1./T_bar));
      T[i]=T[i]+(t_step*(Diff*Grad[i]+Lambda_1*q_T*C[i]-Gamma*(T[i]-To)));
   } 
 }

 ///////////////////

void Calc_prov_2(){

 int i,j;
 double g_1;
 double roo_1;

   roo_1=mass_Diff/Diff;
   g_1=Gamma/cap_Gamma;

    for(i=1;i<=Nx;i++){
      q_T= Q*(exp(-(Tg/(T[i]))))*(1.0-(exp((-1.0*TL)*((1./T[i])-(1./TC)))));
      T[i]=T[i]+t_step*(Grad[i]-(T[i]-To)+(g_1*q_T*C[i])-(roo_1*Grad_conc[i]));
          //printf ("isnan(T[i]): %d\n",isnan(T[i]));
    }   
 }

////////////////////

void Calc_prov_3(){

 int i,j;

    for(i=1;i<=Nx;i++){
      q_T= Q*(exp(-(Tg/(T[i]))))*(1.0-(exp((-1.0*TL)*((1./T[i])-(1./TC)))));
      T[i]=T[i]+t_step*((Grad[i])-(cap_Gamma*(T[i]-To))+(1*q_T*C[i]))/*-(qo*mass_Diff*Grad_conc[i]))*/;
      //printf ("isnan(T[i]): %d\n",isnan(T[i]));     
    }   
 }

/*********** HIGGINS *************/

/***** Main Function *****/

void Calc_higgins(){

 int i,j;
	  
       for(i=1;i<=Nx;i++){    
               if(tc[i]>=0){                   
                    if(T[i]>=Tg){                  
                        if(tc[i]== 0.0){// those reactant who has a time equall zero is a possible reactant for buring                       
                          tc[i]=(t*t_step)+tou_c;//end time of burning
                          alpha[i]=1;}
                        else if(tc[i]>(t*t_step)){
                          tc[i]=tc[i];
                          alpha[i]=1;}          
                        else {
                          tc[i] = -1.0;
                          alpha[i]=0;}
                    }
                    else if(T[i]<Tg){                         
                        if(tc[i]==0.0){      
                          tc[i]=0.0;
                          alpha[i]=0;}
                        else if(tc[i]>(t*t_step)){
                          tc[i]=tc[i];
                          alpha[i]=1;}
                        else {                            
                          tc[i]=-1;
                          alpha[i]=0;}
                   }
               }  
	     else {
              tc[i]=-1;
              alpha[i]=0.0;
           }
      }

 //Updating tempreture afterwards

        for(i=1;i<=Nx;i++){    
         T[i]=T[i]+(t_step*(Grad[i]+((1.0/tou_c)*(1.0/x_step)*alpha[i]*C[i])));
       }
 }


/************** END OF RR CALC ***************/

/************** Boundary Function ***********/

void zero_flux_boundary(){
 int i,j;

    T[0]=T[2];
    T[Nx+1]=T[Nx-1];
 }

/************** END ***************/

 void Dissipation(){
  int i,j;
     T[0]=(To+(T[1]/(cap_Gamma*x_step)))/(1+1/(cap_Gamma*x_step));
     T[Nx+1]=(To+(T[Nx]/(cap_Gamma*x_step)))/(1+1/(cap_Gamma*x_step));
 }
/************** END ***************/

/*****Periodic Boundary Function******/

void periodic(){
 int i,j;
 
   //Periodic Boundary at 2 ends
     T[0]=T[Nx];
     T[Nx+1]=T[1];
 }

/************** END ***************/

 void update_conc_1(){
  int i,j;
   
    for(i=1;i<=Nx;i++){           
        T_bar=T[i]/Tg; 
        q_T= pow((T_bar),beta)*(exp(-1./T_bar));
        C[i]=C[i]-t_step*(q_T*C[i]);
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
      
          q_T= Q*(exp(-(Tg/(T[i]))))*(1.0-(exp((-1.0*TL)*((1./T[i])-(1./TC)))));
          C[i]=C[i]+t_step*((roo_2*Grad_conc[i])+(-1.0*g_2*q_T*C[i]));
    }
 }

////////////////////////

 void update_conc_3(){

 int i,j;

    for(i=1;i<=Nx;i++){
      q_T= Q*(exp(-(Tg/(T[i]))))*(1.0-(exp((-1.0*TL)*((1./T[i])-(1./TC)))));
      C[i]=C[i]+t_step*((mass_Diff*Grad_conc[i])+(-1.0*q_T*C[i]));
    }
 }

/********* Concentertaion Boundary condition ***********/

 void conc_boundary(){

 //zero concenteration flux at boundaries
   C[0]=C[2];
   C[Nx+1]=C[Nx-1];
 }

/********* END OF BOUNDARY CONDITIONS  ***********/


/*********Writing Functions************/

void Temp_write(){

   int num;
   int a,b;

   if (t%printfreq == 0) {			
       sprintf(filename5, "array_%0.5f_%d.txt",x_step,t/printfreq);
        FILE * tempfile = fopen(filename5, "w"); 						
           for (a = 1; a <=Nx; a++) {
             fprintf(tempfile, "%f \n", T[a]);
	   }	         
        fclose(tempfile);
     }
 }

/************** END ***************/


/**********************************/

void Conc_write(){

   int num;
   int a,b;

          if (t%printconcfreq == 0) {
			

               sprintf(filename3, "concent%d.txt", t/printconcfreq);

		  FILE * file3 = fopen(filename3, "w"); 
							
                        for (a = 1; a <=Nx; a++) {

                                  fprintf(file3, "%f \n", C[a]);
		        }
		         
              fclose(file3);
           }
 }

/******Front position Calculation**********/

void front(){

    double  MAX1=0;
    double  avefrontpos=0.;
    int i,j; 
         i=Nx;
         while(i>=1){
           if(i!=Nx && i!=1){
             if(T[i]>T[i-1] && T[i]>T[i+1]){
                  MAX1=T[i];
                  temp_pos=i;
                  break;
              }
           }
           else {                        
            if(T[Nx]>=MAX1){
              MAX1 =T[Nx];
            } 
            if(T[1]>MAX1){
              MAX1=T[1];
            }
           }
          i=i-1;
        }

         ave_pos=temp_pos*x_step;
         max_temp=MAX1;
             
        /* for (i=1; i<=Nx; i++) {
             if (T[i] > MAX1) {
                 MAX1= T[i];
                 temp_pos=i;
              } 
          }


         ave_pos=temp_pos;
         max_temp=MAX1;*/

              //Writing Front position into a File

              if (t%printfrontpos==0){

  	       FILE * frontfile = fopen(filename2, "a");
	 	  fprintf(frontfile, "%f, %f ,%f\n", ave_pos,max_temp, t*t_step);
                  fclose(frontfile);
              }//End of front pos print
}

/****************************************/

//void shuffle(int values[], int size) {

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
      
   // unsigned long s = 74647573837;                         
   // MTRand_int32  mt(s);
    
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

/********Deakkocation Function*********/

void Dealloc(){
int j;

        delete[] T;
        delete[] C;
        delete[] Grad;
        delete[] Grad_conc;
        delete[] tc;
        delete[] alpha;
}
//--------------------------------------/

#include"in_namelist.c"


                             /********************************END OF THE CODE ************************************/
