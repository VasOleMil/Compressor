#include"CBase.h"
#include"CList.h"
#include"CSort.h"
#include"CEmnt.h"
#include"CHold.h"
#include"CMode.h"
#include"CComp.h"
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
long i, j, k, n;

//--------------------------------------------------------------------
void
CompInit(long Dn) //UT = 0 RTL default; UT = 1 after DataInit()
{
    if((Rn != Dn) && (UT == 1)) DataFree(); Rn = Dn;
	// (Rn >= 2 )  is expected, but not tested
    RN = sqrt((double)Rn); LN = Rn * sizeof(double);

    DataInit();                          // https://youtu.be/jaFnirguL54
}//resets all containers and data, if space dimension is changed
//--------------------------------------------------------------------
void
CompFree(void)
{
	if (UT == 1) { DataFree(); UT = 0; } // https://youtu.be/4L_yCwFD6Jo
}
//--------------------------------------------------------------------
void
CompLoad(double GC)
{
	Te = 0.0; Gc = GC; GG = Gc * Gc; GR = GM = 1.0; //reset time

    Sx = St = Sv->Vc; //set sized radii Rt as constant initial Rc
    while ((St = St->n) != Sx)
    {
        Si = St->v; Si->Vc = Si->Rt * Gc;
        Si->Rc = Si->Rt; Si->Mc = Si->Mt;      
    }
	Lv = Tv->Vc; ListClrV(); //clear time list, save items for reuse
    TimeCalcST(); //calculate all elements tti, for stepping
	Lv = Tv->Vc; ListSize();//clear free items, release resources
}//init tti for stepping, arguments: sizing speed
//--------------------------------------------------------------------
void
CompLoad(double KT, double KS, double GC)
{
    Te = 0.0; Gc = GC; GG = Gc * Gc; GR = GM = 1.0; //reset time
    
    Sn = 100; Sc = 0; Vg = Vgamma(); 
    
    if (KS <= 0.0)
    {   SetGBound(); }
    else
    {   SetVolume(); Rb = pow(Ve / KS, 1.0 / Rn); }
    
    SetRanges();

    /* yet a lot to implement in random initialization */
    
	TimeCalcST(); //finally initiate all elements tti
}//init tti for stepping for kT, volume density,  sizing speed
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
    { Sc = 0; EmntMove(); SortGrow(); } //globals refresh
    else //impulse loop watchdog, stop on clustering
    { Sc++  ; if (Sc >= Sn) return;   }

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





    