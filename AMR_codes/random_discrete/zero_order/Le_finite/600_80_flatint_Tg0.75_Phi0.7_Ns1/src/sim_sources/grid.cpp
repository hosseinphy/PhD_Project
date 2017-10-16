#include <time.h>

#include <stdlib.h>
#include <stdio.h>
#include "initial.h"
#include "grid.h"
#include <omp.h>

#include <cstdlib>
#include <climits>


using namespace std;

grid::~grid()
{
	delete [] nodeArray;
}

grid::grid(int Periodic)
{
	int i,j,q,l,c,k;
	int ii,jj;
	
	tmpnodes = new list<node*>;
	nodes = new list<node*>;
	ghosts = new list<node*>;
	nodeNULL = nodes->end();
	ghostNULL = ghosts->end();
	
	nodeArray = new arrayNode[12000000];
	
	AdaptiveTree = new tree*[1];
	nTrees = 1;

	node ***initNodes;
	treenode ***initEle1,***initEle0;



	if(Periodic == 1) // Periodic Boundaries
	{
		/******************** DOMAIN ****************************/

		initNodes = new node**[Numx];
		for(i=0;i<Numx;i++)
			initNodes[i] = new node*[Numy];
	
		initEle0 = new treenode**[nx];
		for(i=0;i<nx;i++)
			initEle0[i] = new treenode*[ny];

		initEle1 = new treenode**[nx*2];
		for(i=0;i<nx*2;i++)
			initEle1[i] = new treenode*[ny*2];

		/********** Create Nodes ***********/
		for(i=0;i<Numx;i++)
		{
			for(j=0;j<Numy;j++)
			{
				initNodes[i][j] = new node(i*w/4,j*w/4,nodeNULL,ghostNULL);
				initNodes[i][j]->me = nodeNULL;
				initNodes[i][j]->gme = ghostNULL;
			}
		}

		/*********** Set Node Neighbours **********/
	
		//4 Point Neighbours

		for(i=0;i<Numx;i+=2)
		{
			for(j=0;j<Numy;j+=2)
			{
				initNodes[i][j]->NN[0] = initNodes[i][(Numy+j+2)%Numy];
				initNodes[i][j]->NN[1] = initNodes[(Numx+i+2)%Numx][j];
				initNodes[i][j]->NN[2] = initNodes[i][(Numy+j-2)%Numy];
				initNodes[i][j]->NN[3] = initNodes[(Numx+i-2)%Numx][j];
			}
		}

		/*********** Create Elements **************/
		c=0;
		for(i=0;i<nx;i++)  // Create Zeroth Level Elements
		{
			for(j=0;j<ny;j++)
			{
				initEle0[i][j] = new treenode(NULL,c,0);
				c++;
			}
		}

		for(i=0;i<nx*2;i++)//Create First Level Elements
		{
			for(j=0;j<ny*2;j++)
			{
				initEle1[i][j] = new treenode(initEle0[i/2][j/2],c,1);
				c++;
			}
		}
		for(i=0;i<nx;i++)	// Set Element number
		{
			for(j=0;j<ny;j++)
			{
				initEle1[i*2][j*2]->num = 3;
				initEle1[i*2+1][j*2]->num = 2;
				initEle1[i*2+1][j*2+1]->num = 1;
				initEle1[i*2][j*2+1]->num = 0;
			}
		}
		/***************** Apply Nodes To Elements ***************/

		for(i=0;i<nx;i++)
		{
			for(j=0;j<ny;j++)
			{
				q = 2*i;
				l = 2*j;
				initEle0[i][j]->N[0] = initNodes[(q)*2][((l+2)*2)%Numy];
				initEle0[i][j]->N[1] = initNodes[(q+1)*2][((l+2)*2)%Numy];
				initEle0[i][j]->N[2] = initNodes[((q+2)*2)%Numx][((l+2)*2)%Numy];
				initEle0[i][j]->N[3] = initNodes[(q)*2][(l+1)*2];
				initEle0[i][j]->N[4] = initNodes[(q+1)*2][(l+1)*2];
				initEle0[i][j]->N[5] = initNodes[((q+2)*2)%Numx][(l+1)*2];
				initEle0[i][j]->N[6] = initNodes[(q)*2][l*2];
				initEle0[i][j]->N[7] = initNodes[(q+1)*2][l*2];
				initEle0[i][j]->N[8] = initNodes[((q+2)*2)%Numx][l*2];
			}
		}
		for(i=0;i<nx*2;i++)
		{
			for(j=0;j<ny*2;j++)
			{
				q = 2*i;
				l = 2*j;
				initEle1[i][j]->N[0] = initNodes[q][(l+2)%Numy];
				initEle1[i][j]->N[1] = initNodes[q+1][(l+2)%Numy];
				initEle1[i][j]->N[2] = initNodes[(q+2)%Numx][(l+2)%Numy];
				initEle1[i][j]->N[3] = initNodes[q][l+1];
				initEle1[i][j]->N[4] = initNodes[q+1][l+1];
				initEle1[i][j]->N[5] = initNodes[(q+2)%Numx][l+1];
				initEle1[i][j]->N[6] = initNodes[q][l];
				initEle1[i][j]->N[7] = initNodes[q+1][l];
				initEle1[i][j]->N[8] = initNodes[(q+2)%Numx][l];	
			}
		}
		/********************* Element Neighbours **************************/

		for(i=0;i<nx;i++)
		{
			for(j=0;j<ny;j++)
			{
				initEle0[i][j]->EN[0] = initEle0[i][(ny+j+1)%ny];
				initEle0[i][j]->EN[1] = initEle0[(nx+i+1)%nx][j];
				initEle0[i][j]->EN[2] = initEle0[i][(ny+j-1)%ny];
				initEle0[i][j]->EN[3] = initEle0[(nx+i-1)%nx][j];
			}
		}

		for(i=0;i<2*nx;i++)
		{
			for(j=0;j<2*ny;j++)
			{
				initEle1[i][j]->EN[0] = initEle1[i][(2*ny+j+1)%(2*ny)];
				initEle1[i][j]->EN[1] = initEle1[(2*nx+i+1)%(2*nx)][j];
				initEle1[i][j]->EN[2] = initEle1[i][(2*ny+j-1)%(2*ny)];
				initEle1[i][j]->EN[3] = initEle1[(2*nx+i-1)%(2*nx)][j];
			}
		}

		/********************** Set Element Root level Children **************************/

		for(i=0;i<nx;i++)
		{
			for(j=0;j<ny;j++)
			{
				initEle0[i][j]->TN[0] = initEle1[i*2][j*2+1];
				initEle0[i][j]->TN[1] = initEle1[i*2+1][j*2+1];
				initEle0[i][j]->TN[2] = initEle1[i*2+1][j*2];
				initEle0[i][j]->TN[3] = initEle1[i*2][j*2];
			}
		}
		/*************** Create Tree ***********************/

		AdaptiveTree[0] = new tree(initEle0,initEle1,initNodes,nodes,ghosts);
	}
	else // Non-Periodic
	{
		/******************** DOMAIN ****************************/
		cout << "Create Domain" << endl;
		initNodes = new node**[2*nx+1];
		for(i=0;i<2*nx+1;i++)
			initNodes[i] = new node*[2*ny+1];
		
		initEle0 = new treenode**[nx];
		for(i=0;i<nx;i++)
			initEle0[i] = new treenode*[ny];

		/********** Create Nodes ***********/
		cout << "Create Nodes" << endl;
		for(i=0;i<2*nx+1;i++)
		{
			for(j=0;j<2*ny+1;j++)
			{
				initNodes[i][j] = new node(i*w/2.0,j*w/2.0,nodeNULL,ghostNULL);
				initNodes[i][j]->me = nodeNULL;
				initNodes[i][j]->gme = ghostNULL;
			}
		}
		/*********** Set Node Neighbours **********/
		cout << "Set Node Neighbours" << endl;
		//4 Point Neighbours

		for(i=0;i<2*nx+1;i+=2)
		{
			for(j=0;j<2*ny+1;j+=2)
			{
				if(j == 2*ny)
					initNodes[i][j]->NN[0] = NULL;
				else
					initNodes[i][j]->NN[0] = initNodes[i][j+2];
				if(i == 2*nx)
					initNodes[i][j]->NN[1] = NULL;
				else
					initNodes[i][j]->NN[1] = initNodes[i+2][j];
				if(j == 0)
					initNodes[i][j]->NN[2] = NULL;
				else
					initNodes[i][j]->NN[2] = initNodes[i][j-2];
				if(i == 0)
					initNodes[i][j]->NN[3] = NULL;
				else
					initNodes[i][j]->NN[3] = initNodes[i-2][j];
			}
		}
					
		for(i=0;i<2*nx+1;i+=2)
		{
			for(j=0;j<2*ny+1;j+=2)
			{
				if(i==0 && j==2*ny)//Top left
				{
					for(k=2;k<5;k++)
					{
						for(l=0;l<3;l++)
						{
							initNodes[i][j]->N25[k][l] = initNodes[i-2+k][j-2+l];
						}
					}
				}
				else if(i==2*nx && j==2*ny)//Top Right
				{
					for(k=0;k<3;k++)
					{
						for(l=0;l<3;l++)
						{
							initNodes[i][j]->N25[k][l] = initNodes[i-2+k][j-2+l];
						}
					}
				}
				else if(i==2*nx && j==0)//Bottom Right
				{
					for(k=0;k<3;k++)
					{
						for(l=2;l<5;l++)
						{
							initNodes[i][j]->N25[k][l] = initNodes[i-2+k][j-2+l];
						}
					}
				}
				else if(i==0 && j ==0)//Bottom Left
				{
					for(k=2;k<5;k++)
					{
						for(l=2;l<5;l++)
						{
							initNodes[i][j]->N25[k][l] = initNodes[i-2+k][j-2+l];
						}
					}
				}
				else if(i==0)//Left
				{
					for(k=2;k<5;k++)
					{
						for(l=0;l<5;l++)
						{
							initNodes[i][j]->N25[k][l] = initNodes[i-2+k][j-2+l];
						}
					}	
				}
				else if(j==2*ny)//Top
				{
					for(k=0;k<5;k++)
					{
						for(l=0;l<3;l++)
						{
							initNodes[i][j]->N25[k][l] = initNodes[i-2+k][j-2+l];
						}
					}						
				}
				else if(i==2*nx)//Right
				{
					for(k=0;k<3;k++)
					{
						for(l=0;l<5;l++)
						{
							initNodes[i][j]->N25[k][l] = initNodes[i-2+k][j-2+l];
						}
					}					
				}
				else if(j==0)//Bottom
				{
					for(k=0;k<5;k++)
					{
						for(l=2;l<5;l++)
						{
							initNodes[i][j]->N25[k][l] = initNodes[i-2+k][j-2+l];
						}
					}						
				}
				else//Core
				{
					for(k=0;k<5;k++)
					{
						for(l=0;l<5;l++)
						{
							initNodes[i][j]->N25[k][l] = initNodes[i-2+k][j-2+l];
						}
					}
				}
				
			}
		}
		
		

		/*********** Create Elements **************/
		cout << "Create Elements" << endl;
		c=0;
		for(i=0;i<nx;i++)  // Create Zeroth Level Elements
		{
			for(j=0;j<ny;j++)
			{
				initEle0[i][j] = new treenode(NULL,c,0);
				c++;
			}
		}

		/***************** Apply Nodes To Elements ***************/

		cout << "Apply Nodes to Elements" << endl;
		for(i=0;i<nx;i++)
		{
			for(j=0;j<ny;j++)
			{
				q = 2*i;
				l = 2*j;
				initEle0[i][j]->N[0] = initNodes[q][l+2];
				initEle0[i][j]->N[1] = initNodes[q+1][l+2];
				initEle0[i][j]->N[2] = initNodes[q+2][l+2];
				initEle0[i][j]->N[3] = initNodes[q][l+1];
				initEle0[i][j]->N[4] = initNodes[q+1][l+1];
				initEle0[i][j]->N[5] = initNodes[q+2][l+1];
				initEle0[i][j]->N[6] = initNodes[q][l];
				initEle0[i][j]->N[7] = initNodes[q+1][l];
				initEle0[i][j]->N[8] = initNodes[q+2][l];
			}
		}
		
		/********************* Element Neighbours **************************/
		cout << "Set Element Neighbours" << endl;
		for(i=0;i<nx;i++)
		{
			for(j=0;j<ny;j++)
			{
				if(j==ny-1)
					initEle0[i][j]->EN[0] = NULL;
				else
					initEle0[i][j]->EN[0] = initEle0[i][j+1];
				if(i==nx-1)
					initEle0[i][j]->EN[1] = NULL;
				else
					initEle0[i][j]->EN[1] = initEle0[i+1][j];
				if(j==0)
					initEle0[i][j]->EN[2] = NULL;
				else
					initEle0[i][j]->EN[2] = initEle0[i][j-1];
				if(i==0)
					initEle0[i][j]->EN[3] = NULL;
				else
					initEle0[i][j]->EN[3] = initEle0[i-1][j];
			}
		}
		

		/********************** Set Element Root level Children **************************/
		cout << "Set Element Children" << endl;
		for(i=0;i<nx;i++)
		{
			for(j=0;j<ny;j++)
			{
				initEle0[i][j]->TN[0] = NULL;
				initEle0[i][j]->TN[1] = NULL;
				initEle0[i][j]->TN[2] = NULL;
				initEle0[i][j]->TN[3] = NULL;
			}
		}
		/*************** Create Tree ***********************/
		cout << "Create Tree" << endl;
		AdaptiveTree[0] = new tree(initEle0,initNodes,nodes,ghosts);
	}
	/*************** Set Node Size ********************/
	nodesize = (int)nodes->size();
	ghostsize=0;
}

void grid::initializeGrid()
{
	int trees;
	
	for(trees=0;trees<nTrees;trees++)
    	AdaptiveTree[trees]->initialize(nodes,ghosts);

	nodesize = (int)nodes->size();
	ghostsize = (int)ghosts->size();
}

void grid::setdx()
{
	int trees;
	for(trees=0;trees<nTrees;trees++)
	    AdaptiveTree[trees]->setdx(w);
}

void grid::updateGrid(int npt)
{
	int i,trees;
	int ii;
	
	list<node*>::iterator it;
	for(it = nodes->begin();it != nodes->end();it++)
	{
	    i = (*it)->index;
		
		for(ii=0;ii<npt;ii++)
			(*it)->Phi[ii] = nodeArray[i].Phi[ii];

		(*it)->C = nodeArray[i].C;
	    (*it)->Ws = nodeArray[i].Ws;
	}
	
	for(it = ghosts->begin();it != ghosts->end();it++)
	{
		i = (*it)->index;

		for(ii=0;ii<npt;ii++)
			(*it)->Phi[ii] = nodeArray[i].Phi[ii];
        
		(*it)->C = nodeArray[i].C;
	    (*it)->Ws = nodeArray[i].Ws;
	}
	
	for(trees=0;trees<nTrees;trees++)
		AdaptiveTree[trees]->regrid(nodes, ghosts);

	nodesize = (int)nodes->size();
	ghostsize = (int)ghosts->size();
}

void grid::createArray(int npt)
{
	int i,j;
	int B[4];
	int Bsize;
	int ii;

	list<node*>::iterator it;
	list<node*>::iterator it2;
	list<node*>::iterator itmp;
	
	for(i=0;i<4;i++)
		B[i]=0;

	/******************COPY LIST***************************/
	i = 0;
	for(it = nodes->begin();it != nodes->end();it++)
	{
		for(ii=0;ii<npt;ii++)
		{
			nodeArray[i].Phi[ii] = (*it)->Phi[ii];						
		}
        
		nodeArray[i].C = (*it)->C;
		nodeArray[i].Ws = (*it)->Ws;
	    nodeArray[i].x = (*it)->x;
	    nodeArray[i].y = (*it)->y;
	    nodeArray[i].dx = (*it)->dx;			
	    (*it)->index = i;
	    i++;
	}

	i = nodesize;
	for(it = ghosts->begin();it != ghosts->end();it++)
	{  
		for(ii=0;ii<npt;ii++)
		{
			nodeArray[i].Phi[ii] = (*it)->Phi[ii];						
		}			
		nodeArray[i].C= (*it)->C;
		nodeArray[i].Ws = (*it)->Ws;
		nodeArray[i].x = (*it)->x;
		nodeArray[i].y = (*it)->y;
		(*it)->index = i;
		i++;
	}

	/******************4 point neighbours***************************/
	for(it = nodes->begin();it != nodes->end();it++)
	{
	    for(i=0;i<4;i++)
		{
			if((*it)->NN[i] != NULL)
			{
				nodeArray[(*it)->index].NN[i] = (*it)->NN[i]->index;    
			}
			else
			{
				nodeArray[(*it)->index].NN[i] = -1;
			}
		}
		for(i=0;i<5;i++)
		{
			for(j=0;j<5;j++)
			{
				if((*it)->N25[i][j] != NULL)
				{
					nodeArray[(*it)->index].N25[i][j] = (*it)->N25[i][j]->index;
				}
				else
				{
					nodeArray[(*it)->index].N25[i][j] = -1;
				}
			}
		}
	}

	//Ghosts
	i=nodesize;
	for(it = ghosts->begin();it != ghosts->end();it++)
	{
	    nodeArray[i].NN[0] = (*it)->NN[0]->index;
	    nodeArray[i].NN[1] = (*it)->NN[1]->index;
	    if((*it)->NN[2] != NULL)
	    {
			nodeArray[i].NN[2] = (*it)->NN[2]->index;
			nodeArray[i].NN[3] = (*it)->NN[3]->index;
		}
		else
		{
			nodeArray[i].NN[2] = -1;
			nodeArray[i].NN[3] = -1;
		}
		i++;
	}
	
	// Boundaries
	/***********
	3-----0-----0
	|			|
	|			|
	|			|
	3			1
	|			|
	|			|
	|			|
	2-----2-----1
	**********/

	/******************Boundary Node Copy***************************/
	bt=4;
	b0 = 0;
	b1=0;
	b2=0;
	b3=0;
	Bsize = nodesize+ghostsize;
	/*********************** Boundary 0 *****************************/
	i=0;
	for(it = nodes->begin();it != nodes->end();it++)
	{
		
		if((*it)->NN[0] == NULL)
		{
			
			if((*it)->NN[1] != NULL && (*it)->NN[3] != NULL)
			{
				nodeArray[Bsize+bt+i].NN[2]  =(*it)->N25[1][1]->index;
				nodeArray[Bsize+bt+i+1].NN[2]=(*it)->N25[2][1]->index;
				nodeArray[Bsize+bt+i+2].NN[2]=(*it)->N25[3][1]->index;
				nodeArray[(*it)->index].N25[1][3] = Bsize+bt+i;
				nodeArray[(*it)->index].N25[2][3] = Bsize+bt+i+1;
				nodeArray[(*it)->index].N25[3][3] = Bsize+bt+i+2;
				nodeArray[(*it)->index].NN[0] = Bsize+bt+i+1;
				i+=3;
				b0+=3;
			}
			else if((*it)->NN[1] == NULL)
			{
				nodeArray[Bsize+bt+i].NN[2]=(*it)->N25[1][1]->index;
				nodeArray[Bsize+bt+i+1].NN[2]=(*it)->N25[2][1]->index;
				nodeArray[Bsize].NN[2]=(*it)->N25[1][1]->index;
				nodeArray[(*it)->index].N25[1][3] = Bsize+bt+i;
				nodeArray[(*it)->index].N25[2][3] = Bsize+bt+i+1;
				nodeArray[(*it)->index].N25[3][3] = Bsize;
				nodeArray[(*it)->index].NN[0] = Bsize+bt+i+1;
				i+=2;
				b0+=2;
			}
			else if((*it)->NN[3] == NULL)
			{
				nodeArray[Bsize+3].NN[2]=(*it)->N25[3][1]->index;
				nodeArray[Bsize+bt+i].NN[2]=(*it)->N25[2][1]->index;
				nodeArray[Bsize+bt+i+1].NN[2]=(*it)->N25[3][1]->index;
				nodeArray[(*it)->index].N25[1][3] = Bsize+3;
				nodeArray[(*it)->index].N25[2][3] = Bsize+bt+i;
				nodeArray[(*it)->index].N25[3][3] = Bsize+bt+i+1;
				nodeArray[(*it)->index].NN[0] = Bsize+bt+i;
				i+=2;
				b0+=2;
			}
		}	
	}
	
	/*********************** Boundary 1 *****************************/
	i=0;
	bt += b0;
	for(it = nodes->begin();it != nodes->end();it++)
	{
		if((*it)->NN[1] == NULL)
		{
		
			if((*it)->NN[0] != NULL && (*it)->NN[2] != NULL)
			{
				nodeArray[Bsize+bt+i].NN[2]  =(*it)->N25[1][1]->index;
				nodeArray[Bsize+bt+i+1].NN[2]=(*it)->N25[1][2]->index;
				
				nodeArray[Bsize+bt+i+2].NN[2]=(*it)->N25[1][3]->index;
				nodeArray[(*it)->index].N25[3][1] = Bsize+bt+i;
				nodeArray[(*it)->index].N25[3][2] = Bsize+bt+i+1;
				nodeArray[(*it)->index].N25[3][3] = Bsize+bt+i+2;
				nodeArray[(*it)->index].NN[1] = Bsize+bt+i+1;
				i+=3;
				b1+=3;
			}
			else if((*it)->NN[0] == NULL)
			{
				
				nodeArray[Bsize+bt+i].NN[2]=(*it)->N25[1][1]->index;
				nodeArray[Bsize+bt+i+1].NN[2]=(*it)->N25[1][2]->index;
				nodeArray[(*it)->index].N25[3][1] = Bsize+bt+i;
				nodeArray[(*it)->index].N25[3][2] = Bsize+bt+i+1;
				nodeArray[(*it)->index].NN[1] = Bsize+bt+i+1;
				i+=2;
				b1+=2;
			}
			else if((*it)->NN[2] == NULL)
			{
				
				nodeArray[Bsize+bt+i].NN[2]=(*it)->N25[1][2]->index;
				nodeArray[Bsize+bt+i+1].NN[2]=(*it)->N25[1][3]->index;
				nodeArray[(*it)->index].N25[3][2] = Bsize+bt+i;
				nodeArray[(*it)->index].N25[3][3] = Bsize+bt+i+1;
				nodeArray[(*it)->index].NN[1] = Bsize+bt+i;
				i+=2;
				b1+=2;
			}
		}
	}
	
	/*********************** Boundary 2 *****************************/
	i=0;
	bt+=b1;
	for(it = nodes->begin();it != nodes->end();it++)
	{
		if((*it)->NN[2] == NULL)
		{
			if((*it)->NN[1] != NULL && (*it)->NN[3] != NULL)
			{
				nodeArray[Bsize+bt+i].NN[2]  =(*it)->N25[1][3]->index;
				nodeArray[Bsize+bt+i+1].NN[2]=(*it)->N25[2][3]->index;
				nodeArray[Bsize+bt+i+2].NN[2]=(*it)->N25[3][3]->index;
				nodeArray[(*it)->index].N25[1][1] = Bsize+bt+i;
				nodeArray[(*it)->index].N25[2][1] = Bsize+bt+i+1;
				nodeArray[(*it)->index].N25[3][1] = Bsize+bt+i+2;
				nodeArray[(*it)->index].NN[2] = Bsize+bt+i+1;
				i+=3;
				b2+=3;
			}
			else if((*it)->NN[1] == NULL)
			{
				nodeArray[Bsize+bt+i].NN[2]=(*it)->N25[1][3]->index;
				nodeArray[Bsize+bt+i+1].NN[2]=(*it)->N25[2][3]->index;
				nodeArray[Bsize+1].NN[2]=(*it)->N25[1][3]->index;
				nodeArray[(*it)->index].N25[1][1] = Bsize+bt+i;
				nodeArray[(*it)->index].N25[2][1] = Bsize+bt+i+1;
				nodeArray[(*it)->index].N25[3][1] = Bsize+1;
				nodeArray[(*it)->index].NN[2] = Bsize+bt+i+1;
				i+=2;
				b2+=2;
			}
			else if((*it)->NN[3] == NULL)
			{
				nodeArray[Bsize+2].NN[2]=(*it)->N25[3][3]->index;
				nodeArray[Bsize+bt+i].NN[2]=(*it)->N25[2][3]->index;
				nodeArray[Bsize+bt+i+1].NN[2]=(*it)->N25[3][3]->index;
				nodeArray[(*it)->index].N25[1][1] = Bsize+2;
				nodeArray[(*it)->index].N25[2][1] = Bsize+bt+i;
				nodeArray[(*it)->index].N25[3][1] = Bsize+bt+i+1;
				nodeArray[(*it)->index].NN[2] = Bsize+bt+i;
				i+=2;
				b2+=2;
			}
		}
	}
	
	/*********************** Boundary 3 *****************************/
	i=0;
	bt+=b2;
	for(it = nodes->begin();it != nodes->end();it++)
	{
		if((*it)->NN[3] == NULL)
		{
			if((*it)->NN[0] != NULL && (*it)->NN[2] != NULL)
			{
				nodeArray[Bsize+bt+i].NN[2]  =(*it)->N25[3][1]->index;
				nodeArray[Bsize+bt+i+1].NN[2]=(*it)->N25[3][2]->index;
				nodeArray[Bsize+bt+i+2].NN[2]=(*it)->N25[3][3]->index;
				nodeArray[(*it)->index].N25[1][1] = Bsize+bt+i;
				nodeArray[(*it)->index].N25[1][2] = Bsize+bt+i+1;
				nodeArray[(*it)->index].N25[1][3] = Bsize+bt+i+2;
				nodeArray[(*it)->index].NN[3] = Bsize+bt+i+1;
				i+=3;
				b3+=3;
			}
			else if((*it)->NN[0] == NULL)
			{
				nodeArray[Bsize+bt+i].NN[2]=(*it)->N25[3][1]->index;
				nodeArray[Bsize+bt+i+1].NN[2]=(*it)->N25[3][2]->index;
				nodeArray[(*it)->index].N25[1][1] = Bsize+bt+i;
				nodeArray[(*it)->index].N25[1][2] = Bsize+bt+i+1;
				nodeArray[(*it)->index].NN[3] = Bsize+bt+i+1;

				i+=2;
				b1+=2;
			}
			else if((*it)->NN[2] == NULL)
			{
				nodeArray[Bsize+bt+i].NN[2]=(*it)->N25[3][2]->index;
				nodeArray[Bsize+bt+i+1].NN[2]=(*it)->N25[3][3]->index;
				nodeArray[(*it)->index].N25[1][2] = Bsize+bt+i;
				nodeArray[(*it)->index].N25[1][3] = Bsize+bt+i+1;
				nodeArray[(*it)->index].NN[3] = Bsize+bt+i;
			
				i+=2;
				b1+=2;
			}

		}
	}
}

void grid::updateGhosts(int l,int npt)
{
	int i,j;
	int ii;

	double Phi[ng],C;
	double dPdt[ng];
	double Ws;
	
	if(l == 1)
	{
		//begin openmp
		#pragma omp parallel default(shared) private(ii,Phi,C,j,Ws) num_threads(numprocs)
		{
			#pragma omp for schedule(static)
		    for(i=nodesize;i<nodesize+ghostsize;i++)
		    {
				C = 0.0;
				Ws = 0.0;
				for(ii=0;ii<npt;ii++)
				{
					Phi[ii] = 0.0;

					for(j=0;nodeArray[i].NN[j] != -1 && j<4;j++)
					{
						Phi[ii] += nodeArray[nodeArray[i].NN[j]].Phi[ii];
					}

					nodeArray[i].Phi[ii] = Phi[ii]/j;
				}
				
				for(j=0;nodeArray[i].NN[j] != -1 && j<4;j++)
				{					
					C += nodeArray[nodeArray[i].NN[j]].C;					
					Ws += nodeArray[nodeArray[i].NN[j]].Ws;					
				}

				nodeArray[i].C = C/j;				
				nodeArray[i].Ws = Ws/j;  
		    }
		}//end openmp
	}
	else if(l ==2)
	{
		//begin openmp
		#pragma omp parallel default(shared) private(ii,j,dPdt) num_threads(numprocs)
	    {
			#pragma omp for schedule(static)				
			for(i=nodesize;i<nodesize+ghostsize;i++)
			{
				for(ii=0;ii<npt;ii++)
				{
					dPdt[ii] = 0.0;

					for(j=0;nodeArray[i].NN[j] != -1 && j<4;j++)
					{
						dPdt[ii] += nodeArray[nodeArray[i].NN[j]].dPdt[ii];
					}
					nodeArray[i].dPdt[ii] = dPdt[ii]/j;
				}
			}
	    }//end openmp 	
	}
}


/*
void data_restart()
{
	int t;
	
	begin openmp
	#pragma omp parallel default(shared) private(t) num_threads(numprocs)
	{
		#pragma omp for schedule(static)			
		for(t=0;t<nodesize;t++)	
		{
			nodeArray[t].Phi[0] = rest_tempr[t] ;
			nodeArray[t].C = rest_cont[t] ;
			nodeArray[t].x = rest_x[t] ;
			nodeArray[t].y = rest_y[t] ;

		}
	}// end openmp
}
*/

void grid::calcprePhi(int npt)
{
	//int t;
	
	//begin openmp
	//#pragma omp parallel default(shared) private(t) num_threads(numprocs)
	//{
	//	#pragma omp for schedule(static)			
	//	for(t=0;t<nodesize;t++)	
	//	{
	//		if( nodeArray[t].Phi[0] < Tig )
	//			nodeArray[t].W = 0.;
	//		else if( nodeArray[t].Phi[0] >= Tig )
	//			nodeArray[t].W = A*nodeArray[t].C;
	//	}
	//}// end openmp
}

void grid::calcWs(double *Parpx,double *Parpy, double *Parrad , double *ovParrads, int Np)		// sets the position,shape,size of discrete sources
{
	int t,ii;
	double px,py,x0,y0;
        int p_num =Np;
	//begin openmp
	#pragma omp parallel default(shared) private(t,px,py,x0,y0) num_threads(numprocs)
	{
 		#pragma omp for schedule(static)			

		for(t=0;t<nodesize;t++)	
		{
			px = nodeArray[t].x ;
			py = nodeArray[t].y ;
			nodeArray[t].Ws = 0. ;
            
            for(ii=0;ii<p_num;ii++)
            {
                if ( (px-Parpx[ii])*(px-Parpx[ii]) + (py-Parpy[ii])*(py-Parpy[ii]) < Parrad[ii]*Parrad[ii] ){ nodeArray[t].Ws = ovParrads[ii]; }
            }


		}
	}// end openmp
}

void grid::calcdPdt(int niter,int npt)
{
	int t,n0,n1,n2,n3;
	int pp,mm,pm,mp;
	double dx,GradPhi,Ws;

	int ii;

	//begin openmp
	#pragma omp parallel default(shared) private(t,n0,n1,n2,n3,pp,mm,pm,mp,GradPhi,dx,Ws) num_threads(numprocs)
	{
		#pragma omp for schedule(static)
		for(t=0;t<nodesize;t++)
		{
			n0 = nodeArray[t].NN[0];
			n1 = nodeArray[t].NN[1];
			n2 = nodeArray[t].NN[2];
			n3 = nodeArray[t].NN[3];
			pp = nodeArray[t].N25[3][3];
			mm = nodeArray[t].N25[1][1];
			pm = nodeArray[t].N25[3][1];
			mp = nodeArray[t].N25[1][3];
			
			dx = nodeArray[t].dx;
			Ws = nodeArray[t].Ws;
           	                                     		
			GradPhi = 2.0/3.0*(nodeArray[n0].Phi[0] +nodeArray[n1].Phi[0] +nodeArray[n2].Phi[0] +nodeArray[n3].Phi[0] 
						+ 0.25*(nodeArray[pp].Phi[0] +nodeArray[mp].Phi[0] +nodeArray[pm].Phi[0] +nodeArray[mm].Phi[0] ) 
						- 5.0*nodeArray[t].Phi[0] )/dx/dx;				
				
			nodeArray[t].dPdt[0] = GradPhi ;

		if( nodeArray[t].Phi[0] >= Tig && nodeArray[t].C > A * Ws * dt ) nodeArray[t].dPdt[0] = nodeArray[t].dPdt[0] + A * Ws;

		}
	}// end openmp
}

void grid::calcdCdt(int niter, int npt)
{
	int t,n0,n1,n2,n3;
	int pp,mm,pm,mp;
	double dx,GradC,Ws;

	//begin openmp
	#pragma omp parallel default(shared) private(t,n0,n1,n2,n3,pp,mm,pm,mp,GradC,dx,Ws) num_threads(numprocs)
	{
		#pragma omp for schedule(static)
		for(t=0;t<nodesize;t++)
		{
			n0 = nodeArray[t].NN[0];
			n1 = nodeArray[t].NN[1];
			n2 = nodeArray[t].NN[2];
			n3 = nodeArray[t].NN[3];
			pp = nodeArray[t].N25[3][3];
			mm = nodeArray[t].N25[1][1];
			pm = nodeArray[t].N25[3][1];
			mp = nodeArray[t].N25[1][3];
			
			dx = nodeArray[t].dx;
			Ws =  nodeArray[t].Ws;
           	                                     		
			GradC = 2.0/3.0*(nodeArray[n0].C +nodeArray[n1].C +nodeArray[n2].C +nodeArray[n3].C 
						+ 0.25*(nodeArray[pp].C +nodeArray[mp].C +nodeArray[pm].C +nodeArray[mm].C ) 
						- 5.0*nodeArray[t].C )/dx/dx;
		
			nodeArray[t].dCdt = ovLe*GradC ;

			if( nodeArray[t].Phi[0] >= Tig && nodeArray[t].C > A * Ws * dt ) nodeArray[t].dCdt = nodeArray[t].dCdt - A * Ws;

		}
	}//end openmp
}

void grid::step(int npt)
{
	int t;

	//begin openmp
	#pragma omp parallel default(shared) private(t) num_threads(numprocs)
	{
		#pragma omp for schedule(static)
		for(t=0;t<nodesize;t++)	
		{
			nodeArray[t].updatePhi(npt);
			nodeArray[t].updateC();			
		}
	}//end openmp	
}
	
void grid::output(int l,int npt)
{
	int t,ii;
	char filename[BUFSIZ];
	FILE *fp;
    
	sprintf(filename,"./aaa%d.dat",l);
	fp = fopen(filename,"w");

	/********************** ADAPTIVE OUTPUT *******************************/
	fprintf(fp,"%d\n",nodesize);
	//begin openmp
	#pragma omp parallel default(shared) private(t) num_threads(numprocs)
	{
		#pragma omp for schedule(static)
		for(t=0;t<nodesize;t++)	
		{             
        	fprintf(fp,"%1.6f %1.6f %1.6f %1.6f\n ", nodeArray[t].x, nodeArray[t].y, nodeArray[t].Phi[0], nodeArray[t].C);
    	}
    }

	fclose(fp);
}

void grid::updateBC(int npt)
{
	int i,j,k;

	//begin openmp
	#pragma omp parallel default(shared) private(i,j,k) num_threads(numprocs)
	{
		#pragma omp for nowait
		for(i=nodesize+ghostsize;i<nodesize+ghostsize+b0+b1+b2+b3+4;i++)
		{
			
		    j = nodeArray[i].NN[2];
			for(k=0;k<npt;k++)
			{				
				nodeArray[i].Phi[k] = nodeArray[j].Phi[k];
				nodeArray[i].dPdt[k] = nodeArray[j].dPdt[k];
			}

			nodeArray[i].x = nodeArray[j].x;
		    nodeArray[i].y = nodeArray[j].y;
			nodeArray[i].C = nodeArray[j].C;
			nodeArray[i].dCdt = nodeArray[j].dCdt;
			nodeArray[i].Ws = nodeArray[j].Ws;
		}
	}//end openmp
}

	
