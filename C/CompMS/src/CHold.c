#include"CBase.h"
#include"CHold.h"
//--------------------------------------------------------------------
CSList     *Tv;//Malloc list items container 
CLItem     *Tx;//list service register
CLItem     *Tt;//list  service  register
CHold      *Ti;//object service register
//--------------------------------------------------------------------
double     *Xi, *Xj, *Vi, *Vj; 
//--------------------------------------------------------------------
static void
HoldItemNew(void)
{
    Lx = (CLItem*)Malloc(sizeof(CLItem), UT, UT);   //create ListItem
    Ti = (CHold*) Malloc(sizeof(CHold),  UT, UT);   //create Object
    //Loop Lx ListItem and connect to Ei Object 
    Lx->p = Lx;
    Lx->n = Lx;
    Lx->v = Ti;
    //Holder initial setiings
    Ti->Xi = (double*)Malloc(sizeof(double), Rn, UT);
    Ti->Xj = (double*)Malloc(sizeof(double), Rn, UD);
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

    Tv = (CSList*)Malloc(sizeof(CSList), UT, UD);
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

    