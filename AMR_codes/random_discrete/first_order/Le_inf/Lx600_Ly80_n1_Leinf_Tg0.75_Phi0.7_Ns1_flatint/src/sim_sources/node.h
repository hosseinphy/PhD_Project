#ifndef NODEDEF
#define NODEDEF

#include <iostream>
#include <list>

using namespace std;

class node
{
    public:
        double x,y,C,Ws,*Phi,dx;

        int index;
        node *NN[4];
        node *N25[5][5];
        list<node *>::iterator me;
        list<node *>::iterator gme;

        node(double xi, double yi,list<node *>::iterator n, list<node *>::iterator g);
    	~node();
        void average(node *N1, node *N2);
        void average(node *N1, node *N2,node *N3, node *N4);
        void initialize();
        void setNN(int i, node *n);
};
#endif
