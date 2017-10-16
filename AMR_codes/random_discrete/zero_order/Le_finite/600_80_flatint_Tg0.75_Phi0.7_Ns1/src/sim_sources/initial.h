#ifndef INITDEF
#define INITDEF

#include <cmath>

/*******OpenMP variables********/
const int numprocs = 16;		//maximum number of threads

const double 		Pi = (2.*acos(0.));
extern unsigned int 	seed;

//variables  for initial condition for interface
const double 		yo = 10.;				//initial offset
const double 		Amp= 0;				//Amplitude of morphological perturbation in the interface
const double 		pad_Amp = 1.;				//Amplitude of initial adaption, pad_Amp > Amp 
const int 		flag=1; 				// 0: noisy morphological perturbation, 1: cos(2pi/lambda) morphological perturbation
const int 		flag_readFile=1;			// to read the initial cooordinates from the file
const double 		lambda = 40.;
const double 		dt = 0.00001;				//time step, follows stability criterion dt<dx^2/4D
const double 		Le = 2.25;
const double 		RR = 0.61 ;
const double 		Tig = ( 1. - exp(-1.*RR) ) / RR ; 	// with RR=0.61, Tig=0.74860513266475 ~ 0.75;

//const double A = aux_Tig*(1.+aux_Tig*ovLe) ;
const double 		A = 1./RR ;
const double 		aux_Tig = Tig/(1.-Tig);
const double 		ovLe = 1./Le;

//domain and size parameters
const int 		resolution = 9 ;			//min dx = size / 2^(resolution-1)
const double		size = 10 ;				//Size of one domain in units of W
const int 		nx = 8;					//Number of domains along the x direction
const int 		ny = 60 ;				//Number of domains along the y direction


// particles: quantity, radius and separation (between particle centers) 
//int     		Np = 32000; //(nx * size ) * ( ny * size ) ;
const int     		npart = 32000; //(nx * size ) * ( ny * size ) ;
const double  		Par_rad_min = 0.25 ;			//size / (2 * 2 * 2 * 2 * 2 * 2 * 2 * 2);  //particles' radius ranged from smallest dx to 0.5
const double  		Par_rad_max = 0.25 ;			//size / (2 * 2 * 2 * 2 * 2 * 2 * 2 * 2); 
const int 		nmax_limit = 10000000; 			// max intents to avoid particle overlap

const int 		PBoundaries = 0;			// if set to 1 Periodic boundaries, non-periodic otherwise.

//runtime and output
const int 		printFreq = 16000 ;			//printing frequency
const int 		endt = 12000000 ;			//Total number of iterations

const double 		Beta = 1.;				// parameter to include concentration gradients in adaption
const double 		AdaptThresh = .05;			// adaption criteria threshold
const int 		AdaptFreq = 100;			//adaption time cycle

//There should be no reason why you would need to change anything below this comment//

const int 		ng = 1 ;				//number of phase fields
const int 		padlevel = 0;				//Level of which the interface is padded with nodes
const double 		w = size;				//Width of a single domain
const double		h = size;				//Height of a single domain

const double 		XCen = (double)size*nx/2.0;		//X Center of entire domain
const double 		YCen = (double)size*ny/2.0;		//Y Center of entire domain

const int 		Numx = 4*nx;				//Number of initial nodes needed along x
const int 		Numy = 4*ny;				//Number of initial nodes needed along y

#endif



