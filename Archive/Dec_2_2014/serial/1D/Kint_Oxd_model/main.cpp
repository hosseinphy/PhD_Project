/*  1D version of the code
    modified at SEP 24 , 7:00 pm
    in this code the non_dimensional 
    form of the eqautions has been 
    represented for sake of comparison
                  
    there is an option for 

  - heat dissipation  
  - Temperature Update
  - Concenteration Update
  - Temperature Boundary Condition
  - Concenteration Boundary Condition 
   
 Updated on Thu , Aug 21th , 2014   */    

/********************************************/

 //File Inclusion
 /************************/
 #include"initial.cpp"
 /************************/

 /**********************************************///
 //Global Variable Definitions:                  //
                                                 //
 double Ctshold,Gamma,q_T,Lambda_1,beta,Diff;    //
 double tou_c;                                   //
 double Q,cap_Gamma,TC,TL,T_bar,mass_Diff,qo;    //
 long   seed;                                    //
 int    printfreq, printfrontpos,printconcfreq;  //
 int    t;                                       //
 int    f_n,t_n;                                 //
 char   filename[15];                            //
 char   filename2[15];                           //
 char   filename3[15];                           //
 char   filename4[15];                           //
 char   filename5[15];                           //
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
   sprintf(filename2,"frontname%0.5f%0.6f_pr2.txt",t_step,To);
   FILE * frontfile = fopen(filename2, "w");
   fprintf(frontfile, "");
   fclose(frontfile);

        //** Simulation Step **//

            for(t=0;t<Nstep;t++){  

                grad();              //Laplacian term calculation

                /********* Reaction Rate options and temperature update *******/

                //Calc_higgins();    //Oxidation model reaction rate
                //Calc_prov_1();     //reaction rate  (T_bat)^(3/2)*exp(-1/T_bar)      
                //Calc_prov_2();     // Nik's -explosive crystalisation       
                Calc_prov_3();       // Nik's -explosive crystalisation       
                
                /********* update concenteration **********/
                // update_conc_1();  //reaction rate  (T_bat)^(3/2)*exp(-1/T_bar)
                // update_conc_2();  // Nik's -explosive crystalisation
                update_conc_3();     // Nik's -explosive crystalisation

                /********* Boundary conditions **********/

                zero_flux_boundary();
                //Dissipation();
                //periodic();
                //conc_boundary();   //implicable only if there is mass diffusion
          
                /********* printing out data **********/

                Temp_write();
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

double number ;
int i,j;

  for(i=1;i<=Nx;i++){  
     C[i]=1.0;}       
  /* srand48(seed);
       for(i=1;i<=Nx;i++){  
              number=drand48();
          if (number<=Ctshold){ 
            C[i]=1.0;}       
          else { 
            C[i]=0.0;       
          } 
      }*/
 }
/************** END ***************/

/********Grdaient Function*********/

void grad(){

 int i,j;

  for(i=1;i<=Nx;i++){ 
      Grad[i] = ((T[i+1]+T[i-1])-2*(T[i]))/((x_step*x_step));
  }

  //Gradient calculation for concenteration not necessary if there is no mass diffusion
  for(i=1;i<=Nx;i++){ 
       // Grad_conc[i] = ((C[ip]+C[im])-2*(C[i]))/(Sqr(x_step));
      Grad_conc[i] = 0.0;
  }
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
      T[i]=T[i]+t_step*((Diff*Grad[i])-(cap_Gamma*(T[i]-To))+(qo*q_T*C[i])-(qo*mass_Diff*Grad_conc[i]));
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
              alpha[i]=0;
           }
       }

 //Updating tempreture afterwards

        for(i=1;i<=Nx;i++){
     
                  T[i]=T[i]+(t_step*(Grad[i]+(1/tou_c)*alpha[i]*C[i]-Gamma*(T[i]-To)));
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
       sprintf(filename5, "array_kint%d%0.5f%0.6f.txt",t/printfreq,t_step,To);
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
                  
         for (i=1; i<=Nx; i++) {
             if (T[i] > MAX1) {
                 MAX1= T[i];
                 temp_pos=i;
              } 
          }

         ave_pos=temp_pos;
         max_temp=MAX1;
          
              //Writing Front position into a File

              if (t%printfrontpos==0){

  	       FILE * frontfile = fopen(filename2, "a");
	 	  fprintf(frontfile, "%f, %f ,%f\n", ave_pos,max_temp, t*t_step);
                  fclose(frontfile);
              }//End of front pos print
}
/****************************************/



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
