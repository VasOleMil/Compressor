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
    vv =  0.0 ; rv =  0.0 ; rr =  0.0 ;
    //Calculate scalar product
    for (k = 0; k < Rn; k++)
    {
        rk = Xi[k]; rr += rk*rk;
        vk = Vi[k]; rv += rk*vk; vv += vk*vk;
    }
    //Gr = N
    RR = Rb - Si->Rt; VV = Si->Vc;
    RV = RR*VV; VV*=VV;  RR *= RR;

    a = vv - VV; b = rv - RV;  c = rr - RR; Mi = a; Mj = b;
    // b or even rv for Ds, zero processing
    if  (  (c >= 0.0)   &&   (b >= 0.0)   )
    {
                       dt = -0.0;
    }
    else if(a == 0.0)
    {
        if (b == 0.0){ dt = -1.0;         }
        else         { dt = -0.5 * c / b; }
    }
    else
    {   //inbound should be reachable
        if((RV = a * c)   >=  (rv = b * b))
        {  dt = +(sqrt(rv + RV) - b) / a; } // wiki ? 
        else
        {  dt = +(sqrt(rv - RV) - b) / a; } // wiki +
    }
}//
//--------------------------------------------------------------------
static void
TimeCalcES(void)
{
    Xi = Ei->X; Vi = Ei->V; Ri = Ei->S->Rc; 
    Xj = Ej->X; Vj = Ej->V; Rj = Ej->S->Rc;
    vv =  0.0 ; rv =  0.0 ; rr =   0.0    ;
    //Calculate scalar product
    for (k = 0; k < Rn; k++)
    {
        rk = Xj[k] - Xi[k]; vk = Vj[k] - Vi[k];
		rr += rk*rk; rv += rk*vk;  vv += vk*vk;
    }

    RR = Ri + Rj; RR *= RR; VV = GG * RR;
    RV = Gc * GR * RR;     RR *= GR * GR;

    a = vv - VV; b = rv - RV;  c = rr - RR;
    // b or even rv for Ds, zero processing
    if  (  (c <= 0.0)    &&   (b <= 0.0)  ) 
    {
                       dt = -0.0;           
    }
    else if(a == 0.0)
    {
        if (b == 0.0){ dt = -1.0;         }
        else         { dt = -0.5 * c / b; }
    }
    else
    {
        if((RV = a * c)   >=  (rv = b * b))
        {              dt  = -1.0;        }
        else
        {  dt = -(sqrt(rv - RV) + b) / a; } // wiki + 
    }
}//
//--------------------------------------------------------------------
static void
TimeSaveBE(void)
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
TimeSaveEE(void)
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
        TimeCalcBS(); if (dt >= De) TimeSaveBE();
        
    while ((Et = Et->n) != Es)  //Calc elements, Ei != Ej
    {
        Ej = Et->v; 
        TimeCalcES(); if (dt >= De) TimeSaveEE();     
    }  
}//Calculate  Ex element tti, Lv initilized in TimeCalc[S/T]T
//--------------------------------------------------------------------
void
TimeCalcTT(void)
{
	Lv = Tv; // Lv in spepping initilized in TimeDelStp 
    
    { Es = Ex = ei; TimeCalcEx(); }
    if (ej != NULL)
    { Es = Ex = ej; TimeCalcEx(); }
}//Calculate  ei,ej elements times to interaction - tti
//--------------------------------------------------------------------
void
TimeCalcST(void)
{
    Es = Ex = Ev->Vc; Lv = Tv; // Lv initilized in TimeDelStp     

    do TimeCalcEx(); while ((Ex = Ex->n) != Es);
}//Calculate   all  elements times to interaction - tti
//--------------------------------------------------------------------
//#include<stdio.h> 
//static void
//StructTestTime(void)
//{
//    int a, i, j, t; a = 0;
//
//    //printf("\n\tTest CHold->...");
//    if ((t = Tv->Vn) >= 1)
//    {
//        Tt = Tx; i = j = 0; do { i++; } while ((Tt = Tt->n) != Tx);
//        if (i != t) { a++; j++; }
//        if (j > 0) printf("\n\tEnumeration n Vn FAIL");
//        Tt = Tx; i = j = 0; do { i++; } while ((Tt = Tt->p) != Tx);
//        if (i != t) { a++; j++; }
//        if (j > 0) printf("\n\tEnumeration p Vn FAIL");
//    }
//    if ((t = Tv->Fn) >= 1)
//    {
//        Tt = Tx; i = j = 0; do { i++; } while ((Tt = Tt->n) != Tx);
//        if (i != t) { a++; j++; }
//        if (j > 0) printf("\n\tEnumeration n Fn FAIL");
//        Tt = Tx; i = j = 0; do { i++; } while ((Tt = Tt->p) != Tx);
//        if (i != t) { a++; j++; }
//        if (j > 0) printf("\n\tEnumeration p Fn FAIL");
//    }
//
//    if (a > 0)
//    {
//        printf("\n\tHold.c:Error on step:\n");
//    }
//    //else { printf("\n\tHold.c: Test passed:\n"); }
//}
//--------------------------------------------------------------------


    