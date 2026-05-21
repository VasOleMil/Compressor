#include"CBase.h"
#include"CSort.h"
#include"CEmnt.h"
#include"CMode.h"
//--------------------------------------------------------------------
CDList *Mv;//Modes resources container
//--------------------------------------------------------------------
long    k, n, i, j, rn; double *X;
//--------------------------------------------------------------------
void
ModeInit(void)
{
	if (Mv != NULL) return; rn = Rn + 2;

    X = (double*)Malloc(sizeof(double), rn, UD);
    Mv = (CDList*)X; //non zero value to mark initialization, ModeFree
   
}//Inits start values in Mode container.                        Set: X
//--------------------------------------------------------------------
void
ModeFree(void)
{
    if (Mv == NULL) return;
    free(X); X = NULL; Mv = NULL;
}//Releases Free resources  
//--------------------------------------------------------------------
double
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
void
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
void
SetVolume(void)
{
    RV = VV = 0.0; Bn = 0; Sx = St = Sv->Vc;//Bn refreshed with volume
    do
    {
		Si = St->v; Bn += n = Si->Bn; Ri = Si->Rc;
        Mi = Si->Mc;  VV += Mi * n;  RR = Vg * Ri;
        for (k = 1; k < Rn; k++) RR *= Ri; RV += RR * n;
	} while ((St = St->n) != Sx); 
    // Ks - volume density, Ks = GM * Ve / Vb, GM = (1.0 + Gc * Te)^Rn
    Ve = RV; Me = VV; // 
}//Summary elements volume Ve & Me based on Rc, Ve *=GM to get current
//--------------------------------------------------------------------
void 
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
RandomSphere(void)
{
    RR = 0.0; //rr = 2.0 / (double)RAND_MAX; //rn = Rn + 2
    // pick one index coordinate, will belong to cube face
    i = rand() % rn;
    // generate others coordinates, uniformly distributed
    for (k = 0; k < rn; k++)
    {
        if (k != i) { rk = X[k] = (double)rand() * rr - 1.0; }
        else { rk = X[k] = (rand() % 2) ? 1.0 : -1.0; }
        RR += rk * rk;
    }
    // project cube face point to, RA = Rb - Ri scaled, sphere surface
    RR = RA / sqrt(RR); for (k = 0; k < rn; k++) { X[k] *= RR; }
    // random rotations of coordinates, to smooth distribution
    for (k = 0; k < rn; k++)
    {
        i = rand() % rn; do  j = rand() % rn;  while (i == j);

        Ri = X[i]; Rj = X[j];
        X[i] = s * (Rj - Ri);
        X[j] = s * (Rj + Ri); // s = 1.0 / sqrt(2.0), CData.h
    }
}//Generate random point on Rn-sphere of radius Rb
//--------------------------------------------------------------------
void
EngPhases(void)
{
    VV = sqrt(3.0 * kT * Bn / Me); rr = 2.0 / (double)RAND_MAX;
    Es = Ex = Ev->Vc; Sc = 0; //Tries counter, not in steping use 
    do
    {
		Et = Ex; Ei = Ex->v; Xi = Ei->X; Vi = Ei->V; 
        //Generate random point X on (Rn+2) sphere
        RA = Rb - Ei->S->Rt; RandomSphere(); 	
       
		for (k = 0; k < Rn; k++)//Set random speeds, and positions
        { 
			Xi[k] = X[k]; // Rn projection is distribution in ball
            Vi[k] = VV * ((double)rand() * rr - 1.0);
        }

        while (Et != Es)  // test intersection, Ei != Ej
        {        
            Et = Et->p; Ej = Et->v; Xj = Ej->X; 
            rv = Ei->S->Rt + Ej->S->Rt; rv *= rv; vv = 0.0;
            for (k = 0; k < Rn; k++) // rv & vv - distance squared
            {
				rk = Xi[k] - Xj[k]; vv += rk * rk;       
            }
            //check distances, if intersect, regenerate Ei
            if (rv > vv)
            { 
                if(Sc >= Tn){ DataFree(); exit(8); }//tries control
                else    { Sc++; Ex = Ex->p; break; }//regenerate Ei
            }//exits since single threaded, global scope control             
        }
    } while ((Ex = Ex->n) != Es);   
}//Engage phase space, random values {X,V}
//--------------------------------------------------------------------


