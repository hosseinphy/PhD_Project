/*Function definition */
/***************************************************************/
void AllocArrays (void);
int  GetNameList (int , char **);
void PrintNameList (FILE *);
void PrintSummary (FILE *);
void front_data();
void Ran_Conc();
void initialize();
void array_Init();
void Dissipation();
void periodic();
void Mixed();
void Mixed_oxg();
void grad();
void source();
void zero_flux_boundary();
void conc_boundary();
void Calc_higgins();
void Calc_prov_1();       
void Calc_prov_2(); 
void Calc_prov_3(); 
void update_conc();
void temp_write();
void Conc_write();
int  mpi_setup();
void mpi_initialize();
void proc_exchange();
void temp_proc_exchange();
void conc_proc_exchange();
void kinetic();
void oxidation();
void restart(int);
void set_file(int);
void front();
void Dealloc();

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
void read_data();
void data();
void front_data();
void co_moving();
double Vel_calc(int *id);
/***************************************************************/
