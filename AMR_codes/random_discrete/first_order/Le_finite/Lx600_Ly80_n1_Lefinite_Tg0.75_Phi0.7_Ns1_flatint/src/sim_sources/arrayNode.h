#ifndef ARRAYNODEDEF
#define ARRAYNODEDEF

#include <iostream>
#include <vector>
#include <list>
#include "initial.h"

using namespace std;

class arrayNode
{
    public:
        double x,y,dx;
        double Phi[ng],dPdt[ng];
        double C,dCdt;   //concentration flux related arrays

        //Create new variable Ws
        double Ws;


        int NN[4];
        int N25[5][5];
        arrayNode();
        ~arrayNode();
        void updatePhi(int npt);
        void updateC();
};


#endif
