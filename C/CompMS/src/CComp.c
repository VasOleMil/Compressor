#include"CBase.h"
#include"CList.h"
#include"CSort.h"
#include"CEmnt.h"
#include"CHold.h"
#include"CComp.h"
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
long i, j, k, n;
//--------------------------------------------------------------------
void
CompInit(long Dn) //UT = 0 RTL default; UT = 1 after DataInit()
{
    if (Rn != Dn)  if (UT == 1) DataFree(); Rn = Dn;
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
    static double Pk, Pi; static long i, n, d, Rh;

    n = 1; d = 2; Pk = 1.0; Pi = 3.141592653589793;

    if (Rn % 2 == 0)
    {
        for (i = 1, Rh = (Rn - 0) / 2; i <= Rh; i++)
        {
            Pk *= Pi; n *= i; // n! factorial
        }

        return Pk * 1 / n;
    }
    else
    {
        for (i = 1, Rh = (Rn - 1) / 2; i <= Rh; i++)
        {
            Pk *= Pi; n *= 2 * i + 1; d *= 2;
        }

        return Pk * d / n;
    }
}//Multiplier for Rn-sphere volume: Vb = Vg * Rb^Rn, Vg = Vgamma(Rn)
//--------------------------------------------------------------------
static void
SetGBound(void)
{
    //Weighted Average radius (RA) over all elements.
    RA = 0.0; Bn = 0; Sx = St = Sv->Vc;
    do
    { 
        Si = St->v; Bn += n = Si->Bn; RA += n * Si->Rc;
    }   while ((St = St->n) != Sx);
	RA /= (double)Bn; rr = 0.5;  // P(move) = rr = 0.5
	//Set radius for even gemetrical probality P = 1/2
    //intersect or move freely. Simultanious drop, all elements.
    Rb = RA * (1.0 + 2.0 * pow(
        (1.0 - pow(rr, 2.0 / (Bn * (Bn - 1)))),
		-1.0 / Rn));//provides easy initial random drop 
}//Set radius for even interaction/move probality
//--------------------------------------------------------------------
static void
SetVolume(void)
{
	RV = 0.0; Bn = 0; Sx = St = Sv->Vc; // Bn calculated with volume
    do
    {
        Si = St->v; Bn += n = Si->Bn; Ri = Si->Rc; RR = Vg * Ri;
        for (k = 1; k < Rn; k++) { RR *= Ri; };    RV += RR * n;
	} while ((St = St->n) != Sx); 
    // Ks - volume density, Ks = GM * Ve / Vb
    Ve = RV;       // GM = (1.0 + Gc * Te)^Rn
}//Set summary elements volume Ve based on Rc, use GM to get current
//--------------------------------------------------------------------
static void 
SetRanges(void)
{
    static double e; static long m; //Precision adaptation:

    e = 1.0; m = 0; while (1.0 < 1.0 + e) { e /= 10.0; m++; }
	Fm = m; //Mantisse test, decimal digits: 1.0 + 10^-Fm == 1.0

    e = 1.0; m = 0; while (0.0 < 0.0 + e) { e /= 10.0; m++; }
	Fe = m; //Exponent test, decimal power : 0.0 + 10^-Fe == 0.0
    //De should be greater without poition verifier, De *= Bn
	//ranging not implemented, currently for reporting only
    e = pow(10.0, -Fm); Ds = -(Rb * RN * e); De = 2e1 * Ds;
}//Zero drift range: (-De;+De) as dT = 0.0; (-Ds;+Ds) as rv - RV = 0.0
//--------------------------------------------------------------------
static void 
RandomSphere(int Rn, double Rb, double* X) 
{
    static double s = 0.707106781186547;// s = 1.0 / sqrt(2.0)
    
    RR = 0.0; rr = 2.0 / (double)RAND_MAX;
    // pick one index coordinate belong to cube face
    i = rand() % Rn;  
	//generate others coordinates, uniformly distributed
    for (k = 0; k < Rn; k++) 
    {
        if (k != i) { rk = X[k] = (double)rand() * rr - 1.0; }
        else        { rk = X[k] = (rand() % 2) ? 1.0 : -1.0; }
        RR += rk * rk;
    }
    // project cube face point to sphere surface
    rr = sqrt(RR); for (k = 0; k < Rn; k++) { X[k] /= rr; }
	// random rotations of coordinates, to smooth distribution
    for (k = 0; k < Rn; k++) 
    {
        i = rand() % Rn; do { j = rand() % Rn; } while (i == j);
        
        Ri = X[i];
        Rj = X[j];
        X[i] = s * (Rj - Ri);
        X[j] = s * (Rj + Ri);
    }
    // scale to bound radius
    for (k = 0; k < Rn; k++) { X[k] *= Rb; }
} // Generate random point on Rn-sphere of radius Rb
//--------------------------------------------------------------------


    