#include "element.h"
#include "initial.h"
#include <cmath>


treenode::treenode(treenode *parent,int i, int lvl)
{
	P = parent;
	TN[0] = NULL;
	TN[1] = NULL;
	TN[2] = NULL;
	TN[3] = NULL;
	num = i;
	level = lvl;
}

int treenode::pad()
{
	int ii;

	for (ii=0;ii<ng;ii++)
	{
		if(N[0]->Phi[ii] < 0)
			return 1;
	}
	return 0;
}

int treenode::checkSplit()
{	
	int ii;
	double gradP=0.0;
	double gradC=0.0;

	gradC = fabs(N[0]->C - N[2]->C) + fabs(N[0]->C - N[6]->C) + fabs(N[6]->C - N[8]->C) + fabs(N[2]->C - N[8]->C);

	gradP = fabs( N[0]->Phi[0] - N[2]->Phi[0]) + fabs(N[0]->Phi[0] - N[6]->Phi[0] ) 
					  + fabs( N[6]->Phi[0] - N[8]->Phi[0]) + fabs(N[2]->Phi[0] - N[8]->Phi[0] );
	
	if( (gradP + Beta*gradC) > AdaptThresh || P==NULL )
	{
	    return 1;
	}
	else 		
	{
	    return 0;
	}
}
    
int treenode::checkinit()
{
	double dx;
	int ne,i;
	double y1,y2,y;		
	
	dx = size/pow(2.0,resolution-1);
	y = 0.0;
	y1 = N[6]->y;
	y2 = N[0]->y;	
	
	ne = (int)(2.*pad_Amp/dx);
	for (i=0;i<ne;i++)
	{
		if( y1 <= yo+pad_Amp-i*dx && y2 >= yo-pad_Amp+i*dx )
		{
			return 1;	
		}			
	}
	return 0;
}

int treenode::haveChildren()
{	
	if(TN[0] == NULL)
	    return 0;
	else
	    return 1;
}

int treenode::haveGrandchild(int i)
{

	if(TN[i] != NULL)
	{
		if(TN[i]->haveChildren())
			return 1;
		else
			return 0;
	}
	else
		return 0;
}

int treenode::neighChildren(int i)
{
	if(EN[i] != NULL)
	{
	    if(EN[i]->haveChildren())
			return 1;
		else
			return 0;
	}
	else
	    return 0;
}

int treenode::neighGrandChild()
{
	int i;
	for(i=0;i<4;i++)
	{
	    if(EN[i] != NULL)
	    {
		if(EN[i]->haveGrandchild((i+3)%4))
		{
		    return 1;
		}
		if(EN[i]->haveGrandchild((i+2)%4))
		{
		    return 1;
		}
		if(EN[i]->EN[(i+1)%4] != NULL)
		{
		    if(EN[i]->EN[(i+1)%4]->haveGrandchild((i+3)%4))
		    {
			return 1;
		    }
		}
	    }
	}
	
	return 0;
}

int treenode::noChildren()
{
	if(TN[0] == NULL)
	{
	    return 1;
	}
	else
	{
	    return 0;
	}	
}

void treenode::setEN(int i, treenode *e)
{
	EN[i] = e;
}

void treenode::setN(int i, node *n)
{
	N[i] = n;
}

void treenode::unsplit(int maxLevel)
{
	//RESET CORNER NODE NEIGHBOURS

	//N[0]
	if(N[0]->NN[0] != NULL && N[0]->NN[3]!=NULL) //if not on a boundary
	{
	    if(EN[0]->TN[0]==NULL && EN[3]->TN[0]==NULL) //if this node is no longer attached to a lower level
	    {
			if(EN[0]->EN[3] == NULL)  //if diagonal is not same level(or refined)
			{
			    N[0]->NN[0] = EN[0]->N[0];
			    N[0]->NN[1] = N[2];
			    N[0]->NN[2] = N[6];
			    N[0]->NN[3] = EN[3]->N[0];
			}
			else if (EN[0]->EN[3]->TN[0] == NULL) //if diagonal has no children but exists
			{
			    N[0]->NN[0] = EN[0]->N[0];
			    N[0]->NN[1] = N[2];
			    N[0]->NN[2] = N[6];
			    N[0]->NN[3] = EN[3]->N[0];
			}
	    }
	}
	else if(N[0]->NN[0] != NULL)  // if node if not on a boundary
	{
	    if(EN[0]->TN[0]==NULL) // if element neighbour
	    {
			N[0]->NN[0] = EN[0]->N[0];
			N[0]->NN[1] = N[2];
			N[0]->NN[2] = N[6];
			N[0]->NN[3] = NULL;
	    }
	}
	else if(N[0]->NN[3] != NULL)
	{
	    if(EN[3]->TN[0]==NULL)
	    {
			N[0]->NN[0] = NULL;
			N[0]->NN[1] = N[2];
			N[0]->NN[2] = N[6];
			N[0]->NN[3] = EN[3]->N[0];
	    }
	}
	else
	{
	    N[0]->NN[0] = NULL;
	    N[0]->NN[1] = N[2];
	    N[0]->NN[2] = N[6];
	    N[0]->NN[3] = NULL;
	}

	//N[2]
	if(N[2]->NN[0] != NULL && N[2]->NN[1]!=NULL)
	{
	    if(EN[1]->TN[0]==NULL && EN[0]->TN[0]==NULL)
	    {
			if(EN[1]->EN[0] == NULL)
			{
			    N[2]->NN[0] = EN[0]->N[2];
			    N[2]->NN[1] = EN[1]->N[2];
			    N[2]->NN[2] = N[8];
			    N[2]->NN[3] = N[0];
			}
			else if (EN[1]->EN[0]->TN[0] == NULL)
			{
			    N[2]->NN[0] = EN[0]->N[2];
			    N[2]->NN[1] = EN[1]->N[2];
			    N[2]->NN[2] = N[8];
			    N[2]->NN[3] = N[0];		    
			}
	    }
	}
	else if(N[2]->NN[0] != NULL)
	{
	    if(EN[0]->TN[0]==NULL)
	    {
		    N[2]->NN[0] = EN[0]->N[2];
		    N[2]->NN[1] = NULL;
		    N[2]->NN[2] = N[8];
		    N[2]->NN[3] = N[0];
	    }
	}
	else if(N[2]->NN[1] != NULL)
	{
	    if(EN[1]->TN[0]==NULL)
	    {
		    N[2]->NN[0] = NULL;
		    N[2]->NN[1] = EN[1]->N[2];
		    N[2]->NN[2] = N[8];
		    N[2]->NN[3] = N[0];
	    }
	}
	else
	{
	    N[2]->NN[0] = NULL;
	    N[2]->NN[1] = NULL;
	    N[2]->NN[2] = N[8];
	    N[2]->NN[3] = N[0];
	}

	//N[6]
	if(N[6]->NN[2] != NULL && N[6]->NN[3]!=NULL)
	{
	    if(EN[3]->TN[0]==NULL && EN[2]->TN[0]==NULL)
		{
			if(EN[2]->EN[3] == NULL)
			{
			    N[6]->NN[0] = N[0];
			    N[6]->NN[1] = N[8];
			    N[6]->NN[2] = EN[2]->N[6];
			    N[6]->NN[3] = EN[3]->N[6];
			}
			else if (EN[2]->EN[3]->TN[0] == NULL)
			{
				N[6]->NN[0] = N[0];
			    N[6]->NN[1] = N[8];
			    N[6]->NN[2] = EN[2]->N[6];
				N[6]->NN[3] = EN[3]->N[6];		    
			}
		}
	}
	else if(N[6]->NN[3] != NULL)
	{
		if(EN[3]->TN[0]==NULL)
		{
			N[6]->NN[0] = N[0];
			N[6]->NN[1] = N[8];
			N[6]->NN[2] = NULL;
			N[6]->NN[3] = EN[3]->N[6];
		}
	}
	else if(N[6]->NN[2] != NULL)
	{
		if(EN[2]->TN[0]==NULL)
		{
		    N[6]->NN[0] = N[0];
		    N[6]->NN[1] = N[8];
		    N[6]->NN[2] = EN[2]->N[6];
		    N[6]->NN[3] = NULL;
	    }
	}
	else
	{
	    N[6]->NN[0] = N[0];
	    N[6]->NN[1] = N[8];
	    N[6]->NN[2] = NULL;
	    N[6]->NN[3] = NULL;
	}

	//N[8]
	if(N[8]->NN[1] != NULL && N[8]->NN[2]!=NULL)
	{
		if(EN[2]->TN[0]==NULL && EN[1]->TN[0]==NULL)
		{
			if(EN[1]->EN[2] == NULL)
			{
			    N[8]->NN[0] = N[2];
			    N[8]->NN[1] = EN[1]->N[8];
			    N[8]->NN[2] = EN[2]->N[8];
			    N[8]->NN[3] = N[6];
			}
			else if (EN[1]->EN[2]->TN[0] == NULL)
			{
			    N[8]->NN[0] = N[2];
			    N[8]->NN[1] = EN[1]->N[8];
			    N[8]->NN[2] = EN[2]->N[8];
			    N[8]->NN[3] = N[6];		    
			}
	    }
	}
	else if(N[8]->NN[1] != NULL)
	{
	    if(EN[1]->TN[0]==NULL)
	    {
			N[8]->NN[0] = N[2];
			N[8]->NN[1] = EN[1]->N[8];
			N[8]->NN[2] = NULL;
			N[8]->NN[3] = N[6];
	    }
	}
	else if(N[8]->NN[2] != NULL)
	{
	    if(EN[2]->TN[0]==NULL)
	    {
			N[8]->NN[0] = N[2];
			N[8]->NN[1] = NULL;
			N[8]->NN[2] = EN[2]->N[8];
			N[8]->NN[3] = N[6];
	    }
	}
	else
	{
	    N[8]->NN[0] = N[2];
	    N[8]->NN[1] = NULL;
	    N[8]->NN[2] = NULL;
	    N[8]->NN[3] = N[6];	    
	}

	//RESET ELEMENT NEIGHBOURS CHILDREN TO NULL

	if(EN[0] != NULL)
	{
	    if(EN[0]->TN[3] != NULL)
	    {
			EN[0]->TN[3]->EN[2] = NULL;
			EN[0]->TN[2]->EN[2] = NULL;
	    }
	}
	if(EN[1] != NULL)
	{
		if(EN[1]->TN[0] != NULL)
		{
			EN[1]->TN[0]->EN[3] = NULL;
			EN[1]->TN[3]->EN[3] = NULL;
		}
	}
	if(EN[2] != NULL)
	{
		if(EN[2]->TN[0] != NULL)
		{
			EN[2]->TN[0]->EN[0] = NULL;
			EN[2]->TN[1]->EN[0] = NULL;
		}
	}
	if(EN[3] != NULL)
	{
		if(EN[3]->TN[1] != NULL)
		{
			EN[3]->TN[1]->EN[1] = NULL;
			EN[3]->TN[2]->EN[1] = NULL;
		}
	}

	// DELETE CHILDREN GHOSTS IF THEY ARE GHOSTS
	if(level +1 < maxLevel -1)
	{
		if(TN[0]->EN[3] == NULL)
		{
		    delete TN[0]->N[3];
		}
		if(TN[0]->EN[0] == NULL)
		{
		    delete TN[0]->N[1];
		}
		if(TN[1]->EN[0] == NULL)
		{
		    delete TN[1]->N[1];
		}
		if(TN[1]->EN[1] == NULL)
		{
		    delete TN[1]->N[5];
		}
		if(TN[2]->EN[1] == NULL)
		{
		    delete TN[2]->N[5];
		}
		if(TN[2]->EN[2] == NULL)
		{
		    delete TN[2]->N[7];
		}
		if(TN[3]->EN[2] == NULL)
		{
		    delete TN[3]->N[7];
		}
		if(TN[3]->EN[3] == NULL)
		{
		    delete TN[3]->N[3];
		}

		//delete middle ghosts
		delete TN[0]->N[4];
		delete TN[1]->N[4];
		delete TN[2]->N[4];
		delete TN[3]->N[4];
		//delete shared edge ghosts
		delete TN[0]->N[5];
		delete TN[1]->N[7];
		delete TN[2]->N[3];
		delete TN[3]->N[1];
	}
	//DELETE CHILDREN
	delete TN[0];
	delete TN[1];
	delete TN[2];
	delete TN[3];
	TN[0] = NULL;
	TN[1] = NULL;
	TN[2] = NULL;
	TN[3] = NULL;
}

void treenode::split(int maxlevel,list<node *>::iterator n, list<node *>::iterator g)
{
	/*
  	Intricate:
  	Must maintain node neighbours and element neighbours.
  	Rules:
  	Everything is stored counter clockwise starting at the top, except nodes storage in the element
  	(since it is not cyclic).  These are shown below.
	  
  	Element Neighbours for element E.
	            E0
	            |
	       	E3--E--E1
				|
	            E2
			
  	Node Neighbours for node N.
                N0
                |
           	N3--N--N1
    	      	|
                N2
			
	Nodes and Ghost Nodes in the Element
        	N0----N1----N2
         	|	  |		|
	       	N3----N4----N5
	        |	  |		|
     	 	N6----N7----N8
	      
	All elements E0-3 are on the same level(may be NULL)
	All Nodes are on the same level, N4 is a ghost node;
      	N1,3,5,7 may be ghost nodes depending on the lowest level of the neighbour.
      	N0,2,6,8 are real nodes irregardless of lowest level of neighbour.
      	All Nodes are shared with another element(and must be checked for when splitting) except for N4.
	 
	Splitting:
  		Element neighbours must be maintained when splitting.
      	Element neighbours CANNOT be more than 1 level apart.
      		If splitting will violate this, split the neighbour first.
	      	Create children elements.
	      	Set element neighbours, and reset neighbours to new elements.
	      	Set Node Neighbours
	      	Create Ghost Nodes/Set new level boundary nodes to neighbour nodes if they exist.
	      	Maintanance of lists done by tree.
          		Tree checks each element in turn to see if it needs to be split.
			If it is split tree removes the current element from the leaves list and adds the resultant children to the corresponding level list.
			It also moves the children's nodes into the corresponding list, ghost or node.
		    Midline and center Nodes will always be ghosts, even if it exhists in a neighbour element, since splitting
		       only occurs if the element neighbours are on the same level OR 1 level below it(never if 1 level above it since
		       splitting would violate the 1 level apart rule)
		       Therefore splitting will ALWAYS bring an element to the level of its neighbour or to 1 level below it.
		    Every node has the iterator to its list position(NULL if not in a list;for ease of removing nodes to and from the lists without
                                       performing searches)
	*/

	treenode *T1,*T2,*T3,*T4;
	node *N5,*N9,*N12,*N8;
	double dx;
	
	/*Create Children*/	
	T1 = new treenode(this,0,level+1);
	T2 = new treenode(this,1,level+1);
	T3 = new treenode(this,2,level+1);
	T4 = new treenode(this,3,level+1);
	TN[0] = T1;
	TN[1] = T2;
	TN[2] = T3;
	TN[3] = T4;

	/*Set Element Neighbours*/
	if(EN[0] != NULL)
	{
	    TN[0]->EN[0] = EN[0]->TN[3];
	    TN[1]->EN[0] = EN[0]->TN[2];
	}
	else
	{
	    TN[0]->EN[0] = NULL;
	    TN[1]->EN[0] = NULL;
	}
	if(EN[1] != NULL)
	{
	    TN[1]->EN[1] = EN[1]->TN[0];
	    TN[2]->EN[1] = EN[1]->TN[3];
	}
	else
	{
	    TN[1]->EN[1] = NULL;
	    TN[2]->EN[1] = NULL;
	}
	if(EN[2] != NULL)
	{
	    TN[2]->EN[2] = EN[2]->TN[1];
	    TN[3]->EN[2] = EN[2]->TN[0];
	}
	else
	{
	    TN[2]->EN[2] = NULL;
	    TN[3]->EN[2] = NULL;
	}
	if(EN[3] != NULL)
	{
	    TN[0]->EN[3] = EN[3]->TN[1];
	    TN[3]->EN[3] = EN[3]->TN[2];
	}
	else
	{
	    TN[0]->EN[3] = NULL;
	    TN[3]->EN[3] = NULL;
	}
	TN[0]->EN[1] = TN[1];
	TN[0]->EN[2] = TN[3];
	TN[1]->EN[2] = TN[2];
	TN[1]->EN[3] = TN[0];
	TN[2]->EN[0] = TN[1];
	TN[2]->EN[3] = TN[3];
	TN[3]->EN[0] = TN[0];
	TN[3]->EN[1] = TN[2];
	
	/* set neighbour elements of same level's neighbours to new elements if they exist*/
	if(EN[0] != NULL)
	{
	    if(EN[0]->TN[3] != NULL)
			EN[0]->TN[3]->EN[2] = TN[0];
	    if(EN[0]->TN[2] != NULL)
			EN[0]->TN[2]->EN[2] = TN[1];
	}
	if(EN[1] != NULL)
	{
	    if(EN[1]->TN[0] != NULL)
			EN[1]->TN[0]->EN[3] = TN[1];
	    if(EN[1]->TN[3] != NULL)
			EN[1]->TN[3]->EN[3] = TN[2];
	}
	if(EN[2] != NULL)
	{
	    if(EN[2]->TN[1] != NULL)
			EN[2]->TN[1]->EN[0] = TN[2];
	    if(EN[2]->TN[0] != NULL)
			EN[2]->TN[0]->EN[0] = TN[3];
	}
	if(EN[3] != NULL)
	{
	    if(EN[3]->TN[2] != NULL)
			EN[3]->TN[2]->EN[1] = TN[3];
	    if(EN[3]->TN[1] != NULL)
			EN[3]->TN[1]->EN[1] = TN[0];
	}

	/*Set new element Nodes*/
	TN[0]->N[0] = N[0];
	TN[0]->N[2] = N[1];
	TN[0]->N[6] = N[3];
	TN[0]->N[8] = N[4];
	TN[1]->N[0] = N[1];
	TN[1]->N[2] = N[2];
	TN[1]->N[6] = N[4];
	TN[1]->N[8] = N[5];
	TN[2]->N[0] = N[4];
	TN[2]->N[2] = N[5];
	TN[2]->N[6] = N[7];
	TN[2]->N[8] = N[8];
	TN[3]->N[0] = N[3];
	TN[3]->N[2] = N[4];
	TN[3]->N[6] = N[6];
	TN[3]->N[8] = N[7];

	/*Create new ghost nodes if needed*/
	if(level +1 < maxlevel-1)
	{
		/*
		0 1 2
		3 4 5
		6 7 8
		*/
	    //cout << num << endl;
	    if(N[6]->x < N[7]->x)			
			dx = N[7]->x - N[6]->x;
	    else
			dx = N[8]->x - N[7]->x;
		   
		/*
		0  a  1  a  2
		a  a  5
		3  8  4  9  5
		a  a 12
		6  a  7  a  8
		*/

		N5 = new node(N[4]->x, N[4]->y+dx/2.0,n,g);
		N5->average(N[1],N[4]);
		N9 = new node(N[4]->x+dx/2.0,N[4]->y,n,g);
		N9->average(N[5],N[4]);
		N12 = new node(N[4]->x,N[4]->y - dx/2.0,n,g);
		N12->average(N[7],N[4]);
		N8 = new node(N[4]->x - dx/2.0,N[4]->y,n,g);
		N8->average(N[3],N[4]);
		
		/*
		0  a  1  a  2
		a  a  5
		3  8  4  9  5
		a  a 12
		6  a  7  a  8
		*/

		TN[0]->N[4] = new node(N[4]->x - dx/2.0, N[4]->y + dx/2.0,n,g);
		TN[0]->N[4]->average(N[0],N[1],N[3],N[4]);
		
		TN[0]->N[5] =N5;
		TN[0]->N[7] = N8;
		TN[1]->N[3] = N5;
		TN[1]->N[4] = new node(N[4]->x + dx/2.0, N[4]->y + dx/2.0,n,g);
		TN[1]->N[4]->average(N[1],N[2],N[4],N[5]);
		
		TN[1]->N[7] = N9;
		TN[2]->N[1] = N9;
		TN[2]->N[3] = N12;
		TN[2]->N[4] = new node(N[4]->x + dx/2.0, N[4]->y - dx/2.0,n,g);
		TN[2]->N[4]->average(N[4],N[5],N[7],N[8]);
		
		TN[3]->N[1] = N8;
		TN[3]->N[4] = new node(N[4]->x - dx/2.0, N[4]->y - dx/2.0,n,g);
		TN[3]->N[4]->average(N[3],N[4],N[6],N[7]);
		TN[3]->N[5] = N12;
		
		/********************************************************************************
		  NEW ADDITION TO UPDATE dPdt AND dUdt -> Feb 15 2005
		  *******************************************************************************/
		N[4]->average(N[0],N[2],N[6],N[8]);
		N[1]->average(N[0],N[2]);
		N[3]->average(N[0],N[6]);
		N[5]->average(N[8],N[2]);
		N[7]->average(N[8],N[6]);
		
		/********************************************************************************/
		/*Edge Ghost Nodes*/
		/*
		0  a  1  a  2
		a  a  5
		3  8  4  9  5
		a  a 12
		6  a  7  a  8
		*/
		if(EN[0] != NULL)
		{
		    if(EN[0]->TN[3] == NULL)
			{
				TN[0]->N[1] = new node(N[4]->x - dx/2.0, N[4]->y + dx,n,g);
				TN[0]->N[1]->average(N[0],N[1]);
				TN[1]->N[1] = new node(N[4]->x + dx/2.0, N[4]->y + dx,n,g);
				TN[1]->N[1]->average(N[1],N[2]);
		    }
		    else
			{
				TN[0]->N[1] = EN[0]->TN[3]->N[7];
				TN[1]->N[1] = EN[0]->TN[2]->N[7];
		    }
		}
		else
		{
		    TN[0]->N[1] = new node(N[4]->x - dx/2.0, N[4]->y + dx,n,g);
		    TN[0]->N[1]->average(N[0],N[1]);
		    TN[1]->N[1] = new node(N[4]->x + dx/2.0, N[4]->y + dx,n,g);
		    TN[1]->N[1]->average(N[1],N[2]);
		}
		if(EN[1] != NULL)
		{
		    if(EN[1]->TN[0] == NULL)
		    {
				TN[1]->N[5] = new node(N[4]->x + dx, N[4]->y + dx/2.0,n,g);
				TN[1]->N[5]->average(N[2],N[5]);
				TN[2]->N[5] = new node(N[4]->x + dx, N[4]->y - dx/2.0,n,g);
				TN[2]->N[5]->average(N[5],N[8]);
		    }
		    else
		    {
				TN[1]->N[5] = EN[1]->TN[0]->N[3];
				TN[2]->N[5] = EN[1]->TN[3]->N[3];
		    }
		}
		else
		{
		    TN[1]->N[5] = new node(N[4]->x + dx, N[4]->y + dx/2.0,n,g);
		    TN[1]->N[5]->average(N[2],N[5]);
		    TN[2]->N[5] = new node(N[4]->x + dx, N[4]->y - dx/2.0,n,g);
		    TN[2]->N[5]->average(N[5],N[8]);
		}
		if(EN[2] != NULL)
		{
		    if(EN[2]->TN[0] == NULL)
		    {
			TN[2]->N[7] = new node(N[4]->x + dx/2.0, N[4]->y - dx,n,g);
			TN[2]->N[7]->average(N[7],N[8]);
			TN[3]->N[7] = new node(N[4]->x - dx/2.0, N[4]->y - dx,n,g);
			TN[3]->N[7]->average(N[6],N[7]);
		    }
		    else
		    {
			TN[2]->N[7] = EN[2]->TN[1]->N[1];
			TN[3]->N[7] = EN[2]->TN[0]->N[1];
		    }
		}
		else
		{
		    TN[2]->N[7] = new node(N[4]->x + dx/2.0, N[4]->y - dx,n,g);
		    TN[2]->N[7]->average(N[7],N[8]);
		    TN[3]->N[7] = new node(N[4]->x - dx/2.0, N[4]->y - dx,n,g);
		    TN[3]->N[7]->average(N[6],N[7]);
		}
		if(EN[3] != NULL)
		{
		    if(EN[3]->TN[1] == NULL)
		    {
				TN[3]->N[3] = new node(N[4]->x - dx, N[4]->y - dx/2.0,n,g);
				TN[3]->N[3]->average(N[6],N[3]);
				TN[0]->N[3] = new node(N[4]->x - dx, N[4]->y + dx/2.0,n,g);
				TN[0]->N[3]->average(N[3],N[0]);
			}
			else
			{
				TN[3]->N[3] = EN[3]->TN[2]->N[5];
				TN[0]->N[3] = EN[3]->TN[1]->N[5];
		    }
		}
		else
		{
		    TN[3]->N[3] = new node(N[4]->x - dx, N[4]->y - dx/2.0,n,g);
		    TN[3]->N[3]->average(N[6],N[3]);
		    TN[0]->N[3] = new node(N[4]->x - dx, N[4]->y + dx/2.0,n,g);	    
		    TN[0]->N[3]->average(N[3],N[0]);
		}
    }
	
	/*Set node neighbours do not need to reset neighbour element node neighbours(already set by rules or are ghosts and have no need)*/
	if(EN[0] != NULL)
	{
	    N[0]->NN[0] = EN[0]->N[3];
	    N[1]->NN[0] = EN[0]->N[4];
	    N[2]->NN[0] = EN[0]->N[5];
	}
	else
	{
	    N[0]->NN[0] = NULL;
	    N[1]->NN[0] = NULL;
	    N[2]->NN[0] = NULL;
	}
	if(EN[1] != NULL)
	{
	    N[2]->NN[1] = EN[1]->N[1];
	    N[5]->NN[1] = EN[1]->N[4];
	    N[8]->NN[1] = EN[1]->N[7];
	}
	else
	{
	    N[2]->NN[1] = NULL;
	    N[5]->NN[1] = NULL;
	    N[8]->NN[1] = NULL;
	}
	if(EN[2] != NULL)
	{
	    N[8]->NN[2] = EN[2]->N[5];
	    N[7]->NN[2] = EN[2]->N[4];
	    N[6]->NN[2] = EN[2]->N[3];
	}
	else
	{
	    N[8]->NN[2] = NULL;
	    N[7]->NN[2] = NULL;
	    N[6]->NN[2] = NULL;
	}
	if(EN[3] != NULL)
	{
	    N[0]->NN[3] = EN[3]->N[1];	
	    N[3]->NN[3] = EN[3]->N[4];
	    N[6]->NN[3] = EN[3]->N[7];
	}
	else
	{
	    N[0]->NN[3] = NULL;
	    N[3]->NN[3] = NULL;
	    N[6]->NN[3] = NULL;
	}

	N[0]->NN[1] = N[1];
	N[0]->NN[2] = N[3];
	N[1]->NN[1] = N[2];
	N[1]->NN[2] = N[4];
	N[1]->NN[3] = N[0];
	N[2]->NN[2] = N[5];
	N[2]->NN[3] = N[1];
	N[3]->NN[0] = N[0];
	N[3]->NN[1] = N[4];
	N[3]->NN[2] = N[6];
	N[4]->NN[0] = N[1];
	N[4]->NN[1] = N[5];
	N[4]->NN[2] = N[7];
	N[4]->NN[3] = N[3];
	N[5]->NN[0] = N[2];
	N[5]->NN[2] = N[8];
	N[5]->NN[3] = N[4];
	N[6]->NN[0] = N[3];
	N[6]->NN[1] = N[7];
	N[7]->NN[0] = N[4];
	N[7]->NN[1] = N[8];
	N[7]->NN[3] = N[6];
	N[8]->NN[0] = N[5];
	N[8]->NN[3] = N[7];
}
