#ifndef gridDEF
#define gridDEF

#include "tree.h"
#include "node.h"
#include "arrayNode.h"

class grid
{
    node *boundary;
    list<node*> *tmpnodes;
    list<node*> *ghosts;
	list<node *>::iterator nodeNULL;
	list<node *>::iterator ghostNULL;
    arrayNode *nodeArray;

    int nTrees;
	    
    tree **AdaptiveTree;
	 
    public:
        //seed for random number generator
        //unsigned sd;
        
        int nodesize,ghostsize,b0,b1,b2,b3,bt;    
        list<node*> *nodes;
        grid(int Periodic);
        virtual ~grid();
        
        void setdx();
    	void updateGrid(int npt);
        void initializeGrid();
        void createArray(int npt);
        void updateGhosts(int l,int npt);
    	void calcprePhi(int npt);
        void calcdPdt(int niter,int npt);
        void calcdCdt(int niter, int npt);
        void step(int npt);
        void outputelement(int l,int npt);
        void output(int l,int npt);
    	void updateBC(int npt);
        void calcWs(double *Parpx, double *Parpy, double *Parrad , double *ovParrads, int Np);  // Ws
};

#endif

