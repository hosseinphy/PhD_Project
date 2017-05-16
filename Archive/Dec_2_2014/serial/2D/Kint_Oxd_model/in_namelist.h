
typedef enum {N_I, N_R , N_C} VType;

#define NameI(x)  {#x, &x, N_I, sizeof (x) / sizeof (int)}
#define NameR(x)  {#x, &x, N_R, sizeof (x) / sizeof (double)}
#define NameC(x)  {#x, &x, N_C, sizeof (x) / sizeof (char)}

typedef struct {
  char *vName;
  void *vPtr;
  VType vType;
  int vLen, vStatus;
} NameList;

#define ValI(x)  {&x, N_I, sizeof (x) / sizeof (int)}
#define ValR(x)  {&x, N_R, sizeof (x) / sizeof (double)}
#define ValC(x)  {&x, N_C, sizeof (x) / sizeof (char)}

typedef struct {
  void *vPtr;
  VType vType;
  int vLen;
} ValList;

