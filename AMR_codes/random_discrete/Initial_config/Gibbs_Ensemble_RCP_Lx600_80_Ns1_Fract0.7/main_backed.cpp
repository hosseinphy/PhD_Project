//======================================================================
//======================================================================
//
//  Monte carlo simulation of equlibrium canonical esemble of hard disks
//
//======================================================================
//======================================================================

#include "mdef.h"
#include "read_input.h"
#include <RandomLib/Random.hpp>
#include <RandomLib/NormalDistribution.hpp>
#include <RandomLib/RandomSelect.hpp>
#include <omp.h>
//using namespace RandomLib; 
using RandomLib::Random;
  /*------- global params --------*/
 
  //unsigned seed;					// in case rand_r is used
  //seed = time(NULL);
  //Random r;        					// in case of RandomLib : create random number object or....
  //Random r(Random::SeedVector()) ;			// <-----
  std::string seedstr;
  bool seedgiven = false;
  std::vector<unsigned long> master_seed =
  seedgiven ? Random::StringToVector(seedstr) : Random::SeedVector();
  read_input input;					// creat an object for read_input class
  const double Pi  = (2.*acos(0.));
  int 	flag_Negh = 0 ; 
  //arrays and variables
  int		*Hist ;					//array contians histogram  
  //int 	Maxbin;  		            	// Maximum bin-wdith around a particle
  VecI 		Maxbin;  		            	// Maximum bin-wdith around a particle
  int 		M_num ;					// Number of articles in one dimension
  int 		Nconfig, Iratio, Iprint,Isave ; 
  int 	 	N; 		 			//Number of particles
  int 		nrealize ;
  double 	Delr , Dim, Fract, Drmax ;		//\delta r , shell-width, dimater , surface fraction & max displacement   
  double 	*Grat, *Gra ;     		 	// Position of particles  (Rx , Ry) and g2(r) related arrays
  double 	Acm, Acmmva;
  int 		MAX_Maxbin ; 

  typedef struct {
  VecR r;
  } particle;

  particle        *part;
  VecR            region ;
  VecI            initUcell ;
  real            timeNow ;
  real            deltaT, Density, rCut ;
  int             moreCycles, Np, randSeed, stepAvg, stepCount, stepLimit;
  VecI            cells;
  int             *cellList;
  real            dispHi, rNebrShell;
  int             *nebrTab, nebrNow, nebrTabFac, nebrTabLen, nebrTabMax;
  int 		  kk, ll ;	
  // for plott the particles (.py code)
  signed int 	xlow;
  signed int 	xhigh;
  signed int 	ylow;
  signed int 	yhigh;
  int 		scale;
  /*Functions*/
  bool 		hard_wall_test (double,double) ;// hard wall test
  bool 		Overlap_test (double,double,int,int) ;	// Overlap test
  void 		Radial (double,int,int) ;		// calculate and print g2(r)	
  void 		Allocate(int , int) ;			// allocate arrays based on number of particle (M_num*M_num) and Maxbin
  void 		Dealloc() ;				// deallocate the arrays -> free memory
  void 		Initiate() ; 				// Initiate arrays
  void 		ApplyPeriodicBond(VecR) ;		// implement periodic boundary condition
  double        RandomDouble() ; 			// return random number
  void		SetParams() ;
  void		SetupJob() ;
  void		SingleStep() ;
  void		InitCoords() ;
  void 		PrintToFile(FILE *, int) ; 
  void          PrntSummary (FILE *,int,double,double);
  void 		ApplyBoundaryCond () ;
  void 		BuildNebrList () ;
  /*file_name allocation*/	
  char   	filename1[BUFSIZ];                         
  char   	filename2[BUFSIZ];                       
  char   	filename3[BUFSIZ];                       
  char   	filename4[BUFSIZ];                       




/********* MAIN LOOP *********/
int main(int argc, char* argv[])
{

  clock_t begin = clock();	 // start ticking 
  int kk = 100;                  // Number of tasks
  int ll = 4;                    // The leapfrogging stride
  input.read(argc, argv) ;
  SetParams() ; 
  SetupJob () ; 
  moreCycles = 1;
  std::cout<<"START OF MARKOV CHAIN"<<"\n\n"<<std::endl;
  std::cout<<"ACM:"<<"\t"<<"Ratio:"<<"\t"<<"Drmax:"<<"\n\n"<<std::endl;
  ApplyBoundaryCond () ;
  if(flag_Negh)	
  {
    BuildNebrList () ;
  }   
  while (moreCycles) {
     SingleStep ();
     if (stepCount >= nrealize ) moreCycles = 0;
  }

 //write RDF
  sprintf(filename1, "RDF_Lx%d_Ly%d_tot_real%d_Fract%.1f_Ns%d_Dim%0.3f.dat",input.boxX, input.boxY, Nconfig,Fract,(int)Density,Dim);
  FILE *fp = fopen ( filename1, "w" );
  PrintToFile(fp,1) ;

//write final shifted config
  /*sprintf(filename3, "Partconf_Final_%d_%.1f_%d.dat",stepCount,Fract,(int)Density);
  FILE *fp3 = fopen ( filename3, "w" );
  PrintToFile(fp3,3) ;*/
  std::cout<<"Simulation is done!!"<<std::endl;

  clock_t end =clock();	

  double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;


  Dealloc() ;
  return 0;
}//.................................................... 
// ** ENDS THE MAIN FUNCTION



 


//function definition :
/***********************************************************************/
void SingleStep()
{
 	 //local varaibles 
  	int 	ii,jj ; 
  	bool  	Overlap; 									//true if particle i overlaps
  	double 	temp_double;
  	VecR 	Rold,Rnew ;
  	double 	Ratio ;				 	 					//Acceptance ratio
  	double 	random ;
 	 ++ stepCount;

	#if HAVE_OPENMP
	#pragma omp parallel for
	#endif
	//LOOP OVER PARTICLES
	 for (ii = 0 ; ii< kk; ii++)
	 {
		int jj = (N * (ii + 1))/kk - (N * ii)/kk ;		
    		Random r;                   // task specific Random
    		{
      			std::vector<unsigned long> seed(master_seed); // task specific seed
      			seed.back() = ii / ll;      // include task id in seed
      			r.Reseed(seed);
      			// Turn on leapfrogging with an offset that depends on the task id
      			r.SetStride(ll, ii % ll);
    		}
		VCopy(Rold, part[jj].r) ; 
		//MOVE P ARTICLE I AND PICKUP THE CENTRAL IMAGE
       	  	Rnew.x = Rold.x + (double)((2.0 * r.FixedS<double>() )) * Drmax ;	  		// move particles in the interval [-Drmax , Drmax]
	  	Rnew.y = Rold.y + (double)((2.0 * r.FixedS<double>() )) * Drmax ;
		//Apply periodic boundary condition
		VWrapAll(Rnew) ;
		//CHECK FOR ACCEPTANCE (NO OVERLAP)
	 	if(!Overlap_test(Rnew.x,Rnew.y,jj,N))
         	{
			VCopy(part[jj].r, Rnew) ;			
		  	Acmmva = Acmmva + 1. ;							//Accepted moves in one step
 		}	
 		Acm = Acm + 1. ;								// Total number of moves (including those result in overlap
      	} //.............................................
	// ** LOOP OVER P ARTICLES COMPLETE

	//CHANGE MAXIMUM DISPLACEMENT
	if ( (stepCount % Iratio) == 0 ) 	
   	{
     	  	Ratio = Acmmva / ((double)N) /((double)Iratio)  ;
     	  		if ( Ratio > 0.5 ) 							//Ration should be close or equal to 0.5 if we chose Drmax appropriately !
     	  		{
      	   			Drmax = Drmax * 1.05 ;
     	  		}
     	  		else 
     	  		{
      	   			Drmax = Drmax * 0.95 ;
     	  		}
     	 	Acmmva = 0.0 ;									//reset the number of accepted move to zero
   	}

	// WRITE OUT RUNTIME INFORMATION & PARTICLES CONFIG
 	
	if ( (stepCount % Iprint) == 0 ) 
 	{
 	 	PrntSummary(stdout, Acm/N,Ratio,Drmax) ;	     	
	 	sprintf(filename2, "Partconf_real_Lx_Ly_%d_%d_%d_Fract%.1f_Ns%d_Dim%0.3f.dat",input.boxX, input.boxY, stepCount, Fract, (int)Density,Dim);
   		FILE *fp = fopen ( filename2, "w" );
  		PrintToFile(fp,2) ;
       	}

	//calculating radial dist. func.
	if ( (stepCount % Isave) == 0 )
	{
	 	if (Drmax > 0.5 * (Delr * Dim )) nebrNow = 1;					// upon this condition -> BuildNeighborList() 
  		if (nebrNow and flag_Negh==1) {
			nebrNow = 0;
			BuildNebrList ();
		}

   	  	Radial (Fract ,N, stepCount) ;								// We can use the function instead
	}  	

 }// END OF SINGLE STEP FUNCTION

//...................................................

void SetParams()
{
 double Side ;
 Delr	  = input.deltar ;			   		//bin width (in units of particle diamater) for g2(r) radial dist func.
 Fract 	  = input.fraction ;
 Density  = input.density ;
 Drmax 	  = input.deltar_max ;
 //M_num 	  = input.p_num ; 					//Sqrt (N)
 Nconfig  = input.confignum;					//number of configurations to calculate g2(r)
 Isave 	  = input.save_freq  ; 		  			//saving frequncy for calculation of radial. dist. func. g2(r)
 Iratio   = input.ratio_freq ;  				//update interval for max displacement of particles (Drmax) -typical value 1
 Iprint   = input.print_freq ;  				//printing frequency of datas
 //N 	  = M_num * M_num ; 					//Number of particles
 nrealize = Nconfig * Isave ;
 initUcell.x = input.boxX ;						// For square array config, each unitCell contains one particle 
 initUcell.y = input.boxY ;
 

 //COMPUTE DISK DIAMETER FROM DISK AREA FRACTION
 //Dim = sqrt(4.* Fract/Pi/((double)N)) ;
 Dim = sqrt(4.* Fract/Pi/Density) ;					// when region is equl to 1.0, density is equal to N as we used to define
 Drmax = Drmax * Dim ;							// maximum displacement in units of particle diameter 
 Side = 1./Dim ;						

 region.x =(double)(input.boxX) ;
 region.y =(double)(input.boxY) ;

 N = Density * region.x * region.y ;

 ////VSCopy (region, 1. / sqrt(Density), initUcell);
 //Np = VProd (initUcell);
 rCut = 16.  ;//1. - .5 * Delr ;					//max rCut is a radius of a circle centered about particle above which there is no interaction 
 ////VSCopy (cells, 1. / (rCut + Delr), region );
 cells.x = 1./(rCut + Delr) * region.x * Side ;
 cells.y = 1./(rCut + Delr) * region.y * Side ;

 // INTERPARTICLE SEPARATIONS MUST BE i 1/2 BOX LENGTH

 Maxbin.x = (int)(((region.x * Side / 2.) - 1.) / Delr) ;		// Maxbin (n-1) = (Rij - (rCut(t=0) + "\Delta r" /2)) / Delta r									        								    // where we assume the rCut(t=0) is equal to Dim - "\Delta r" /2
 Maxbin.y = (int)(((region.y * Side / 2.) - 1.) / Delr) ;								
 MAX_Maxbin =std::max(Maxbin.x,Maxbin.y) ;
 nebrTabMax = 10000 * N;
 if (region.x >= (4. * rCut * Dim))   flag_Negh =1 ;			// condition for using neghbour list
//nebrTabMax = nebrTabFac * N;

  xlow  = -(int)((region.x/2) + 1);
  xhigh =  (int)((region.x/2) + 1);
  ylow  = -(int)((region.y/2) + 1);
  yhigh =  (int)((region.y/2) +	1);
  scale = 1;

 std::cout <<"Dim			:"<<(double)Dim<<" \n "
	  <<"Delr			:"<<(double)Delr<<" \n "
	  <<"N				:"<<N<<" \n "
	  <<"Maxbin_x			:"<<(int)Maxbin.x<<" \n "
	  <<"Maxbin_y			:"<<(int)Maxbin.y<<" \n "
	  <<"Max_Maxbin			:"<<(int)MAX_Maxbin<<" \n "
	  <<"Drmax			:"<<Drmax<<" \n "
	  <<"Isave			:"<<Isave<<" \n "
	  <<"Iprint			:"<<Iprint<<" \n "
	  <<"Iratio			:"<<Iratio<<" \n "
	  <<"Cells.x			:"<<cells.x<<" \n "
	  <<"Cells.y			:"<<cells.y<<" \n "
	  <<"Region.x			:"<<region.x<<" \n "
	  <<"Region.y			:"<<region.y<<" \n "<<std::endl;
}

//...................................................
void SetupJob ()
{
 Allocate(MAX_Maxbin , N) ;
 //InitRand (randSeed);
 //seed = time(NULL);
 stepCount = 0;
 Initiate() ;
 InitCoords ();
 nebrNow = 1;
 Acm = 0. ;												//# of moves in one step 
 Acmmva = 0. ;												//# of accepted moves in one step
}


//...................................................
void InitCoords()
{
//Creating initial configuration ....

//Equlibrium

std::cout<<"Setting up initial square array configuration ....	"<<" \n "<<std::endl ;

  VecR c, gap , pp;
  int nx, ny, nz,ii;

  if(region.x==region.y)
  {  
   int n ;
   pp.x = sqrt(N) ;
   pp.y = sqrt(N) ;
   VDiv (gap, region, pp);
   n = 0;
     for (ny = 0; ny < pp.y; ny ++) {
       for (nx = 0; nx < pp.x; nx ++) {
         VSet (c, nx + 0.5, ny + 0.5);
         VMul (c, c, gap);
         VVSAdd (c, -0.5, region);
         part[n].r = c;
         ++ n;
       }
     }
  }
  else
  {
   int m ;
   gap.x = 1./sqrt(Density) ;
   gap.y = 1./sqrt(Density) ;
   pp.x = region.x / gap.x ;
   pp.y = region.y / gap.y ;
   m = 0;
     for (ny = 0; ny < pp.y; ny ++) {
       for (nx = 0; nx < pp.x; nx ++) {
         part[m].r.x = (nx + 0.5) * gap.x ;
         part[m].r.y = (ny + 0.5 ) * gap.y ;
         ++ m;
       }
     }
     std::cout<<"number of particle	:"<<m<<std::endl;
     for (ii=0; ii<N ;ii++) {
         part[ii].r.x = part[ii].r.x - region.x /2. ;
         part[ii].r.y = part[ii].r.y - region.y /2. ;
     }
  }

 //WRITE OUT PARTICLES INITIAL CONFIG
  std::cout<<"write out the inital configuration ..."<<"\n"<<std::endl ; 
      sprintf(filename3, "aaa_config.dat");
      FILE * initw = fopen(filename3, "w");
      fprintf(initw, "%d %d %d %d %d \n", xlow,xhigh,ylow,yhigh,scale);
       for (ii = 0; ii<N; ii++) {
         fprintf(initw, "%f %f %f \n", part[ii].r.x, part[ii].r.y, Dim/2.);
       }
    fclose(initw);          
}

//...................................................

bool Overlap_test ( double NEW1,double NEW2, int I, int N ) 
{
  int 	 J;
  VecR 	 Rij ;
  double R2 ;
  double Diamsq ;
  
   Diamsq =  Dim * Dim  ;

	//LOOPS OVER MOLECULES EXCEPT I
  	for (J=0; J<N ;J++)
  	{
		if(J!=I)
     		{
       			Rij.x = NEW1 - part[J].r.x ;
       			Rij.y = NEW2 - part[J].r.y ;
			VWrapAll(Rij) ; 
     			R2 = (Rij.x * Rij.x) + (Rij.y * Rij.y) ;
       			if( R2 < Diamsq )
      			{	 		    	 
          			return true ;
       			}
		}	
  	}

 return false ; 
}

//..........................................

void Radial (double Fract, int N , int freq)
{
 int 	Ibin ;
 int 	I, J, n, jj,ii ;
 VecR   Rij ; 
 double R2  ;
 double Rijsq ,Side ,temp_double ;
 double Const ;
 double Rlower, Rupper, Cideal ;
 int 	MIN , MAX ;
  	Side = 1./Dim ;
 	MAX  = std::max(Maxbin.x ,Maxbin.y) ;
 	MIN  = std::min(Maxbin.x ,Maxbin.y) ;

  	Hist[0]  = 0;
  	Gra[0]  = 0.0;
  	Grat[0]  = 0.0;

  		for(Ibin = 1 ; Ibin < (MAX + 1) ; Ibin++)
  			Hist[Ibin] = 0 ;
  		
		if(flag_Negh==1)
		{
  			// BUILD HISTOGRAM 		
  			for (n = 0; n < nebrTabLen; n ++) {
    				I = nebrTab[2 * n];
    				J = nebrTab[2 * n + 1];
      				Rij.x = part[I].r.x - part[J].r.x ;
      				Rij.y = part[I].r.y - part[J].r.y ;	
				VWrapAll(Rij) ; 
      				Rijsq = (Rij.x * Rij.x) + (Rij.y * Rij.y) ;
      				Rijsq = sqrt(Rijsq) ;
      				Rijsq = Rijsq * Side; 
 				Ibin = (int)((Rijsq - 1.)/Delr) + 1 ;
      				if(Ibin <= Maxbin.x and Ibin <= Maxbin.y)
      				{
       	 				Hist[Ibin] = Hist[Ibin] + 2 ;
      				}
			}
		}
		else
		{
  			//** BUILD HISTOGRAM 
   			for (I=0; I<N-1 ;I++)									//Exluding self-correlation and prevent repeating 
   			{
     				for (J=I+1 ; J<N ;J++)
      				{
      					Rij.x = part[I].r.x - part[J].r.x ;
      					Rij.y = part[I].r.y - part[J].r.y ;	
					VWrapAll(Rij) ; 
      					Rijsq = (Rij.x * Rij.x) + (Rij.y * Rij.y) ;
      					Rijsq = sqrt(Rijsq) ;
      					Rijsq = Rijsq * Side; 
 			     		Ibin = (int)((Rijsq - 1.)/Delr) + 1 ;
      						if(Ibin <= Maxbin.x and Ibin <= Maxbin.y)
      						{
       	 						Hist[Ibin] = Hist[Ibin] + 2 ;
      						}
      				}	
    			}
		}

   		Const = 4. * Fract ;
   		for (Ibin = 1; Ibin < (MAX + 1) ; Ibin++)
   		{
    			Rlower = ((double)(Ibin - 1)) * Delr + 1. ;
    			Rupper = Rlower + Delr ;
    			Cideal = Const * ( (Rupper * Rupper) - (Rlower * Rlower) ) ;
    			Gra[Ibin] = (double)(Hist[Ibin])/ (((double)(N)) * Cideal) ;
    			Grat[Ibin] = Grat[Ibin] + Gra[Ibin]  ;
   		}

		if(freq%25000==0){
 			sprintf(filename4, "aaa-%d.dat", freq);
        		FILE * RWW = fopen(filename4, "w");
        	   		for (Ibin = 0; Ibin < MAX + 1; Ibin++) {
             				fprintf(RWW, "%f \n", Grat[Ibin]);
           			}
 			fclose(RWW);
		}

}


//.......................
void Allocate(int Bin_max , int Np)
{

 	Grat   	= new double [Bin_max];
 	Gra    	= new double [Bin_max];
 	Hist 	= new int    [Bin_max] ; 	// Arry contains bins 
  	AllocMem (part, Np, particle);
  	AllocMem (cellList, VProd (cells) + Np, int);
  	AllocMem (nebrTab, 2 * nebrTabMax, int);
}
//.......................

void Initiate() 
{
  int ii ;

 	for (ii=0 ; ii< MAX_Maxbin ; ii++)
 	{
   		Hist[ii] = 0   ;
   		Gra[ii]  = 0.0 ;
   		Grat[ii] = 0.0 ;
 	}
}

//.......................

void Dealloc() 
{
	delete [] Gra ;
	delete [] Grat ;
	delete [] Hist ;
}


//.......................
/*double RandomDouble()
{
  double rdm ;
	//r.Reseed(); 		                // seed with a "unique" seed
	//r.Reseed(Random::SeedWord()); 
	r.Reseed(Random::SeedVector());
return rdm = r.FixedS<double>() ;

}
*/
//........................................

int read_input::read(int argc ,char* argv[])
{

int error =0 ;
  if (argc != 2) 
    {
    std::cout << "Syntax: spheres input" << std::endl;
    error = 1;
    } 
  else 
    {
    std::ifstream infile;
    infile.open(argv[1]);
    if(!infile)
      {
	std::cout << "Can't open " << argv[1] << " for input." << std::endl;
	error = 2;
	return error;
      } 
    else 
      {
	std::cout << "Reading input from file " << argv[1] << std::endl;
      }
    char buf[100],c;
    infile.get(buf,100,'='); infile.get(c); infile >> input.confignum;
    infile.get(buf,100,'='); infile.get(c); infile >> input.save_freq;
    infile.get(buf,100,'='); infile.get(c); infile >> input.ratio_freq;
    infile.get(buf,100,'='); infile.get(c); infile >> input.print_freq;
    infile.get(buf,100,'='); infile.get(c); infile >> input.boxX;
    infile.get(buf,100,'='); infile.get(c); infile >> input.boxY;
    infile.get(buf,100,'='); infile.get(c); infile >> input.fraction;
    infile.get(buf,100,'='); infile.get(c); infile >> input.density;
    infile.get(buf,100,'='); infile.get(c); infile >> input.deltar;
    infile.get(buf,100,'='); infile.get(c); infile >> input.deltar_max ;
    infile.width(NAME_LEN-1); infile >> input.readfile;
    infile.get(buf,100,'='); infile.get(c); 
    infile.width(NAME_LEN-1); infile >> input.writefile;
    infile.get(buf,100,'='); infile.get(c); 
    infile.width(NAME_LEN-1); infile >> input.configuration;

    if(infile.eof()) 
      {
	std::cout << "Error reading input file " << argv[1] << std::endl;
	error = 3;
      }
    }
return error ;

}

//........................................

void ApplyPeriodicBond (VecR R)
{
  VWrapAll (R);
}
//........................................

void ApplyBoundaryCond ()
{
  int n;

  DO_MOL VWrapAll (part[n].r);
}

//........................................

bool hard_wall_test(double x , double y)
{
	if (x < (-.5 + Dim/2.) or ( x > (0.5 - Dim/2.) ) )  
	{
		//std::cout<<(double)x<<"	\t"<<"Excedd the boundary"<<" \n "<<std::endl;
		return true ; 
	}
	if (y < (-.5 + Dim/2.) or ( y > (0.5 - Dim/2.) ) )  
	{	
		return true ;		
	}
return false ;
}

//...................................................
void PrintToFile(FILE *fp, int prtFlg)
{
  int ii ;
 int MAX ;
	MAX =std::max(Maxbin.x , Maxbin.y) ;
	if(prtFlg==1)
	{
	  std::cout<<"write the RDF for 	"<<
		Nconfig<<"\t"<<"configs"<<" \n "<<std::endl;
  		for (int ii = 1; ii < (MAX + 1); ii++) 
  		{
       			fprintf(fp, "%f %f \n", 
			(1. + (ii -.5) * Delr), Grat[ii]/(double)Nconfig);
       		}
 	fclose(fp);
	}
	else if(prtFlg==2)
	{

		xlow  = -(int)((region.x/2) + 1) + region.x / 2. ;
  		xhigh =  (int)((region.x/2) + 1) + region.x / 2. ;
  		ylow  = -(int)((region.y/2) + 1) + region.y / 2. ;
  		yhigh =  (int)((region.y/2) + 1) + region.y / 2. ;

     	 	fprintf(fp, "%d %d %d %d %d \n", xlow,xhigh,ylow,yhigh,scale);
      	  		for (ii = 0; ii<N; ii++) {
           			fprintf(fp, "%f %f %f \n", part[ii].r.x + region.x / 2., part[ii].r.y + region.y / 2., Dim/2.);
      	  		}
     	 	/*fprintf(fp, "%d %d %d %d %d \n", xlow,xhigh,ylow,yhigh,scale);
      	  		for (ii = 0; ii<N; ii++) {
           			fprintf(fp, "%f %f %f \n", part[ii].r.x, part[ii].r.y, Dim/2.);
      	  		}*/
   		fclose(fp);
	}
	else if (prtFlg==3)
	{

		xlow  = -(int)((region.x/2) + 1) + region.x / 2. ;
  		xhigh =  (int)((region.x/2) + 1) + region.x / 2. ;
  		ylow  = -(int)((region.y/2) + 1) + region.y / 2. ;
  		yhigh =  (int)((region.y/2) + 1) + region.y / 2. ;

     	 	fprintf(fp, "%d %d %d %d %d \n", xlow,xhigh,ylow,yhigh,scale);
      	  		for (ii = 0; ii<N; ii++) {
           			fprintf(fp, "%f %f %f \n", part[ii].r.x + region.x / 2., part[ii].r.y + region.y / 2., Dim/2.);
      	  		}
   		fclose(fp);
	}
	else
	{
	std::cout<<"something is wrong !"<<std::endl;
	}
}

//...................................................
void PrntSummary (FILE *fp, int a, double b , double c)
{
   fprintf(fp,"%5d %7.4f %7.4f \n", a,b,c) ;
   fflush (fp);
}
//...................................................
void BuildNebrList ()
{
  VecR dr, invWid, rs, shift;
  VecI cc, m1v, m2v, vOff[] = OFFSET_VALS;
  real rrNebr;
  int c, j1, j2, m1, m1x, m1y, m1z, m2, n, offset;
 
  rrNebr = Sqr (rCut + Delr);
  VDiv (invWid, cells, region);
//	std::cout<<cells.x<<"	\t	"<<cells.y<<std::endl;
  for (n = N; n < N + VProd (cells); n ++) cellList[n] = -1;
  DO_MOL {
    VSAdd (rs, part[n].r, 0.5, region);
    VMul (cc, rs, invWid);
    c = VLinear (cc, cells) + N;
    cellList[n] = cellList[c];
    cellList[c] = n;
  }
  nebrTabLen = 0;
    for (m1y = 0; m1y < cells.y; m1y ++) {
      for (m1x = 0; m1x < cells.x; m1x ++) {
        VSet (m1v, m1x, m1y);
        m1 = VLinear (m1v, cells) + N;
        for (offset = 0; offset < N_OFFSET; offset ++) {
          VAdd (m2v, m1v, vOff[offset]);
          VZero (shift);
          VCellWrapAll ();
          m2 = VLinear (m2v, cells) + N;
          DO_CELL (j1, m1) {
            DO_CELL (j2, m2) {
              if (m1 != m2 || j2 < j1) {
                VSub (dr, part[j1].r, part[j2].r);
                VVSub (dr, shift);
                if (VLenSq (dr) < rrNebr) {
                  	if (nebrTabLen >= nebrTabMax){
		     	std::cout<<"a problem is occuired!"<<std::endl ;
		     	//return ;
			exit(-1) ;
			}
                     //ErrExit (ERR_TOO_MANY_NEBRS);
                  nebrTab[2 * nebrTabLen] = j1;
                  nebrTab[2 * nebrTabLen + 1] = j2;
        	  //printf("%5d %5d %5d %5d %5d %5d\n" , m1,m2,j1,j2,nebrTab[2 * nebrTabLen],nebrTab[2 * nebrTabLen +1]) ;
                ++ nebrTabLen;
               }
             }
           }
         }
       }
     }
   }
 }





