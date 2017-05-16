#include<iomanip>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <signal.h>
#include <sys/time.h>
#include <assert.h>
#include <string.h>
#include "in_namelist.h"
#include <unistd.h>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <map>
#include "random.cpp"





/////////////////////////////////////////////

//Math Function Definitions

#define Max(x1, x2) (((x1) >(x2)) ? (x1) : (x2))
		                       
#define Sqr(x)  ((x)*(x))

#define Cube(x) ((x)*(x)*(x))

#define Rev(x)  (1./x)
//------------------------Statics--------------------------
#define PropZero(v)  v.sum = v.sum2 = 0.
#define PropEval(v)  v.sum += v.val, v.sum2 += Sqr (v.val)
#define PropAvg(v, n) \
   v.sum /= n, v.sum2 = sqrt (Max (v.sum2 / n - Sqr (v.sum), 0.))

#define PropEst(v)  v.sum, v.sum2

//----------------------Allocation------------------------
#define AllocMem(a ,n, t)    a = (t *) malloc ((n) * sizeof(t))
#define CallocMem(a ,n, t)   a = (t *) calloc (n,sizeof(t))



/*********************/

