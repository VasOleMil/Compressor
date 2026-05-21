#include"CBase.h"
#include"CList.h"
#include"CSort.h"
#include"CEmnt.h"
#include"CHold.h"
#include"CMode.h"
#include"CData.h"

//--------------------------------------------------------------------
//      UT == 0 || thread start indicator value, RTL default
//--------------------------------------------------------------------
long    UT  = 0;// 1, unit memory allocation and UD   start
long    UD;     // 2, dedicated destructor,      UD  finish
long    UF;     // 0, memory allocation for regular free
double  Pi,s;   // s = 1.0 / sqrt(2.0), Pi = Arc/Size
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
CDList* Dv;//Malloc list items container
long	k ;//index, used in dimension 
double *Xi,*Xj,*Vi,*Vj;
double	RA, RR, RV, VV, rr, rv, vv, rk, vk, Ri, Rj, Mi, Mj;
//--------------------------------------------------------------------
CDItem *Dx;//list  service  register
CDItem* Di;//list  service  register
CDItem* Dj;//list  service  register
void   *DA;//Memory allocator register, used in Malloc, CompFree
void   *da;//Memory allocator register, used locally in DataItemDel
long    Da;//Dedicated destructor transaction length, used in Malloc
//--------------------------------------------------------------------
static void
DataItemNew(void)
{
    Dx = (CDItem*)malloc(sizeof(CDItem)); if (Dx == NULL) DataFree();
    Dx->p = Dx;             // static context       
    Dx->n = Dx;             // create looped item,
    Dx->v = NULL;           // with no data attached
}//ListNew item provider for Data.                                  Dx
//--------------------------------------------------------------------
static void
DataItemDel(void)
{
    da = Dx->v; if (da) free(da); free(Dx);
}//ListDel item provider for Data
//--------------------------------------------------------------------
static void
DataAdd(void)
{
    //---------------------------------------------------------
    if (Dv->Fn == 0)
    {
        Dv->ListItemNew();//Create new list element,   in   Dx
    }
    else
    {
        Dx = Dv->Fc; Dv->Fn--;

        Di = Dx->p;  //Lx is caller defined   /*  +--->---+  */          
        Dj = Dx->n;  //Lx assumed not empty   /*  |       |  */          
        Dj->p = Di;  //connect back over Lx   /*--i (<x>) j--*/          
        Di->n = Dj;  //connect next over Lx   /*  |       |  */          
        //Dx->p = Dx;//Loop for uniform add   /*  +---<---+  */          
        Dx->n = Dx;  //Looping in new items is also implemented          

        Dv->Fc = Dj; //DetNF(),      
    }//uncomment or change reduced looping for other stepping   
    //---------------------------------------------------------   
    Di = (Dv->Vn == 0) ? (Dx) : (Dv->Vc);

    Dj = Di->n; //Li is caller defined        /*  0 - +-<-+  */          
    Dx->n = Dj; //Lx is caller defined        /*  |   |   |  */          
    Dj->p = Dx; //Li assumed not empty        /*--i->-x->-j--*/          
    Di->n = Dx; //Lx assumed not empty        /*  |   |   |  */          
    Dx->p = Di; //(Lx == Li),  ensured        /*  +-<-+ - 0  */          

    Dv->Vc = Dx; Dv->Vn++; //AttNV()
    //---------------------------------------------------------
}//Moves Free to Value:                  Fc = Fc->n, Vc->n = Dx  
//--------------------------------------------------------------------
static void
DataDel(void)
{
    if (Dv->Vn == 0) return;                        // done
    //-----------------------------------------------------
    Dx = Dv->Vc; Dv->Vn--; //DetPV()

    Di = Dx->p;  //Lx is caller defined   /*  +--->---+  */          
    Dj = Dx->n;  //Lx assumed not empty   /*  |       |  */          
    Dj->p = Di;  //connect back over Lx   /*--i (<x>) j--*/          
    Di->n = Dj;  //connect next over Lx   /*  |       |  */          
    Dx->p = Dx;  //Loop for uniform add   /*  +---<---+  */          
    //Dx->n = Dx;//Looping in new items is also implemented     
    //uncomment or change reduced looping for other stepping
    Dv->Vc = Di;
    //-----------------------------------------------------     
    Dj = (Dv->Fn == 0) ? (Dx) : (Dv->Fc);

    Di = Dj->p; //Lj is caller defined    /*  0 - +-<-+  */ 
    Dx->p = Di; //Lx is caller defined    /*  |   |   |  */ 
    Di->n = Dx; //Li assumed not empty    /*--i->-x->-j--*/ 
    Dj->p = Dx; //Lx assumed not empty    /*  |   |   |  */  
    Dx->n = Dj; //(Lx == Lj),  ensured    /*  +-<-+ - 0  */ 

    Dv->Fc = Dx; Dv->Fn++; //AttPF() 
    //-----------------------------------------------------
}//Moves Value to Free:              Vc = Vc->p, Fc->p = Dx
//--------------------------------------------------------------------
void
DataSize(void)
{
    while (Dv->Fn)   //ListDelNF()
    {
        Dx = Dv->Fc; Dv->Fn--;
 
        Di = Dx->p;  Dj = Dx->n;
        Dj->p = Di;  Di->n = Dj;
      
        Dv->Fc = Dj; Dv->ListItemDel();
    }
}//Releases Free container  resources
//--------------------------------------------------------------------
void
DataInit()
{
    //Set constants
    UT = 1; UD = 2; Pi = 3.141592653589793;
    UF = 0; Da = 0; s  = 0.707106781186547;

    Dv = Ev = Sv = Tv = Mv = Lv = NULL;//initialize modules
    Dv = (CDList*)malloc(sizeof(CDList));  if (Dv == NULL) exit(1);
    //Set data contaners counters and providers
    Dv->Fn = 0; Dv->ListItemDel = DataItemDel;
    Dv->Vn = 0; Dv->ListItemNew = DataItemNew;

    //Init element and time containers   
    SortInit();
    EmntInit();
    TimeInit();
    ModeInit();
}//Construction
//--------------------------------------------------------------------
void
DataFree()
{
   //======dedicated destructors section========
    ModeFree();
    TimeFree();
    EmntFree();
    SortFree(); 
    //======free registered allocated memory=====
    if (Dv != NULL)
    {
        while (Dv->Vn)   //ListDelNV()
        {
            Dx = Dv->Vc; Dv->Vn--;
            Di = Dx->p;  Dj = Dx->n;
            Dj->p = Di;  Di->n = Dj;          
            Dv->Vc = Dj; Dv->ListItemDel();
        }
        while (Dv->Fn)   //ListDelNF()
        {
            Dx = Dv->Fc; Dv->Fn--;
            Di = Dx->p;  Dj = Dx->n;
            Dj->p = Di;  Di->n = Dj;
            Dv->Fc = Dj; Dv->ListItemDel();
        }      
        free(Dv); Dv = NULL;
    }
    //======Exit if memory allocation fails======
    if (DA == NULL)
    {
        //report error, no system memory allocated.

        exit(1); return;
    }
    if (Da > 0)
    {
        //report dedicated transaction interruption.

        exit(2); return;
    }
}//Destruction
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
        DataFree(); return  DA; 
    }
    else if (UX == UT)
    {
        //Dedicated  destructor:       Start
        DataAdd();                //register 
        Dx->v = DA;   Da++;       //   as UF
        //return pointer to allocated memory
        return DA;
    }
    else if (UX == UD)
    {
        //Dedicated  destructor:      Finish
        while (Da > 0)
        {
            Dx->v = NULL; Da--; //store item
            DataDel();          // as not UF
        }
        //return pointer to allocated memory
        return DA;   //Not optimal, but safe.
    }
    else // UX == UF, regular free
    {
        //register  allocated  memory as  UF
        DataAdd(); Dx->v = DA;
        //test dedicated allocation iterrupt 
        if (Da > 0) DataFree();
        //return pointer to allocated memory
        return DA;
    }
}//malloc wrapper, DataLock, exits if allocation fails.       sets: DA
//--------------------------------------------------------------------
