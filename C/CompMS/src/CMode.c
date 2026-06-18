#include"CBase.h"
#include"CSort.h"
#include"CEmnt.h"
#include"CMode.h"
//--------------------------------------------------------------------
CDList *Mv;//Modes resources container
double *Xc;//Center of mass vector
double *Pc;//Summary impulse
//--------------------------------------------------------------------
long    n, i, j, rn; double *Xs,*Vs, a, b, c, ra, rs, dt;
//--------------------------------------------------------------------
void
ModeInit(void)
{  
	if (Mv != NULL) return; 
    //Set constants, Rn + 2 for random point generation
    rn = Rn + 2; ra = 1.0 / sqrt((double)rn); rs = 1.0 / sqrt(2.0);

    Xs = (double*)Malloc(sizeof(double), rn, UT); //start transaction
    Vs = (double*)Malloc(sizeof(double), rn, UT);
    Xc = (double*)Malloc(sizeof(double), Rn, UT);
    Pc = (double*)Malloc(sizeof(double), Rn, UD); //end   transaction
    
    Mv = (CDList*)Xs; //non zero value to mark initialization, ModeFree
   
}//Inits start values in Mode container.                        Set: X
//--------------------------------------------------------------------
void
ModeFree(void)
{
    if (Mv == NULL) return;

    free(Pc); Pc = NULL;
    free(Xc); Xc = NULL; 
    free(Vs); Vs = NULL;
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
void // Math -> Compressor/wiki#initial-placement-and-size
SetPBound(void)
{   
    Rb = RA = 0.0; Bn = 0; Sx = Sv->Vc; St = Sx;
    do  //Weighted Average radius (RA) over all elements.
    { 
        Si = St->v; Bn += n = Si->Bn; 
        RA += n * Si->Rt;
    }   while ((St = St->n) != Sx); if (Bn <= 1) return;
    //Set radius for even gemetrical probality P = 1/2
    //intersect or move freely. Simultanious drop, all elements:
    VV = vv = (double)Bn; RA /=vv; vv *= vv - 1.0; 
    vv = 2.0 / vv; rr = 0.5; // P(move) = rr = 0.5
    // Rb = r (1 + 2 (1 - P^(2/((Bn - 1) Bn)))^(-1/Rn))
    rv = 1.0 - pow(rr, vv); // inner pow value rv and estimation RV
	// stability test on approximation usage, for large Bn
    // Rb = r (1 + 2 (2 Log[1/P] (Bn+1)/ (Bn^3))^(-(1/Rn)))
    RR = 2.0 * log(1.0 / rr) / VV; RV = RR / (VV - 1.0); 
	RR *= (VV + 1) / (VV * VV);   // upper bound and approximation
    if ((RR <= rv) || (rv <= RV))  rv = RR; // ? approximate inner pow

    VV = pow(rv, 1.0 / (double)Rn);  // outer pow   
    if (VV > 0.0) 
    {   Rb = RA * (1.0 + 2.0 / VV); }
    else //Fallback to a larger radius if calculation fails
    {   Rb = RA * 3.0;              } 
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
    //Fr - bits in RAND_MAX, for random double
    Fr = 0; m = RAND_MAX; while(m > 0) { Fr++; m>>=1; }; 
    
    e = 1.0; m = 0; while (1.0 < 1.0 + e) { e /= 10.0; m++; }
	Fm = m; //Mantisse test, decimal digits: 1.0 + 10^-Fm == 1.0

    e = 1.0; m = 0; while (0.0 < 0.0 + e) { e /= 10.0; m++; }
	Fe = m; //Exponent test, decimal power : 0.0 + 10^-Fe == 0.0
    
    //De should be greater without position verifier, De *= Bn
	//ranging not implemented, currently for reporting mainly
    e = 1.00 / pow(10.0, Fm); Ds = -(Rb * RN * e); 
    //linear dt range
    dA = 2.0 * sqrt(2.0 * e); De = Ds * 50.0;
}//Zero drift range: (-De;+De) as dT = 0.0; (-Ds;+Ds) as rv - RV = 0.0
//--------------------------------------------------------------------
static void //Inflated Cube, faster than rejection sampling
RandomSphereIC(void) // PDF tests in wiki and mathmatica file
{                    // 20% accuracy, on uniformity
    RR = 0.0; //rn = Rn + 2; rd = 2.0 / RAND_MAX; 
    // pick one index coordinate, will belong to cube face
    i = rand() % rn; 
    // generate others coordinates, uniformly distributed
    for (k = 0; k < rn; k++)
    {
        if (k == i) { rk = Xs[k] = (rand() % 2) ? 1.0 : -1.0; }
        else        { rk = Xs[k] = rd * rand() - 1.0; }
        RR += rk * rk;     Vs[k] = rd * rand() - 1.0; 
    }   //Vs can be set outside of this function, if needed
    // project cube face point to, RA = Rb - Ri, sphere surface
    RR = 1.0/sqrt(RR); for (k = 0; k < rn; k++){ Xs[k] *= RR; }
    // random rotations of coordinates, to smooth distribution
    for (k = 0; k < rn; k++)
    {
        i = rand() % rn; do  j = rand() % rn;  while (i == j);

        Ri = Xs[i]; Rj = Xs[j];
        Xs[i] = rs * (Rj - Ri);
        Xs[j] = rs * (Rj + Ri); // rs = 1.0 / sqrt(2.0)
    }   //Simple fast random point generation on rn-sphere of radius 1.0 
}//Generate random point and speed on rn-sphere of radius 1.0
//--------------------------------------------------------------------
static void // Bounce after Cube placing, slower than RandomSphereIC
RandomSphereBC(void)
{
    n = rn + rand() % rn; // rn = Rn + 2; ra = 1.0 / sqrt((double)rn);   
    do  //  coordinates & speeds, first within inscribed cube
	{   //  non RTOS looping, tries counter rely on probability
        for (rr = rv = vv = 0.0, k = 0; k < rn; k++)
        {   // rd = 2.0 / RAND_MAX; 
            rk = Xs[k] = (rd * rand() - 1.0) * ra; // inscribed
			vk = Vs[k] = (rd * rand() - 1.0);      // not scaled
            rr += rk*rk; // init scalar products 
            rv += rk*vk; vv += vk*vk;
		}  
    }   while (vv <= 0.0); // enable bouncing, |v| > 0.0  
    // bounce point by sphere surface, repeat n steps
    for (i = 0; i < n; i++) // (a != 0.0)
    {   // get time dt to reach rn-sphere of radius 1.0  
        // centered in origin, no point radius Rc = Rt = 0.0
		a = vv;  b = rv; c = rr - 1.0;  j = 1;
        VV = a * c;  RV = b * b;//wiki -> time
        RR = VV / RV; RV -= VV; //A=RR && D=RV 
        if  ( (c >= 0.0)      && (b >= 0.0)   )
        {             dt = -0.0;              }
        else if(fabs(RR) < dA && (b != 0.0)   )
        {   //  A = RR, linear approximation
            if (b > 0.0) dt = -0.5 * c / b;
            else// use and correct mantisse
            {   // bound to bound time
                dt = b / a * (0.50 * RR - 2.0);
                vv = a * dt + b;
                VV = dt * (vv + b) + c;
                dt-= vv*VV / (2.0*RV+1.5*a*VV);
            }   j = 0;//   done 
		}   //  dA = 2.0 * sqrt(2.0 * 10^(-Fm))
        else if (j)  if (RV >= 0.0) // root tti
        {   // first and b ==0 cases
            dt = (sqrt(RV) - b) / a;
            vv = a * dt + b;
            VV = dt * (vv + b) + c;
            dt-= vv * VV / (2.0*RV + 1.5*a*VV);
        }   else {    dt = -0.0;             };
        // Move to sphere surface
        for (k = 0; k < rn; k++) Xs[k] += Vs[k] * dt;
        //  reject point by sphere surface, repeat n steps
        for (rv = 0.0, k = 0; k < rn; k++)
	    {   //prepare for bounce, scalar product 
            rk = Xs[k]; 
		    vk = Vs[k]; rv += rk * vk;
        }   RV = 2.0 * rv; rr = rv = vv = 0.0;
        for (k = 0; k < rn; k++) 
        {
            rk = Xs[k]; Vs[k] -= RV * rk; //bounce by sphere surface
            vk = Vs[k];    rr += rk*rk;
            rv+= rk*vk;    vv += vk*vk;   //refresh scalar products
        }
    }      
}//Generate random point, and direction on rn-sphere of radius 1.0
//--------------------------------------------------------------------
void
EngPhases(void)
{   //Tries counter Sc, for control of random point generation
    Mj = sqrt(3.0 * kT * Bn / Me); Es = Ex = Ev->Vc; Sc = 0; 
    do
    {   //Generate random point Xs on (rn = Rn+2) sphere
        Mi = Rb - Ei->S->Rt; RandomSphereBC(); // use IC on small K,	
        Ei = Ex->v; Xi = Ei->X; Vi = Ei->V;    // IC -  20% accuracy 
		for (k = 0; k < Rn; k++)//Set random speeds, and positions
        { 
			Xi[k] = Mi * Xs[k]; // Rn projection is distribution in ball
            Vi[k] = Mj * Vs[k]; // speeds distribution is cubic /rotated
        }   
        Et = Ex;  // test intersection, Ei != Ej
        while (Et != Es)             // Ex != Es
        {        
            Et = Et->p; Ej = Et->v; Xj = Ej->X; 
            rv = Ei->S->Rt + Ej->S->Rt; rv *= rv; vv = 0.0;
            for (k = 0; k < Rn; k++) // rv & vv - distance squared
            {
				rk = Xj[k] - Xi[k]; vv += rk * rk;       
            }
            //check distances, if intersect, regenerate Ei
            if (rv > vv)
            {   //printf("\nLoader: too many tries\n"); // debug
                if(Sc >= Tn){ DataFree(); exit(8); }//tries control
                else    { Sc++; Ex = Ex->p; break; }//regenerate Ei
            }//exits since single threaded, global scope control             
        }
    }   while ((Ex = Ex->n) != Es);   
}//Engage phase space, random values {X,V}
//--------------------------------------------------------------------
void
TestGeometry(void)
{
    Es = Ex = Ev->Vc; Sc = 0; Qg = 0.0; // counter and averager
    do  // Bn*(Bn - 1) / 2 complexity, not for the regular step 
    {   // bound intersection, Ei
        Ei = Ex->v; Xi = Ei->X; rr = 0.0;
        RR = Rb - Ei->S->Rt; RR *= RR; 
        for (k = 0; k < Rn; k++) { rk = Xi[k]; rr += rk * rk; }  
        if (rr > RR) { Sc++; Qg += rr - RR; }; Et = Ex; 
        while (Et != Es) // pair intersection, Ei != Ej
        {
            Et = Et->p; Ej = Et->v; Xj = Ej->X; rr = 0.0;
            RR = Ei->S->Rt + Ej->S->Rt; RR *= RR;
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
{   //Obtain current energy value
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
{   //TestEnergy, copied, with late Qe assign
    Ex = Ev->Vc; Et = Ex; Qe = 0.0; 
    do  // Et == Ex, on leave
    {
        Ei = Et->v; Vi = Ei->V; vv = 0.0;
        for (k = 0; k < Rn; k++)
        {
            vk = Vi[k]; vv += vk * vk;
        }
        Qe += vv * Ei->S->Mt; 

    }   while ((Et = Et->n) != Ex); // devision by 2.0, later
    //Scale to given kT, simplified since not divided
    vv = (Qe > 0.0) ? sqrt(Bn * Rn * kT / Qe) : 1.0; Qe /= 2.0;
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
            RR = sqrt(rk - vk) - rv; // |dr|
            if (RR >= 0.0) // positive time selection
            { RR *= RR; VV = (RR > VV) ? VV : RR; }   
            else //get minimal displacement, dr^2 ? Xc^2
            {    /*  out of bound, error  */      }
        }//else  ->  out of bound, error                  
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
{   //Exclude drift by zoroing summury impulse by coordinate
    Ex = Ev->Vc; Et = Ex; // loops leave state: Et == Ex
    for (k = 0; k < Rn; k++)
    {
        vk = 0.0; rk = 0.0; // get summary impulse for k direction
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
            
        }   while ((Et = Et->n) != Ex && i < n);
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
                Ei = Et->v; xe = Ei->X; ve = Ei->V;

                rr = xe[i] - Xc[i]; // r(i) relative Xc
                rv = xe[j] - Xc[j]; // r(j) relative Xc
                me = Ei->S->Mt;

                RV += me * (rr * ve[j] - rv * ve[i]);
                RR += me * (rr * rr + rv * rv); // Inetrtia

            }   while ((Et = Et->n) != Ex); if (RR <= 0.0) continue;

            VV = RV / RR; // result angular speed in 2D plane 

            do  // suppress rotation, one pass
            {
                Ei = Et->v; xe = Ei->X; ve = Ei->V;

                rr = xe[i] - Xc[i]; // r(i) relative Xc
                rv = xe[j] - Xc[j]; // r(j) relative Xc

                ve[i] += VV * rv; // compensate
                ve[j] -= VV * rr; // by subtraction

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
    printf("\nTa = "); printf(fs, Ta);
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
int
TestStruct(CDList* LV)
{
    int a, i, j, t; a = 0; CDItem * LX, * LT;

    LX = LV->Vc; if ((t = LV->Vn) >= 1)
    {
        LT = LX; i = j = 0; do { i++; } while ((LT = LT->n) != LX);
        if (i != t) { a++; j++; }
        if (j > 0) printf("\nEnumeration n Vn FAIL");
        LT = LX; i = j = 0; do { i++; } while ((LT = LT->p) != LX);
        if (i != t) { a++; j++; }
        if (j > 0) printf("\nEnumeration p Vn FAIL");
    }
    LX = LV->Fc; if ((t = LV->Fn) >= 1)
    {
        LT = LX; i = j = 0; do { i++; } while ((LT = LT->n) != LX);
        if (i != t) { a++; j++; }
        if (j > 0) printf("\nEnumeration n Fn FAIL");
        LT = LX; i = j = 0; do { i++; } while ((LT = LT->p) != LX);
        if (i != t) { a++; j++; }
        if (j > 0) printf("\nEnumeration p Fn FAIL");
    }   
    LX = LV->Vc; if (LV->Vn == 1) if (t = (LX->p != LX || LX->n != LX))
    {
        printf("\nCheck Vn self-loop: p==n==self? FAIL"); a++;
    } 
    LX = LV->Fc; if (LV->Fn == 1) if (t = (LX->p != LX || LX->n != LX))
    {
        printf("\nCheck Fn self-loop: p==n==self? FAIL"); a++;
    }   
    LX = LV->Vc; if (LV->Vn >= 2) if (t = (LX->p == LX || LX->n == LX))
    {
        printf("\nCheck Vn not-looped: p!=n!=self? FAIL"); a++;
    }
    LX = LV->Fc; if (LV->Fn >= 2) if (t = (LX->p == LX || Lx->n == LX))
    {
        printf("\nCheck Fn not-looped: p!=n!=self? FAIL"); a++;
    }

    if (a > 0)
    {
        printf("\nError on step: structure"); return 1;
    }
    {
        return 0;
    }
}//simple structure list test, enumeration + nonlooped, looped for one
//--------------------------------------------------------------------



