#include "arrayNode.h"


	arrayNode::arrayNode()
	{
		int ii;
		C = 1.;
		Ws = 0.0;
		for (ii=0;ii<ng;ii++)
		{
			Phi[ii] = -1.0;
			dPdt[ii] = 0.0;
		}
	}
	arrayNode::~arrayNode()
	{
		
	}
	void arrayNode::updatePhi(int npt)
    {
		int ii;

		for(ii=0;ii<npt;ii++)
			Phi[ii] = Phi[ii] + dPdt[ii]*dt;		
    }
	void arrayNode::updateC()
    {
		C = C + dCdt*dt;
    }
