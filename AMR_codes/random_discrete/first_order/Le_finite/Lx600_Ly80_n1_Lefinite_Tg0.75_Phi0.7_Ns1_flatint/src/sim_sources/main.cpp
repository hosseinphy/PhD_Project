#include <iostream>
#include <iomanip>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include "grid.h"
#include "initial.h"

using namespace std;

unsigned seed;

int main()
{
    int i,ii,jj;
    int npt=1;
    int adapt=0,solver=0,t;
    int Np; 
    //seed for random number generator
	seed = time(NULL);

	cout << "Tig=" << Tig << endl;
	cout << "A=" << A << endl;

	grid *AdaptiveGrid;
    		cout << "create grid" << endl;
    	AdaptiveGrid = new grid(PBoundaries);
		cout << "initialize" << endl;
    	AdaptiveGrid->initializeGrid();
		cout << "dx" << endl;
    	AdaptiveGrid->setdx();
		cout << "Creating the Initial Solver Array" << endl;
    	AdaptiveGrid->createArray(npt);
		cout << "Finished Creating Initial Solver Array" << endl;
    		cout << "Initial Output" << endl;
    	AdaptiveGrid->output(0,npt);
    		cout << "Done Initial Output" << endl;


	if(flag_readFile==1)
	{
		int number_of_lines = 0;
    		std::string line;
		std::ifstream infile("../read_write_sources/Partconf_real_Lx_Ly_600_80_49800_Fract0.7_Ns1_Dim0.944.dat");
                if (!infile) std::cerr << "Could not open the file!" << std::endl; 
		else
		{
    			while (std::getline(infile, line))
			{
        			++number_of_lines;
			}	
			Np = number_of_lines -1;
		}
	}	
	std::cout<<Np<<endl;


    	//// Position and size of the particles: 
   	double *Parpx = new double[Np];
    	double *Parpy = new double[Np];
    	// particle radius
    	double *Parrad = new double[Np];
    	double *ovParrads = new double[Np];
    	double P_sepa;
    	int    nnmax;

   	if(flag_readFile==1)
   	{   
		int n;
    		std::ifstream infile;
		infile.open("../read_write_sources/Partconf_real_Lx_Ly_600_80_49800_Fract0.7_Ns1_Dim0.944.dat");
		/*if (!infile) std::cerr << "Could not open the file!" << std::endl;		
		else
		{ */   		
    		n = 0;
    		int t1,t2,t3,t4,t5 ;
        	infile >>t1>>t2>>t3>>t4>>t5 ;
			while (true) 
			{
    				infile >> Parpx[n] >> Parpy[n] >> Parrad[n];
				ovParrads[n] = (1./Pi) * (1. / Parrad[n] / Parrad[n]) ;
    				if( infile.eof() ) break;
	 		++n ;
			}
   		//}	
	}
   	else
   	{
		Np =npart ;
      		for(ii=0;ii<Np;ii++)
      		{        
        		// particle radius:
        		Parrad[ii] = Par_rad_min + (Par_rad_max-Par_rad_min)*(double)rand_r(&seed)/(double)RAND_MAX;
        		ovParrads[ii] = 1. / ( 2. * Parrad [ii] ) / ( 2. * Parrad[ii] );
        		// particle position:
       			 /*Parpx[ii] = 2.5 + 5.*(double)(npnx);
        		Parpy[ii] = 2.5 + 5.*(double)(npny);
        		npnx = npnx + 1 ;
        		if(Parpx[ii]>(double)size*nx-2.5){
            		npnx = 0;
           		 npny = npny + 1;
        		}*/        
          	
			Parpx[ii] = (double)(nx*size)*(double)rand_r(&seed)/(double)RAND_MAX;
          		Parpy[ii] = (double)(ny*size)*(double)rand_r(&seed)/(double)RAND_MAX;
        
          		// nnmax is a counter to avoid an infinite loop
          		for (jj=0;jj<ii;jj++)
          		{
              			P_sepa = Parrad[jj] + Parrad[ii] ;            
              			nnmax =0.;
              			if ( ((Parpx[jj]-Parpx[ii])*(Parpx[jj]-Parpx[ii]) + (Parpy[jj]-Parpy[ii])*(Parpy[jj]-Parpy[ii])) < P_sepa*P_sepa and nnmax<nmax_limit)
              			{
                  			ii = ii-1;
                 			nnmax=nnmax+1;
                  			break;
              			}
              			if (nnmax>=nmax_limit){
                 			 cout << endl << "Overlap between particles: run ended" << endl;
                  			return 0;
              			}
          		}
       		}
    }

   
    // output distribution and size of particles:
    char filename_par[BUFSIZ];
    FILE *fpar;
    sprintf(filename_par,"./par_list.dat");
    fpar = fopen(filename_par,"w");
    for(ii=0;ii<Np;ii++){
        fprintf(fpar,"%1.6f %1.6f %1.6f\n ", Parpx[ii], Parpy[ii], Parrad[ii]);
    }
    fclose(fpar); 


   AdaptiveGrid->calcWs(Parpx,Parpy,Parrad,ovParrads,Np); // calculate Ws in the initial mesh
    

    for(i=1;i<=endt;i++)
    {
  		if(i%AdaptFreq == 0 )
		{
		    adapt = clock();
			AdaptiveGrid->updateGrid(npt);
			AdaptiveGrid->setdx();
			AdaptiveGrid->createArray(npt);

			AdaptiveGrid->calcWs(Parpx,Parpy,Parrad,ovParrads,Np); // re-calculate Ws in the adapted mesh (to avoid averaging)

		    adapt = clock() - adapt;
		    t = CLOCKS_PER_SEC;

		    if(i%printFreq == 0)
		    {
			    cout << i<< "	"<< (double)(AdaptiveGrid->nodesize*AdaptFreq)/((double)solver/t) 
						<< "	" << (double)solver/t  << "	" << 
						 (double)adapt/t << "	" << AdaptiveGrid->nodesize << "	" << 
						AdaptiveGrid->ghostsize << endl;
			    adapt = 0;
			    solver = 0;
		    }
		}

		t = clock();

		AdaptiveGrid->updateGhosts(1,npt);
		AdaptiveGrid->updateBC(npt);
		//AdaptiveGrid->calcprePhi(npt);		

		AdaptiveGrid->updateGhosts(1,npt);
		AdaptiveGrid->updateBC(npt);
		AdaptiveGrid->calcdPdt(i,npt);

		AdaptiveGrid->updateGhosts(1,npt);
		AdaptiveGrid->updateBC(npt);
		AdaptiveGrid->calcdCdt(i,npt);

		AdaptiveGrid->step(npt);
				
 		solver += clock() - t;
	
		if(i%printFreq == 0)
		{
		    AdaptiveGrid->output(i,npt);
		    cout << "Time:   " << i << endl;
		}		
	}
    
    return 0;
}	

