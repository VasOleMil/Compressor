#include"CBase.h"
#include"CSort.h"
#include"CEmnt.h"
#include"CHold.h"
//--------------------------------------------------------------------
CDList     *Tv;//Time list items container 

CDItem     *Tx;//list service register
CDItem     *Tt;//list service register
CDItem     *Tm;//list service register

CHold      *Ti;//object service register
//--------------------------------------------------------------------
long         i,  n;
double       a,  b,  c, dt;
//--------------------------------------------------------------------
static void
HoldItemNew(void)
{
    Lx = (CDItem*)Malloc(sizeof(CDItem),UT, UT);//create ListItem
    Ti = (CHold*) Malloc(sizeof(CHold), UT, UT);//create Object,!Gv:UD
    //Loop Lx ListItem and connect to Ti object 
    Lx->p = Lx;
    Lx->n = Lx;
    Lx->v = Ti;
	//if (Gv), attach array for poisition saving 
    Ti->Xi = (double*)Malloc(sizeof(double), Rn, UT); //restore context
    Ti->Xj = (double*)Malloc(sizeof(double), Rn, UD); //Lv = Tv;
}//ListNew item provider for CHold,                            Set: Lx
//--------------------------------------------------------------------
static void
HoldItemDel(void)
{
    Ti = Lx->v;
    
    free(Ti->Xi);
    free(Ti->Xj);
    
    free(Ti);
    free(Lx);
}//ListDel item provider for CHold
//--------------------------------------------------------------------
void
TimeInit(void)
{
    if (Tv != NULL) return;

    Tv = (CDList*)Malloc(sizeof(CDList), UT, UD);
    //Set Element contaners counters and providers
    Tv->Fn = 0; Tv->ListItemDel = HoldItemDel;
    Tv->Vn = 0; Tv->ListItemNew = HoldItemNew;
}//Inits start values in CHold container.                      Set: Tv
//--------------------------------------------------------------------
void
TimeFree(void)
{
    if (Tv == NULL) return;

    Lv = Tv; ListFree();
    free(Tv); Tv = NULL;
}//Releases Free resources  Tv
//--------------------------------------------------------------------
//{Tv};{Tm,ei,ej,dT}; Lv,Ti,Tx,Tt
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void 
TimeGetStp(void)
{
    Tm = Tx = Tt = Tv->Vc; 
    dT = (Ti = Tt->v)->dt;
   
    while ((Tt = Tt->n) != Tx)    
    if ((Ti = Tt->v)->dt < dT)
    {  Tm = Tt; dT = Ti->dt; }
    //Tm is minimal tti item
	ei = (Ti = Tm->v)->ei;
    ej = (Ti = Tm->v)->ej;
}//Select minimal tti Tm from Tv
//--------------------------------------------------------------------
void 
TimeDecStp(void)
{
	Tx = Tt = Tv->Vc; //dT = (Ti=Tm->v)->dt;//evaluated in TimeGetStp  

    do  (Ti = Tt->v)->dt -= dT; 
    while ((Tt = Tt->n) != Tx);
}//Decreases times in container Tv, using Tm
//--------------------------------------------------------------------
static void 
TimeDelCol(void)
{
    n = Tv->Vn, Tx = Tv->Vc; Lv = Tv;
    for (i = 0; i < n; i++)
    {
        Ti = Tx->v; //ListDel sets Vc to previous
        if ((Ti->ei == Ex) || (Ex == Ti->ej))
        {
            Tv->Vc = Tx; ListDel(); Tx = Tv->Vc;
        };  
        Tx = Tx->n; //continue with next
    }
}//Delete (Ex, #) or (#, Ex) tti in Tv
//--------------------------------------------------------------------
void
TimeDelStp(void)
{
    { Ex = ei; TimeDelCol(); }
    if (ej != NULL)  
    { Ex = ej; TimeDelCol(); }  
}//Delete ei, ej tti in Tv
//--------------------------------------------------------------------
void
TimeValStp(void)
{   
    dt = (Ti = Tm->v)->dT; //restoring of saved predicted position 
        //Tm is minimal tti CHold list item, evaluated in TimeGetStp
        Ei = ei->v; Xi = Ei->X; Xj = Ti->Xi; Vj = Ei->V; 
        for (k = 0; k < Rn; k++) Xi[k] = Xj[k] + Vj[k] * dt;

    if (ej != NULL)  
	{
        Cx = 1; Ce++; 
        
        Ej = ej->v; Xi = Ej->X; Xj = Ti->Xj; Vj = Ej->V;
        for (k = 0; k < Rn; k++) Xi[k] = Xj[k] + Vj[k] * dt;
	}
    else
    { 
        Cx = 0; Cb++;
    }
}//Verify position, to prevent Te += dT summing errors
//--------------------------------------------------------------------
static void
TimeCalcBS(void)
{
    Xi = Ei->X; Vi = Ei->V; Si = Ei->S;
    rv =  0.0 ; vv =  0.0 ; rr =  0.0 ;
    //Calculate scalar product
    for (k = 0; k < Rn; k++)
    {
        rk = Xi[k]; rr += rk*rk;
        vk = Vi[k]; rv += rk*vk; vv += vk*vk;
    }
    //Gr = N, scatter by element surface
    RR = Rb - Si->Rt;  RV = RR * (Si->Vc);
    RR *= RR; VV = (Si->Vs); // Vs = Vc*Vc
   
    a = vv - VV; b = rv + RV; c = rr - RR; 
    VV = a * c;  RV = b * b;//wiki -> time
    RR = VV / RV; RV -= VV; //A=RR && D=RV 
    // b or even rv for Ds, zero processing
    if  (  (c >= 0.0)   &&   (b >= 0.0)   )
    {                  dt = -0.0;         }
	else if(a == 0.0) // theoretical case
    {
        if (b == 0.0){ dt = -0.0;/*c = 0*/}
        else         { dt = -0.5 * c / b; }
    }   
    else if(fabs(RR) < dA  &&  (b != 0.0) )
    {   //  A = RR, linear approximation
        if (b > 0.0) dt = -0.5 * c / b;
        else // use and correct mantisse
        {
            dt = b / a * (0.50 * RR - 2.0);
            vv = a * dt + b;
            VV = dt * (vv + b) + c;
            dt -=vv*VV / (2.0*RV+1.5*a*VV);
        }   return; //done      
    }  
	else if(RV >= 0.0) // one root tti
    {   
        dt = (+sqrt(RV) - b) / a; 
        vv = a * dt + b;
        VV = dt * (vv + b) + c;
        dt -= vv*VV / (2.0*RV  + 1.5*a*VV);
    } 
    else// inbound should be reachable  
    {                  dt  = -0.0;        }
}// Ei sizing bound-element interaction, calculates tti
//--------------------------------------------------------------------
static void
TimecalcES(void)
{
    VV = a * c; RV = b * b; //wiki -> time
    RR = VV / RV; RV -= VV; //A=RR && D=RV 
    // b or even rv for Ds, zero processing
    if (   (c <= 0.0)     && (b <= 0.0)   )
    {                       dt = -0.0;    }
    else if (a == 0.0) // theoretical case
    {
        if (b == 0.0){ dt = -0.0;/*c = 0*/ }
        else {         dt = -0.5 * c / b; }
    }
    else if(RV >= 0.0) // one root tti
    {
        if (fabs(RR) < dA && (b != 0.0)   )
        {   // use linear approximation
        if (b < 0.0) { dt = -0.5 * c / b; }
        else {              dt = -1.0;    }
        }
        else // full sqrt computation
        {
            dt = (-sqrt(RV) - b) / a;
            vv = a * dt + b;
            VV = dt * (vv + b) + c;
            dt-= vv*VV / (2.0*RV+1.5*a*VV);
        }
    }
    else// main time span discriminant
    {                       dt = -1.0;    }
}// Ei Ej sizing element-element interaction, calculates tti
//--------------------------------------------------------------------
static void
TimeCalcES(void)
{
    Xi = Ei->X; Vi = Ei->V; Ri = Ei->S->Rc; 
    Xj = Ej->X; Vj = Ej->V; Rj = Ej->S->Rc;
    rv =  0.0 ; vv =  0.0 ; rr =   0.0    ;
    //Calculate scalar product
    for (k = 0; k < Rn; k++)
    {
        rk = Xj[k] - Xi[k]; vk = Vj[k] - Vi[k];
		rr += rk*rk; rv += rk*vk;  vv += vk*vk;
    }

    RR = Ri + Rj;  RR *= RR; VV = GG * RR;
    RV = Gc * GR * RR; RR *= GR * GR;

    a = vv - VV; b = rv - RV; c = rr - RR;
    if  (  (c >  0.0)     &&  (b >  0.0)  )
    {  /*e-e speedup*/ dt = -1.0;         }
    else
    { /*full  solver*/ TimecalcES();      }
}// Ei Ej sizing element-element interaction, calculates tti
//--------------------------------------------------------------------
static void
TimeSaveBS(void)
{
	ListAdd(); Ti = Lx->v; Ti->dt = dt; //TimeCalcEx evaluates dt
    Ti->ei =   Ex; 
	Ti->ej = NULL; //indicates bound interaction
    //if (Gv) //Gv(Y) 
    { 
        memcpy(Ti->Xi, Ei->X, LN); Ti->dT = dt;
        //dT and X are saved for calculation economy
		//not all saved tti require verification
    }
}//add {Ei,Ej,dt,Xi,Xj} to Tv, Lv = Tv: initilized in TimeCalcEx
//--------------------------------------------------------------------
static void
TimeSaveES(void)
{
    ListAdd(); Ti = Lx->v; Ti->dt = dt; //TimeCalcEx evaluates dt
    Ti->ei = Ex;
    Ti->ej = Et;
	//if (Gv) //Gv(Y)
    {
        memcpy(Ti->Xi, Ei->X, LN); Ti->dT = dt;
        memcpy(Ti->Xj, Ej->X, LN);
    }
}//add {Ei,Ej,dt,Xi,Xj} to Tv, Lv initilized in TimeCalcEx
//--------------------------------------------------------------------
static void 
TimeCalcEx(void)
{   
    Ei = Ex->v; Et = Ex; //Ej not used in bound interaction, ej = NULL
        TimeCalcBS(); if (dt >= De) TimeSaveBS();
        
    while ((Et = Et->n) != Es)  //Calc elements, Ei != Ej
    {
        Ej = Et->v; 
        TimeCalcES(); if (dt >= De) TimeSaveES();     
    }  
}//Calculate  Ex element tti, Lv initilized in TimeCalc[S/T]T
//--------------------------------------------------------------------
//{ei,ej};{Tv}; Lv, Ev, Ex, Et, Es, Ti, Ei, Ej, Ri, rr-rv, RR-RV, dt
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void
TimeCalcTT(void)
{   
    Lv = Tv; // Lv in stepping also initilized in TimeDelStp 
                                  //  mean free time, for all events
    { Es = Ex = ei; TimeCalcEx(); if (Sc == 0) Ta += Ar * (dT - Ta); }
    if (ej != NULL)               //  mean square relative speed
    { Es = Ex = ej; TimeCalcEx(); if (Sc == 0) Va += Ar * (a  - Va); }
}//Calculate  ei,ej elements times to interaction - tti
//--------------------------------------------------------------------
//{};{Tv}; Lv, Ev, Ex, Et, Es, Ti, Ei, Ej, Ri, rr-rv, RR-RV, dt, a-c
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void
TimeCalcST(void)
{
    Es = Ev->Vc; Ex = Es; Lv = Tv; // Prepare list context

    do TimeCalcEx(); while ((Ex = Ex->n) != Es);
}//Calculate   all  elements times to interaction - tti
//--------------------------------------------------------------------
   