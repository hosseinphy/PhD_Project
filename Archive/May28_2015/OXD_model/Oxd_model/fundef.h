/*Function definition */
/***************************************************************/
void AllocArrays (void);
int  GetNameList (int , char **);
void PrintNameList (FILE *);
void PrintSummary (FILE *);
void front_data();
void Ran_Conc();
void Initialize();
void Dissipation();
void periodic();
void grad();
void source();
unsigned long  Seed_gen();
void zero_flux_boundary();
void conc_boundary();
void Calc_higgins();
void Calc_prov_1();       
void Calc_prov_2(); 
void Calc_prov_3(); 
void update_conc_1();
void update_conc_2();
void update_conc_3();
void Temp_write();
void Conc_write();
void front();
void Dealloc();

int GenerateRandomNumbers(int* result, int lower, int upper, int count);

/***************************************************************/


/*post processing Function definition */
/***************************************************************/
void alloc ();
void dealloc ();
void one_point();
void one_line();
void process(); 
void InitAccels (void);
void InitCoords (void);
void InitVels (void);
void Setup();
void Initial();
//void Init();
void read_data();
void data();
void front_data();
void co_moving();
void Vel_calc();
void Temp_deri_calc();
/***************************************************************/
