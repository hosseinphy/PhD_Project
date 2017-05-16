#include<iostream>
#include"fundef.h"
#include<iomanip>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <signal.h>
#include <sys/time.h>
#include <string>
#include <sstream>
#include <assert.h>
#include <unistd.h>
#include <limits>



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


//number of max Molecules
  # define nMol_limit  10000
  # define size 220
  #define NDIM 3
  #define ntab 10000
  #define npmax 7000
  #define nlmax 60000
/*********************/

