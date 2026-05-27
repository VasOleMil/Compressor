#include"CBase.h"
#include"CSort.h"
#include"CEmnt.h"
#include"CMode.h"
//--------------------------------------------------------------------
CDList *Mv;//Modes resources container
double *Xc;//Center of mass vector
//--------------------------------------------------------------------
long    n, i, j, rn; double *Xs;
//--------------------------------------------------------------------
void
ModeInit(void)
{
	if (Mv != NULL) return; rn = Rn + 2;

    Xs = (double*)Malloc(sizeof(double), rn, UD);
    Xc = (double*)Malloc(sizeof(double), Rn, UD);
    
    Mv = (CDList*)Xs; //non zero value to mark initialization, ModeFree
   
}//Inits start values in Mode container.                        Set: X
//--------------------------------------------------------------------
void
ModeFree(void)
{
    if (Mv == NULL) return;
    free(Xs); Xs = NULL;
    free(Xc); Xc = NULL; Mv = NULL;
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
SetPBound(void)
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
    
    //De should be greater without position verifier, De *= Bn
	//ranging not implemented, currently for reporting only
    e = pow(10.0, -Fm); Ds = -(Rb * RN * e); De = 4e6 * Ds;
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
        if (k != i) { rk = Xs[k] = (double)rand() * rr - 1.0; }
        else        { rk = Xs[k] = (rand() % 2) ? 1.0 : -1.0; }
        RR += rk * rk;
    }
    // project cube face point to, RA = Rb - Ri, sphere surface
    RR = RA/sqrt(RR); for (k = 0; k < rn; k++) { Xs[k] *= RR; }
    // random rotations of coordinates, to smooth distribution
    for (k = 0; k < rn; k++)
    {
        i = rand() % rn; do  j = rand() % rn;  while (i == j);

        Ri = Xs[i]; Rj = Xs[j];
        Xs[i] =  s * (Rj - Ri);
        Xs[j] =  s * (Rj + Ri); // s = 1.0 / sqrt(2.0), CData.h
    }
}//Generate random point on rn-sphere of radius RA
//--------------------------------------------------------------------
void
EngPhases(void)
{
    VV = sqrt(3.0 * kT * Bn / Me); rr = 2.0 / (double)RAND_MAX;
    Es = Ex = Ev->Vc; Sc = 0; //Tries counter, not in steping use 
    do
    {
		Et = Ex; Ei = Ex->v; Xi = Ei->X; Vi = Ei->V; 
        //Generate random point X on (rn = Rn+2) sphere
        RA = Rb - Ei->S->Rt; RandomSphere(); 	
       
		for (k = 0; k < Rn; k++)//Set random speeds, and positions
        { 
			Xi[k] = Xs[k]; // Rn projection is distribution in ball
            Vi[k] = VV * ((double)rand() * rr - 1.0);
        }

        while (Et != Es)  // test intersection, Ei != Ej
        {        
            Et = Et->p; Ej = Et->v; Xj = Ej->X; 
            rv = Ei->S->Rt + Ej->S->Rt; rv *= rv; vv = 0.0;
            for (k = 0; k < Rn; k++) // rv & vv - distance squared
            {
				rk = Xj[k] - Xi[k]; vv += rk * rk;       
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
void
TestGeometry(void)
{
    Es = Ex = Ev->Vc; Sc = 0; Qg = 0.0; // counter and averager
    do // Bn*(Bn - 1) / 2 complexity, not for regular step test
    {
        Et = Ex; Ei = Ex->v; Xi = Ei->X; 
        // bound intersection, Ei
        RR = Rb - Ei->S->Rt; RR *= RR; rr = 0.0;
        for (k = 0; k < Rn; k++)
        {
            rk = Xi[k]; rr += rk * rk;
        }  
        if (rr > RR) { Sc++; Qg += rr - RR; }
        // pair intersection, Ei != Ej
        while (Et != Es)  
        {
            Et = Et->p; Ej = Et->v; Xj = Ej->X;
            RR = Ei->S->Rt + Ej->S->Rt; RR *= RR; rr = 0.0;
            for (k = 0; k < Rn; k++)
            {
                rk = Xj[k] - Xi[k]; rr += rk * rk;
            }
            if (rr < RR) { Sc++; Qg -= rr - RR; }          
        }
    } while ((Ex = Ex->n) != Es);

    Qg /= Sc > 0 ? Sc : 1.0; // averager saves zero if clean

}//Calculate average geometry deviation, squared complexity, Sc, Qg
//--------------------------------------------------------------------
void
TestEnergy(void)
{
    //Obtain current energy value
    Ex = Ev->Vc; Et = Ex; Qe = 0.0;
    do  // Et == Ex, on leave
    {
        Ei = Et->v; Vi = Ei->V; vv = 0.0;
        for (k = 0; k < Rn; k++)
        {
            vk = Vi[k]; vv += vk * vk;
        }
        Qe += vv * Ei->S->Mt;
    } while ((Et = Et->n) != Ex); Qe /= 2.0;
   
}//System energy Qe = Rn * kT / 2
//--------------------------------------------------------------------
void
NormEnergy(void)
{
    //TestEnergy, copied
    Ex = Ev->Vc; Et = Ex; Qe = 0.0; 
    do  // Et == Ex, on leave
    {
        Ei = Et->v; Vi = Ei->V; vv = 0.0;
        for (k = 0; k < Rn; k++)
        {
            vk = Vi[k]; vv += vk * vk;
        }
        Qe += vv * Ei->S->Mt; 
    } while ((Et = Et->n) != Ex); Qe /= 2.0; 
    //Scale to given kT
    vv = (Qe != 0.0) ? sqrt(kT * Rn * Bn / Qe) : 1.0; 
    do
    {
        Ei = Et->v; Vi = Ei->V;
        for (k = 0; k < Rn; k++) Vi[k] *= vv;      
    } while ((Et = Et->n) != Ex);

}//Normalize energy to given kT, writes value Qe before correction
//--------------------------------------------------------------------
void
TestMassCenter(void)
{
    for (k = 0; k < Rn; k++) Xc[k] = 0.0; 
    Mj = 0.0; Ex = Ev->Vc; Et = Ex;
    do
    {
        Ei = Et->v; Xi = Ei->X;  Mi = Ei->S->Mt; Mj += Mi;
        for (k = 0; k < Rn; k++) Xc[k] += Mi * Xi[k];
    } while ((Et = Et->n) != Ex);
    Mj = (Mj > 0.0) ? 1.0 / Mj : 0.0;
    for (k = 0; k < Rn; k++) Xc[k] *= Mj;
}// get current mass center vector Xc
//--------------------------------------------------------------------
void
NormMassCenter(void)
{
    TestMassCenter(); // get current mass center vector Xc
    //assume v as vector along Xc direction, v = Xc 
    //norm of vector, |Xc| = sqrt(vv)
    vv = 0.0; for (k = 0; k < Rn; k++)
    {
        vk = Xc[k]; vv += vk * vk; 
    }   if (vv <= 0.0) return; //Done
    //Try shift elements Xc to the center of bound
    //Possible -> VV = vv <- Required
    Es = Ev->Vc; Ex = Es; VV = vv; 
    do
    {
        //Measure possible coordinate displacement:
        // line to sphere equation, with speed multiplier
        // same as repeat math with dt calucalution, wiki 
        // dr = v*dt = 
        // = v * (sqrt(rv^2 - (rr - RR) * vv) - rv) / vv
        Ei = Ex->v;    Xi = Ei->X;     rr = 0.0;
        RR = Rb - Ei->S->Rt; RR *= RR; rv = 0.0;
        for (k = 0; k < Rn; k++)
        {
            rk = Xi[k]; rr += rk * rk;
            vk = Xc[k]; rv += rk * vk;
        }  
        //Select possible RR or required VV shift, squared
        rk = rv * rv; vk = (rr - RR) * vv; 
        if (rk >= vk)
        {
            RR = sqrt(rk - vk) - rv; //|dr|
            if (RR >= 0.0) // positive time selection
            {
                RR *= RR;  //in bound always reachable
                VV = (RR > VV) ? VV : RR; // dr^2 ? Xc^2
            }
            else
            {
                //initial placing was out of bound, error
            }
        }       
        
        Et = Es;//get possible shift defined by elements
        while (Et != Es) // pair intersection test, Ei != Ej
        {
            Et = Et->p; Ej = Et->v; Xj = Ej->X;   rr = 0.0;
            RR = Ei->S->Rt + Ej->S->Rt; RR *= RR; rv = 0.0;
            for (k = 0; k < Rn; k++)
            {
                rk = Xi[k] - Xj[k]; rr += rk * rk;
                vk = Xc[k];          rv += rk * vk;
            }   //rv calculated with opposite sign
            //Select possible RR or required VV shift, squared
            rk = rv * rv; vk = (rr - RR) * vv;
            if (rk >= vk)
            {
                RR = sqrt(rk - vk) - rv; //|dr|
                if (RR >= 0.0) // positive time selection
                {
                    RR *= RR;  //get minimal displacement
                    VV = (RR > VV) ? VV : RR; // dr^2 ? Xc^2
                }
            }
        }    
        //shift with selected displacement
        VV = sqrt(VV / vv); //scale and norm
        for (k = 0; k < Rn; k++) Xi[k] -= VV * Xc[k];  

    } while ((Ex = Ex->n) != Es); // dt expressions on wiki in link    
// https://github.com/VasOleMil/Compressor/wiki#time-change-prediction
}//Try shift elements Xc to the center of bound
//--------------------------------------------------------------------

//--------------------------------------------------------------------
void
GetVolume(void)
{
    long rm = Rn - 1; double Rt = (Si = Sv->Vc->v)->Rt, Rr;

    char fs[6]; sprintf(fs, "%%.%lde", Fm);  Rr = Rb / Rt;

    Ex = Et = Ev->Vc;               //filters
    do 
    {
        Ei = Et->v; Xi = Ei->X;     //no out print
        for (k = 0; k < Rn; k++)    //mark out state
            if (fabs(Xi[k]) >= Rb)  { printf("\n<0"); return; }
    } while ((Et = Et->n) != Ex);

    printf("\nkT = "); printf(fs, kT);
    printf("\nGc = "); printf(fs, Gc);
    printf("\nDe = "); printf(fs, De);
    printf("\nCb = "); printf("%ld", Cb);
    printf("\nCe = "); printf("%ld", Ce);
    printf("\nRn = "); printf("%ld", Rn);
    printf("\nBn = "); printf("%ld", Bn);
    printf("\nRb = "); printf(fs, Rb);
    printf("\nRt = "); printf(fs, Rt);
    printf("\n\nRb/Rt = "); printf(fs, Rr);
                                    //no coordinate print on rare
    if (Rr > 3.01)                  { printf("\n>3"); return; }
      
    Ex = Et = Ev->Vc; printf("\n"); //output selected coordinates 
    do
    {
        Ei = Et->v; Xi = Ei->X;    printf("\n{");
        for (k = 0; k < rm; k++) { printf(fs, Xi[k]); printf(", "); };
        printf(fs, Xi[rm]);      printf("}%c", (Et->n == Ex)?' ':',');
    } while ((Et = Et->n) != Ex);
   
}//output params and array to console, debug
//--------------------------------------------------------------------



