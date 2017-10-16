#include <cmath>
#include <stdlib.h>
#include "initial.h"
#include "node.h"
#include "time.h"

node::node(double xi, double yi,list<node *>::iterator n, list<node *>::iterator g)
{
	int i,j;
	int ii;
	
	Phi = new double[ng];

	x = xi;
	y = yi;
	C = 1.;
	Ws = 0;
	
	for (ii=0;ii<ng;ii++)
		Phi[ii] = 1.0;
    
	NN[0] = NULL;
	NN[1] = NULL;
	NN[2] = NULL;
	NN[3] = NULL;

	for(i=0;i<5;i++)
	{
	    for(j=0;j<5;j++)
	    {
			N25[i][j] = NULL;
	    }
	}
	me = n;
	gme = g;
}

node::~node()
{
	delete Phi;
}

void node::average(node *N1, node *N2)
{
	int ii;

	for(ii=0;ii<ng;ii++)
		Phi[ii] = (N1->Phi[ii] + N2->Phi[ii])/2;
	                        		
	C = (N1->C + N2->C)/2;
	Ws = (N1->Ws + N2->Ws)/2;
}

void node::average(node *N1, node *N2,node *N3, node *N4)
{
	int ii;

	for(ii=0;ii<ng;ii++)
		Phi[ii] = (N1->Phi[ii] + N2->Phi[ii] +N3->Phi[ii] + N4->Phi[ii])/4;
	        
	C = (N1->C + N2->C + N3->C+ N4->C)/4;
	Ws = (N1->Ws + N2->Ws + N3->Ws+ N4->Ws)/4;
}

void node::initialize()
{
	int ii;
	double pos;

	pos = y - yo - Amp * ( 2.*(double)rand_r(&seed)/(double)RAND_MAX - 1) ;

	if(flag==1){ 
		pos = y - yo - Amp * cos(2.*Pi*x/lambda); 
	}

	Phi[0] = 1. ;
	C = 0. ;

	if (-RR <= pos && pos <= 0.){
		Phi[0] = (Tig * (exp ( RR ) - exp( -1.*pos )))/(exp( RR )- 1.) - ( pos/RR) ;
		C = ((1. -  exp ( Le *( RR + pos )))/( Le * RR * exp(Le * ( RR + pos )))) + (RR + pos)/RR;
	}

	else if (pos > 0.0 ){
		Phi[0] = Tig * exp( -1.*pos ) ;
		C = 1. + ( (1.- exp(Le * RR) )*exp(-1.*Le*pos) ) / ( Le * RR * exp(Le*RR) ) ;
	}
	
}	

void node::setNN(int i, node *n)
{
	NN[i] = n;
}
