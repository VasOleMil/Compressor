#include"CList.h"
#include"CSort.h"
#include"CEmnt.h"
#include"CHold.h"
#include"CData.h"

//--------------------------------------------------------------------
long    UT;// 1, unit memory allocation and UD   start
long    UD;// 2, dedicated destructor,      UD  finish
long    UF;// 0, memory allocation for regular free
//--------------------------------------------------------------------
CDList *Dv;//Malloc list items container 
void   *DA;//Memory allocator register, used in Dalloc, CompFree
void   *Dx;//Memory allocator register, used locally in DataItemDel
long    Da;//Dedicated destructor transaction length, used in Dalloc
//--------------------------------------------------------------------
void*
Malloc(size_t LN, long CN, long UX)
{
    //No memory allocation:        safe case
    if (CN <= 0 || LN <= 0) return NULL; 
	//Try to allocate memory
    if ((DA = malloc(LN * CN))  == NULL)
    {
        //Set controll of termination thread
        DataFree();
    }
    else if (UX == UT)
    {
        //Dedicated  destructor:       Start
        Lv = Dv; ListAdd();       //register 
        Lx->v = DA;   Da++;       //   as UF
        //return pointer to allocated memory
        return DA;
    }
    else if (UX == UD)
    {
		//Dedicated  destructor:      Finish
        while (Da > 0) 
        { 
            Lv = Dv; ListDel();//save CDItem
            Lx->v = NULL; Da--;//  as not UF
        }
        //return pointer to allocated memory
		return DA;   //Not optimal, but safe.
    }
	else // UX == UF, regular free
    {
        //register  allocated  memory as  UF
		Lv = Dv; ListAdd(); Lx->v = DA; 
        //test dedicated allocation iterrupt 
        if (Da > 0) DataFree(); 
        //return pointer to allocated memory
        return DA;
    }
}//malloc wrapper, DataLock, exits if allocation fails.       sets: DA
//--------------------------------------------------------------------
void
DataFree()
{
    //======dedicated destructors section========
    EmntFree();
    SortFree();
    TimeFree();
    
    //======free registered allocated memory=====
    if (Dv != NULL)
    {
        Lv = Dv; ListFree();
        free(Dv); Dv = NULL;
    }
  
	//======Exit if memory allocation fails======
    if (DA == NULL)
    {
        //report error, no system memory allocated.

        exit(1);
    }

    if (Da > 0)
    {
        //report dedicated transaction interruption.

        exit(1);
    }
}//Destruction
//--------------------------------------------------------------------
static void
DataItemNew(void)
{
    Lx = (CDItem*) Dalloc(sizeof(CDItem), UT, UD);
	Lx->p = Lx;    // create looped item,
	Lx->n = Lx;    // prereqiered for list, 
    Lx->v = NULL;  // with no data attached
}//ListNew item provider for Data.                      Set: looped Lx
//--------------------------------------------------------------------
static void
DataItemDel(void)
{
    if(Dx = Lx->v) free(Dx); free(Lx);
}//ListDel item provider for Data
//--------------------------------------------------------------------
void
DataInit()
{
    Dv = Ev = Sv = Tv = NULL; UT=1; UD=2; UF=Da=0;
    Dv = (CDList*) Malloc(sizeof(CDList), UT, UD);
	//Set data contaners counters and providers
    Dv->Fn = 0; Dv->ListItemDel = DataItemDel;
    Dv->Vn = 0; Dv->ListItemNew = DataItemNew;
    //Init element and time containers
    SortInit(); 
    EmntInit(); 
    TimeInit();
}//Construction
//--------------------------------------------------------------------