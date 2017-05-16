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

// void shuffle(int *,int);
 
/**********************************************///
 //Global Variable Definitions:                  //
                                                 //
 double Ctshold,Gamma,q_T,Lambda_1,beta,Diff;    //
 double tou_c,lambda;                            //
 double Q,cap_Gamma,TC,TL,T_bar,mass_Diff,qo;    //
 unsigned long  seed;                            //
 int    printfreq, printfrontpos,printconcfreq;  //
 int    t;                                       //
 int    run_number,t_n;                          //
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
  NameI (run_number),
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


   sprintf(filename2,"frontoxd_%d_%0.2f_%0.3f.txt",run_number,tou_c,Tg);
   FILE * frontfile = fopen(filename2, "w");
   fprintf(frontfile, "");
   fclose(frontfile);

        //** Simulation Step **//

            for(t=0;t<Nstep;t++){  

               
                grad();              //Laplacian term calculation

                /********* Reaction Rate options and temperature update *******/

                Calc_higgins();    //Oxidation model reaction rate

                /********* Boundary conditions **********/

                zero_flux_boundary();
                Oxy_zero_flux_boundary();

                //Dissipation();
                //periodic();
                //conc_boundary();   //implicable only if there is mass diffusion
          
                /********* printing out data **********/

                Temp_write();
                Conc_write();
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
    /****************************/
     i=1;                       //
       while(i<=Nx){            //
         C[i]=1.0;              //
         i=i+conc_step;}        //
                                //
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                                //     
     i=1;                       //
       while(i<=Nx){            //
         co2[i]=1.0;            //
         i=i+1;}                //
                                //
    /****************************/  

    sprintf(filename7, "Conc_dist.dat");
     FILE * concr = fopen(filename7, "w"); //if first process, create the file              

         for (i = 1; i <= Nx; i++) {
           fprintf(concr, "%d %f %f \n", i,C[i],co2[i]);
         }
    fclose(concr);

  }

  /*****************************************/

/********Grdaient Function*********/

void grad(){

 int i,j;
 double X_2;
 X_2=(x_step*x_step);
   
   /*for(i=1;i<=Nx;i++){ 
      Grad[i] = ((T[i+1]+T[i-1])-2.0*(T[i]))/(X_2);*/

   for(i=1;i<=Nx;i++){
     if(i==1){     
       //Forward
       Grad[i]= (T[i+2]-2*T[i+1]+T[i])/(X_2);
      }
      //Backward
     else if (i==Nx){ 
       Grad[i]= (T[i]-2*T[i-1]+T[i-2])/(X_2);
      } 
     else {
       //Central
       Grad[i]= (T[i+1]-2*T[i]+T[i-1])/(X_2);
     }
   }

   for(i=1;i<=Nx;i++){
     if(i==1){     
       //Forward
       Grad_co2[i]= (co2[i+2]-2*co2[i+1]+co2[i])/(X_2);
      }
      //Backward
     else if (i==Nx){ 
       Grad_co2[i]= (co2[i]-2*co2[i-1]+co2[i-2])/(X_2);
      } 
     else {
       //Central
       Grad_co2[i]= (co2[i+1]-2*co2[i]+co2[i-1])/(X_2);
     }
   }
 }

/************** END OF GRAD FUNCTION  ***************/


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
         //lambda is heat of reaction and lambda is stochimetric coeffient, co2[i] is the concenteration of oxygen
         T[i]=T[i]+(t_step*(Grad[i]+((1.0/tou_c)*(1.0/x_step)*(lambda)*co2[i]*(Gamma)*alpha[i]*C[i])));
       }

 //Updating oxygen concenteration
 
        for(i=1;i<=Nx;i++){    
         //lambda is heat of reaction and lambda is stochimetric coeffient, co2[i] is the concenteration of oxygen
         co2[i]=co2[i]+(t_step*(mass_Diff*Grad_co2[i]-((1.0/tou_c)*(1.0/x_step)*co2[i]*alpha[i]*C[i])));
       }
 }


/************** END OF RR CALC ***************/

/************** Boundary Function ***********/

void zero_flux_boundary(){
 int i,j;

    //temperature bc
    T[0]=T[2];
    T[Nx+1]=T[Nx-1];
 }

/***************************/

void Oxy_zero_flux_boundary(){
 int i,j;

    //temperature bc
    co2[0]=co2[2];
    co2[Nx+1]=co2[Nx-1];
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

       sprintf(filename5, "temp_field_1D_diffusive_regular_%d_%0.2f_%.3f_%d.txt",run_number,tou_c,Tg,t/printfreq);
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
	
               sprintf(filename3, "concenteration_1D_diffusive_regular_%d_%0.2f_%.3f_%d.txt",run_number,tou_c,Tg,t/printconcfreq);
		  FILE * file3 = fopen(filename3, "w"); 							
                        for (a = 1; a <=Nx; a++) {
                          fprintf(file3, "%f %f \n", C[a],co2[a]);
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

/********Deallocation Function*********/

void Dealloc(){
int j;

        delete[] T;
        delete[] C;
        delete[] co2;
        delete[] Grad;
        delete[] Grad_co2;
        delete[] tc;
        delete[] alpha;
}
//--------------------------------------/

#include"in_namelist.c"


                             /********************************END OF THE CODE ************************************/
