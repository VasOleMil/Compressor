#define _CRT_SECURE_DEPRECATE_MEMORY
#include <memory.h>

#include"CBase.h"
#include"CHold.h"
//--------------------------------------------------------------------
CDList     *Tv;//Time list items container 
CDItem     *Tx;//list service register
CDItem     *Tt;//list service register
CHold      *Ti;//object service register
//--------------------------------------------------------------------
long        i, n;
double     *Xi, *Xj, *Vi, *Vj, dt; 
CDItem     *Tm;//list service register
//--------------------------------------------------------------------
static void
HoldItemNew(void)
{
    Lx = (CDItem*)Malloc(sizeof(CDItem),UT, UT);//create ListItem
    Ti = (CHold*) Malloc(sizeof(CHold), UT, UT);//create Object,!Gv=UD
    //Loop Lx ListItem and connect to Ei Object 
    Lx->p = Lx;
    Lx->n = Lx;
    Lx->v = Ti;
	//if (Gv) //Gv = Y, for Gm = N use UD object Ti initialization 
    {
        Ti->Xi = (double*)Malloc(sizeof(double), Rn, UT); //Gv = Y
        Ti->Xj = (double*)Malloc(sizeof(double), Rn, UD); //Gv = Y
    }
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
	Tt = Tx = Tv->Vc; //dT = (Ti=Tm->v)->dt;//evaluated in TimeGetStp  

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
        if ((Ex == (Ti=Tx->v)->ei) || (Ex == (Ti=Tx->v)->ej))
        {
            Tv->Vc = Tx; ListDel();
        };  Tx = Tx->n;
    }
}//Delete (ei, #) or (#, ei) tti in Tv
//--------------------------------------------------------------------
void
TimeDelStp()
{
    { Ex = ei; TimeDelCol(); }
    if (ej != NULL)  
    { Ex = ej; TimeDelCol(); }  
}//Delete ei, ej tti in Tv
//--------------------------------------------------------------------
static void
TimeCalcBS()
{
    dt = 1.0;
}//
//--------------------------------------------------------------------
static void
TimeCalcES()
{
    dt = 1.0;
}//
//--------------------------------------------------------------------
static void
TimeSaveBE()
{
	ListAdd(); Ti = Lx->v; Ti->dt = dt; //TimeCalcEx evaluates dt
    Ti->ei =   Ex; 
	Ti->ej = NULL; //indicates bound interaction
    //if (Gv) //Gv = Y 
    { 
        memcpy(Ti->Xi, Ei->X, LN); Ti->dT = dt; 
    }
}//add {Ei,Ej,dt,Xi,Xj} to Tv, Lv = Tv: initilized in TimeCalcEx
//--------------------------------------------------------------------
static void
TimeSaveEE()
{
    ListAdd(); Ti = Lx->v; Ti->dt = dt; //TimeCalcEx evaluates dt
    Ti->ei = Ex;
    Ti->ej = Et;
	//if (Gv) //Gv = Y
    {
        memcpy(Ti->Xi, Ei->X, LN); Ti->dT = dt;
        memcpy(Ti->Xj, Ej->X, LN);
    }
}//add {Ei,Ej,dt,Xi,Xj} to Tv, Lv = Tv: initilized in TimeCalcEx
//--------------------------------------------------------------------
static void 
TimeCalcEx()
{   
    Ei = Ex->v; Et = Ex; Lv = Tv; // Lv initilized in TimeDelStp     
	    //Ej not used for bound interaction, ej = NULL in TimeSaveBE
        TimeCalcBS(); if (dt >= De) TimeSaveBE();
    
    while ((Et = Et->n) != Ex)  //Calc elements, Ei != Ej
    {
        Ej = Et->v; 
        TimeCalcES(); if (dt >= De) TimeSaveEE();     
    }  
}//Calculate  Ex element tti
//--------------------------------------------------------------------
void
TimeCalcTT()
{
    { Ex = ei; TimeCalcEx(); }
    if (ej != NULL)
    { Ex = ej; TimeCalcEx(); }
}//Calculate  ei,ej elements tti
//--------------------------------------------------------------------


    