

void AccumBondAngDistn (int);
void AccumDiffusion (void);
void AccumDihedAngDistn (int);

void AdjustInitTemp (void);
void AccumProps (int);

void AccumSpacetimeCorr (void);
void AccumVacf (void);
void AddBondedPair (int, int);
void AdjustDipole (void);
void AdjustLinkAngles (void);
void AdjustPressure (void);


void AdjustTemp (void);
void AllocArrays (void);

void AnalClusterSize (void);
void AnalVorPoly (void);
void AnlzConstraintDevs (void);
void ApplyBarostat (void);

void ApplyBoundaryCond (void);
void *ApplyBoundaryCondT (void *);
void ApplyThermostat (void);

void ApplyWallBoundaryCond (void);

void AssignMpCells (void);
void AssignToChain (void);
void BisectPlane (void);
void BuildClusters (void);
void BuildConstraintMatrix (void);
void BuildIntTree (void);
void BuildLinkInertiaMats (void);
void BuildLinkMmat (real *, int);
void BuildLinkPhimatT (real *, int);

void BuildLinkXYvecs (int);

void BuildNebrList (void);
void *BuildNebrListT (void *);


void ComputeForces ();
void ErrExit (int);


void EvalProps (void);

void EvalRdf (void);
void EvalSinCos (void);
void EvalSpacetimeCorr (void);
void EvalVacf (void);
void EvalVelDist (void);

void FindDistVerts (void);
void FindTestSites (int);
void GatherWellSepLo (void);
void GenSiteCoords (void);
void GetCheckpoint (void);
int  GetConfig (void);
int  GetGridAverage (void);
int  GetNameList (int, char **);
void GridAverage (int);
void InitAccels (void);
void InitAngAccels (void);
void InitAngCoords (void);
void InitAngVels (void);
void InitBoxVars (void);
void InitCharges (void);
void InitClusters (void);
void InitCoords (void);
void InitCoordsWalls (real);
void InitDiffusion (void);
void InitEventList (void);
void InitFeedbackVars (void);
void InitFreePath (void);
void InitLinkState (void);
void InitPairEng (void);
void InitRand (int);
void InitSlaves (void);
void InitSpacetimeCorr (void);
void InitState (void);
void InitVacf (void);
void InitVels (void);
void InitVorPoly (void);
real Integrate (real *, int);
void LeapfrogStep (int);
void LeapfrogStepLinks (int);
void *LeapfrogStepT (void *);
void LocateIntTreeCellCm (void);
void MeasureTrajDev (void);
void MulMat (real *, real *, real *, int);
void MulMatVec (real *, real *, real *, int);
void MultipoleCalc (void);
void NebrParlProcs (void);
void NextEvent (void);
void PackCopiedData (int, int, int *, int);
void PackMovedData (int, int, int *, int);

void PerturbCoords (void);
void PerturbTrajDev (void);
void PolyGeometry (void);
void PolySize (void);
void PredictEvent (int, int);
void PredictorStep (void);
void PredictorStepBox (void);
void PredictorStepPT (void);
void PredictorStepQ (void);
void PredictorStepS (void);
void PrintChainProps (FILE *);
void PrintDiffusion (FILE *);
void PrintDihedAngCorr (FILE *);
void PrintFreePath (FILE *);
void PrintHelp (char *);
void PrintNameList (FILE *);
void PrintPairEng (FILE *);
void PrintProfile (FILE *);
void PrintRdf (FILE *);
void PrintSpacetimeCorr (FILE *);
void PrintSummary (FILE *);
void PrintFile (FILE *);
void PrintTrajDev (FILE *);
void PrintVacf (FILE *);
void PrintVelDist (FILE *);
void ProcCutEdges (void);
void ProcCutFaces (void);
void ProcDelVerts (void);
void ProcessCellCrossing (void);
void ProcessCollision (void);
void ProcInterrupt ();
void PropagateCellLo (void);
void ProcNewFace (void);
void ProcNewVerts (void);
void PutCheckpoint (void);
void PutConfig (void);
void PutPlotData (void);
void PutGridAverage (void);
real RandR (void);
void RemoveOld (void);
void RepackMolArray (void);
void ReplicateMols (void);
void RestoreConstraints (void);
void ScaleCoords (void);
void ScaleVels (void);
void ScanIntTree (void);
void ScheduleEvent (int, int, real);
void SetMolType (void);
void SetBase (void);
void SetCellSize (void);
void SetMolSizes (void);
void SetParams (void);
void SetupFiles (void);
void SetupInterrupt (void);
void SetupJob (void);

void SingleEvent (void);
void SingleStep (void);


void VRand (VecR *);


