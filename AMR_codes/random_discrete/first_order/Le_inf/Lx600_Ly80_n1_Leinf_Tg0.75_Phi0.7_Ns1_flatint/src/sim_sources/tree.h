/*
This Code is the intellectual Property of Michael Greenwood.  
I am making this code free to use, and only ask that you acknowledge 
the work that I've done in any publications that come from the use of this code.
Thanks

Michael Greenwood
*/

#ifndef TreeDEF
#define TreeDEF

#include <vector>
#include <list>
#include <stack>
#include "node.h"
#include "element.h"


class tree
{
	int maxLevel;
	treenode *root;
	stack<treenode *> splitStack;
	list<treenode *> *treeLevels; //0 being root level, N being the lowest level of refinement
	list<treenode *>::iterator itLevel;
	list<treenode *>::iterator itLevel2;
	list<node *>::iterator nodeNULL;
	list<node *>::iterator ghostNULL;

	public:
		/******************* Element Array Initialization ************************************/
		tree(treenode ***initElement0,treenode ***initElement1, node ***initNodes,list<node*> *nodes,list<node*> *ghosts);
		tree(treenode ***initElement0, node ***initNodes,list<node*> *nodes,list<node*> *ghosts);
		/******** Single Domain Initialization ************/
		tree(treenode **Begin_Element,int *element_level,int elementsize,int init_level, int max_level,list<node*> *nodes,list<node*> *ghosts);
		int getElementNum();
		void setdx(double w);
		void regrid(list<node*> *nodes, list<node*> *ghosts);
		int padGrid(int n,list<node*> *nodes);
		int splitList(list<node*> *nodes);
		int unsplitList(list<node*> *nodes);
		int splitListUpdate(list<node*> *nodes);
		int unsplitListUpdate(list<node*> *nodes);
		void initialize(list<node*> *nodes, list<node*> *ghosts);
};
			
#endif
