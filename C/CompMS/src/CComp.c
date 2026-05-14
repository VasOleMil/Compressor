#include"CBase.h"
#include"CComp.h"
//--------------------------------------------------------------------
CSList *Mv;//Malloc list items container 
void   *MA;//Memory allocator register, used in Malloc, CompFree
//--------------------------------------------------------------------
void   *Mx;//Memory allocator register, used locally in DataItemDel
long    Ma;//Dedicated destructor transaction length, used in Malloc
//--------------------------------------------------------------------
void*
Malloc(size_t LN, long CN, long UX)
{
    //No memory allocation:        safe case
    if (CN <= 0 || LN <= 0) return NULL; 
	//Try to allocate memory
    if ((MA = malloc(LN * CN))  == NULL)
    {
        //Set controll of termination thread
        CompFree();
    }
    else if (UX == UT)
    {
        //Dedicated  destructor:       Start
        Lv = Mv; ListAdd();       //register 
        Lx->v = MA;   Ma++;       //   as UF
        //return pointer to allocated memory
        return MA;
    }
    else if (UX == UD)
    {
		//Dedicated  destructor:      Finish
        while (Ma > 0) 
        { 
            Lv = Mv; ListDel();//save CLItem
            Lx->v = NULL; Ma--;//  as not UF
        }
        //return pointer to allocated memory
		return MA;   //Not optimal, but safe.
    }
	else // UX == UF, regular free
    {
        //register  allocated  memory as  UF
		Lv = Mv; ListAdd(); Lx->v = MA; 
        //test dedicated allocation iterrupt 
        if (Ma > 0) CompFree(); 
        //return pointer to allocated memory
        return MA;
    }
}//malloc wrapper, exits if allocation fails.                 sets: MA
//--------------------------------------------------------------------
void
CompFree()
{
    //======dedicated destructors section========
    EmntFree();
    SortFree();
    TimeFree();
    
    //======free registered allocated memory=====
    if (Mv != NULL) 
    { 
        Lv = Mv; ListFree(); 
        free(Mv); Mv = NULL; 
    }
    
	//======Exit if memory allocation fails======
    if (MA == NULL)
    {
        //report error, no system memory allocated.

        exit(1);
    }

    if (Ma > 0)
    {
        //report dedicated transaction interruption.

        exit(1);
    }
}
//--------------------------------------------------------------------
static void
DataItemNew(void)
{
    Lx = (CLItem*) Malloc(sizeof(CLItem), UT, UD);
	Lx->p = Lx;    // create looped item,
	Lx->n = Lx;    // prereqiered for list, 
    Lx->v = NULL;  // with no data attached
}//ListNew item provider for Data.                      Set: looped Lx
//--------------------------------------------------------------------
static void
DataItemDel(void)
{
    if(Mx = Lx->v) free(Mx); free(Lx);
}//ListRem item provider for Data
//--------------------------------------------------------------------
void
CompInit()
{
    Mv = Ev = Sv = Tv = NULL; UT=1; UD=2; UF=Ma=0;
    Mv = (CSList*) Malloc(sizeof(CSList), UT, UD);
	//Set data contaners counters and providers
    Mv->Fn = 0; Mv->ListItemDel = DataItemDel;
    Mv->Vn = 0; Mv->ListItemNew = DataItemNew;
    //Init element and time containers
    SortInit(); EmntInit(); TimeInit();

}
//--------------------------------------------------------------------



//--------------------------------------------------------------------
double
Vgamma(long Dim)
{
    double Pk, Pi; long i, k, d, Rh;

    k = 1; d = 2; Pk = 1.0; Pi = 3.141592653589793;

    if (Dim % 2 == 0)
    {
        for (i = 1, Rh = (Dim - 0) / 2; i <= Rh; i++)
        {
            Pk *= Pi; k *= i;
        }

        return Pk / k;
    }
    else
    {
        for (i = 1, Rh = (Dim - 1) / 2; i <= Rh; i++)
        {
            Pk *= Pi;  d *= 2; k *= 2 * i + 1;
        }

        return Pk * d / k;
    }
}
//--------------------------------------------------------------------
    