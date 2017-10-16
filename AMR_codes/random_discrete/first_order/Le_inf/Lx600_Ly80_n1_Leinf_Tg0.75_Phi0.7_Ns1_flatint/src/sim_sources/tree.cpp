
#include <cmath>
#include "initial.h"
#include "tree.h"
#include <stdlib.h>

using namespace std;


// Periodic
tree::tree(treenode ***initElement0,treenode ***initElement1, node ***initNodes,list<node*> *nodes,list<node*> *ghosts)
{
    int i,j;
	maxLevel = resolution;
	treeLevels = new list<treenode *>[maxLevel];
	nodeNULL = nodes->end();
	ghostNULL = ghosts->end();
	
	//Put Elements on treeLevels;
	cout << "Add First Level Elements to Tree" << endl;
	// First Level
	for(i=0;i<2*nx;i++)
	{
		for(j=0;j<2*ny;j++)
		{
			treeLevels[1].push_front(initElement1[i][j]);
			initElement1[i][j]->me = treeLevels[1].begin();
		}
	}

	// Add Nodes to Node List
	cout << "Add Nodes to Nodelist" << endl;
	for(i=0;i<2*nx;i++)
	{
		for(j=0;j<2*ny;j++)
		{
			if(initElement1[i][j]->N[0]->me == nodeNULL)
			{
				nodes->push_front(initElement1[i][j]->N[0]);	//Place corner nodes on node list
				initElement1[i][j]->N[0]->me = nodes->begin();	
			}
			if(initElement1[i][j]->N[2]->me == nodeNULL)
			{
				nodes->push_front(initElement1[i][j]->N[2]);	//Place corner nodes on node list
				initElement1[i][j]->N[2]->me = nodes->begin();	
			}
			if(initElement1[i][j]->N[6]->me == nodeNULL)
			{
				nodes->push_front(initElement1[i][j]->N[6]);	//Place corner nodes on node list
				initElement1[i][j]->N[6]->me = nodes->begin();	
			}
			if(initElement1[i][j]->N[8]->me == nodeNULL)
			{
				nodes->push_front(initElement1[i][j]->N[8]);	//Place corner nodes on node list
				initElement1[i][j]->N[8]->me = nodes->begin();	
			}
		}
	}
	cout << "Finished Creating Tree" << endl;
}

// Non-Periodic
tree::tree(treenode ***initElement0, node ***initNodes,list<node*> *nodes,list<node*> *ghosts)
{
    int i,j;
	nodeNULL = nodes->end();
	ghostNULL = ghosts->end();
	maxLevel = resolution;
	treeLevels = new list<treenode *>[maxLevel];
	root = initElement0[0][0];
	
	//Put Elements on treeLevels;
	cout << "Add Elements to Tree" << endl;
	for(i=0;i<nx;i++)
	{
		for(j=0;j<ny;j++)
		{
			treeLevels[0].push_front(initElement0[i][j]);
			initElement0[i][j]->me = treeLevels[0].begin();
		}
	}

	// Add Nodes to Node List
	cout << "Add Nodes to Nodelist" << endl;
	for(i=0;i<nx;i++)
	{
		for(j=0;j<ny;j++)
		{
			if(initElement0[i][j]->N[0]->me == nodeNULL)
			{
				nodes->push_front(initElement0[i][j]->N[0]);	//Place corner nodes on node list
				initElement0[i][j]->N[0]->me = nodes->begin();	
			}
			if(initElement0[i][j]->N[2]->me == nodeNULL)
			{
				nodes->push_front(initElement0[i][j]->N[2]);	//Place corner nodes on node list
				initElement0[i][j]->N[2]->me = nodes->begin();	
			}
			if(initElement0[i][j]->N[6]->me == nodeNULL)
			{
				nodes->push_front(initElement0[i][j]->N[6]);	//Place corner nodes on node list
				initElement0[i][j]->N[6]->me = nodes->begin();	
			}
			if(initElement0[i][j]->N[8]->me == nodeNULL)
			{
				nodes->push_front(initElement0[i][j]->N[8]);	//Place corner nodes on node list
				initElement0[i][j]->N[8]->me = nodes->begin();	
			}
		}
	}
	cout << "Finished Creating Tree" << endl;
}

/******** Single Domain Initialization ************/
tree::tree(treenode **Begin_Element,int *element_level,int elementsize,int init_level, int max_level,list<node*> *nodes,list<node*> *ghosts)
{
    int i;
	nodeNULL = nodes->end();
	ghostNULL = ghosts->end();
	maxLevel = max_level;
	treeLevels = new list<treenode *>[maxLevel];

	root = Begin_Element[0]; //Set root in tree
	
	
	for(i=1;i<elementsize;i++)
	{
	   
	    treeLevels[element_level[i]].push_front(Begin_Element[i]);
	    Begin_Element[i]->me = treeLevels[element_level[i]].begin();
	    
	    if(Begin_Element[i]->N[0]->me == nodeNULL)
	    {
		nodes->push_front(Begin_Element[i]->N[0]);	//Place corner nodes on node list
		Begin_Element[i]->N[0]->me = nodes->begin();	
	    }
	    if(Begin_Element[i]->N[2]->me == nodeNULL)
	    {
		nodes->push_front(Begin_Element[i]->N[2]);
		Begin_Element[i]->N[2]->me = nodes->begin();
	    }
	    if(Begin_Element[i]->N[6]->me == nodeNULL)
	    {
		nodes->push_front(Begin_Element[i]->N[6]);
		Begin_Element[i]->N[6]->me = nodes->begin();
	    }
	    if(Begin_Element[i]->N[8]->me == nodeNULL)
	    {
		nodes->push_front(Begin_Element[i]->N[8]);
		Begin_Element[i]->N[8]->me = nodes->begin();
	    }
	}
}

int tree::getElementNum()
{
	int size = 0,i;
	for(i=0;i<maxLevel;i++)
	{
		size += treeLevels[i].size();
	}
	return size;
}
	
void tree::setdx(double w)
{
    int i;
    double c=1;
    for(i=0;i< maxLevel; i++)
	{
		itLevel = treeLevels[i].begin();
		while(itLevel != treeLevels[i].end())
		{
			(*itLevel)->N[0]->dx = w/c;
			(*itLevel)->N[2]->dx = w/c;
			(*itLevel)->N[6]->dx = w/c;
			(*itLevel)->N[8]->dx = w/c;
			itLevel++;
		}
		c = c*2;
  	}
}

void tree::regrid(list<node*> *nodes, list<node*> *ghosts)
{
	int i,c=1,j,k,l;
	list<node*>::iterator it;
	treenode *tmp,*tP;
	treenode *Et[4];
	node *Nt[7][7];
	while(c != 0)
	{
		c=0;	
		for(it =ghosts->begin(); it != ghosts->end();it++)
		    (*it)->gme = ghostNULL;
		ghosts->clear();
		c += unsplitList(nodes);	
		c += splitList(nodes);
	}
	
	padGrid(padlevel,nodes);
	
	//Set 25 node neighbours
	for(i=1; i < maxLevel; i++)
	{
		itLevel = treeLevels[i].begin();
		
	    while(itLevel != treeLevels[i].end())//If not empty level
	    {
			tmp = (*itLevel);
			tP = tmp->P;
		
			//Set all nodes neigbours to NULL
			for(j=0;j<7;j++)
			{
				for(k=0;k<7;k++)
				{
					Nt[j][k] = NULL;
				}
			}

			//N[0]->N25[x][y]
			tP->N[0]->N25[2][2]  = tP->N[0];
			tP->N[1]->N25[2][2]  = tP->N[1];
			tP->N[2]->N25[2][2]  = tP->N[2];
			tP->N[3]->N25[2][2]  = tP->N[3];
			tP->N[4]->N25[2][2]  = tP->N[4];
			tP->N[5]->N25[2][2]  = tP->N[5];
			tP->N[6]->N25[2][2]  = tP->N[6];
			tP->N[7]->N25[2][2]  = tP->N[7];
			tP->N[8]->N25[2][2]  = tP->N[8];

			// Get corner element neighbours
			for(j=0; j<4;j++)
			{
				if(tP->EN[j] == NULL && tP->EN[(j+1)%4] == NULL)
				{
					Et[j] = NULL;
				}
				else if(tP->EN[j] == NULL)
				{
					Et[j] = tP->EN[(j+1)%4]->EN[j];
				}
				else
				{
					Et[j] = tP->EN[j]->EN[(j+1)%4];
				}
			}

			// Build 7x7 Array
			// Middle
			for(j=0;j<9;j++)
			{
				Nt[2+j%3][(int)(4-floor(j/3.0))] = tP->N[j];
			}

			//Top Left Corner
			if(Et[3] != NULL)
			{
				for(j=0;j<9;j++)
				{
					Nt[j%3][(int)(6-floor(j/3.0))] = Et[3]->N[j];
				}
			}

			//Top Right Corner
			if(Et[0] != NULL)
			{
				for(j=0;j<9;j++)
				{
						Nt[4+j%3][(int)(6-floor(j/3.0))] = Et[0]->N[j];
				}
			}
			//Bottom Right Corner
			if(Et[1] != NULL)
			{
				for(j=0;j<9;j++)
				{
						Nt[4+j%3][(int)(2-floor(j/3.0))] = Et[1]->N[j];
				}
			}
			//Bottom Left Corner
			if(Et[2] != NULL)
			{
				for(j=0;j<9;j++)
				{
						Nt[j%3][(int)(2-floor(j/3.0))] = Et[2]->N[j];
				}
			}
			// Top
			if(tP->EN[0] != NULL)
			{
				for(j=0;j<9;j++)
				{
						Nt[2+j%3][(int)(6-floor(j/3.0))] = tP->EN[0]->N[j];
				}
			}
			// Right
			if(tP->EN[1] != NULL)
			{
				for(j=0;j<9;j++)
				{
					Nt[4+j%3][(int)(4-floor(j/3.0))] = tP->EN[1]->N[j];
				}
			}
			// Bottom
			if(tP->EN[2] != NULL)
			{
				for(j=0;j<9;j++)
				{
					Nt[2+j%3][(int)(2-floor(j/3.0))] = tP->EN[2]->N[j];
				}
			}
			// Left
			if(tP->EN[3] != NULL)
			{
				for(j=0;j<9;j++)
				{
					Nt[j%3][(int)(4-floor(j/3.0))] = tP->EN[3]->N[j];
				}
			}

			// Assign Points from 7x7 array
			for(l=0;l<9;l++)
			{
				for(j=0;j<5;j++)
				{
					for(k=0;k<5;k++)
					{
						tP->N[l]->N25[j][k] = Nt[j+l%3][k+2-(int)(floor(l/3.0))];
					}
				}
			}
			itLevel++;
	    }
	}
	
	//Set ghost averaging info
	for(i=maxLevel-2; i >= 0; i--)//Cycle through levels to set ghost neigh
	{
		itLevel = treeLevels[i].begin();
		while(itLevel != treeLevels[i].end())//If not empty level
		{
		    tmp = (*itLevel);
		    if(tmp->N[1]->me == nodeNULL)
		    {
				tmp->N[1]->NN[0] = tmp->N[0];
				tmp->N[1]->NN[1] = tmp->N[2];
				tmp->N[1]->NN[2] = NULL;
				tmp->N[1]->NN[3] = NULL;
		    }
		    if(tmp->N[3]->me == nodeNULL)
		    {
				tmp->N[3]->NN[0] = tmp->N[0];
				tmp->N[3]->NN[1] = tmp->N[6];
				tmp->N[3]->NN[2] = NULL;
				tmp->N[3]->NN[3] = NULL;
		    }
		    if(tmp->N[5]->me == nodeNULL)
		    {
			tmp->N[5]->NN[0] = tmp->N[2];
			tmp->N[5]->NN[1] = tmp->N[8];
			tmp->N[5]->NN[2] = NULL;
			tmp->N[5]->NN[3] = NULL;
		    }
		    if(tmp->N[7]->me == nodeNULL)
		    {
			tmp->N[7]->NN[0] = tmp->N[6];
			tmp->N[7]->NN[1] = tmp->N[8];
			tmp->N[7]->NN[2] = NULL;
			tmp->N[7]->NN[3] = NULL;
		    }
		    tmp->N[4]->NN[0] = tmp->N[0];
		    tmp->N[4]->NN[1] = tmp->N[2];
		    tmp->N[4]->NN[2] = tmp->N[6];
		    tmp->N[4]->NN[3] = tmp->N[8];
		    itLevel++;
		}
    }
	it = nodes->begin();

	while(it != nodes->end())
	{
	    
	    //4 point ghosts aquisition
	    for(i=0;i<4;i++)
	    {
			if((*it)->NN[i] != NULL)
			{
			    if((*it)->NN[i]->me == nodeNULL && (*it)->NN[i]->gme == ghostNULL)
			    {
					ghosts->push_front((*it)->NN[i]);
					(*it)->NN[i]->gme = ghosts->begin();
			    }
			}
    	}
	    
	    //25 point ghost aquisition
	    for(j=0;j<5;j++)
	    {
			for(k=0;k<5;k++)
			{
			    if((*it)->N25[j][k] != NULL)
			    {
					if((*it)->N25[j][k]->me == nodeNULL && (*it)->N25[j][k]->gme == ghostNULL)
					{
					    ghosts->push_front((*it)->N25[j][k]);
					    (*it)->N25[j][k]->gme = ghosts->begin();		   
					}
			    }
			    
			}
	    }
	    it++;
	}
}
	
int tree::padGrid(int n,list<node*> *nodes)
{
    int i,j,c,k;
    treenode *tmp;
    list<treenode*> elementList;
    for(k=0;k<n;k++)
    {
		i = maxLevel -2;
		itLevel = treeLevels[i].begin();
		while(itLevel != treeLevels[i].end())//Cycle through bottom level to initialize
		{
	    	tmp = (*itLevel);
	    	itLevel++;
	    	c = 0;
	    	for(j=0;j<4;j++)
	    	{
				if(tmp->EN[j] != NULL)
				{
		    		if(tmp->EN[j]->haveChildren())
		    		{
						c++;
						break;
		    		}
				}
	    	}
	    	if(c && tmp->pad())//Check to see if this element needs splitting
	    	{
				elementList.push_front(tmp);
	    	}
		}
		itLevel = elementList.begin();
		while(itLevel != elementList.end())
		{
		    tmp = (*itLevel);
		    itLevel++;
		    if( !tmp->haveChildren())
		    {
			    //count++;
			    splitStack.push(tmp);//push onto stack
			    while(!splitStack.empty())//while there are elements to split
			    {	
					tmp = splitStack.top();//get the element on the top of the stack
					if(tmp->EN[tmp->num] == NULL)//check if neighbours need splitting first and if so add put on the stack
					{
				    	if(tmp->P != NULL && tmp->P->EN[tmp->num] != NULL)
					    {
							splitStack.push(tmp->P->EN[tmp->num]);
					    }
					}
					if(tmp->EN[(tmp->num+3)%4] == NULL)
					{
					    if(tmp->P != NULL && tmp->P->EN[(tmp->num+3)%4] != NULL)
					    {
							splitStack.push(tmp->P->EN[(tmp->num+3)%4]);
					    }
					}
					if(tmp->P != NULL)//Diagonal neighbour
					{
					    if(tmp->P->EN[(tmp->num+3)%4] != NULL)
					    {
							if(tmp->P->EN[(tmp->num+3)%4]->EN[tmp->num] != NULL)
							{
							    if(tmp->P->EN[(tmp->num+3)%4]->EN[tmp->num]->TN[0] == NULL)
						    	{
									splitStack.push(tmp->P->EN[(tmp->num+3)%4]->EN[tmp->num]);
					    		}
							}
				    	}
					}
					if(tmp == splitStack.top())//If no neighbours need splitting first, pop the top and split it.
					{
					    splitStack.pop();
					    tmp->split(maxLevel,nodeNULL,ghostNULL);
					    c++;
				    	treeLevels[tmp->level].erase(tmp->me);//erase the element from its level list
						//add its children to the lower level
					    treeLevels[tmp->level+1].push_front(tmp->TN[0]);
					    tmp->TN[0]->me = treeLevels[tmp->level+1].begin();
					    treeLevels[tmp->level+1].push_front(tmp->TN[1]);
					    tmp->TN[1]->me = treeLevels[tmp->level+1].begin();
					    treeLevels[tmp->level+1].push_front(tmp->TN[2]);
				    	tmp->TN[2]->me = treeLevels[tmp->level+1].begin();
					    treeLevels[tmp->level+1].push_front(tmp->TN[3]);
					    tmp->TN[3]->me = treeLevels[tmp->level+1].begin();
					    //Add Nodes to Node list(and remove them from ghost list if needed)
					    if(tmp->N[1]->me == nodeNULL)
					    {
							nodes->push_front(tmp->N[1]);
							tmp->N[1]->me = nodes->begin();
					    }	
					    if(tmp->N[3]->me == nodeNULL)
					    {
							nodes->push_front(tmp->N[3]);
							tmp->N[3]->me = nodes->begin();
					    }
					    if(tmp->N[4]->me == nodeNULL)
					    {
							nodes->push_front(tmp->N[4]);
							tmp->N[4]->me = nodes->begin();
				    	}
					    if(tmp->N[5]->me == nodeNULL)
					    {
							nodes->push_front(tmp->N[5]);
							tmp->N[5]->me = nodes->begin();
					    }	
					    if(tmp->N[7]->me == nodeNULL)
					    {
							nodes->push_front(tmp->N[7]);
							tmp->N[7]->me = nodes->begin();
				    	} 
					}// end if split stack
			    }// end while stack not empty
			}
		}//end list cycle
    }
    return 1;
}

int tree::splitList(list<node*> *nodes)
{
	int i,c=0;
	treenode *tmp;
	list<node*>::iterator it;
	for(i=maxLevel-2;i >= 0; i--)//Cycle through levels to check for splitting
	{
		itLevel = treeLevels[i].begin();
		while(itLevel != treeLevels[i].end())//If not empty level
		{
			tmp = (*itLevel);
			itLevel++;
			if(tmp->checkSplit())//Check to see if this element needs splitting
			{
				
				splitStack.push(tmp);//push onto stack
				while(!splitStack.empty())//while there are elements to split
				{	
					tmp = splitStack.top();//get the element on the top of the stack
					if(tmp->EN[tmp->num] == NULL)//check if neighbours need splitting first and if so add put on the stack
					{
						if(tmp->P != NULL && tmp->P->EN[tmp->num] != NULL)
						{
							splitStack.push(tmp->P->EN[tmp->num]);
						}
					}
					if(tmp->EN[(tmp->num+3)%4] == NULL)
					{
						if(tmp->P != NULL && tmp->P->EN[(tmp->num+3)%4] != NULL)
						{
						    splitStack.push(tmp->P->EN[(tmp->num+3)%4]);
						}
					}
					if(tmp->P != NULL)//Diagonal neighbour
					{
					    if(tmp->P->EN[(tmp->num+3)%4] != NULL)
					    {
						if(tmp->P->EN[(tmp->num+3)%4]->EN[tmp->num] != NULL)
						{
						    if(tmp->P->EN[(tmp->num+3)%4]->EN[tmp->num]->TN[0] == NULL)
						    {
							splitStack.push(tmp->P->EN[(tmp->num+3)%4]->EN[tmp->num]);
						    }
						}
					    }
					}
					if(tmp == splitStack.top())//If no neighbours need splitting first, pop the top and split it.
					{
						splitStack.pop();
						tmp->split(maxLevel,nodeNULL,ghostNULL);
						c++;
						treeLevels[tmp->level].erase(tmp->me);//erase the element from its level list
						//add its children to the lower level
						treeLevels[tmp->level+1].push_front(tmp->TN[0]);
						tmp->TN[0]->me = treeLevels[tmp->level+1].begin();
						treeLevels[tmp->level+1].push_front(tmp->TN[1]);
						tmp->TN[1]->me = treeLevels[tmp->level+1].begin();
						treeLevels[tmp->level+1].push_front(tmp->TN[2]);
						tmp->TN[2]->me = treeLevels[tmp->level+1].begin();
						treeLevels[tmp->level+1].push_front(tmp->TN[3]);
						tmp->TN[3]->me = treeLevels[tmp->level+1].begin();
						//Add Nodes to Node list(and remove them from ghost list if needed)
						if(tmp->N[1]->me == nodeNULL)
						{
							nodes->push_front(tmp->N[1]);
							tmp->N[1]->me = nodes->begin();
						}
						if(tmp->N[3]->me == nodeNULL)
						{
							nodes->push_front(tmp->N[3]);
							tmp->N[3]->me = nodes->begin();
						}
						if(tmp->N[4]->me == nodeNULL)
						{
							nodes->push_front(tmp->N[4]);
							tmp->N[4]->me = nodes->begin();
						}
						if(tmp->N[5]->me == nodeNULL)
						{
							nodes->push_front(tmp->N[5]);
							tmp->N[5]->me = nodes->begin();
						}
						if(tmp->N[7]->me == nodeNULL)
						{
							nodes->push_front(tmp->N[7]);
							tmp->N[7]->me = nodes->begin();
						} 
					}// end if split stack
				}// end while stack not empty
			}//end if check split
		}
	}
	return c;
}

int tree::unsplitList(list<node*> *nodes)
{
	int i,c=0;
	treenode *tmp,*P;
	list<node*>::iterator it;
	for(i=maxLevel-1;i>0;i--)//Cycle through levels to check for unsplitting(starting from bottom level)
	{
	    
		itLevel = treeLevels[i].begin();
		
		while(itLevel != treeLevels[i].end())//cycle through level
		{
			tmp = (*itLevel);
			itLevel++;	
			if(!(tmp->checkSplit()))//if this element will not split
			{
				P = tmp->P;
				if(!(P->checkSplit()))//Check if Parent would not split
				{
					if(!(P->TN[0]->haveChildren()) 
					   && !(P->TN[1]->haveChildren())
					   && !(P->TN[2]->haveChildren())
					   && !(P->TN[3]->haveChildren()))//check if children have no children
					{
						if(!(P->neighGrandChild()))//check if neighbours are the same level or one level higher
						{	
							//ADD Parent to element list
							treeLevels[P->level].push_front(P);
							P->me = treeLevels[P->level].begin();
							//REMOVE Children from element list
							if(itLevel == P->TN[0]->me)
							{
								itLevel++;
							}
							treeLevels[i].erase(P->TN[0]->me);
							if(itLevel == P->TN[1]->me)
							{
								itLevel++;
							}
							treeLevels[i].erase(P->TN[1]->me);
							if(itLevel == P->TN[2]->me)
							{
								itLevel++;
							}
							treeLevels[i].erase(P->TN[2]->me);
							if(itLevel == P->TN[3]->me)
							{
								itLevel++;
							}
							treeLevels[i].erase(P->TN[3]->me);

							//UNSPLIT Parent(Rearrange node neighbours)
							P->unsplit(maxLevel);
							c++;
							//CHECK Edge Nodes; if they are real do nothing, if they aren't remove from node list
							if(P->EN[0] == NULL)//TOP EDGE
							{
								nodes->erase(P->N[1]->me);//erase from node list
								P->N[1]->me = nodeNULL;
							}
							else if(!(P->EN[0]->haveChildren()))
							{
								nodes->erase(P->N[1]->me);//erase from node list
								P->N[1]->me = nodeNULL;
							}
							if(P->EN[1] == NULL)//RIGHT EDGE
							{
								nodes->erase(P->N[5]->me);//erase from node list
								P->N[5]->me = nodeNULL;
							}
							else if(!(P->EN[1]->haveChildren()))
							{
								nodes->erase(P->N[5]->me);//erase from node list
								P->N[5]->me = nodeNULL;
							}
							if(P->EN[2] == NULL)//BOTTOM EDGE
							{
								nodes->erase(P->N[7]->me);//erase from node list
								P->N[7]->me = nodeNULL;
							}
							else if(!(P->EN[2]->haveChildren()))
							{
								nodes->erase(P->N[7]->me);//erase from node list
								P->N[7]->me = nodeNULL;
							}
							if(P->EN[3] == NULL)//LEFT EDGE
							{
								nodes->erase(P->N[3]->me);//erase from node list
								P->N[3]->me = nodeNULL;
							}
							else if(!(P->EN[3]->haveChildren()))
							{
								nodes->erase(P->N[3]->me);//erase from node list
								P->N[3]->me = nodeNULL;
							}
							//RESET Remove center node from node list and add to ghost if necessary
							nodes->erase(P->N[4]->me);
							P->N[4]->me = nodeNULL;
						}
					}//end check if children have no children
				}//end check parent split
			}//end check child split
		}// end level loop
	}
	return c;
}
	
int tree::splitListUpdate(list<node*> *nodes)
{
	int i,c=0;
	treenode *tmp;
	list<node*>::iterator it;
	for(i=maxLevel-2;i >= 0; i--)//Cycle through levels to check for splitting
	{
		itLevel = treeLevels[i].begin();
		
		while(itLevel != treeLevels[i].end())//If not empty level
		{
		   
			tmp = (*itLevel);
			itLevel++;
			if(tmp->checkinit())//Check to see if this element needs splitting
			{
			    
				splitStack.push(tmp);//push onto stack
				
				while(!splitStack.empty())//while there are elements to split
				{	
					tmp = splitStack.top();//get the element on the top of the stack

					if(tmp->EN[tmp->num] == NULL)//check if neighbours need splitting first and if so add put on the stack
					{
						if(tmp->P != NULL && tmp->P->EN[tmp->num] != NULL)
						{
							splitStack.push(tmp->P->EN[tmp->num]);
						}
					}
					if(tmp->EN[(tmp->num+3)%4] == NULL)
					{
						if(tmp->P != NULL && tmp->P->EN[(tmp->num+3)%4] != NULL)
						{
							splitStack.push(tmp->P->EN[(tmp->num+3)%4]);
						}
					}
				
					if(tmp->P != NULL)//Diagonal neighbour
					{
					    if(tmp->P->EN[(tmp->num+3)%4] != NULL)
					    {
							if(tmp->P->EN[(tmp->num+3)%4]->EN[tmp->num] != NULL)
							{
							    if(tmp->P->EN[(tmp->num+3)%4]->EN[tmp->num]->TN[0] == NULL)
							    {
									splitStack.push(tmp->P->EN[(tmp->num+3)%4]->EN[tmp->num]);
								}
							}
					    }
					}
					if(tmp == splitStack.top())//If no neighbours need splitting first, pop the top and split it.
					{
						splitStack.pop();
						
						tmp->split(maxLevel,nodeNULL,ghostNULL);
						
						c++;
						//Remove element from tree level
						treeLevels[tmp->level].erase(tmp->me);//erase the element from its level list

						//add its children to the lower level
						treeLevels[tmp->level+1].push_front(tmp->TN[0]);
						tmp->TN[0]->me = treeLevels[tmp->level+1].begin();
						treeLevels[tmp->level+1].push_front(tmp->TN[1]);
						tmp->TN[1]->me = treeLevels[tmp->level+1].begin();
						treeLevels[tmp->level+1].push_front(tmp->TN[2]);
						tmp->TN[2]->me = treeLevels[tmp->level+1].begin();
						treeLevels[tmp->level+1].push_front(tmp->TN[3]);
						tmp->TN[3]->me = treeLevels[tmp->level+1].begin();

						//Add Nodes to Node list(and remove them from ghost list if needed)
						if(tmp->N[1]->me == nodeNULL)
						{
							nodes->push_front(tmp->N[1]);
							tmp->N[1]->me = nodes->begin();
						}
						if(tmp->N[3]->me == nodeNULL)
						{
							nodes->push_front(tmp->N[3]);
							tmp->N[3]->me = nodes->begin();
						}
						if(tmp->N[4]->me == nodeNULL)
						{
							nodes->push_front(tmp->N[4]);
							tmp->N[4]->me = nodes->begin();
						}
						if(tmp->N[5]->me == nodeNULL)
						{
							nodes->push_front(tmp->N[5]);
							tmp->N[5]->me = nodes->begin();
						}
						if(tmp->N[7]->me == nodeNULL)
						{
							nodes->push_front(tmp->N[7]);
							tmp->N[7]->me = nodes->begin();
						}
					}// end if split stack
				}// end while stack not empty
			}//end if check split
		}
	}
	return c;
}

int tree::unsplitListUpdate(list<node*> *nodes)
{
	int i,c=0;
	treenode *tmp,*P;
	list<node*>::iterator it;
	for(i=maxLevel-1;i>0;i--)//Cycle through levels to check for unsplitting(starting from bottom level)
	{
		itLevel = treeLevels[i].begin();
		
		while(itLevel != treeLevels[i].end())//cycle through level
		{
		    
			tmp = (*itLevel);
			itLevel++;	
			if(!(tmp->checkinit()))//if this element will not split
			{
				P = tmp->P; 
				if(!(P->checkinit()))//Check if Parent would not split
				{
				    
					if(!(P->TN[0]->haveChildren()) 
					   && !(P->TN[1]->haveChildren())
					   && !(P->TN[2]->haveChildren())
					   && !(P->TN[3]->haveChildren()))//check if children have no children
					{
					   
						if(!(P->neighGrandChild()))//check if neighbours are the same level or one level higher
						{	
						    
							//ADD Parent to element list
							treeLevels[P->level].push_front(P);
							P->me = treeLevels[P->level].begin();
							//REMOVE Children from element list
							if(itLevel == P->TN[0]->me)
							{
								itLevel++;
							}
							treeLevels[i].erase(P->TN[0]->me);
							if(itLevel == P->TN[1]->me)
							{
								itLevel++;
							}
							treeLevels[i].erase(P->TN[1]->me);
							if(itLevel == P->TN[2]->me)
							{
								itLevel++;
							}
							treeLevels[i].erase(P->TN[2]->me);
							if(itLevel == P->TN[3]->me)
							{
								itLevel++;
							}
							treeLevels[i].erase(P->TN[3]->me);
							
							//UNSPLIT Parent(Rearrange node neighbours)
							P->unsplit(maxLevel);
							c++;
							//CHECK Edge Nodes; if they are real do nothing, if they aren't remove from node list
							if(P->EN[0] == NULL)//TOP EDGE
							{
								nodes->erase(P->N[1]->me);//erase from node list
								P->N[1]->me = nodeNULL;
							}
							else if(!(P->EN[0]->haveChildren()))
							{
								nodes->erase(P->N[1]->me);//erase from node list
								P->N[1]->me = nodeNULL;
							}
							if(P->EN[1] == NULL)//RIGHT EDGE
							{
								nodes->erase(P->N[5]->me);//erase from node list
								P->N[5]->me = nodeNULL;
							}
							else if(!(P->EN[1]->haveChildren()))
							{
								nodes->erase(P->N[5]->me);//erase from node list
								P->N[5]->me = nodeNULL;
							}
							if(P->EN[2] == NULL)//BOTTOM EDGE
							{
								nodes->erase(P->N[7]->me);//erase from node list
								P->N[7]->me = nodeNULL;
							}
							else if(!(P->EN[2]->haveChildren()))
							{
								nodes->erase(P->N[7]->me);//erase from node list
								P->N[7]->me = nodeNULL;
							}
							if(P->EN[3] == NULL)//LEFT EDGE
							{
								nodes->erase(P->N[3]->me);//erase from node list
								P->N[3]->me = nodeNULL;
							}
							else if(!(P->EN[3]->haveChildren()))
							{
								nodes->erase(P->N[3]->me);//erase from node list
								P->N[3]->me = nodeNULL;
							}
							//RESET Remove center node from node list and add to ghost if necessary
							nodes->erase(P->N[4]->me);
							P->N[4]->me = nodeNULL;
							//CHECK if element neighbours ghosts need to be reset
						}
					}//end check if children have no children
				}//end check parent split
			}//end check child split
		}// end level loop
	}
	return c;
}
	
void tree::initialize(list<node*> *nodes, list<node*> *ghosts)
{
    treenode *tmp,*tP;
	int i,c=1,j,k,l;
	treenode *Et[4];
	node *Nt[7][7];

	list<node*>::iterator it;
	
	for(it = ghosts->begin(); it != ghosts->end();it++)
	    (*it)->gme = ghostNULL;
	ghosts->clear();


	cout << "Initialization: Adapt circles" << endl;
	while(c != 0)
	{
	    c=0;
		
	    c += splitListUpdate(nodes);
	}
	
	//Set 25 node neighbours
	cout << "Initialization: Set 25 node neighbours" << endl;
	for(i=1; i < maxLevel; i++)
	{
		itLevel = treeLevels[i].begin();
		
	    while(itLevel != treeLevels[i].end())//If not empty level
	    {
			tmp = (*itLevel);
			tP = tmp->P;
		
			//Set all nodes neigbours to NULL
		
			for(j=0;j<7;j++)
			{
				for(k=0;k<7;k++)
				{
					Nt[j][k] = NULL;
				}
			}

			//N[0]->N25[x][y]
			tP->N[0]->N25[2][2]  = tP->N[0];
			tP->N[1]->N25[2][2]  = tP->N[1];
			tP->N[2]->N25[2][2]  = tP->N[2];
			tP->N[3]->N25[2][2]  = tP->N[3];
			tP->N[4]->N25[2][2]  = tP->N[4];
			tP->N[5]->N25[2][2]  = tP->N[5];
			tP->N[6]->N25[2][2]  = tP->N[6];
			tP->N[7]->N25[2][2]  = tP->N[7];
			tP->N[8]->N25[2][2]  = tP->N[8];

			// Get corner element neighbours
			for(j=0; j<4;j++)
			{
				if(tP->EN[j] == NULL && tP->EN[(j+1)%4] == NULL)
				{
					Et[j] = NULL;
				}
				else if(tP->EN[j] == NULL)
				{
					Et[j] = tP->EN[(j+1)%4]->EN[j];
				}
				else
				{
					Et[j] = tP->EN[j]->EN[(j+1)%4];
				}
			}

			// Build 7x7 Array
			// Middle
			
			for(j=0;j<9;j++)
			{
				Nt[2+j%3][(int)(4-floor(j/3.0))] = tP->N[j];
			}

			//Top Left Corner
			if(Et[3] != NULL)
			{
				for(j=0;j<9;j++)
				{
					Nt[j%3][(int)(6-floor(j/3.0))] = Et[3]->N[j];
				}
			}
			//Top Right Corner
			if(Et[0] != NULL)
			{
				for(j=0;j<9;j++)
				{
					Nt[4+j%3][(int)(6-floor(j/3.0))] = Et[0]->N[j];
				}
			}
			//Bottom Right Corner
			if(Et[1] != NULL)
			{
				for(j=0;j<9;j++)
				{
					Nt[4+j%3][(int)(2-floor(j/3.0))] = Et[1]->N[j];
				}
			}
			//Bottom Left Corner
			if(Et[2] != NULL)
			{
				for(j=0;j<9;j++)
				{
						Nt[j%3][(int)(2-floor(j/3.0))] = Et[2]->N[j];
				}
			}
			// Top
			if(tP->EN[0] != NULL)
			{
				for(j=0;j<9;j++)
				{
						Nt[2+j%3][(int)(6-floor(j/3.0))] = tP->EN[0]->N[j];
				}
			}
			// Right
			if(tP->EN[1] != NULL)
			{
				for(j=0;j<9;j++)
				{
					Nt[4+j%3][(int)(4-floor(j/3.0))] = tP->EN[1]->N[j];
				}
			}
			// Bottom
			if(tP->EN[2] != NULL)
			{
				for(j=0;j<9;j++)
				{
					Nt[2+j%3][(int)(2-floor(j/3.0))] = tP->EN[2]->N[j];
				}
			}
			// Left
			if(tP->EN[3] != NULL)
			{
				for(j=0;j<9;j++)
				{
					Nt[j%3][(int)(4-floor(j/3.0))] = tP->EN[3]->N[j];
				}
			}

			// Assign Points from 7x7 array
			for(l=0;l<9;l++)
			{
				for(j=0;j<5;j++)
				{
					for(k=0;k<5;k++)
					{
						tP->N[l]->N25[j][k] = Nt[j+l%3][k+2-(int)(floor(l/3.0))];
					}
				}
			}
			itLevel++;
	    }
	}

	cout << "Initialization: Ghost Averaging" << endl;
	//Set ghost averaging info
	for(i=maxLevel-2; i >= 0; i--)//Cycle through levels to set ghost neigh
	{
	    				
	    itLevel = treeLevels[i].begin();
	    while(itLevel != treeLevels[i].end())//If not empty level
	    {
		tmp = (*itLevel);
		if(tmp->N[1]->me == nodeNULL)
		{
		    tmp->N[1]->NN[0] = tmp->N[0];
		    tmp->N[1]->NN[1] = tmp->N[2];
		    tmp->N[1]->NN[2] = NULL;
		    tmp->N[1]->NN[3] = NULL;
		}
		if(tmp->N[3]->me == nodeNULL)
		{
		    tmp->N[3]->NN[0] = tmp->N[0];
		    tmp->N[3]->NN[1] = tmp->N[6];
		    tmp->N[3]->NN[2] = NULL;
		    tmp->N[3]->NN[3] = NULL;
		}
		if(tmp->N[5]->me == nodeNULL)
		{
		    tmp->N[5]->NN[0] = tmp->N[2];
		    tmp->N[5]->NN[1] = tmp->N[8];
		    tmp->N[5]->NN[2] = NULL;
		    tmp->N[5]->NN[3] = NULL;
		}
		if(tmp->N[7]->me == nodeNULL)
		{
		    tmp->N[7]->NN[0] = tmp->N[6];
		    tmp->N[7]->NN[1] = tmp->N[8];
		    tmp->N[7]->NN[2] = NULL;
		    tmp->N[7]->NN[3] = NULL;
		}
		tmp->N[4]->NN[0] = tmp->N[0];
		tmp->N[4]->NN[1] = tmp->N[2];
		tmp->N[4]->NN[2] = tmp->N[6];
		tmp->N[4]->NN[3] = tmp->N[8];
		itLevel++;
	    }
	}

  	cout << "Initialization: 25 point ghost aquisition" << endl;
	it = nodes->begin();
	while(it != nodes->end())
	{
	    for(i=0;i<4;i++)
	    {
			if((*it)->NN[i] != NULL)
			{
			    if((*it)->NN[i]->me == nodeNULL && (*it)->NN[i]->gme == ghostNULL)
			    {
					ghosts->push_front((*it)->NN[i]);
					(*it)->NN[i]->gme = ghosts->begin();
			    }
			}
	    }
	    
	    //25 point ghost aquisition
	 
	    for(j=0;j<5;j++)
	    {
			for(k=0;k<5;k++)
			{
			    if((*it)->N25[j][k] != NULL)
			    {
					if((*it)->N25[j][k]->me == nodeNULL && (*it)->N25[j][k]->gme == ghostNULL)
					{
					    ghosts->push_front((*it)->N25[j][k]);
						(*it)->N25[j][k]->gme = ghosts->begin();
					}
			    }    
			}
	    }
	    
	    it++;
	}

	cout << "Initialization: Initialize values" << endl;
	for(i=maxLevel-2; i >= 0; i--)//Cycle through levels to initialize
	{
		
	    itLevel = treeLevels[i].begin();
	    while(itLevel != treeLevels[i].end())//If not empty level
	    {
			
			for(j=0;j<=8;j++)
			{
				(*itLevel)->N[j]->initialize();
				
			}	
			itLevel++;
	    }
	}
	
	i = maxLevel -1;
	itLevel = treeLevels[i].begin();
	while(itLevel != treeLevels[i].end())//Cycle through bottom level to initialize
	{
	    for(j=0;j<=8;j+=2)
	    {
			if(j!=4)
			{
			    (*itLevel)->N[j]->initialize();
			}
	    }
	    itLevel++;
	}	
	cout << "Initialization: Done Initialization" << endl;
}

