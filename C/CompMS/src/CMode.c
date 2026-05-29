#include"CBase.h"
#include"CSort.h"
#include"CEmnt.h"
#include"CMode.h"
//--------------------------------------------------------------------
CDList *Mv;//Modes resources container
double *Xc;//Center of mass vector
double *Pc;//Summary impulse
//--------------------------------------------------------------------
long    n, i, j, rn; double *Xs;
//--------------------------------------------------------------------
void
ModeInit(void)
{
	if (Mv != NULL) return; rn = Rn + 2;

    Xs = (double*)Malloc(sizeof(double), rn, UD);
    Xc = (double*)Malloc(sizeof(double), Rn, UD);
    Pc = (double*)Malloc(sizeof(double), Rn, UD);
    
    Mv = (CDList*)Xs; //non zero value to mark initialization, ModeFree
   
}//Inits start values in Mode container.                        Set: X
//--------------------------------------------------------------------
void
ModeFree(void)
{
    if (Mv == NULL) return;

    free(Pc); Pc = NULL;
    free(Xc); Xc = NULL; 
    free(Xs); Xs = NULL; Mv = NULL;
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
        Si = St->v; Bn += n = Si->Bn; 
        RA += n * Si->Rc;
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

	}   while ((St = St->n) != Sx); 
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
    do  //Bn*(Bn - 1) / 2 complexity, not for regular step test
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
    }   while ((Ex = Ex->n) != Es);

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

    }   while ((Et = Et->n) != Ex); Qe /= 2.0;
   
}// Get system energy Qe = Bn * Rn * kT / 2.0
//--------------------------------------------------------------------
void
NormEnergy(void)
{
    //TestEnergy, copied, with late Qe assign
    Ex = Ev->Vc; Et = Ex; Qe = 0.0; 
    do  // Et == Ex, on leave
    {
        Ei = Et->v; Vi = Ei->V; vv = 0.0;
        for (k = 0; k < Rn; k++)
        {
            vk = Vi[k]; vv += vk * vk;
        }
        Qe += vv * Ei->S->Mt; 

    }   while ((Et = Et->n) != Ex); //devision by 2.0, later
    //Scale to given kT, simplified since not divided
    vv = (Qe != 0.0) ? sqrt(Bn * Rn * kT / Qe) : 1.0; Qe /= 2.0;
    do  // NormMassCenter() -> NormImpulse() -> NormMomenta()
    {   // should be called before this function
        Ei = Et->v; Vi = Ei->V;
        for (k = 0; k < Rn; k++) Vi[k] *= vv;  

    }   while ((Et = Et->n) != Ex);   
    
}// Normalize energy to given kT, writes value Qe before correction
//--------------------------------------------------------------------
void
TestMassCenter(void)
{
    Ex = Ev->Vc; Et = Ex; RR = 0.0; 
    do // get current elements Mt masses, RR - summary mass
    {
        Ei = Et->v; RR += Ei->S->Mt;

    }   while ((Et = Et->n) != Ex);
    RR = (RR > 0.0) ? 1.0 / RR : 0.0; // reverse for speed
    for (k = 0; k < Rn; k++)
    {
        rk = 0.0;       // Xc[k] = 0.0;
        do              // Xc[k] component 
        {
            Ei = Et->v; Xi = Ei->X; 
            rk += Ei->S->Mt * Xi[k];

        }   while ((Et = Et->n) != Ex);
        Xc[k] = rk * RR; // next k
    }
}// Get current mass center vector Xc
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
    Ex = Ev->Vc; Et = Ex; VV = vv; 
    do  //Measure possible coordinate displacement:
    {   // line to sphere equation, with speed multiplier
        // same as repeat math with dt calucalution, wiki 
        // dr = v * dt, v = Xc  
        // dr = v * (sqrt(rv^2 - (rr-RR) * vv) - rv) / vv
        Ei = Et->v;    Xi = Ei->X;     rr = 0.0;
        RR = Rb - Ei->S->Rt; RR *= RR; rv = 0.0;
        for (k = 0; k < Rn; k++)
        {
            rk = Xi[k]; rr += rk * rk;
            vk = Xc[k]; rv += rk * vk;
        }  
        //Select possible RR or required VV shift, squared
        rk = rv * rv; vk = (rr - RR) * vv; 
        if (rk >= vk) //discriminant test
        {   //in bound should always be reachable
            RR = sqrt(rk - vk) - rv; //|dr|
            if (RR >= 0.0) // positive time selection
            {   
                RR *= RR; VV = (RR > VV) ? VV : RR;
            }   //get minimal displacement, dr^2 ? Xc^2
            else
            {
                //out of bound, error
            }
        }//else-> out of bound, error          
        
    }   while ((Et = Et->n) != Ex); // Et == Ex on leave 

    VV = sqrt(VV / vv); //shift with scaled displacement

    do                  //no pair distances changed
    {     
        Ei = Et->v; Xi = Ei->X;
        for (k = 0; k < Rn; k++) Xi[k] -= VV * Xc[k];

    }   while ((Et = Et->n) != Ex); // dt expressions on wiki in link 
   
// https://github.com/VasOleMil/Compressor/wiki#time-change-prediction
}//Try shift elements Xc to the center of bound
//--------------------------------------------------------------------
void
TestImpulse(void)
{
    Ex = Ev->Vc; Et = Ex; 
    for (k = 0; k < Rn; k++)
    {
        vk = 0.0;   // Pc[k] = 0.0;
        do          // Pc[k] component 
        {
            Ei = Et->v; Vi = Ei->V;  
            vk += Ei->S->Mt * Vi[k]; 

        }   while ((Et = Et->n) != Ex); 
        Pc[k] = vk; // next k
    }
}// Get current summary inpulse Pc
//--------------------------------------------------------------------
void
NormImpulse(void)
{
    //Exclude drift by zoroing summury impulse by coordinate
    Ex = Ev->Vc; Et = Ex; // loops leave state: Et == Ex
    for (k = 0; k < Rn; k++)
    {
        vk = 0.0; rk = 0.0;// get summary impulse for k direction
        do
        {
            Ei = Et->v; Vi = Ei->V; Mi = Ei->S->Mt;
            vk += rv = Mi * Vi[k];   // summary impulse
            rk += fabs(rv);          // average impulse

        }   while ((Et = Et->n) != Ex); if (vk == 0.0) continue;
        // Dynamically distribute drift compensation: 
        // either uniformly across all particles
        // or targeted using j-scaled average unit impulse
        j = 4; // log_2(j) + 1 = 3 last bits loss on summing
        rk /= 2.0 * (double)Bn; n = (j * (long)(rk / fabs(vk)));
        n = ((n < 1) ? 1 : n);  n = ((n > Bn) ? Bn : n);
        vk /= (double)n; i = 0;
        do  // near or uniform drift supress
        {   // static bound do not demand lsb rigor
            Ei = Et->v;  Vi = Ei->V;
            Vi[k] -= vk / Ei->S->Mt; i++;
            
        }   while ((Et = Et->n) != Ex || i < n);
    }
}//Zero system drift
//--------------------------------------------------------------------
void
NormMomenta(void)
{
    TestMassCenter();       // NormMassCenter() and NormImpulse()
                            // should be called before this function
    for (i = 0; i < Rn; i++)
    {
        for (j = i + 1; j < Rn; j++)
        {
            Ex = Ev->Vc; Et = Ex; RV = 0.0; RR = 0.0;
            do  // get angular speed in plane (i, j)
            {
                Ei = Et->v; Xi = Ei->X; Vi = Ei->V;

                rr = Xi[i] - Xc[i]; // r(i) relative Xc
                rv = Xi[j] - Xc[j]; // r(j) relative Xc
                Mi = Ei->S->Mt;

                RV += Mi * (rr * Vi[j] - rv * Vi[i]);
                RR += Mi * (rr * rr + rv * rv); // Inetrtia

            }   while ((Et = Et->n) != Ex); if (RR <= 0.0) continue;

            VV = RV / RR; // result angular speed in 2D plane 

            do  // suppress rotation, one pass
            {
                Ei = Et->v; Xi = Ei->X; Vi = Ei->V;

                rr = Xi[i] - Xc[i]; // r(i) relative Xc
                rv = Xi[j] - Xc[j]; // r(j) relative Xc

                Vi[i] += VV * rv; // compensate
                Vi[j] -= VV * rr; // by subtaction

            }   while ((Et = Et->n) != Ex);
        }   //  use accumulated angular speed as indicator 
    }       //  or iterator for complete rotation exclusion
}// suppress system rotation
//--------------------------------------------------------------------
// debug
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
    }   while ((Et = Et->n) != Ex);

    TestEnergy(); TestGeometry();

    printf("\nQg = "); printf(fs, Qg);
    printf("\nkT = "); printf(fs, (2.0 * Qe) / (Rn * Bn));
    printf("\nKT = "); printf(fs, kT);
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
    }   while ((Et = Et->n) != Ex);
   
}//output params and array to console, debug
//--------------------------------------------------------------------



