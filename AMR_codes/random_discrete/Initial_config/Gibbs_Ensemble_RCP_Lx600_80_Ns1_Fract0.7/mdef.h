#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <iomanip>
#include <limits>
#include <vector>
#include <string>
#include <sstream>
#include <numeric>

#include <algorithm>

typedef double real;

const int numprocs =16 ; 

#define CHAR_MINUS  '-'
#define CHAR_ZERO   '0'


#define AllocMem(a, n, t)  a = (t *) malloc ((n) * sizeof (t))

#define AllocMem2(a, n1, n2, t)                             \
   AllocMem (a, n1, t *);                                   \
   AllocMem (a[0], (n1) * (n2), t);                         \
   for (k = 1; k < n1; k ++) a[k] = a[k - 1] + n2;

#define MAX_MPEX_ORD  2
#define I(i, j)  ((i) * ((i) + 1) / 2 + (j))
#define c(i, j)  c[I(i, j)]
#define s(i, j)  s[I(i, j)]


#include "in_vdefs.h"
//#include "in_namelist.h"
//#include "in_proto.h"

#define DO_MOL  for (n = 0; n < N; n ++)
#define DO_CELL(j, m)  for (j = cellList[m]; j >= 0; j = cellList[j])

#define VWrap(v, t)                                         \
   if (v.t >= 0.5 * region.t)      v.t -= region.t;         \
   else if (v.t < -0.5 * region.t) v.t += region.t

#define VShift(v, t)                                        \
   if (v.t >= 0.5 * region.t)      shift.t -= region.t;     \
   else if (v.t < -0.5 * region.t) shift.t += region.t

#define VShiftWrap(v, t)                                    \
   if (v.t >= 0.5 * region.t) {                             \
     shift.t -= region.t;                                   \
     v.t -= region.t;                                       \
   } else if (v.t < -0.5 * region.t) {                      \
     shift.t += region.t;                                   \
     v.t += region.t;                                       \
   }

#define VCellWrap(t)                                        \
   if (m2v.t >= cells.t) {                                  \
     m2v.t = 0;                                             \
     shift.t = region.t;                                    \
   } else if (m2v.t < 0) {                                  \
     m2v.t = cells.t - 1;                                   \
     shift.t = - region.t;                                  \
   }

#if NDIM == 2

#define VWrapAll(v)                                         \
   {VWrap (v, x);                                           \
   VWrap (v, y);}
#define VShiftAll(v)                                        \
   {VShift (v, x);                                          \
   VShift (v, y);}
#define VCellWrapAll()                                      \
   {VCellWrap (x);                                          \
   VCellWrap (y);}
#define OFFSET_VALS                                         \
   {{0,0}, {1,0}, {1,1}, {0,1}, {-1,1}}
#define N_OFFSET  5

#endif

#if NDIM == 3

#define VWrapAll(v)                                         \
   {VWrap (v, x);                                           \
   VWrap (v, y);                                            \
   VWrap (v, z);}
#define VShiftAll(v)                                        \
   {VShift (v, x);                                          \
   VShift (v, y);                                           \
   VShift (v, z);}
#define VCellWrapAll()                                      \
   {VCellWrap (x);                                          \
   VCellWrap (y);                                           \
   VCellWrap (z);}

#define OFFSET_VALS                                           \
   { {0,0,0}, {1,0,0}, {1,1,0}, {0,1,0}, {-1,1,0},            \
     {0,0,1}, {1,0,1}, {1,1,1}, {0,1,1}, {-1,1,1}, {-1,0,1},  \
     {-1,-1,1}, {0,-1,1}, {1,-1,1}                            \
   }

#define N_OFFSET  14

#endif

/*
#define MOL_LIMIT  10000000

#define NameVal(x)                                          \
   if (! strncmp (bp, #x, strlen (#x))) {                   \
     bp += strlen (#x);                                     \
     x = strtod (bp, &bp);                                  \
   }

#define NHIST  (NDIM + 2)

#define ReadF(x)       fread  (&x, sizeof (x), 1, fp)
#define WriteF(x)      fwrite (&x, sizeof (x), 1, fp)
#define ReadFN(x, n)   fread  (x, sizeof (x[0]), n, fp)
#define WriteFN(x, n)  fwrite (x, sizeof (x[0]), n, fp)
								
enum {FL_CHECKA, FL_CHECKB, FL_CKLAST, FL_SNAP};
char *fileNameR[] = {"xxnnchecka.data", "xxnncheckb.data",
   "xxnncklast.data", "xxnnsnap.data"}, fileName[5][20];

char *progId = "md";

enum {ERR_NONE, ERR_BOND_SNAPPED, ERR_CHECKPT_READ, ERR_CHECKPT_WRITE,
   ERR_COPY_BUFF_FULL, ERR_EMPTY_EVPOOL, ERR_MSG_BUFF_FULL,
   ERR_OUTSIDE_REGION, ERR_SNAP_READ, ERR_SNAP_WRITE,
   ERR_SUBDIV_UNFIN, ERR_TOO_MANY_CELLS, ERR_TOO_MANY_COPIES,
   ERR_TOO_MANY_LAYERS, ERR_TOO_MANY_LEVELS, ERR_TOO_MANY_MOLS,
   ERR_TOO_MANY_MOVES, ERR_TOO_MANY_NEBRS, ERR_TOO_MANY_REPLICAS};

char *errorMsg[] = {"", "bond snapped", "read checkpoint data",
   "write checkpoint data", "copy buffer full", "empty event pool",
   "message buffer full", "outside region", "read snap data",
   "write snap data", "subdivision unfinished", "too many cells",
   "too many copied mols", "too many layers", "too many levels",
   "too many mols", "too many moved mols", "too many neighbors",
   "too many replicas"};
*/
