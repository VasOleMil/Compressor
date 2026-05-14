#include"CBase.h"
#include"CEmnt.h"
//--------------------------------------------------------------------
CSList  *Ev;//Elements list items container 
    
CLItem  *Ex;//list service register
CLItem  *Em;//list service register
CLItem  *Et;//list service register
    
CEmnt   *Ei;//object service register
CEmnt   *Ej;//object service register
//--------------------------------------------------------------------
long    k, c, n;

double  *rc, *Xi, *Xj, *Vi, *Vj; //rc = r(j) - r(i)

double  RA, RR, RV, rr, rv, vv, rk, vk, Ri, Rj, Mi, Mj;
//--------------------------------------------------------------------
static void 
EmntItemNew(void)
{
    Lx = (CLItem*)  Malloc(sizeof(CLItem),UT, UT); //create ListItem
    Ei = (CEmnt*)   Malloc(sizeof(CEmnt) ,UT, UT); //create Object
    //Loop Lx ListItem and connect to Ei Object 
    Lx->p = Lx;
    Lx->n = Lx;    
    Lx->v = Ei;
    //Emnt initial setiings
    Ei->S = NULL; 
    Ei->X = (double*)Malloc(sizeof(double), Rn, UT);
    Ei->V = (double*)Malloc(sizeof(double), Rn, UD);
}//ListNew item provider for Emnt, sets looped Lx
//--------------------------------------------------------------------
static void 
EmntItemDel(void)
{
    Ei = Lx->v;
    
    free(Ei->X);
    free(Ei->V);
    
    free(Ei);
    free(Lx);  
}//ListRem item provider for Emnt
//--------------------------------------------------------------------
void 
EmntInit(void)
{
	if(Ev != NULL) return;

    Ev = (CSList*) Malloc(sizeof(CSList), UT, UT);
    rc = (double*) Malloc(sizeof(double), Rn, UD); 
	//Set Element contaners counters and providers
	Ev->Fn = 0; Ev->ListItemDel = EmntItemDel;
    Ev->Vn = 0; Ev->ListItemNew = EmntItemNew;
 }//Inits start values in Emnt container.                   Set: Ev
//--------------------------------------------------------------------
void 
EmntFree(void)
{
	if(Ev == NULL) return;

    Lv = Ev; ListFree(); 
    free(Ev); Ev = NULL;
    free(rc); rc = NULL;
}//Releases Free resources  Ev
//--------------------------------------------------------------------
void 
EmntSize(void)
{
    if(Ev == NULL) return; 

    n = Ev->Vn; c = 0; Ex = Ev->Vc; Et = Ex; 
    // calc number, existent
    if(n > 0) do
    {
        c += ((Ei = Et->v) == Si) ? 1 : 0;
	}   while ((Et = Et->n) != Ex); 
    c = Si->Bn - c; n += c; Bn = n; Lv = Ev;
    // increase number
    if(c > 0) for (k = 0; k < c; k++)
    {
		ListAdd(); Ex = Lx; Ei = Ex->v; Ei->S = Si;
    }
    // decrease number
    if(c < 0) for (k = 0; k < n; k++)
    {
        if ((Ei = Et->v)->S == Si)
        {
            Ev->Vc = Ex; ListDel(); 
            if((++c) == 0)   break;
        };  Ex = Ex->n;
    }
}//align number to Si->Bn, Ev->Vn == Bn on success
//--------------------------------------------------------------------
void 
EmntMove(void)
{
    Ex = Ev->Vc; Et = Ex; 
    
    do
    {
        Ei = Et->v; Xi = Ei->X; Vi = Ei->V;

        for (k = 0; k < Rn; k++)
		{
		    Xi[k] += Vi[k] * dT;
		}
	}   while ((Et = Et->n) != Ex);  
}//Moves all elements
//--------------------------------------------------------------------
void 
EmntCollBS(void)
{ 
    Si = Ei->S; Xi = Ei->X; Vi = Ei->V;
    //Calculate scalar product (r*v), r = ri - rb, |r|= Rb - Rt
    for(rv = 0.0, k = 0; k < Rn; k++)
	{
        rk = Xi[k]; 
		vk = Vi[k]; rv += rk * vk;
    }

    Si = Ei->S; Ri = Si->Rt; RR = Rb - Ri;
    RV = Gc * Ri * RR; RR *= RR; RA = 1.0 / RR;
    rv = (Gc < 0.0)
        ? ((rv > -RV) ? 2.0*rv*RA : 0.0)
        : ((rv > +RV) ? 2.0*rv*RA : (rv > -RV) ? (rv + RV)*RA : 0.0);

    for(k = 0; k < Rn; k++) Vi[k] -= rv * Xi[k];         
}//Ei sizing bound-element interaction
//--------------------------------------------------------------------
void 
EmntCollBE(void)
{ 
    Si = Ei->S; Xi = Ei->X; Vi = Ei->V;
    //(r*v), r = ri -rb, |r|= Rb - Rt
    for(rv = 0.0, k = 0; k < Rn; k++)
	{
        rk = Xi[k]; 
		vk = Vi[k]; rv += rk * vk;
    }
	Si = Ei->S; Ri = Si->Rt; RR = Rb - Ri;
    RR *= RR; RA = 1.0 / RR;
    rv = (rv > 0.0)? 2.0*rv*RA : 0.0;

    for(k = 0; k < Rn; k++) Vi[k] -= rv * Xi[k];         
}//Ei elastic bound-element interaction
//--------------------------------------------------------------------
void 
EmntCollBF(void)
{
    Xi = Ei->X; Vi = Ei->V;

    for(rr = rv = vv = 0.0, k = 0; k < Rn; k++)
	{
        rk = Xi[k]; rr += rk * rk; 
		vk = Vi[k]; rv += rk * vk; vv += vk * vk;            
    }   
    
    rv = (rv > 0.0)? -sqrt(vv / rr) : 0.0;

    for(k = 0; k < Rn; k++) Vi[k] = rv * Xi[k]; 
}//Ei bound-element interaction, force speed alignment to center
//--------------------------------------------------------------------
void 
EmntCollES(void)
{
    Xi = Ei->X; Vi = Ei->V; Xj = Ej->X; Vj = Ej->V;
    //Calculate scalar product, rc = rj - ri  
    for(rv = 0.0, k = 0; k < Rn; k++)
    {
        rk = Xj[k] - Xi[k]; rc[k]  = rk;       
        vk = Vj[k] - Vi[k]; rv += rk*vk; 
    }  

    Si = Ei->S; Ri = Si->Rt;  Mi = Si->Mt; 
    Sj = Ej->S; Rj = Sj->Rt;  Mj = Sj->Mt;
    RR = Ri + Rj; RR *= RR; RV = Gc * RA; RA = 1.0 / (RR * (Mi + Mj));

    rv = (Gc < 0.0)
        ? ((rv < +RV) ? 2.0*rv*RA : 0.0)
        : ((rv < -RV) ? 2.0*rv*RA : (rv < +RV) ? (rv - RV)*RA : 0.0);
    
    for(k = 0; k < Rn; k++)
    {
        vk  = rv * rc[k]; 
        Vi[k] += Mj * vk;
        Vj[k] -= Mi * vk;
    } 
}//Ei, Ej sizing element-element interaction
//--------------------------------------------------------------------
void 
EmntCollEE(void)
{
    Xi = Ei->X; Vi = Ei->V; Xj = Ej->X; Vj = Ej->V; 
    //Calculate scalar product, rc = rj - ri  
    for(rv = 0.0, k = 0; k < Rn; k++)
    {
        rk = Xj[k] - Xi[k]; rc[k]  = rk;       
        vk = Vj[k] - Vi[k]; rv += rk*vk; 
    }  

    Si = Ei->S; Ri = Si->Rt;  Mi = Si->Mt; 
    Sj = Ej->S; Rj = Sj->Rt;  Mj = Sj->Mt;
    RR = Ri + Rj; RR *= RR; RA = 1.0 / (RR * (Mi + Mj));
        
    rv = (rv < 0.0)? 2.0*rv*RA : 0.0; 
       
    for(k = 0; k < Rn; k++)
    {
        vk  = rv * rc[k]; 
        Vi[k] += Mj * vk;
        Vj[k] -= Mi * vk;
    } 
}//Ei, Ej elastic element-element interaction
//--------------------------------------------------------------------
void 
EmntCollEF(void)
{
    Si = Ei->S; Mi = Si->Mt; Sj = Ej->S; Mj = Sj->Mt;    
    rv = 2.0 / (Mi + Mj);   Vi = Ei->V;   Vj = Ej->V;
           
    for(k = 0; k < Rn; k++)
    {
        vk  = rv * (Vj[k] - Vi[k]); 
        Vi[k] += Mj * vk;
        Vj[k] -= Mi * vk;
    } 
}//Ei, Ej fast element-element interaction, unconditional
//--------------------------------------------------------------------
    