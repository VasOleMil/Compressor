#include"CBase.h"
#include"CList.h"
#include"CSort.h"
#include"CEmnt.h"
#include"CHold.h"
#include"CComp.h"
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
long k, n;
//--------------------------------------------------------------------
void
CompInit(long Dn) //UT = 0 RTL default; UT = 1 after DataInit()
{
    if (Rn != Dn)  if (UT == 1) DataFree(); Rn = Dn;
	// (Rn >= 2 )  is expected, but not tested
    RN = sqrt((double)Rn); LN = Rn * sizeof(double);

    DataInit();
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

    St = Sx = Sv->Vc; //set sized radii Rt as constant initial Rc
    while ((St = St->n) != Sx)
    {
        Si = St->v; Si->Vc = Si->Rt * Gc;
        Si->Rc = Si->Rt; Si->Mc = Si->Mt;      
    }
	Lv = Tv->Vc; ListClr(); //clear time list, save items for reuse
    TimeCalcST(); //calculate all elements tti, for stepping
	Lv = Tv->Vc; ListSize();//clear free items, release resources
}//init tti for stepping, arguments: sizing speed
//--------------------------------------------------------------------
void
CompLoad(double KT, double KS, double GC)
{
    Te = 0.0; Gc = GC; GG = Gc * Gc; GR = GM = 1.0; //reset time
    
    Sn = 100; Sc = 0; Vg = Vgamma(); Te = 0.0; GR = GM = 1.0;
    
    if (KS <= 0.0)
    {   SetGBound(); }
    else
    {   SetVolume(); Rb = pow(Ve / KS, 1.0 / Rn); }
    
    De = Rb * RN * 8e-10; Ds = Rb * RN * 1e-12;

    /* yet a lot to implement in random initializatio */
    
	TimeCalcST(); //finally initiate all elements tti
}//init tti for stepping for kT, volume density,  sizing speed
//--------------------------------------------------------------------
void
CompStep()
{
	if (Tv->Vn == 0) return; //safety, no events to process
    
    //Select minimal tti  from holder Tv
	TimeGetStp();// Tm, ei, ej, dT are set
    
    //mean free time and path tests, before interaction
    
    //Change geometry and variables, if not simultanious 
    if (dT > 0.0) //Te += dT in SortGrow
    { Sc = 0; EmntMove(); SortGrow(); }
    else //check impulse loop, stop processing
    { Sc++  ; if (Sc >= Sn) return;   }

    //if Gv(Y), verify position, to prevent time summing errors 
	TimeValStp(); //initiate counters Cx, Ce, Cb
    
    //Elements ei or ej interaction, 
    EmntColl(); //procced to new time calculation

	//mean free time and path tests, after interaction
    
    //Clear interacted elements tti in Tv
    TimeDelStp();//Tm in free container
    
    //Change tti in Tv, Tm not touched
    if (Sc == 0) TimeDecStp();
    
    //Calculate ei, ej elements tti
    TimeCalcTT();// ei, ej, dT safe
    //Tm is overwritted and reused
}//main loop, is called until Sc >= Sn, or x or t span is covered
//--------------------------------------------------------------------


//--------------------------------------------------------------------
static double
Vgamma(void)
{
    double Pk, Pi; long i, k, d, Rh;

    k = 1; d = 2; Pk = 1.0; Pi = 3.141592653589793;

    if (Rn % 2 == 0)
    {
        for (i = 1, Rh = (Rn - 0) / 2; i <= Rh; i++)
        {
            Pk *= Pi; k *= i;
        }

        return Pk / k;
    }
    else
    {
        for (i = 1, Rh = (Rn - 1) / 2; i <= Rh; i++)
        {
            Pk *= Pi;  d *= 2; k *= 2 * i + 1;
        }

        return Pk * d / k;
    }
}//Multiplier for volume of Rn-sphere:Vb = Vg * Rb^Rn, Vg = Vgamma(Rn)
//--------------------------------------------------------------------
static void
SetGBound(void)
{
    //get current Bn and Average radius, P = 1/rr = 0.5
    RA = 0.0; Bn = 0; St = Sx = Sv->Vc; rr = 2.0;
    do
    { 
        Si = St->v; Bn += n = Si->Bn; RA += n * Si->Rc;
    }   while ((St = St->n) != Sx);
	RA /= (double)Bn;
	//Set radius for even gemetrical probality P = 0.5
    //intersect or not. Simultanois drop, all elements.
    Rb = RA * (1.0 + 2.0 * pow(
        (1.0 - pow(rr, -2.0 / (Bn * (Bn - 1)))),
		-1.0 / Rn));//provides easy initial random drop 
}//Set radius for even interaction probality
//--------------------------------------------------------------------
static void
SetVolume(void)
{
	RV = 0.0; Bn = 0; St = Sx = Sv->Vc; //Bn calculated with volume
    do
    {
        Si = St->v; Ri = Si->Rc; Bn += Si->Bn;
        RR = Vg * Ri; for (k = 1; k < Rn; k++) RR *= Ri;
		RV += RR * Si->Bn;
    }   while ((St = St->n) != Sx); Ve = RV;
}//Set summary elements volume, Ks - volume density, Ks = GM * Ve / Vb
//--------------------------------------------------------------------
    