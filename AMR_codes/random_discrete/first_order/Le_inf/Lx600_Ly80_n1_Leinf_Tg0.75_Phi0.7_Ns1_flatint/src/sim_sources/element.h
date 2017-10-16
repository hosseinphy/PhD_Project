#ifndef ElementDEF
#define ElementDEF

#include <iostream>
#include <cmath>
#include "node.h"

using namespace std;

class treenode
{
  public:
    int num,level;
    treenode *TN[4];   //children
  	treenode *P;       //parent
  	node *N[9];        //9 nodes/ghostnodes of the element
  	treenode *EN[4];   //Element Neighbours
  	list<treenode *>::iterator me;
     
    treenode(treenode *parent,int i, int lvl);
    
    /****************CODE IN ELEMENT.CPP***************/
    void split(int maxlevel,list<node *>::iterator n, list<node *>::iterator g);
    void unsplit(int maxLevel);
    int haveChildren();
    int haveGrandchild(int i);
    int neighChildren(int i);
    int neighGrandChild();
    int noChildren();
    void setEN(int i, treenode *e);
    void setN(int i, node *n);
    /*************************************************************/

    int pad();
    int checkSplit();
    int checkinit();
};
#endif
