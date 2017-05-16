//File Inclusion

#include"initial.cpp"

/*****************************/

 using namespace std;

/*****************************/

 //pointer to the files


 ofstream OUTPUT("position.txt");
 ofstream beriz("PPP.txt");
 ofstream OUTPUT_2("velocity.txt");
 ofstream OUTPUT_3("velvst.txt");
 ofstream myWriteFile;
 ofstream myWriteFile2;

/****************/

//  Main loop  //               

/****************/                             

int main() {                
                            
system("clear");             
Setup();                    
Vel_calc();
dealloc();
return 0;

}

/*****************/

//Function Definition

/***********Velocity Function***********/

void Vel_calc(){

 int i,j,k;

 double V_sum,Vel_sum,Vel_sum2,Vel_diff;
 int    t_trans=100;

    for(i=0;i<filenumber;i++){

      for(k=t_trans;k<front_data_size;k++){

            if(k==0){
               velocity[i][k-0]=0;
             }
          
            else{    
             velocity[i][k-t_trans]=((pos[i][k+1]-pos[i][k-1])/(2*(Time[i][k+1]-Time[i][k])));
             acc[i][k-t_trans]=((pos[i][k+1]+pos[i][k-1]-2*pos[i][k])/((Time[i][k+1]-Time[i][k])*(Time[i][k+1]-Time[i][k])));
           }
      }
  }  
             
   for(i=0;i<filenumber;i++){

      Vel_sum=0;
      int jj=0;

      for(k=t_trans;k<front_data_size;k++){

       // if(velocity[i][k-t_trans]>0 && acc[i][k-t_trans]>=0){
        
          Vel_sum += velocity[i][k-t_trans];

          jj+=1;
      
      //  }
     }

    Vel_mean[i]=Vel_sum/jj;
 }


  for(i=0;i<filenumber;i++){


    Vel_sum2=0;
    int ll=0;

     for(k=t_trans;k<front_data_size;k++){
 
      // if(velocity[i][k-t_trans]>0 && acc[i][k-t_trans]>=0){
       

        Vel_diff=Sqr(velocity[i][k-t_trans]-Vel_mean[i]);
        Vel_sum2+=Vel_diff;

        ll+=1;
     //  }
    }


   Vel_var[i]=Vel_sum2/ll;
   Vel_div[i]=sqrt(Vel_var[i]);

 }


    string st2 = "velocity";
    string ext2 = ".txt";
    string filename2;


    for(i=0;i<filenumber;i++){
   

      V_sum=0;
      int kk=0;

      stringstream pp;
      pp<<i;
      filename2 = st2 + pp.str() +ext2;

      myWriteFile.open(filename2.c_str());

     
      for(k=t_trans;k<front_data_size;k++){
    
       // if(velocity[i][k-t_trans]>0 && acc[i][k-t_trans]>=0){


          if(velocity[i][k-t_trans]<(Vel_mean[i]+Vel_div[i]) || velocity[i][k-t_trans]>=(Vel_mean[i]-Vel_div[i])){
            
             V_sum+=velocity[i][k-t_trans];
             kk=kk+1;
          }
      // }

          if (myWriteFile.is_open()) {
               
             myWriteFile<< "\t "<<velocity[i][k-t_trans]<<"\t"<<acc[i][k-t_trans]<<"\t"<<Time[i][k-t_trans]<<endl;

         }
    }



   

     V_ave[i]=V_sum/kk;


     cout<<V_ave[i]<<" \t "<<Vel_mean[i]<<"\t"<<Vel_div[i]<<endl;
     cout<<"\n"<<endl;

     myWriteFile.close();

 }		



 for(i=0;i<filenumber;i++){

  OUTPUT_3<<i<<"\t"<<V_ave[i]<<"\t"<<Tign[i]<<"\t"<<Tbck[i]<<endl;

  }


/******************************/
/*
int L=10; // Average Interval
int j=0,k;


//Smoothing Procedure
for(i=0;i<filenumber)
for(int p=0; p<front_data_size;p++){
    
    V[p]=0;//Velocity*time
    T[p]=0;//Time_sum
  }

    while(j<SIZE){
    
       if(j>=(L) & j<=(SIZE-L)){

          for(k=j-L;k<j+L;k++){

             V[j]+=velocity[k];

          }

       }

    else if (j>(SIZE-L)){

        for (k=j-L;k<SIZE;k++){
            
             V[j]+=velocity[k];
            
            }
     }
        
      else {

         for(k=0;k<j+L;k++){

           V[j]+=velocity[k];
      
         }
     }
       


         OUTPUT_2<<" \t "<<V[j]/L<< "\t" <<time1[j]<<endl;

         j++;


   }*/

}


//Deallocating Arrays
/****************************************/

void dealloc(){

 int i;

     for (i=0; i <filenumber; i++ ) {

              delete[] pos[i];
              delete[] Time[i];
              delete[] velocity[i];
              delete[] acc[i];
      }

              delete[] pos;
              delete[] Time;
              delete[] velocity;
              delete[] acc;
              delete[] V_ave;
              delete[] Tign;
 }

/*******************************************/



