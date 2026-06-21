#include"CBase.h"
#include"CSort.h"
#include"CEmnt.h"
#include"CHold.h"
#include"CMode.h"
#include"CComp.h"
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
long i, j, n;
//--------------------------------------------------------------------
void
CompInit(long Dn) //UT = 0 RTL default; UT = 1 after DataInit()
{
    if((Rn == Dn) && (UT == 1)) return;
    if((Rn != Dn) && (UT == 1)) DataFree(); 
	// (Rn >= 2 ) is expected,  but rely on external validators
	Rn = Dn; RN = sqrt((double)Rn); LN = Rn * sizeof(double); 
    Ar = 1.0 / 64.0; An = 400;

    Vg = Vgamma(); DataInit();           // https://youtu.be/jaFnirguL54
}//if space dimension is changed, resets all containers and data
//--------------------------------------------------------------------
void
CompFree(void)
{
	if (UT == 1) { DataFree(); UT = 0; } // https://youtu.be/4L_yCwFD6Jo
}
//--------------------------------------------------------------------
void
CompTemp(double GC)
{
	Te = 0.0; Gc = GC; GG = Gc * Gc; GR = GM = 1.0; // Reset time
    //prepare values and constants for given compression stepiing
    Sx = St = Sv->Vc;
    do  // set sized radii Rt as constant initial Rc
    {   // renew dependent constants 
        Si = St->v;Si->Rc = Si->Rt; Si->Mc = Si->Mt;
		Si->Vc = VV = Si->Rt * Gc; Si->Vs = VV * VV;
    }   while ((St = St->n) != Sx);
    Lv = Tv; ListClrV();//clear time items, save items for reuse
    TimeCalcST();//calculate all elements tti, Bn*(Bn-1)/2 complexity!
	Lv = Tv; ListSize();//clear free items, release resources
    //Reset counters. Stepping ready
    Sc = 0; Ce = 0; Cb = 0;
}//init tti for stepping, arguments: sizing speed 
//--------------------------------------------------------------------
void
CompLoad(double KT, double KS, double GC)
{
    Te = 0.0; GR = GM = 1.0; // set time, set constants
    Gc = GC ; GG = Gc * Gc ; kT = KT; Tn = 2 * Bn * Bn;
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    Lv = Ev;  ListClrV(); //move all elements to free buffer Ev->Fc
    Sx = Sv->Vc; St = Sx; //Aling elements number to sorts Si->Bn
    do { Si = St->v; SortLoad(); } while ((St = St->n) != Sx);
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    SetVolume();     //Ve, Me - prepare for bound size solution
    if (KS <= 0.0)   // calculate bound radius Rb
    { Tn = 8 * Bn * Bn;SetPBound(); }  // P-bound
    else
    { Rb = pow(Ve / KS, 1.0 / Rn);  }  // K-bound
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    SetRanges();     // Zero drift ranges, for reporting mainly
    EngPhases();     // Engage phase space, random values {X,V} 
    //printf("\nInitial random state:\n"); GetVolume(); //debug
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    NormMassCenter();// Supress waving, 
    NormImpulse();   // Zero drift
    NormMomenta();   // Supress rotation
    NormEnergy();    // Correct to accqurate
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    
    // Initiate new elements tti, flush time free container Tv->Fc
    Lv = Tv; ListClrV(); TimeCalcST(); ListSize();//squared complexity
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // Flush other free containers, Dv = Dv, Data has own context
    Lv = Sv; ListSize(); Lv = Ev; ListSize(); DataSize();
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// set counters. Stepping & Testing ready.
    Sc = 0; Ce = 0; Cb = 0; Sn = Bn; 
    Ls = Rb * Rb / pow(Bn, 2.0 / Rn);   // mean squared path
    Va = Rn * kT * Bn / Me;             // mean squared speed
    Ta =  sqrt(Ls / Va);                // mean free time
    // use Ls threeshold An = 400, for safe: Delta=(1−Ar​)^An;
}//init tti for stepping: kT, volume density Ks, sizing speed Gc
//--------------------------------------------------------------------
void
CompStep()
{   // * Tests place. After initialisation
    if  (Tv->Vn == 0 || UT == 0) return;//safety, no events to process
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - 1 - 
    //Select minimal tti  from holder Tv
    TimeGetStp();// Tm, ei, ej, dT are set
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - 1 - 
    // * Tests place. Before interaction 
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - = 2 = 
    //Change geometry and variables, if not simultanious 
    if  (dT > 0.0) // values set
    {   // GR, GM, Te += dT in SortGrow
        Sc = 0; EmntMove(); SortGrow(); 
    }   // Ta, Va, Ls - set in section 3: TimeCalcTT
    else// -> impulse loop watchdog, stop on clustering
    {
        Sc++; if (Sc >= Sn) return;
    }
    //Verify position, to prevent time summing errors
    //elements ei, ej interaction, initiate counters Cx, Ce, Cb
    TimeValStp();  EmntColl(); //procced to new time step
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - = 2 = 
    // * Tests place. After interaction  
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - < 3 > 
    //Clear interacted elements tti, Tm move to free container
    //Change tti in time container Tv, Tm values not touched
    TimeDelStp(); if (Sc == 0) TimeDecStp();   
    //Calculate ei, ej elements tti, mean values Ta, Va, Ls
    TimeCalcTT();//ei, ej, dT not actual, but safe to use, Tm reset
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - < 3 >
    // * Tests place. Before interaction

}   //main loop, is called until Sc >= Sn, (or x or t span is covered)
//--------------------------------------------------------------------
// debug
//--------------------------------------------------------------------
void 
CompHold()
{   // * Tests place. After initialisation
    if (Tv->Vn == 0 || UT == 0) return; //safety, no events to process
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - 1 - 
    //Select minimal tti  from holder Tv
    TimeGetStp();// Tm, ei, ej, dT are set

    printf("\n>Step begin:\n\n"); Tx = Tt = Tv->Vc;
    do  // show time holder, Step begin
    {
        Ti = Tt->v; printf("\tdt = %f\tei = %p\tej = %p\n", Ti->dt, Ti->ei, Ti->ej);
    }   while ((Tt = Tt->n) != Tx); if (TestStruct(Tv)) return; // Test list
    // show selected minimal dT
    Ti = Tm->v;   printf("\n\tdt = %f\tei = %p\tej = %p\n", Ti->dt, Ti->ei, Ti->ej);
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - 1 - 
 
    // * Tests place. Before interaction

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - = 2 = 
    //Change geometry and variables, if not simultanious 
    if (dT > 0.0) // values set
    {   // GR, GM, Te += dT in SortGrow
        Sc = 0; EmntMove(); SortGrow();
    }   // Ta, Va, Ls - set in section 3: TimeCalcTT
    else// -> impulse loop watchdog, stop on clustering
    {
        Sc++; if (Sc >= Sn) return; 
    }
    //Verify position, to prevent time summing errors
    //elements ei, ej interaction, initiate counters Cx, Ce, Cb
    TimeValStp();  EmntColl(); //procced to new time step
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - = 2 = 

    // * Tests place. After interaction

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - < 3 > 
    //Clear interacted elements tti, Tm moved to free container
    TimeDelStp();
      
    printf("\n after delete:\n\n"); Tx = Tt = Tv->Vc;
    do  // show time holder, After delete
    {
        Ti = Tt->v; printf("\tdt = %f\tei = %p\tej = %p\n", Ti->dt, Ti->ei, Ti->ej);
    }   while ((Tt = Tt->n) != Tx); if (TestStruct(Tv)) return; // Test list

    //Change tti in time container Tv. Set  ei, ej; mean Ta, Va, Ls     
    if (Sc == 0) TimeDecStp(); TimeCalcTT();  
    //ei, ej, dT not actual, but safe to use, Tm reset in TT
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - < 3 >
    
    // * Tests place. Before interaction

}//main loop steping, direct reporting, for testing and development
//--------------------------------------------------------------------
