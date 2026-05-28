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
	Te = 0.0; Gc = GC; GG = Gc * Gc; GR = GM = 1.0; //Reset time

    Sx = St = Sv->Vc; //set sized radii Rt as constant initial Rc
    while ((St = St->n) != Sx)
    {
        Si = St->v; Si->Vc = Si->Rt * Gc;
        Si->Rc = Si->Rt; Si->Mc = Si->Mt;      
    }
    Lv = Tv; ListClrV();//clear time items, save items for reuse
    TimeCalcST();//calculate all elements tti, Bn*(Bn-1)/2 complexity!
	Lv = Tv; ListSize();//clear free items, release resources
    //Reset counters. Stepping ready
    Sc = 0; Ce = 0; Cb = 0;
}//init tti for stepping, arguments: sizing speed, 
//--------------------------------------------------------------------
void
CompLoad(double KT, double KS, double GC)
{
    Te = 0.0; GR = GM = 1.0; //set time, set constants
    Gc = GC ; GG = Gc * Gc ; kT = KT; 
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
    EngPhases();     // Engage phase space, random values {X,V} 

    SetRanges();     // Zero drift ranges, for reporting mainly
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
}//init tti for stepping: kT, volume density Ks, sizing speed Gc
//--------------------------------------------------------------------
void
CompStep()
{
    if (Tv->Vn == 0 || UT == 0) return; //safety, no events to process

    //Select minimal tti  from holder Tv
    TimeGetStp();// Tm, ei, ej, dT are set

    //mean free time and path tests. Before interaction

    //Change geometry and variables, if not simultanious 
    if (dT > 0.0) //Time change: Te += dT in SortGrow
    {
        Sc = 0; EmntMove(); SortGrow();
    } //globals refresh
    else //impulse loop watchdog, stop on clustering
    {
        Sc++; if (Sc >= Sn) return;
    }

    //if Gv(Y), verify position, to prevent time summing errors
    //elements ei, ej interaction, initiate counters Cx, Ce, Cb
    TimeValStp();  EmntColl(); //procced to new time step

    //mean free time and path tests. After interaction

    //Clear interacted elements tti in Tv
    TimeDelStp();//Tm in free container

    //Change tti in time container Tv, Tm values not touched
    if (Sc == 0) TimeDecStp();

    //Calculate ei, ej elements tti, Tm is overwritted and reused
    TimeCalcTT();//ei, ej, dT not actual, but safe to use

    //mean free time and path tests. Before interaction

}//main loop, is called until Sc >= Sn, or x or t span is covered
//--------------------------------------------------------------------
//#include<stdio.h>
//static int
//StructTestComp(void)
//{
//    int a, i, j, t; a = 0;
//
//    Tx = Tv->Vc; if ((t = Tv->Vn) >= 1)
//    {
//        Tt = Tx; i = j = 0; do { i++; } while ((Tt = Tt->n) != Tx);
//        if (i != t) { a++; j++; }
//        if (j > 0) printf("\nEnumeration n Vn FAIL");
//        Tt = Tx; i = j = 0; do { i++; } while ((Tt = Tt->p) != Tx);
//        if (i != t) { a++; j++; }
//        if (j > 0) printf("\nEnumeration p Vn FAIL");
//    }
//    Tx = Tv->Fc; if ((t = Tv->Fn) >= 1)
//    {
//        Tt = Tx; i = j = 0; do { i++; } while ((Tt = Tt->n) != Tx);
//        if (i != t) { a++; j++; }
//        if (j > 0) printf("\nEnumeration n Fn FAIL");
//        Tt = Tx; i = j = 0; do { i++; } while ((Tt = Tt->p) != Tx);
//        if (i != t) { a++; j++; }
//        if (j > 0) printf("\nEnumeration p Fn FAIL");
//    }
//
//    if (a > 0)
//    {
//        printf("\nError on step:structure"); return 1;
//    }
//    {
//        return 0;
//    }
//}
//--------------------------------------------------------------------
//void //Debug version with direct reporting, for testing and development
//CompStep()
//{
//    if (Tv->Vn == 0 || UT == 0) return; //safety, no events to process
//
//    printf("\n Step begin:\n\n"); Tx = Tt = Tv->Vc;
//    do
//    {
//        Ti = Tt->v; printf("\tdt = %f\tei = %p\tej = %p\n", Ti->dt, Ti->ei, Ti->ej);
//    } while ((Tt = Tt->n) != Tx);
//
//    if (StructTestComp()) return;
//
//    //Select minimal tti  from holder Tv
//    TimeGetStp();// Tm, ei, ej, dT are set
//
//    Ti = Tm->v; printf("\n\tdt = %f\tei = %p\tej = %p\n", Ti->dt, Ti->ei, Ti->ej);
//
//    if (StructTestComp()) return;
//
//    //mean free time and path tests. Before interaction
//
//    // Refresh geometry and globals, if not simultanious 
//    if (dT > 0.0) // time change: Te += dT in SortGrow
//    {
//        Sc = 0; EmntMove(); SortGrow();
//    }
//    else // impulse loop watchdog, stop on clustering
//    {
//        Sc++; if (Sc >= Sn) return;
//    }
//
//    //if Gv(Y), verify position, to prevent time summing errors
//    //elements ei, ej interaction, initiate counters Cx, Ce, Cb
//    TimeValStp();  EmntColl(); //procced to new time step
//
//    //mean free time and path tests. After interaction
//
//    //Clear interacted elements tti in Tv
//    TimeDelStp();//Tm in free container
//
//    printf("\n After delete:\n\n"); Tx = Tt = Tv->Vc;
//    do
//    {
//        Ti = Tt->v; printf("\tdt = %f\tei = %p\tej = %p\n", Ti->dt, Ti->ei, Ti->ej);
//    } while ((Tt = Tt->n) != Tx);
//
//    if (StructTestComp()) return;
//
//    //Change tti in time container Tv, Tm values not touched
//    if (Sc == 0) TimeDecStp();
//
//    //Calculate ei, ej elements tti, Tm is overwritted and reused
//    TimeCalcTT();//ei, ej, dT not actual, but safe to use
//
//    printf("\n After update:\n\n"); Tx = Tt = Tv->Vc;
//    do
//    {
//        Ti = Tt->v; printf("\tdt = %f\tei = %p\tej = %p\n", Ti->dt, Ti->ei, Ti->ej);
//    } while ((Tt = Tt->n) != Tx);
//
//    if (StructTestComp()) return;
//
//}//main loop, is called until Sc >= Sn, or x or t span is covered
//--------------------------------------------------------------------





    